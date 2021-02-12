/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file sgwc_app.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#include "conversions.hpp"
#include "itti.hpp"
#include "logger.hpp"
#if SGW_AUTOTEST
#include "enb_s1u.hpp"
#include "mme_s11.hpp"
#endif
#include "sgwc_app.hpp"
#include "pgw_config.hpp"
#include "sgwc_s11.hpp"
#include "sgwc_s5s8.hpp"

#include <stdexcept>

using namespace gtpv2c;
using namespace sgwc;
using namespace std;
// C includes

#if SGW_AUTOTEST
using namespace spgwu;
mme_s11* mme_s11_inst = nullptr;
enb_s1u* enb_s1u_inst = nullptr;
#endif
sgw_s11* sgw_s11_inst   = nullptr;
sgw_s5s8* sgw_s5s8_inst = nullptr;

extern itti_mw* itti_inst;
extern sgwc_app* sgwc_app_inst;
extern pgwc::pgw_config pgw_cfg;

void sgwc_app_task(void*);

//------------------------------------------------------------------------------
teid_t sgwc_app::generate_s11_cp_teid() {
  teid_t loop_detect_teid = teid_s11_cp;
  teid_t teid             = ++teid_s11_cp;
  while ((is_s11c_teid_exist(teid)) || (teid == UNASSIGNED_TEID)) {
    teid = ++teid_s11_cp;
    if (loop_detect_teid == teid) return UNASSIGNED_TEID;
  }
  return teid;
}
//------------------------------------------------------------------------------
teid_t sgwc_app::generate_s5s8_cp_teid() {
  teid_t loop_detect_teid = teid_s5s8_cp;
  teid_t teid             = ++teid_s5s8_cp;
  while ((is_s5s8c_teid_exist(teid)) || (teid == UNASSIGNED_TEID)) {
    teid = ++teid_s5s8_cp;
    if (loop_detect_teid == teid) return UNASSIGNED_TEID;
  }
  return teid;
}

//------------------------------------------------------------------------------
bool sgwc_app::is_s11c_teid_exist(const teid_t& teid_s11_cp) const {
  return bool{s11lteid2sgw_eps_bearer_context.count(teid_s11_cp) > 0};
}
//------------------------------------------------------------------------------
bool sgwc_app::is_s5s8c_teid_exist(const teid_t& teid_s5s8_cp) const {
  return bool{s5s8lteid2sgw_contexts.count(teid_s5s8_cp) > 0};
}
//------------------------------------------------------------------------------
fteid_t sgwc_app::generate_s11_cp_fteid(const struct in_addr ipv4_address) {
  fteid_t fteid        = {};
  fteid.interface_type = S11_S4_SGW_GTP_C;
  fteid.v4             = 1;
  fteid.ipv4_address   = ipv4_address;
  fteid.v6             = 0;
  fteid.ipv6_address   = in6addr_any;
  fteid.teid_gre_key   = generate_s11_cp_teid();
  return fteid;
}
//------------------------------------------------------------------------------
fteid_t sgwc_app::generate_s5s8_cp_fteid(const struct in_addr ipv4_address) {
  fteid_t fteid        = {};
  fteid.interface_type = S5_S8_SGW_GTP_C;
  fteid.v4             = 1;
  fteid.ipv4_address   = ipv4_address;
  fteid.v6             = 0;
  fteid.ipv6_address   = in6addr_any;
  fteid.teid_gre_key   = generate_s5s8_cp_teid();
  return fteid;
}
//------------------------------------------------------------------------------
bool sgwc_app::is_s5s8sgw_teid_2_sgw_contexts(const teid_t& sgw_teid) const {
  return bool{s5s8lteid2sgw_contexts.count(sgw_teid) > 0};
}
//------------------------------------------------------------------------------
bool sgwc_app::is_s11sgw_teid_2_sgw_eps_bearer_context(
    const teid_t& sgw_teid) const {
  return bool{s11lteid2sgw_eps_bearer_context.count(sgw_teid) > 0};
}
//------------------------------------------------------------------------------
std::pair<
    std::shared_ptr<sgw_eps_bearer_context>,
    std::shared_ptr<sgw_pdn_connection>>
sgwc_app::s5s8sgw_teid_2_sgw_contexts(const teid_t& sgw_teid) const {
  return s5s8lteid2sgw_contexts.at(sgw_teid);
}
//------------------------------------------------------------------------------
shared_ptr<sgw_eps_bearer_context>
sgwc_app::s11sgw_teid_2_sgw_eps_bearer_context(const teid_t& sgw_teid) const {
  return s11lteid2sgw_eps_bearer_context.at(sgw_teid);
}
//------------------------------------------------------------------------------
void sgwc_app::set_s5s8sgw_teid_2_sgw_contexts(
    const teid_t& sgw_teid, shared_ptr<sgw_eps_bearer_context> sebc,
    std::shared_ptr<sgw_pdn_connection> spc) {
  s5s8lteid2sgw_contexts[sgw_teid] = std::make_pair(sebc, spc);
}
//------------------------------------------------------------------------------
void sgwc_app::delete_s5s8sgw_teid_2_sgw_contexts(const teid_t& sgw_teid) {
  s5s8lteid2sgw_contexts.erase(sgw_teid);
}

//------------------------------------------------------------------------------
void sgwc_app::set_s11sgw_teid_2_sgw_eps_bearer_context(
    const teid_t& sgw_teid, shared_ptr<sgw_eps_bearer_context> sebc) {
  s11lteid2sgw_eps_bearer_context[sgw_teid] = sebc;
}
//------------------------------------------------------------------------------
bool sgwc_app::is_imsi64_2_sgw_eps_bearer_context(
    const imsi64_t& imsi64) const {
  return bool{imsi2sgw_eps_bearer_context.count(imsi64) > 0};
}
//------------------------------------------------------------------------------
shared_ptr<sgw_eps_bearer_context> sgwc_app::imsi64_2_sgw_eps_bearer_context(
    const imsi64_t& imsi64) const {
  return imsi2sgw_eps_bearer_context.at(imsi64);
}
//------------------------------------------------------------------------------
void sgwc_app::set_imsi64_2_sgw_eps_bearer_context(
    const imsi64_t& imsi64, shared_ptr<sgw_eps_bearer_context> sebc) {
  imsi2sgw_eps_bearer_context[imsi64] = sebc;
}
//------------------------------------------------------------------------------
void sgwc_app::delete_sgw_eps_bearer_context(
    std::shared_ptr<sgw_eps_bearer_context> sebc) {
  if (sebc.get()) {
    imsi64_t imsi64 = sebc->imsi.to_imsi64();
    Logger::sgwc_app().debug(
        "Delete SGW EPS BEARER CONTEXT IMSI " IMSI_64_FMT " ", imsi64);
    imsi2sgw_eps_bearer_context.erase(imsi64);
    s11lteid2sgw_eps_bearer_context.erase(
        sebc->sgw_fteid_s11_s4_cp.teid_gre_key);
    sebc->release();
  }
}
//------------------------------------------------------------------------------
void sgwc_app_task(void* args_p) {
  const task_id_t task_id = TASK_SGWC_APP;
  itti_inst->notify_task_ready(task_id);

  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case S5S8_CREATE_SESSION_RESPONSE:
        if (itti_s5s8_create_session_response* m =
                dynamic_cast<itti_s5s8_create_session_response*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S5S8_DELETE_SESSION_RESPONSE:
        if (itti_s5s8_delete_session_response* m =
                dynamic_cast<itti_s5s8_delete_session_response*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S5S8_DOWNLINK_DATA_NOTIFICATION:
        if (itti_s5s8_downlink_data_notification* m =
                dynamic_cast<itti_s5s8_downlink_data_notification*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S5S8_MODIFY_BEARER_RESPONSE:
        if (itti_s5s8_modify_bearer_response* m =
                dynamic_cast<itti_s5s8_modify_bearer_response*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S5S8_RELEASE_ACCESS_BEARERS_RESPONSE:
        if (itti_s5s8_release_access_bearers_response* m =
                dynamic_cast<itti_s5s8_release_access_bearers_response*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S5S8_REMOTE_PEER_NOT_RESPONDING:
        if (itti_s5s8_remote_peer_not_responding* m =
                dynamic_cast<itti_s5s8_remote_peer_not_responding*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S11_CREATE_SESSION_REQUEST:
        /*
         * We received a create session request from MME (with GTP abstraction
         * here) procedures might be: E-UTRAN Initial Attach UE requests PDN
         * connectivity
         */
        if (itti_s11_create_session_request* m =
                dynamic_cast<itti_s11_create_session_request*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S11_DELETE_SESSION_REQUEST:
        if (itti_s11_delete_session_request* m =
                dynamic_cast<itti_s11_delete_session_request*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S11_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE:
        if (itti_s11_downlink_data_notification_acknowledge* m =
                dynamic_cast<itti_s11_downlink_data_notification_acknowledge*>(
                    msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S11_MODIFY_BEARER_REQUEST:
        if (itti_s11_modify_bearer_request* m =
                dynamic_cast<itti_s11_modify_bearer_request*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S11_RELEASE_ACCESS_BEARERS_REQUEST:
        if (itti_s11_release_access_bearers_request* m =
                dynamic_cast<itti_s11_release_access_bearers_request*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case S11_REMOTE_PEER_NOT_RESPONDING:
        if (itti_s11_remote_peer_not_responding* m =
                dynamic_cast<itti_s11_remote_peer_not_responding*>(msg)) {
          sgwc_app_inst->handle_itti_msg(ref(*m));
        }
        break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          Logger::sgwc_app().info("TIME-OUT event timer id %d", to->timer_id);
        }
        break;
      case TERMINATE:
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::sgwc_app().info("Received terminate message");
          return;
        }
        break;

      case HEALTH_PING:
        break;

      default:
        Logger::sgwc_app().info(
            "no handler for ITTI msg type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
sgwc_app::sgwc_app(const std::string& config_file)
    : s11lteid2sgw_eps_bearer_context() {
  Logger::sgwc_app().startup("Starting...");
  teid_s11_cp                     = 0;
  teid_s5s8_cp                    = 0;
  imsi2sgw_eps_bearer_context     = {};
  s11lteid2sgw_eps_bearer_context = {};
  s5s8lteid2sgw_contexts          = {};

  try {
    sgw_s5s8_inst = new sgw_s5s8();
    sgw_s11_inst  = new sgw_s11();
  } catch (std::exception& e) {
    Logger::sgwc_app().error("Cannot create SGW_APP: %s", e.what());
    throw;
  }

  if (itti_inst->create_task(TASK_SGWC_APP, sgwc_app_task, nullptr)) {
    Logger::sgwc_app().error("Cannot create task TASK_SGWC_APP");
    throw std::runtime_error("Cannot create task TASK_SGWC_APP");
  }
}

//------------------------------------------------------------------------------
sgwc_app::~sgwc_app() {
  if (sgw_s5s8_inst) delete sgw_s5s8_inst;
  if (sgw_s11_inst) delete sgw_s11_inst;
}

//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s11_create_session_request& csreq) {
  //  mme_sgw_tunnel_t                       *new_endpoint_p = NULL;
  //  s_plus_p_gw_eps_bearer_context_information_t
  //  *s_plus_p_gw_eps_bearer_ctxt_info_p = NULL; sgw_eps_bearer_ctxt_t
  //  *eps_bearer_ctxt_p = NULL;

  Logger::sgwc_app().debug(
      "Received S11 CREATE_SESSION_REQUEST sender teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT " ",
      csreq.teid, csreq.gtpc_tx_id);
  /*
   * Upon reception of create session request from MME,
   * S-GW should create UE, eNB and MME contexts and forward message to P-GW.
   */
  if (csreq.gtp_ies.rat_type.rat_type < RAT_TYPE_E_EUTRAN_WB_EUTRAN) {
    Logger::sgwc_app().warn(
        "Received S11 CSReq with RAT < RAT_TYPE_E_EUTRAN_WB_EUTRAN: type %d",
        csreq.gtp_ies.rat_type.rat_type);
  }

  /*
   * As we are abstracting GTP-C transport, FTeid ip address is useless.
   *  We just use the teid to identify MME tunnel. Normally we received either:
   *  - ipv4 address if ipv4 flag is set
   *  - ipv6 address if ipv6 flag is set
   *  - ipv4 and ipv6 if both flags are set
   *  Communication between MME and S-GW involves S11 interface so we are
   * expecting S11_MME_GTP_C (11) as interface_type.
   */
  if (csreq.gtp_ies.sender_fteid_for_cp.interface_type != S11_MME_GTP_C) {
    Logger::sgwc_app().warn(
        "Received S11 CSReq with sender_fteid_for_cp != S11_MME_GTP_C %d, "
        "ignore CSreq",
        csreq.gtp_ies.sender_fteid_for_cp.interface_type);
    return;
  }
  if (csreq.gtp_ies.sender_fteid_for_cp.teid_gre_key == 0) {
    // MME sent request with teid = 0. This is not valid...
    Logger::sgwc_app().warn(
        "Received S11 CSReq with sender_fteid_for_cp.teid = 0, ignore CSR");
    return;
  }

  if ((csreq.teid) && (not sgwc_app_inst->is_s11c_teid_exist(csreq.teid))) {
    Logger::sgwc_app().warn(
        "Received S11 CSReq with dest teid " TEID_FMT " unknown, ignore CSreq",
        csreq.teid);
    cause_t cause = {
        .cause_value = CONTEXT_NOT_FOUND, .pce = 0, .bce = 0, .cs = 0};
    send_create_session_response_cause(
        csreq.gtpc_tx_id, csreq.teid, csreq.r_endpoint, cause);
    return;
  }

  shared_ptr<sgw_eps_bearer_context> ebc;
  imsi_t imsi = {};
  if (csreq.gtp_ies.get(imsi)) {
    Logger::sgwc_app().debug(
        "S11 CREATE_SESSION_REQUEST IMSI %s ", imsi.toString());
    // imsi not authenticated
    indication_t indication = {};
    bool delete_pdn_flag    = false;
    if ((csreq.gtp_ies.get(indication)) && (indication.uimsi)) {
      Logger::sgwc_app().debug(
          "TODO S11 CREATE_SESSION_REQUEST (no AUTHENTICATED IMSI) sender "
          "teid " TEID_FMT "  gtpc_tx_id " PROC_ID_FMT " ",
          csreq.teid, csreq.gtpc_tx_id);
      return;
    } else {
      imsi64_t imsi64 = imsi.to_imsi64();
      if (is_imsi64_2_sgw_eps_bearer_context(imsi64)) {
        Logger::sgwc_app().debug(
            "S11 CREATE_SESSION_REQUEST IMSI %s found sgw eps bearer context",
            imsi.toString());
        ebc = imsi64_2_sgw_eps_bearer_context(imsi64);

        // case identified in: the existing PDN connection context locally,
        // if the Create Session Request is received with the TEID set to zero
        // in the header, or if it is received with a TEID not set to zero in
        // the header and it collides with the default bearer of an existing PDN
        // connection context;
        ebi_t ebi = EPS_BEARER_IDENTITY_UNASSIGNED;
        std::shared_ptr<sgw_pdn_connection> sp;
        if (csreq.gtp_ies.bearer_contexts_to_be_created.size() >= 1) {
          ebi = csreq.gtp_ies.bearer_contexts_to_be_created[0].eps_bearer_id;
          if (ebc->find_pdn_connection(ebi, sp)) {
            delete_pdn_flag = true;
          }
        }
        if (!csreq.teid) {
          ebi = csreq.gtp_ies.bearer_contexts_to_be_created[0].eps_bearer_id;
          if (ebc->find_pdn_connection(ebi, sp)) {
            delete_pdn_flag = true;
          }
        }
        if (delete_pdn_flag) {
          // Create delete session request
          // Quick fix: emulate reception DSR by SGW on S11
          // TODO: 3 options
          //       1) delete ctxt and send CSR (delegate tricky job to PGW)
          //       2) send a single DSR (no procedure), delete ctxt and send CSR
          //       3) create DSR procedure, delete ctxt and send CSR
          // emulate reception by
          itti_s11_delete_session_request dsr{TASK_SGWC_APP, TASK_SGWC_APP};
          dsr.gtp_ies.set(ebc->mme_fteid_s11, 0);  // sender FTEID
          dsr.gtp_ies.set(ebi);
          indication_t indication_flags = {};
          indication_flags.oi           = 1;
          dsr.gtp_ies.set(indication_flags);
          dsr.teid = ebc->sgw_fteid_s11_s4_cp.teid_gre_key;
          // send 'S11' DSR
          ebc->handle_itti_msg(dsr);

          // finally delete the pdn connection
          ebc->delete_pdn_connection(sp);
        }
      } else {
        Logger::sgwc_app().debug(
            "S11 CREATE_SESSION_REQUEST IMSI %s sgw eps bearer context not "
            "found!",
            imsi.toString());
        ebc = std::shared_ptr<sgw_eps_bearer_context>(
            new sgw_eps_bearer_context());
        set_imsi64_2_sgw_eps_bearer_context(imsi64, ebc);
      }
    }
  } else {
    Logger::sgwc_app().debug("S11 CREATE_SESSION_REQUEST no IMSI !");
    if (csreq.teid) {
      if (is_s11sgw_teid_2_sgw_eps_bearer_context(csreq.teid)) {
        ebc = s11sgw_teid_2_sgw_eps_bearer_context(csreq.teid);
      } else {
        Logger::sgwc_app().debug(
            "Discarding S11 CREATE_SESSION_REQUEST sender teid " TEID_FMT
            "  gtpc_tx_id " PROC_ID_FMT ", invalid teid",
            csreq.teid, csreq.gtpc_tx_id);
        return;
      }
    } else {
      // TODO
      Logger::sgwc_app().debug(
          "TODO S11 CREATE_SESSION_REQUEST (no IMSI) sender teid " TEID_FMT
          "  gtpc_tx_id " PROC_ID_FMT "",
          csreq.teid, csreq.gtpc_tx_id);
      return;
    }
  }
  ebc->handle_itti_msg(csreq);
  Logger::sgwc_app().debug(
      "sgw_eps_bearer_context: %s!", ebc->toString().c_str());
}
//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s11_delete_session_request& m) {
  Logger::sgwc_app().debug(
      "Received S11 DELETE_SESSION_REQUEST sender teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT "",
      m.teid, m.gtpc_tx_id);
  if (m.teid) {
    if (is_s11sgw_teid_2_sgw_eps_bearer_context(m.teid)) {
      shared_ptr<sgw_eps_bearer_context> ebc =
          s11sgw_teid_2_sgw_eps_bearer_context(m.teid);
      ebc->handle_itti_msg(m);
      Logger::sgwc_app().debug(
          "sgw_eps_bearer_context: %s!", ebc->toString().c_str());
    } else {
      Logger::sgwc_app().debug(
          "Discarding S11 DELETE_SESSION_REQUEST sender teid " TEID_FMT
          "  gtpc_tx_id " PROC_ID_FMT ", invalid teid",
          m.teid, m.gtpc_tx_id);
      return;
    }
  } else {
    // TODO
    Logger::sgwc_app().debug(
        "Discarding S11 DELETE_SESSION_REQUEST sender teid " TEID_FMT
        "  gtpc_tx_id " PROC_ID_FMT "",
        m.teid, m.gtpc_tx_id);
    return;
  }
}
//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s11_modify_bearer_request& m) {
  Logger::sgwc_app().debug(
      "Received S11 MODIFY_BEARER_REQUEST sender teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT "",
      m.teid, m.gtpc_tx_id);
  if (m.teid) {
    if (is_s11sgw_teid_2_sgw_eps_bearer_context(m.teid)) {
      shared_ptr<sgw_eps_bearer_context> ebc =
          s11sgw_teid_2_sgw_eps_bearer_context(m.teid);
      ebc->handle_itti_msg(m);
      Logger::sgwc_app().debug(
          "sgw_eps_bearer_context: %s!", ebc->toString().c_str());
    } else {
      Logger::sgwc_app().debug(
          "Discarding S11 MODIFY_BEARER_REQUEST sender teid " TEID_FMT
          "  gtpc_tx_id " PROC_ID_FMT ", invalid teid",
          m.teid, m.gtpc_tx_id);
      return;
    }
  } else {
    // TODO
    Logger::sgwc_app().debug(
        "Discarding S11 MODIFY_BEARER_REQUEST sender teid " TEID_FMT
        "  gtpc_tx_id " PROC_ID_FMT ", invalid teid",
        m.teid, m.gtpc_tx_id);
    return;
  }
}
//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s11_release_access_bearers_request& m) {
  Logger::sgwc_app().debug(
      "Received S11 RELEASE_ACCESS_BEARERS_REQUEST sender teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT "",
      m.teid, m.gtpc_tx_id);
  if (m.teid) {
    if (is_s11sgw_teid_2_sgw_eps_bearer_context(m.teid)) {
      shared_ptr<sgw_eps_bearer_context> ebc =
          s11sgw_teid_2_sgw_eps_bearer_context(m.teid);
      ebc->handle_itti_msg(m);
      Logger::sgwc_app().debug(
          "sgw_eps_bearer_context: %s!", ebc->toString().c_str());
    } else {
      Logger::sgwc_app().debug(
          "Discarding S11 RELEASE_ACCESS_BEARERS_REQUEST sender teid " TEID_FMT
          "  gtpc_tx_id " PROC_ID_FMT ", invalid teid",
          m.teid, m.gtpc_tx_id);
      return;
    }
  } else {
    // TODO
    Logger::sgwc_app().debug(
        "Discarding S11 RELEASE_ACCESS_BEARERS_REQUEST sender teid " TEID_FMT
        "  gtpc_tx_id " PROC_ID_FMT ", invalid teid",
        m.teid, m.gtpc_tx_id);
    return;
  }
}
//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s11_remote_peer_not_responding& m) {
  Logger::sgwc_app().debug(
      "Received S11 REMOTE_PEER_NOT_RESPONDING event, gtpc_tx_id " PROC_ID_FMT
      ", todo when necessary.",
      m.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(
    itti_s11_downlink_data_notification_acknowledge& m) {
  Logger::sgwc_app().debug(
      "Received S11 DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE sender "
      "teid " TEID_FMT "  gtpc_tx_id " PROC_ID_FMT " ",
      m.teid, m.gtpc_tx_id);
  if (m.teid) {
    if (is_s11sgw_teid_2_sgw_eps_bearer_context(m.teid)) {
      shared_ptr<sgw_eps_bearer_context> ebc =
          s11sgw_teid_2_sgw_eps_bearer_context(m.teid);
      ebc->handle_itti_msg(m);
    } else {
      Logger::sgwc_app().debug(
          "Discarding S11 DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE sender "
          "teid " TEID_FMT "  gtpc_tx_id " PROC_ID_FMT ", invalid teid",
          m.teid, m.gtpc_tx_id);
      return;
    }
  } else {
    Logger::sgwc_app().debug(
        "Discarding S11 DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE sender "
        "teid " TEID_FMT "  gtpc_tx_id " PROC_ID_FMT ", invalid teid",
        m.teid, m.gtpc_tx_id);
  }
}

//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s5s8_create_session_response& m) {
  Logger::sgwc_app().debug(
      "Received S5S8 CREATE_SESSION_RESPONSE sender teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT " ",
      m.teid, m.gtpc_tx_id);
  if (m.gtp_ies.cause.second.cause_value < CONTEXT_NOT_FOUND) {
    if (m.gtp_ies.s5_s8_pgw_fteid.second.interface_type != S5_S8_PGW_GTP_C) {
      Logger::sgwc_app().warn(
          "Received S5S8 CREATE_SESSION_RESPONSE with "
          "s5_s8_pgw_fteid.interface_type != S5_S8_PGW_GTP_C %d, ignore CSResp",
          m.gtp_ies.sender_fteid_for_cp.second.interface_type);
      return;
    }
    if (m.gtp_ies.s5_s8_pgw_fteid.second.teid_gre_key == 0) {
      // MME sent request with teid = 0. This is not valid...
      Logger::sgwc_app().warn(
          "Received S5S8 CREATE_SESSION_RESPONSE with s5_s8_pgw_fteid.teid = "
          "0, ignore CSResp");
      return;
    }
  }
  if (is_s5s8sgw_teid_2_sgw_contexts(m.teid)) {
    std::pair<
        std::shared_ptr<sgw_eps_bearer_context>,
        std::shared_ptr<sgw_pdn_connection>>
        p = s5s8sgw_teid_2_sgw_contexts(m.teid);
    if ((p.first.get()) && (p.second.get())) {
      p.first->handle_itti_msg(m, p.second);
      // cleanup
      if (0 == p.first->get_num_pdn_connections()) {
        delete_sgw_eps_bearer_context(p.first);
      } else {
        Logger::sgwc_app().debug(
            "sgw_eps_bearer_context: %s!", p.first->toString().c_str());
      }
    } else {
      Logger::sgwc_app().debug(
          "Received S5S8 CREATE_SESSION_RESPONSE with dest teid " TEID_FMT
          ", SGW contexts not found, ignore CSResp",
          m.teid);
    }
  } else {
    Logger::sgwc_app().debug(
        "Received S5S8 CREATE_SESSION_RESPONSE with dest teid " TEID_FMT
        " unknown, ignore CSResp",
        m.teid);
  }
}
//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s5s8_delete_session_response& m) {
  if (is_s5s8sgw_teid_2_sgw_contexts(m.teid)) {
    std::pair<
        std::shared_ptr<sgw_eps_bearer_context>,
        std::shared_ptr<sgw_pdn_connection>>
        p = s5s8sgw_teid_2_sgw_contexts(m.teid);
    if ((p.first.get()) && (p.second.get())) {
      p.first->handle_itti_msg(m, p.second);
      // cleanup
      if (0 == p.first->get_num_pdn_connections()) {
        delete_sgw_eps_bearer_context(p.first);
      } else {
        Logger::sgwc_app().debug(
            "get_num_pdn_connections() = %d",
            p.first->get_num_pdn_connections());
      }
      Logger::sgwc_app().debug(
          "sgw_eps_bearer_context: %s!", p.first->toString().c_str());
    } else {
      Logger::sgwc_app().debug(
          "Received S5S8 DELETE_SESSION_RESPONSE with dest teid " TEID_FMT
          ", SGW contexts not found, ignore!",
          m.teid);
    }
  } else {
    Logger::sgwc_app().debug(
        "Received S5S8 DELETE_SESSION_RESPONSE with dest teid " TEID_FMT
        " unknown, ignore!",
        m.teid);
  }
}
//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s5s8_modify_bearer_response& m) {
  Logger::sgwc_app().debug(
      "Received S5S8 MODIFY_BEARER_RESPONSE sender teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT " ",
      m.teid, m.gtpc_tx_id);
  if (is_s5s8sgw_teid_2_sgw_contexts(m.teid)) {
    std::pair<
        std::shared_ptr<sgw_eps_bearer_context>,
        std::shared_ptr<sgw_pdn_connection>>
        p = s5s8sgw_teid_2_sgw_contexts(m.teid);
    if ((p.first.get()) && (p.second.get())) {
      p.first->handle_itti_msg(m, p.second);
      Logger::sgwc_app().debug(
          "sgw_eps_bearer_context: %s!", p.first->toString().c_str());
    } else {
      Logger::sgwc_app().debug(
          "Received S5S8 MODIFY_BEARER_RESPONSE with dest teid " TEID_FMT
          ", SGW contexts not found, ignore!",
          m.teid);
    }
  } else {
    Logger::sgwc_app().debug(
        "Received S5S8 MODIFY_BEARER_RESPONSE with dest teid " TEID_FMT
        " unknown, ignore!",
        m.teid);
  }
}
//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s5s8_release_access_bearers_response& m) {
  Logger::sgwc_app().debug(
      "Received S5S8 RELEASE_ACCESS_BEARERS_RESPONSE sender teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT " ",
      m.teid, m.gtpc_tx_id);
  if (is_s5s8sgw_teid_2_sgw_contexts(m.teid)) {
    std::pair<
        std::shared_ptr<sgw_eps_bearer_context>,
        std::shared_ptr<sgw_pdn_connection>>
        p = s5s8sgw_teid_2_sgw_contexts(m.teid);
    if ((p.first.get()) && (p.second.get())) {
      p.first->handle_itti_msg(m, p.second);
      Logger::sgwc_app().debug(
          "sgw_eps_bearer_context: %s!", p.first->toString().c_str());
    } else {
      Logger::sgwc_app().debug(
          "Received S5S8 RELEASE_ACCESS_BEARERS_RESPONSE with dest "
          "teid " TEID_FMT ", SGW contexts not found, ignore!",
          m.teid);
    }
  } else {
    Logger::sgwc_app().debug(
        "Received S5S8 RELEASE_ACCESS_BEARERS_RESPONSE with dest teid " TEID_FMT
        " unknown, ignore!",
        m.teid);
  }
}

//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s5s8_remote_peer_not_responding& m) {
  if (!m.l_teid) {
    // Node not responding to path management messages: ECHO
    // TODO
    Logger::sgwc_app().debug(
        "Received S5S8 REMOTE_PEER_NOT_RESPONDING event gtpc_tx_id " PROC_ID_FMT
        " local teid 0",
        m.gtpc_tx_id);
  } else if (is_s5s8sgw_teid_2_sgw_contexts(m.l_teid)) {
    Logger::sgwc_app().debug(
        "Received S5S8 REMOTE_PEER_NOT_RESPONDING event gtpc_tx_id " PROC_ID_FMT
        " local teid " TEID_FMT ".",
        m.gtpc_tx_id, m.l_teid);

    std::pair<
        std::shared_ptr<sgw_eps_bearer_context>,
        std::shared_ptr<sgw_pdn_connection>>
        p = s5s8sgw_teid_2_sgw_contexts(m.l_teid);
    if ((p.first.get()) && (p.second.get())) {
      p.first->handle_itti_msg(m, p.second);
      // cleanup
      if (0 == p.first->get_num_pdn_connections()) {
        delete_sgw_eps_bearer_context(p.first);
      } else {
        Logger::sgwc_app().debug(
            "sgw_eps_bearer_context: %s!", p.first->toString().c_str());
      }
    } else {
      Logger::sgwc_app().debug(
          "Received S5S8 REMOTE_PEER_NOT_RESPONDING with local teid " TEID_FMT
          ", SGW contexts not found, ignore!",
          m.l_teid);
    }
  } else {
    Logger::sgwc_app().debug(
        "Received S5S8 REMOTE_PEER_NOT_RESPONDING with local teid " TEID_FMT
        ", SGW EPS bearer context not found, ignore!",
        m.l_teid);
  }
}

//------------------------------------------------------------------------------
void sgwc_app::handle_itti_msg(itti_s5s8_downlink_data_notification& m) {
  Logger::sgwc_app().debug(
      "Received S5S8 DOWNLINK_DATA_NOTIFICATION sender teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT " ",
      m.teid, m.gtpc_tx_id);
  if (is_s5s8sgw_teid_2_sgw_contexts(m.teid)) {
    std::pair<
        std::shared_ptr<sgw_eps_bearer_context>,
        std::shared_ptr<sgw_pdn_connection>>
        p = s5s8sgw_teid_2_sgw_contexts(m.teid);
    if ((p.first.get()) && (p.second.get())) {
      p.first->handle_itti_msg(m, p.second);
      Logger::sgwc_app().debug(
          "sgw_eps_bearer_context: %s!", p.first->toString().c_str());
    } else {
      Logger::sgwc_app().debug(
          "Received S5S8 DOWNLINK_DATA_NOTIFICATION with dest teid " TEID_FMT
          ", SGW contexts not found, ignore!",
          m.teid);
    }
  } else {
    Logger::sgwc_app().debug(
        "Received S5S8 DOWNLINK_DATA_NOTIFICATION with dest teid " TEID_FMT
        " unknown, ignore!",
        m.teid);
  }
}

//------------------------------------------------------------------------------
void sgwc_app::send_create_session_response_cause(
    const uint64_t gtpc_tx_id, const teid_t teid, const endpoint& r_endpoint,
    const cause_t& cause) const {
  itti_s11_create_session_response* s11 =
      new itti_s11_create_session_response(TASK_SGWC_APP, TASK_SGWC_S11);
  //------
  // GTPV2C-Stack
  //------
  s11->gtpc_tx_id = gtpc_tx_id;
  s11->teid       = teid;
  s11->r_endpoint = r_endpoint;
  s11->gtp_ies.set(cause);
  std::shared_ptr<itti_s11_create_session_response> msg =
      std::shared_ptr<itti_s11_create_session_response>(s11);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::sgwc_app().error(
        "Could not send ITTI message %s to task TASK_SGWC_S11",
        s11->get_msg_name());
  }
}
