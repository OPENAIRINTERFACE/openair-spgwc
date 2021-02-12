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

/*! \file pgw_app.cpp
   \brief
   \author  Lionel GAUTHIER
   \date 2018
   \email: lionel.gauthier@eurecom.fr
*/
#include "pgw_app.hpp"
#include "pgw_config.hpp"
#include "async_shell_cmd.hpp"
#include "common_defs.h"
#include "conversions.hpp"
#include "itti.hpp"
#include "logger.hpp"
#include "pgw_paa_dynamic.hpp"
#include "pgw_s5s8.hpp"
#include "pgwc_sxab.hpp"
#include "PfcpUpNodes.hpp"
#include "string.hpp"

#include <stdexcept>

using namespace pgwc;

#define SYSTEM_CMD_MAX_STR_SIZE 512
extern util::async_shell_cmd* async_shell_cmd_inst;
extern pgw_app* pgw_app_inst;
pgw_s5s8* pgw_s5s8_inst   = nullptr;
pgwc_sxab* pgwc_sxab_inst = nullptr;
extern itti_mw* itti_inst;

void pgw_app_task(void*);

//------------------------------------------------------------------------------
int pgw_app::apply_config() {
  Logger::pgwc_app().info("Apply config...");
  int pool_id = 0;
  for (auto p : pgw_config::spgw_app_.pdns) {
    int range = be32toh(p.ue_pool_range_high.s_addr) -
                be32toh(p.ue_pool_range_low.s_addr);
    paa_dynamic::get_instance().add_pool(
        p.apn_label, pool_id++, p.ue_pool_range_low, range);
    paa_dynamic::get_instance().add_pool(
        p.apn_label, pool_id++, p.paa_pool6_prefix, p.paa_pool6_prefix_len);
  }
  Logger::pgwc_app().info("Applied config");
  return RETURNok;
}

//------------------------------------------------------------------------------
teid_t pgw_app::generate_s5s8_cp_teid() {
  std::unique_lock<std::mutex> ls(m_s5s8_cp_teid_generator);
  teid_t teid = ++teid_s5s8_cp_generator;
  while ((is_s5s8c_teid_exist(teid)) || (teid == UNASSIGNED_TEID)) {
    teid = ++teid_s5s8_cp_generator;
  }
  s5s8cplteid.insert(teid);
  ls.unlock();
  return teid;
}

//------------------------------------------------------------------------------
bool pgw_app::is_s5s8c_teid_exist(const teid_t& teid_s5s8_cp) const {
  return bool{s5s8cplteid.count(teid_s5s8_cp) > 0};
}

//------------------------------------------------------------------------------
void pgw_app::free_s5s8c_teid(const teid_t& teid_s5s8_cp) {
  s5s8cplteid.erase(teid_s5s8_cp);  // can return value of erase
}

//------------------------------------------------------------------------------
bool pgw_app::is_imsi64_2_pgw_context(const imsi64_t& imsi64) const {
  std::shared_lock lock(m_imsi2pgw_context);
  return bool{imsi2pgw_context.count(imsi64) > 0};
}
//------------------------------------------------------------------------------
std::shared_ptr<pgw_context> pgw_app::imsi64_2_pgw_context(
    const imsi64_t& imsi64) const {
  std::shared_lock lock(m_imsi2pgw_context);
  return imsi2pgw_context.at(imsi64);
}
//------------------------------------------------------------------------------
void pgw_app::set_imsi64_2_pgw_context(
    const imsi64_t& imsi64, std::shared_ptr<pgw_context> pc) {
  std::unique_lock lock(m_imsi2pgw_context);
  imsi2pgw_context[imsi64] = pc;
}
//------------------------------------------------------------------------------
void pgw_app::set_seid_2_pgw_context(
    const seid_t& seid, std::shared_ptr<pgw_context>& pc) {
  std::unique_lock lock(m_seid2pgw_context);
  seid2pgw_context[seid] = pc;
}
//------------------------------------------------------------------------------
bool pgw_app::seid_2_pgw_context(
    const seid_t& seid, std::shared_ptr<pgw_context>& pc) const {
  std::shared_lock lock(m_seid2pgw_context);
  std::map<seid_t, std::shared_ptr<pgw_context>>::const_iterator it =
      seid2pgw_context.find(seid);
  if (it != seid2pgw_context.end()) {
    pc = it->second;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
fteid_t pgw_app::build_s5s8_cp_fteid(
    const struct in_addr ipv4_address, const teid_t teid) {
  fteid_t fteid        = {};
  fteid.interface_type = S5_S8_PGW_GTP_C;
  fteid.v4             = 1;
  fteid.ipv4_address   = ipv4_address;
  fteid.v6             = 0;
  fteid.ipv6_address   = in6addr_any;
  fteid.teid_gre_key   = teid;
  return fteid;
}
//------------------------------------------------------------------------------
fteid_t pgw_app::generate_s5s8_cp_fteid(const struct in_addr ipv4_address) {
  teid_t teid = generate_s5s8_cp_teid();
  return build_s5s8_cp_fteid(ipv4_address, teid);
}
//------------------------------------------------------------------------------
void pgw_app::free_s5s8_cp_fteid(const fteid_t& fteid) {
  std::unique_lock lock(m_s5s8lteid2pgw_context);
  s5s8lteid2pgw_context.erase(fteid.teid_gre_key);
  free_s5s8c_teid(fteid.teid_gre_key);
}
//------------------------------------------------------------------------------
bool pgw_app::is_s5s8cpgw_fteid_2_pgw_context(
    const fteid_t& ls5s8_fteid) const {
  std::shared_lock lock(m_s5s8lteid2pgw_context);
  return bool{s5s8lteid2pgw_context.count(ls5s8_fteid.teid_gre_key) > 0};
}
//------------------------------------------------------------------------------
std::shared_ptr<pgw_context> pgw_app::s5s8cpgw_fteid_2_pgw_context(
    fteid_t& ls5s8_fteid) {
  if (is_s5s8cpgw_fteid_2_pgw_context(ls5s8_fteid)) {
    return s5s8lteid2pgw_context.at(ls5s8_fteid.teid_gre_key);
  } else {
    return std::shared_ptr<pgw_context>(nullptr);
  }
}
//------------------------------------------------------------------------------
void pgw_app::set_s5s8cpgw_fteid_2_pgw_context(
    fteid_t& ls5s8_fteid, std::shared_ptr<pgw_context> spc) {
  std::unique_lock lock(m_s5s8lteid2pgw_context);
  s5s8lteid2pgw_context[ls5s8_fteid.teid_gre_key] = spc;
}

//------------------------------------------------------------------------------
void pgw_app::delete_pgw_context(std::shared_ptr<pgw_context> spc) {
  imsi64_t imsi64 = spc.get()->imsi.to_imsi64();
  std::unique_lock lock(m_imsi2pgw_context);
  imsi2pgw_context.erase(imsi64);
}
//------------------------------------------------------------------------------
void pgw_app::restore_sx_sessions(const seid_t& seid) const {
  std::shared_lock lock(m_seid2pgw_context);
  // TODO
}

//------------------------------------------------------------------------------
void pgw_app_task(void*) {
  const task_id_t task_id = TASK_PGWC_APP;
  itti_inst->notify_task_ready(task_id);

  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case SXAB_SESSION_ESTABLISHMENT_RESPONSE:
        if (itti_sxab_session_establishment_response* m =
                dynamic_cast<itti_sxab_session_establishment_response*>(msg)) {
          pgw_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case SXAB_SESSION_MODIFICATION_RESPONSE:
        if (itti_sxab_session_modification_response* m =
                dynamic_cast<itti_sxab_session_modification_response*>(msg)) {
          pgw_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case SXAB_SESSION_DELETION_RESPONSE:
        if (itti_sxab_session_deletion_response* m =
                dynamic_cast<itti_sxab_session_deletion_response*>(msg)) {
          pgw_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case SXAB_SESSION_REPORT_REQUEST:
        pgw_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_sxab_session_report_request>(
                shared_msg));
        break;

      case S5S8_CREATE_SESSION_REQUEST:
        pgw_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_s5s8_create_session_request>(
                shared_msg));
        break;

      case S5S8_DELETE_SESSION_REQUEST:
        pgw_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_s5s8_delete_session_request>(
                shared_msg));
        break;

      case S5S8_MODIFY_BEARER_REQUEST:
        pgw_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_s5s8_modify_bearer_request>(
                shared_msg));
        break;

      case S5S8_RELEASE_ACCESS_BEARERS_REQUEST:
        pgw_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_s5s8_release_access_bearers_request>(
                shared_msg));
        break;

      case S5S8_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE:
        if (itti_s5s8_downlink_data_notification_acknowledge* m =
                dynamic_cast<itti_s5s8_downlink_data_notification_acknowledge*>(
                    msg)) {
          pgw_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          Logger::pgwc_app().trace("TIME-OUT event timer id %d", to->timer_id);
          switch (to->arg1_user) {
            case kTriggerAssociationUpNodes:
              PfcpUpNodes::Instance().TriggerAssociations();
              break;
            default:
              Logger::pgwc_app().error(
                  "TIME-OUT event timer id %d not handled", to->timer_id);
          }
        }
        break;
      case TERMINATE:
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::pgwc_app().info("Received terminate message");
          return;
        }
      case HEALTH_PING:
        break;
      default:
        Logger::pgwc_app().info("no handler for msg type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
pgw_app::pgw_app(const std::string& config_file)
    : m_s5s8_cp_teid_generator(),
      m_imsi2pgw_context(),
      m_s5s8lteid2pgw_context(),
      m_seid2pgw_context() {
  Logger::pgwc_app().startup("Starting...");

  teid_s5s8_cp_generator = 0;
  imsi2pgw_context       = {};
  s5s8lteid2pgw_context  = {};
  s5s8cplteid            = {};

  apply_config();

  if (itti_inst->create_task(TASK_PGWC_APP, pgw_app_task, nullptr)) {
    Logger::pgwc_app().error("Cannot create task TASK_PGWC_APP");
    throw std::runtime_error("Cannot create task TASK_PGWC_APP");
  }

  try {
    pgw_s5s8_inst  = new pgw_s5s8();
    pgwc_sxab_inst = new pgwc_sxab();
    PfcpUpNodes::Instance().TriggerAssociations();
  } catch (std::exception& e) {
    Logger::pgwc_app().error("Cannot create PGW_APP: %s", e.what());
    throw;
  }

  Logger::pgwc_app().startup("Started");
}
//------------------------------------------------------------------------------
void pgw_app::send_create_session_response_cause(
    const uint64_t gtpc_tx_id, const teid_t teid, const endpoint& r_endpoint,
    const cause_t& cause) const {
  itti_s5s8_create_session_response* s5s8 =
      new itti_s5s8_create_session_response(TASK_PGWC_APP, TASK_PGWC_S5S8);
  //------
  // GTPV2C-Stack
  //------
  s5s8->gtpc_tx_id = gtpc_tx_id;
  s5s8->teid       = teid;
  s5s8->r_endpoint = r_endpoint;
  s5s8->gtp_ies.set(cause);
  std::shared_ptr<itti_s5s8_create_session_response> msg =
      std::shared_ptr<itti_s5s8_create_session_response>(s5s8);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5s8->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void pgw_app::send_delete_session_response_cause_request_accepted(
    const uint64_t gtpc_tx_id, const teid_t teid,
    const endpoint& r_endpoint) const {
  cause_t cause = {
      .cause_value = REQUEST_ACCEPTED, .pce = 0, .bce = 0, .cs = 0};
  itti_s5s8_delete_session_response* s5s8 =
      new itti_s5s8_delete_session_response(TASK_PGWC_APP, TASK_PGWC_S5S8);
  //------
  // GTPV2C-Stack
  //------
  s5s8->gtpc_tx_id = gtpc_tx_id;
  s5s8->teid       = teid;
  s5s8->r_endpoint = r_endpoint;
  s5s8->gtp_ies.set(cause);
  std::shared_ptr<itti_s5s8_delete_session_response> msg =
      std::shared_ptr<itti_s5s8_delete_session_response>(s5s8);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5s8->get_msg_name());
  }
}
//------------------------------------------------------------------------------
void pgw_app::send_modify_bearer_response_cause_context_not_found(
    const uint64_t gtpc_tx_id, const teid_t teid,
    const endpoint& r_endpoint) const {
  cause_t cause = {
      .cause_value = CONTEXT_NOT_FOUND, .pce = 0, .bce = 0, .cs = 0};
  itti_s5s8_modify_bearer_response* s5s8 =
      new itti_s5s8_modify_bearer_response(TASK_PGWC_APP, TASK_PGWC_S5S8);
  //------
  // GTPV2C-Stack
  //------
  s5s8->gtpc_tx_id = gtpc_tx_id;
  s5s8->teid       = teid;
  s5s8->r_endpoint = r_endpoint;
  s5s8->gtp_ies.set(cause);
  std::shared_ptr<itti_s5s8_modify_bearer_response> msg =
      std::shared_ptr<itti_s5s8_modify_bearer_response>(s5s8);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5s8->get_msg_name());
  }
}
//------------------------------------------------------------------------------
void pgw_app::send_delete_session_response_cause_context_not_found(
    const uint64_t gtpc_tx_id, const teid_t teid,
    const endpoint& r_endpoint) const {
  cause_t cause = {
      .cause_value = CONTEXT_NOT_FOUND, .pce = 0, .bce = 0, .cs = 0};
  itti_s5s8_delete_session_response* s5s8 =
      new itti_s5s8_delete_session_response(TASK_PGWC_APP, TASK_PGWC_S5S8);
  //------
  // GTPV2C-Stack
  //------
  s5s8->gtpc_tx_id = gtpc_tx_id;
  s5s8->teid       = teid;
  s5s8->r_endpoint = r_endpoint;
  s5s8->gtp_ies.set(cause);
  std::shared_ptr<itti_s5s8_delete_session_response> msg =
      std::shared_ptr<itti_s5s8_delete_session_response>(s5s8);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5s8->get_msg_name());
  }
}
//------------------------------------------------------------------------------
void pgw_app::send_release_access_bearers_response_cause_context_not_found(
    const uint64_t gtpc_tx_id, const teid_t teid,
    const endpoint& r_endpoint) const {
  cause_t cause = {
      .cause_value = CONTEXT_NOT_FOUND, .pce = 0, .bce = 0, .cs = 0};
  itti_s5s8_release_access_bearers_response* s5s8 =
      new itti_s5s8_release_access_bearers_response(
          TASK_PGWC_APP, TASK_PGWC_S5S8);
  //------
  // GTPV2C-Stack
  //------
  s5s8->gtpc_tx_id = gtpc_tx_id;
  s5s8->teid       = teid;
  s5s8->r_endpoint = r_endpoint;
  s5s8->gtp_ies.set(cause);
  std::shared_ptr<itti_s5s8_release_access_bearers_response> msg =
      std::shared_ptr<itti_s5s8_release_access_bearers_response>(s5s8);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5s8->get_msg_name());
  }
}
//------------------------------------------------------------------------------
void pgw_app::send_release_access_bearers_response_cause_request_accepted(
    const uint64_t gtpc_tx_id, const teid_t teid,
    const endpoint& r_endpoint) const {
  cause_t cause = {
      .cause_value = REQUEST_ACCEPTED, .pce = 0, .bce = 0, .cs = 0};
  itti_s5s8_release_access_bearers_response* s5s8 =
      new itti_s5s8_release_access_bearers_response(
          TASK_PGWC_APP, TASK_PGWC_S5S8);
  //------
  // GTPV2C-Stack
  //------
  s5s8->gtpc_tx_id = gtpc_tx_id;
  s5s8->teid       = teid;
  s5s8->r_endpoint = r_endpoint;
  s5s8->gtp_ies.set(cause);
  std::shared_ptr<itti_s5s8_release_access_bearers_response> msg =
      std::shared_ptr<itti_s5s8_release_access_bearers_response>(s5s8);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5s8->get_msg_name());
  }
}
//------------------------------------------------------------------------------
void pgw_app::handle_itti_msg(
    std::shared_ptr<itti_s5s8_create_session_request> scsreq) {
  itti_s5s8_create_session_request* csreq = scsreq.get();
  Logger::pgwc_app().debug(
      "Received S5S8 CREATE_SESSION_REQUEST teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT " ",
      csreq->teid, csreq->gtpc_tx_id);

  if (csreq->gtp_ies.rat_type.rat_type < RAT_TYPE_E_EUTRAN_WB_EUTRAN) {
    Logger::pgwc_app().warn(
        "Received S5_S8 CREATE_SESSION_REQUEST with RAT != "
        "RAT_TYPE_E_EUTRAN_WB_EUTRAN: type %d",
        csreq->gtp_ies.rat_type);
  }

  if (csreq->gtp_ies.sender_fteid_for_cp.interface_type != S5_S8_SGW_GTP_C) {
    Logger::pgwc_app().warn(
        "Received S5_S8 CREATE_SESSION_REQUEST with sender_fteid_for_cp != "
        "S5_S8_SGW_GTP_C %d, ignore message",
        csreq->gtp_ies.sender_fteid_for_cp.interface_type);
    return;
  }
  if (csreq->gtp_ies.sender_fteid_for_cp.teid_gre_key == 0) {
    // MME sent request with teid = 0. This is not valid...
    Logger::pgwc_app().warn(
        "Received S5_S8 CREATE_SESSION_REQUEST with sender_fteid_for_cp.teid = "
        "0, ignore message");
    return;
  }
  if ((csreq->teid) && (not pgw_app_inst->is_s5s8c_teid_exist(csreq->teid))) {
    Logger::pgwc_app().warn(
        "Received S5_S8 CREATE_SESSION_REQUEST with dest teid " TEID_FMT
        " unknown, ignore message",
        csreq->teid);
    cause_t cause = {
        .cause_value = CONTEXT_NOT_FOUND, .pce = 0, .bce = 0, .cs = 0};
    send_create_session_response_cause(
        csreq->gtpc_tx_id, csreq->gtp_ies.sender_fteid_for_cp.teid_gre_key,
        csreq->r_endpoint, cause);
    return;
  }

  if (not pgw_config::IsDottedApnHandled(
          csreq->gtp_ies.apn.access_point_name, csreq->gtp_ies.pdn_type)) {
    // MME sent request with teid = 0. This is not valid...
    Logger::pgwc_app().warn(
        "Received CREATE_SESSION_REQUEST unknown requested APN %s, ignore "
        "message",
        csreq->gtp_ies.apn.access_point_name.c_str());
    // TODO send reply
    cause_t cause = {
        .cause_value = MISSING_OR_UNKNOWN_APN, .pce = 0, .bce = 0, .cs = 0};
    send_create_session_response_cause(
        csreq->gtpc_tx_id, csreq->gtp_ies.sender_fteid_for_cp.teid_gre_key,
        csreq->r_endpoint, cause);
    return;
  }

  std::shared_ptr<pgw_context> pc;
  imsi_t imsi = {};
  if (csreq->gtp_ies.get(imsi)) {
    // imsi not authenticated
    indication_t indication = {};
    if ((csreq->gtp_ies.get(indication)) && (indication.uimsi)) {
      Logger::pgwc_app().debug(
          "TODO S5_S8 CREATE_SESSION_REQUEST (no AUTHENTICATED IMSI) "
          "teid " TEID_FMT "  gtpc_tx_id " PROC_ID_FMT " ",
          csreq->teid, csreq->gtpc_tx_id);
      return;
    } else {
      imsi64_t imsi64 = imsi.to_imsi64();

      if (is_imsi64_2_pgw_context(imsi64)) {
        pc = imsi64_2_pgw_context(imsi64);
      } else {
        pc = std::shared_ptr<pgw_context>(new pgw_context());
        set_imsi64_2_pgw_context(imsi64, pc);
      }
    }
  } else {
    if (csreq->teid) {
      fteid_t l_fteid = pgw_app_inst->build_s5s8_cp_fteid(
          pgw_config::pgw_s5s8_.iface.addr4, csreq->teid);
      if (is_s5s8c_teid_exist(csreq->teid)) {
        pc = s5s8cpgw_fteid_2_pgw_context(l_fteid);
      } else {
        Logger::pgwc_app().debug(
            "Discarding S5_S8 CREATE_SESSION_REQUEST teid " TEID_FMT
            "  gtpc_tx_id " PROC_ID_FMT ", invalid teid",
            csreq->teid, csreq->gtpc_tx_id);
        return;
      }
    } else {
      // TODO
      Logger::pgwc_app().debug(
          "TODO S5_S8 CREATE_SESSION_REQUEST (no IMSI) teid " TEID_FMT
          "  gtpc_tx_id " PROC_ID_FMT " ",
          csreq->teid, csreq->gtpc_tx_id);
      return;
    }
  }
  pc.get()->handle_itti_msg(scsreq);
}
//------------------------------------------------------------------------------
void pgw_app::handle_itti_msg(
    std::shared_ptr<itti_s5s8_modify_bearer_request> smbreq) {
  itti_s5s8_modify_bearer_request* mbreq = smbreq.get();
  Logger::pgwc_app().debug(
      "Received S5S8 MODIFY_BEARER_REQUEST teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT " ",
      mbreq->teid, mbreq->gtpc_tx_id);
  fteid_t sender_fteid      = {};
  bool sender_fteid_present = mbreq->gtp_ies.get(sender_fteid);

  if (sender_fteid_present) {
    if (mbreq->gtp_ies.sender_fteid_for_cp.interface_type != S5_S8_SGW_GTP_C) {
      Logger::pgwc_app().warn(
          "Received S5_S8 MODIFY_BEARER_REQUEST with sender_fteid_for_cp != "
          "S5_S8_SGW_GTP_C %d, ignore message",
          mbreq->gtp_ies.sender_fteid_for_cp.interface_type);
      return;
    }
    // Should be handled by gtpv2-c layer
    if (mbreq->gtp_ies.sender_fteid_for_cp.teid_gre_key == 0) {
      // SPGW sent request with teid = 0. This is not valid...
      Logger::pgwc_app().warn(
          "Received S5_S8 MODIFY_BEARER_REQUEST with sender_fteid_for_cp.teid "
          "= 0, ignore message");
      return;
    }
  }
  fteid_t l_fteid =
      build_s5s8_cp_fteid(pgw_config::pgw_s5s8_.iface.addr4, mbreq->teid);
  std::shared_ptr<pgw_context> pc = s5s8cpgw_fteid_2_pgw_context(l_fteid);
  if (pc.get()) {
    pc.get()->handle_itti_msg(smbreq);
  } else {
    if (sender_fteid_present) {
      Logger::pgwc_app().info(
          "Received S5_S8 DELETE_SESSION_REQUEST with dest teid " TEID_FMT
          " unknown, notify sender",
          mbreq->teid);
      send_delete_session_response_cause_context_not_found(
          mbreq->gtpc_tx_id, sender_fteid.teid_gre_key, mbreq->r_endpoint);
      return;
    } else {
      Logger::pgwc_app().info(
          "Received S5_S8 DELETE_SESSION_REQUEST with dest teid " TEID_FMT
          " unknown, no sender FTEID, discarded!",
          mbreq->teid);
      return;
    }
  }
}
//------------------------------------------------------------------------------
void pgw_app::handle_itti_msg(
    std::shared_ptr<itti_s5s8_release_access_bearers_request> smbreq) {
  Logger::pgwc_app().debug(
      "Received S5S8 RELEASE_ACCESS_BEARERS_REQUEST teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT " ",
      smbreq->teid, smbreq->gtpc_tx_id);

  fteid_t l_fteid =
      build_s5s8_cp_fteid(pgw_config::pgw_s5s8_.iface.addr4, smbreq->teid);
  std::shared_ptr<pgw_context> pc = s5s8cpgw_fteid_2_pgw_context(l_fteid);
  if (pc.get()) {
    pc.get()->handle_itti_msg(smbreq);
  } else {
    Logger::pgwc_app().info(
        "Received S5_S8 RELEASE_ACCESS_BEARERS_REQUEST with dest teid " TEID_FMT
        " unknown, no sender FTEID, discarded!",
        smbreq->teid);
    return;
  }
}

//------------------------------------------------------------------------------
void pgw_app::handle_itti_msg(
    std::shared_ptr<itti_s5s8_delete_session_request> sdsreq) {
  itti_s5s8_delete_session_request* dsreq = sdsreq.get();
  Logger::pgwc_app().debug(
      "Received S5S8 DELETE_SESSION_REQUEST teid " TEID_FMT
      "  gtpc_tx_id " PROC_ID_FMT " ",
      dsreq->teid, dsreq->gtpc_tx_id);

  fteid_t sender_fteid      = {};
  bool sender_fteid_present = dsreq->gtp_ies.get(sender_fteid);

  if (sender_fteid_present) {
    if (dsreq->gtp_ies.sender_fteid_for_cp.interface_type != S5_S8_SGW_GTP_C) {
      Logger::pgwc_app().warn(
          "Received S5_S8 DELETE_SESSION_REQUEST with sender_fteid_for_cp != "
          "S5_S8_SGW_GTP_C %d, ignore message",
          dsreq->gtp_ies.sender_fteid_for_cp.interface_type);
      return;
    }
    // Should be handled by gtpv2-c layer
    if (dsreq->gtp_ies.sender_fteid_for_cp.teid_gre_key == 0) {
      // MME sent request with teid = 0. This is not valid...
      Logger::pgwc_app().warn(
          "Received S5_S8 DELETE_SESSION_REQUEST with sender_fteid_for_cp.teid "
          "= 0, ignore message");
      return;
    }
  }
  fteid_t l_fteid =
      build_s5s8_cp_fteid(pgw_config::pgw_s5s8_.iface.addr4, dsreq->teid);
  std::shared_ptr<pgw_context> pc = s5s8cpgw_fteid_2_pgw_context(l_fteid);
  if (pc.get()) {
    pc.get()->handle_itti_msg(sdsreq);
    if (0 == pc.get()->get_num_apn_contexts()) {
      delete_pgw_context(pc);
    }
  } else {
    if (sender_fteid_present) {
      Logger::pgwc_app().info(
          "Received S5_S8 DELETE_SESSION_REQUEST with dest teid " TEID_FMT
          " unknown, notify sender",
          dsreq->teid);
      send_delete_session_response_cause_context_not_found(
          dsreq->gtpc_tx_id, sender_fteid.teid_gre_key, dsreq->r_endpoint);
      return;
    } else {
      Logger::pgwc_app().info(
          "Received S5_S8 DELETE_SESSION_REQUEST with dest teid " TEID_FMT
          " unknown, no sender FTEID, discarded!",
          dsreq->teid);
      return;
    }
  }
}

//------------------------------------------------------------------------------
void pgw_app::handle_itti_msg(
    itti_s5s8_downlink_data_notification_acknowledge& m) {
  fteid_t l_fteid =
      build_s5s8_cp_fteid(pgw_config::pgw_s5s8_.iface.addr4, m.teid);
  std::shared_ptr<pgw_context> pc = s5s8cpgw_fteid_2_pgw_context(l_fteid);
  if (pc.get()) {
    Logger::pgwc_app().debug(
        "Received S5S8 DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE sender "
        "teid " TEID_FMT "  gtpc_tx_id " PROC_ID_FMT " ",
        m.teid, m.gtpc_tx_id);
    pc.get()->handle_itti_msg(m);
  } else {
    Logger::pgwc_app().debug(
        "Received S5S8 DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE sender "
        "teid " TEID_FMT "  gtpc_tx_id " PROC_ID_FMT " context not found!",
        m.teid, m.gtpc_tx_id);
  }
}

//------------------------------------------------------------------------------
void pgw_app::handle_itti_msg(
    itti_sxab_session_establishment_response& seresp) {
  std::shared_ptr<pgw_context> pc = {};
  if (seid_2_pgw_context(seresp.seid, pc)) {
    pc.get()->handle_itti_msg(seresp);
  } else {
    Logger::pgwc_app().debug(
        "Received SXAB SESSION ESTABLISHMENT RESPONSE seid" TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", pgw_context not found, discarded!",
        seresp.seid, seresp.trxn_id);
  }
}
//------------------------------------------------------------------------------
void pgw_app::handle_itti_msg(itti_sxab_session_modification_response& smresp) {
  std::shared_ptr<pgw_context> pc = {};
  if (seid_2_pgw_context(smresp.seid, pc)) {
    pc.get()->handle_itti_msg(smresp);
  } else {
    Logger::pgwc_app().debug(
        "Received SXAB SESSION MODIFICATION RESPONSE seid" TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", pgw_context not found, discarded!",
        smresp.seid, smresp.trxn_id);
  }
}
//------------------------------------------------------------------------------
void pgw_app::handle_itti_msg(itti_sxab_session_deletion_response& smresp) {
  std::shared_ptr<pgw_context> pc = {};
  if (seid_2_pgw_context(smresp.seid, pc)) {
    pc.get()->handle_itti_msg(smresp);

    if (pc->apns.size() == 0) {
      delete_pgw_context(pc);
    }
  } else {
    Logger::pgwc_app().debug(
        "Received SXAB SESSION DELETION RESPONSE seid" TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", pgw_context not found, discarded!",
        smresp.seid, smresp.trxn_id);
  }
}

//------------------------------------------------------------------------------
void pgw_app::handle_itti_msg(
    std::shared_ptr<itti_sxab_session_report_request> snr) {
  std::shared_ptr<pgw_context> pc = {};
  if (seid_2_pgw_context(snr->seid, pc)) {
    pc.get()->handle_itti_msg(snr);
  } else {
    Logger::pgwc_app().debug(
        "Received SXAB SESSION REPORT REQUEST seid" TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", pgw_context not found, discarded!",
        snr->seid, snr->trxn_id);
  }
}
