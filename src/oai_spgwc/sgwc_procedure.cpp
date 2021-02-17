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

#include "sgwc_procedure.hpp"
#include "common_defs.h"
#include "itti.hpp"
#include "itti_msg_s11.hpp"
#include "itti_msg_s5s8.hpp"
#include "logger.hpp"
#include "pgw_config.hpp"
#include "sgwc_app.hpp"
#include "sgwc_eps_bearer_context.hpp"

using namespace gtpv2c;
using namespace sgwc;
using namespace std;

extern itti_mw* itti_inst;
extern sgwc_app* sgwc_app_inst;

void sebc_procedure::handle_itti_msg(
    itti_s5s8_create_session_response& resp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection> spc) {
  Logger::sgwc_app().error(
      "Unhandled message itti_s5s8_create_session_response");
}
void sebc_procedure::handle_itti_msg(
    itti_s5s8_delete_session_response& resp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection> spc) {
  Logger::sgwc_app().error(
      "Unhandled message itti_s5s8_delete_session_response");
}
void sebc_procedure::handle_itti_msg(
    itti_s5s8_modify_bearer_response& resp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection> spc) {
  Logger::sgwc_app().error(
      "Unhandled message itti_s5s8_modify_bearer_response");
}
void sebc_procedure::handle_itti_msg(
    itti_s5s8_release_access_bearers_response& resp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection> spc) {
  Logger::sgwc_app().error(
      "Unhandled message itti_s5s8_release_access_bearers_response");
}
void sebc_procedure::handle_itti_msg(
    itti_s5s8_remote_peer_not_responding& resp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection> spc) {
  Logger::sgwc_app().error(
      "Unhandled message itti_s5s8_remote_peer_not_responding");
}
void sebc_procedure::handle_itti_msg(
    itti_s5s8_downlink_data_notification& resp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection> spc) {
  Logger::sgwc_app().error(
      "Unhandled message itti_s5s8_downlink_data_notification");
}
void sebc_procedure::handle_itti_msg(
    itti_s11_downlink_data_notification_acknowledge& resp) {
  Logger::sgwc_app().error(
      "Unhandled message itti_s11_downlink_data_notification_acknowledge");
}

//------------------------------------------------------------------------------
int create_session_request_procedure::run(
    shared_ptr<sgw_eps_bearer_context> c) {
  // TODO check if compatible with ongoing procedures if any
  // for (auto p : pending_procedures) {
  //  if (p) {
  //
  //  }
  //}
  std::shared_ptr<sgw_pdn_connection> pdn = {};
  if (c->find_pdn_connection(
          msg.gtp_ies.apn.access_point_name, msg.gtp_ies.pdn_type, pdn)) {
    Logger::sgwc_app().info(
        "PDN connection already exist for APN %s",
        msg.gtp_ies.apn.access_point_name.c_str());
    return RETURNerror;
  }
  ebc = c;

  sgw_pdn_connection* p                   = new sgw_pdn_connection();
  p->apn_in_use                           = msg.gtp_ies.apn.access_point_name;
  p->pdn_type                             = msg.gtp_ies.pdn_type;
  std::shared_ptr<sgw_pdn_connection> spc = ebc->insert_pdn_connection(p);
  // TODO : default_bearer
  p->default_bearer =
      msg.gtp_ies.bearer_contexts_to_be_created.at(0).eps_bearer_id;
  p->sgw_fteid_s5_s8_cp = sgwc_app_inst->generate_s5s8_cp_fteid(
      pgwc::pgw_config::sgw_s5s8_.iface.addr4);
  sgwc_app_inst->set_s5s8sgw_teid_2_sgw_contexts(
      p->sgw_fteid_s5_s8_cp.teid_gre_key, c, spc);

  // Forward to P-GW (temp use ITTI instead of ITTI/GTPv2-C/UDP)
  itti_s5s8_create_session_request* s5s8_csr =
      new itti_s5s8_create_session_request(TASK_SGWC_APP, TASK_SGWC_S5S8);
  s5s8_csr->gtpc_tx_id = get_trxn_id();
  s5s8_csr->l_teid     = p->sgw_fteid_s5_s8_cp.teid_gre_key;

  // transfer IEs from S11 msg to S5 msg
  // Mandatory imsi
  imsi_t imsi = {};
  if (msg.gtp_ies.get(imsi)) s5s8_csr->gtp_ies.set(imsi);
  // The IE shall be included for the case of a UE Requested PDN Connectivity,
  // if the MME has it stored for that UE. It shall be included when used on the
  // S5/S8 interfaces if provided by the MME/SGSN:
  msisdn_t msisdn = {};
  if (msg.gtp_ies.get(msisdn)) s5s8_csr->gtp_ies.set(msisdn);
  // If the SGW receives this IE, it shall forward it to the PGW on the S5/S8
  // interface:
  mei_t mei = {};
  if (msg.gtp_ies.get(mei)) s5s8_csr->gtp_ies.set(mei);
  // The SGW shall include this IE on S5/S8 if it receives the ULI from
  // MME/SGSN.
  uli_t uli = {};
  if (msg.gtp_ies.get(uli)) s5s8_csr->gtp_ies.set(uli);
  // No Serving Network
  // Mandatory rat_type
  rat_type_t rat_type = {};
  if (msg.gtp_ies.get(rat_type)) s5s8_csr->gtp_ies.set(rat_type);
  // Conditional. tweak this later
  indication_t indication = {};
  if (msg.gtp_ies.get(indication)) s5s8_csr->gtp_ies.set(indication);
  // Mandatory
  s5s8_csr->gtp_ies.set_sender_fteid_for_cp(p->sgw_fteid_s5_s8_cp);
  // Conditional
  fteid_t fteid = {};
  if (msg.gtp_ies.get_pgw_s5s8_address_for_cp(fteid))
    s5s8_csr->gtp_ies.set_pgw_s5s8_address_for_cp(fteid);
  // Mandatory
  apn_t apn = {};
  if (msg.gtp_ies.get(apn)) s5s8_csr->gtp_ies.set(apn);
  // Conditional
  selection_mode_t selection_mode = {};
  if (msg.gtp_ies.get(selection_mode)) s5s8_csr->gtp_ies.set(selection_mode);
  // Conditional
  pdn_type_t pdn_type = {};
  if (msg.gtp_ies.get(pdn_type)) s5s8_csr->gtp_ies.set(pdn_type);
  // Conditional
  paa_t paa = {};
  if (msg.gtp_ies.get(paa)) s5s8_csr->gtp_ies.set(paa);
  // Conditional
  apn_restriction_t apn_restriction = {};
  if (msg.gtp_ies.get(apn_restriction)) s5s8_csr->gtp_ies.set(apn_restriction);
  // Conditional
  ambr_t ambr = {};
  if (msg.gtp_ies.get(ambr)) s5s8_csr->gtp_ies.set(ambr);
  // Conditional
  protocol_configuration_options_t protocol_configuration_options = {};
  if (msg.gtp_ies.get(protocol_configuration_options))
    s5s8_csr->gtp_ies.set(protocol_configuration_options);
  if (msg.gtp_ies.has_bearer_context_to_be_created()) {
    for (auto i : msg.gtp_ies.bearer_contexts_to_be_created) {
      bearer_context_to_be_created_within_create_session_request b = {};
      ebi_t ebi                                                    = {};
      if (i.get(ebi)) b.set(ebi);
      bearer_qos_t bearer_qos = {};
      if (i.get(bearer_qos)) b.set(bearer_qos);
      // get_s5_s8_u_sgw_fteid
      // Wrong FTEID
      fteid_t s5s8_up_fteid = {};
      if (i.get(s5s8_up_fteid)) b.set_s5_s8_u_sgw_fteid(s5s8_up_fteid);
      ;
      s5s8_csr->gtp_ies.add_bearer_context_to_be_created(b);

      ebi_t cebi                 = {.ebi = ebi};
      sgw_eps_bearer* eps_bearer = new sgw_eps_bearer();
      eps_bearer->ebi            = cebi;
      // eps_bearer->sgw_fteid_s5_s8_up = s5s8_up_fteid;
      eps_bearer->eps_bearer_qos = bearer_qos;
      spc->add_eps_bearer(std::shared_ptr<sgw_eps_bearer>(eps_bearer));
    }
  }
  if (msg.gtp_ies.has_bearer_context_to_be_removed()) {
    for (auto i : msg.gtp_ies.bearer_contexts_to_be_removed) {
      bearer_context_to_be_removed_within_create_session_request b = {};
      ebi_t ebi                                                    = {};
      if (i.get(ebi)) b.set(ebi);
      s5s8_csr->gtp_ies.add_bearer_context_to_be_removed(b);

      ebi_t cebi                          = {.ebi = ebi};
      std::shared_ptr<sgw_eps_bearer> seb = {};
      if (spc->get_eps_bearer(cebi, seb)) {
        seb->deallocate_ressources();
        // TODO check when have to remove the bearer
        spc->remove_eps_bearer(seb);
      }
    }
  }

  // s5s8_csr->gtp_ies = msg.gtp_ies;
  // s5s8_csr->l_endpoint = {};
  // TODO PGW address in HSS
  s5s8_csr->r_endpoint = endpoint(
      pgwc::pgw_config::pgw_s5s8_.iface.addr4, pgwc::pgw_config::gtpv2c_.port);

  std::shared_ptr<itti_s5s8_create_session_request> msg =
      std::shared_ptr<itti_s5s8_create_session_request>(s5s8_csr);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::sgwc_app().error(
        "Could not send ITTI message %s to task TASK_SGWC_S5S8",
        s5s8_csr->get_msg_name());
    return RETURNerror;
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
void create_session_request_procedure::handle_itti_msg(
    itti_s5s8_create_session_response& csresp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection> pdn) {
  marked_for_removal = true;
  // TODO Get PDN connection and fill the field
  if (nullptr == pdn.get()) {
    Logger::sgwc_app().error(
        "create_session_request_procedure handling CREATE_SESSION_RESPONSE, "
        "Could not get sgw_pdn_connection object, discarding");
    return;
  }

  itti_s11_create_session_response* s11_csresp =
      new itti_s11_create_session_response(TASK_SGWC_APP, TASK_SGWC_S11);
  s11_csresp->gtpc_tx_id = get_trxn_id();
  s11_csresp->r_endpoint = this->msg.r_endpoint;
  s11_csresp->teid       = this->msg.gtp_ies.sender_fteid_for_cp.teid_gre_key;

  // transfer IEs from S5 msg to S11 msg
  // Mandatory imsi
  cause_t cause = {};
  if (csresp.gtp_ies.get(cause)) s11_csresp->gtp_ies.set(cause);

  csresp.gtp_ies.get_s5_s8_pgw_fteid(pdn->pgw_fteid_s5_s8_cp);

  s11_csresp->gtp_ies.set_sender_fteid_for_cp(ebc->sgw_fteid_s11_s4_cp);

  paa_t paa = {};
  if (csresp.gtp_ies.get(paa)) s11_csresp->gtp_ies.set(paa);
  apn_restriction_t apn_restriction = {};
  if (csresp.gtp_ies.get(apn_restriction))
    s11_csresp->gtp_ies.set(apn_restriction);
  ambr_t ambr = {};
  if (csresp.gtp_ies.get(ambr)) s11_csresp->gtp_ies.set(ambr);
  // ebi_t ebi = {}; if (msg.gtp_ies.get(ebi)) s11_csresp->gtp_ies.set(ebi);
  protocol_configuration_options_t pco = {};
  if (csresp.gtp_ies.get(pco)) s11_csresp->gtp_ies.set(pco);

  fq_csid_t fq_csid = {};
  if (msg.gtp_ies.get(fq_csid, 0)) s11_csresp->gtp_ies.set(fq_csid, 0);
  // TODO FQCSID instance 1
  //  local_distinguished_name_t ldn = {}; if (msg.gtp_ies.get(ldn,1)) ;
  epc_timer_t pgw_back_off_time = {};
  if (csresp.gtp_ies.get(pgw_back_off_time))
    s11_csresp->gtp_ies.set(pgw_back_off_time);
  indication_t indication = {};
  if (csresp.gtp_ies.get(indication)) s11_csresp->gtp_ies.set(indication);

  if (csresp.gtp_ies.has_bearer_context_created()) {
    for (auto i : csresp.gtp_ies.bearer_contexts_created.second) {
      bearer_context_created_within_create_session_response b = {};
      cause_t cause                                           = {};
      if (i.get(cause)) b.set(cause);
      ebi_t ebi = {};
      if (i.get(ebi)) b.set(ebi);
      if (cause.cause_value == REQUEST_ACCEPTED) {
        ebi_t cebi                          = {.ebi = ebi};
        std::shared_ptr<sgw_eps_bearer> seb = {};
        if (pdn->get_eps_bearer(cebi, seb)) {
          seb->ebi = cebi;
          i.get(seb->eps_bearer_qos);
          i.get(seb->pgw_fteid_s5_s8_up, 2);

#define SPGW_PLIT 0
#if !SPGW_SPLIT
          seb->sgw_fteid_s1u_s12_s4u_s11u = seb->pgw_fteid_s5_s8_up;
          seb->sgw_fteid_s1u_s12_s4u_s11u.interface_type = S1_U_SGW_GTP_U;
#else
          // TODO
#endif
          if (not seb->sgw_fteid_s1u_s12_s4u_s11u.is_zero())
            b.set_s1_u_sgw_fteid(seb->sgw_fteid_s1u_s12_s4u_s11u);
          bearer_qos_t bearer_qos = {};
          if (i.get(bearer_qos)) {
            b.set(bearer_qos);
          }
        }
      }
      s11_csresp->gtp_ies.add_bearer_context_created(b);
    }
  }
  if (csresp.gtp_ies.has_bearer_context_marked_for_removal()) {
    for (auto i : csresp.gtp_ies.bearer_contexts_marked_for_removal.second) {
      bearer_context_marked_for_removal_within_create_session_response b = {};
      cause_t cause                                                      = {};
      if (i.get(cause)) b.set(cause);
      ebi_t ebi = {};
      if (i.get(ebi)) b.set(ebi);
      s11_csresp->gtp_ies.add_bearer_context_marked_for_removal(b);

      ebi_t cebi                          = {.ebi = ebi};
      std::shared_ptr<sgw_eps_bearer> seb = {};
      if (pdn->get_eps_bearer(cebi, seb)) {
        if (seb.get()) {
          seb->clear();
        }
      }
    }
  } else if (csresp.gtp_ies.cause.second.cause_value >= CONTEXT_NOT_FOUND) {
    if (pdn->get_num_bearers()) {
      ebc->delete_pdn_connection(pdn);
    }
  }

  std::shared_ptr<itti_s11_create_session_response> msg_send =
      std::shared_ptr<itti_s11_create_session_response>(s11_csresp);
  int ret = itti_inst->send_msg(msg_send);
  if (RETURNok != ret) {
    Logger::sgwc_app().error(
        "Could not send ITTI message %s to task TASK_SGW_11",
        s11_csresp->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void create_session_request_procedure::handle_itti_msg(
    itti_s5s8_remote_peer_not_responding& resp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection> pdn) {
  marked_for_removal = true;

  if (pdn->get_num_bearers()) {
    ebc->delete_pdn_connection(pdn);
  }

  itti_s11_create_session_response* s11_csresp =
      new itti_s11_create_session_response(TASK_SGWC_APP, TASK_SGWC_S11);
  s11_csresp->gtpc_tx_id = get_trxn_id();
  s11_csresp->r_endpoint = this->msg.r_endpoint;
  s11_csresp->teid       = this->msg.gtp_ies.sender_fteid_for_cp.teid_gre_key;
  s11_csresp->l_teid     = resp.l_teid;

  cause_t cause = {.cause_value           = REMOTE_PEER_NOT_RESPONDING,
                   .pce                   = 0,
                   .bce                   = 0,
                   .cs                    = 1,
                   .offending_ie_instance = 0,
                   .filler                = 0,
                   .offending_ie_type     = 0,
                   .offending_ie_length   = 0};
  s11_csresp->gtp_ies.set(cause);

  s11_csresp->gtp_ies.set_sender_fteid_for_cp(ebc->sgw_fteid_s11_s4_cp);

  std::shared_ptr<itti_s11_create_session_response> msg_send =
      std::shared_ptr<itti_s11_create_session_response>(s11_csresp);
  int ret = itti_inst->send_msg(msg_send);
  if (RETURNok != ret) {
    Logger::sgwc_app().error(
        "Could not send ITTI message %s to task TASK_SGW_11",
        s11_csresp->get_msg_name());
  }
}

//------------------------------------------------------------------------------
int delete_session_request_procedure::run(
    shared_ptr<sgw_eps_bearer_context> c) {
  if (nullptr == c.get()) {
    return RETURNerror;
  } else {
    ebc = c;

    indication_t indication = {};
    bool oi_set             = false;
    if (msg.gtp_ies.get(indication)) {
      if (indication.oi) {
        oi_set = true;
      }
    }

    // Forward to P-GW (temp use ITTI instead of ITTI/GTPv2-C/UDP)
    itti_s5s8_delete_session_request* s5s8_dsr =
        new itti_s5s8_delete_session_request(TASK_SGWC_APP, TASK_SGWC_S5S8);
    s5s8_dsr->gtpc_tx_id = get_trxn_id();
    s5s8_dsr->teid       = pdn_connection->pgw_fteid_s5_s8_cp.teid_gre_key;
    s5s8_dsr->l_teid     = pdn_connection->sgw_fteid_s5_s8_cp.teid_gre_key;
    s5s8_dsr->r_endpoint = endpoint(
        pgwc::pgw_config::pgw_s5s8_.iface.addr4,
        pgwc::pgw_config::gtpv2c_.port);

    // transfer IEs from S11 msg to S5 msg
    // The SGW shall include this IE on S5/S8 if it receives the Cause from the
    // MME/SGSN.
    cause_t cause = {};
    if (msg.gtp_ies.get(cause)) s5s8_dsr->gtp_ies.set(cause);

    // This IE shall be included on the S4/S11, S5/S8 and S2a/S2b interfaces to
    // indicate the default bearer associated with the PDN being disconnected
    // unless in the handover/TAU/RAU with SGW relocation procedures.
    ebi_t ebi = {};
    if (msg.gtp_ies.get(ebi)) {
      s5s8_dsr->gtp_ies.set(ebi);
    } else {
      s5s8_dsr->gtp_ies.set(pdn_connection->default_bearer);
    }
    // The MME/SGSN shall include this IE on the S4/S11 interface for the Detach
    // procedure. The MME shall include ECGI, SGSN shall include CGI/SAI. The
    // SGW shall include this IE on S5/S8 if it receives the ULI from MME/SGSN.
    uli_t uli = {};
    if (msg.gtp_ies.get(uli)) s5s8_dsr->gtp_ies.set(uli);

    // If the UE includes the PCO IE, then the MME/SGSN shall Configuration
    // Options copy the content of this IE transparently from the PCO IE (PCO)
    // included by the UE. If SGW receives the PCO IE, SGW shall forward it to
    // PGW.
    protocol_configuration_options_t pco = {};
    if (msg.gtp_ies.get(pco)) s5s8_dsr->gtp_ies.set(pco);

    // C This IE may be included on the S5/S8 and S2a/S2b interfaces.
    // If the Sender F-TEID for Control Plane is received by the PGW, the PGW
    // shall only accept the Delete Session Request message when the Sender
    // F-TEID for Control Plane in this message is the same as the Sender F-TEID
    // for Control Plane that was last received in either the Create Session
    // Request message or the Modify Bearer Request message on the given
    // interface. See NOTE 6. CO The SGW shall include this IE on the S5/S8
    // interface if the Delete Session Request is sent to clean up a hanging PDN
    // connection context in the PGW, i.e. as a result of receiving a Create
    // Session Request at the SGW colliding with an existing PDN connection
    // context (see subclause 7.2.1).
    s5s8_dsr->gtp_ies.set_sender_fteid_for_cp(
        pdn_connection->sgw_fteid_s5_s8_cp);

    if (oi_set) {
      // The SGW shall forward this IE on the S5/S8 interface if the SGW
      // receives it from the MME/SGSN, and if the Operation Indication bit
      // received from the MME/SGSN is set to 1.
      ue_time_zone_t ue_time_zone = {};
      if (msg.gtp_ies.get(ue_time_zone)) s5s8_dsr->gtp_ies.set(ue_time_zone);

      // The MME shall include this IE on the S11 interface to indicate the NAS
      // release cause to release the PDN connection, if available and this
      // information is permitted to be sent to the PGW operator according to
      // MME operator's policy. The SGW shall include this IE on the S5/S8
      // interface if it receives it from the MME and if the Operation
      // Indication bit received from the MME is set to 1.
      ran_nas_cause_t ran_nas_cause = {};
      if (msg.gtp_ies.get(ran_nas_cause)) s5s8_dsr->gtp_ies.set(ran_nas_cause);
    }

    // If the UE includes the ePCO IE, then the MME shall copy the content of
    // this IE transparently from the ePCO IE included by the UE. If the SGW
    // receives the ePCO IE, the SGW shall forward it to the PGW.
    extended_protocol_configuration_options_t epco = {};
    if (msg.gtp_ies.get(epco)) s5s8_dsr->gtp_ies.set(epco);

    std::shared_ptr<itti_s5s8_delete_session_request> msg =
        std::shared_ptr<itti_s5s8_delete_session_request>(s5s8_dsr);
    int ret = itti_inst->send_msg(msg);
    if (RETURNok != ret) {
      Logger::sgwc_app().error(
          "Could not send ITTI message %s to task TASK_SGWC_S5S8",
          s5s8_dsr->get_msg_name());
      return RETURNerror;
    }
    return RETURNok;
  }
}
//------------------------------------------------------------------------------
void delete_session_request_procedure::handle_itti_msg(
    itti_s5s8_delete_session_response& dsresp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection>& pdn) {
  itti_s11_delete_session_response* s11_dsresp =
      new itti_s11_delete_session_response(TASK_SGWC_APP, TASK_SGWC_S11);
  s11_dsresp->gtpc_tx_id = get_trxn_id();
  s11_dsresp->r_endpoint = this->msg.r_endpoint;
  s11_dsresp->teid       = this->ebc->mme_fteid_s11.teid_gre_key;

  marked_for_removal = true;

  // transfer IEs from S5 msg to S11 msg
  cause_t cause = {};
  if (dsresp.gtp_ies.get(cause)) {
    s11_dsresp->gtp_ies.set(cause);
  } else {
    Logger::sgwc_app().error(
        "Could not get CAUSE in S5S8 DELETE_SESSION_RESPONSE");
  }

  // TODO Get PDN connection and fill the field
  if (nullptr == pdn.get()) {
    Logger::sgwc_app().error("Could not get sgw_pdn_connection object");
    return;
  }
  // Delete PDN connection even if s5s8 cause is not success
  ebc->delete_pdn_connection(pdn);

  std::shared_ptr<itti_s11_delete_session_response> msg =
      std::shared_ptr<itti_s11_delete_session_response>(s11_dsresp);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::sgwc_app().error(
        "Could not send ITTI message %s to task TASK_SGWC_S11",
        s11_dsresp->get_msg_name());
  }
}
//------------------------------------------------------------------------------
int modify_bearer_request_procedure::run(shared_ptr<sgw_eps_bearer_context> c) {
  // Since SGW is not completely split with PGW, we have to always fw this req
  // to PGW
  if (nullptr == c.get()) {
    return RETURNerror;
  } else {
    ebc               = c;
    bool report_error = false;

    if (msg.gtp_ies.has_bearer_context_to_be_modified()) {
      for (auto it : msg.gtp_ies.bearer_contexts_to_be_modified) {
        std::shared_ptr<sgw_pdn_connection> pdn = {};
        std::shared_ptr<sgw_eps_bearer> sb      = {};
        // bearer_context_to_be_modified_within_modify_bearer_request
        if (not ebc->find_pdn_connection(it.eps_bearer_id, pdn)) {
          bearer_context_modified_within_modify_bearer_response bm = {};
          bm.set(it.eps_bearer_id);
          cause_t cause     = {};
          cause.cause_value = CONTEXT_NOT_FOUND;
          bm.set(cause);
          bearer_contexts_modified.push_back(bm);
        } else {
          // not necessary now, but should be later if S/P split
          bearer_context_to_be_modified_within_modify_bearer_request bm(it);

          bool pdn_registered = false;
          // Should always be true since find_pdn_connection() returne true
          fteid_t v = {};
          if (it.get_s1_u_enb_fteid(v)) {
            if (pdn->sgw_pdn_connection::get_eps_bearer(it.eps_bearer_id, sb)) {
              if (v == sb->enb_fteid_s1u) {
                Logger::sgwc_app().debug(
                    "modify_bearer_procedure: ebi %d enb_fteid_s1u unchanged",
                    it.eps_bearer_id.ebi);
                bearer_context_modified_within_modify_bearer_response bm = {};
                bm.set(it.eps_bearer_id);
                cause_t cause     = {};
                cause.cause_value = REQUEST_ACCEPTED;
                bm.set(cause);
                bearer_contexts_modified.push_back(bm);
              } else {
                for (std::vector<std::shared_ptr<pdn_bearers_to_be_xied>>::
                         iterator it_pdns = pdn_bearers.begin();
                     it_pdns != pdn_bearers.end(); ++it_pdns) {
                  pdn_bearers_to_be_xied* px = it_pdns->get();
                  if (px->pdn == pdn) {
                    pdn_registered = true;
                    px->bearer_contexts_to_be_modified.push_back(bm);
                  }
                }

                if (not pdn_registered) {
                  pdn_bearers_to_be_xied* px = new (pdn_bearers_to_be_xied);
                  px->pdn                    = pdn;
                  px->bearer_contexts_to_be_modified.push_back(bm);
                  pdn_bearers.push_back(
                      std::shared_ptr<pdn_bearers_to_be_xied>(px));
                }
              }
            }
          }
        }
      }
    }
    if (msg.gtp_ies.has_bearer_context_to_be_removed()) {
      for (auto it : msg.gtp_ies.bearer_contexts_to_be_removed) {
        std::shared_ptr<sgw_pdn_connection> pdn = {};
        std::shared_ptr<sgw_eps_bearer> sb      = {};
        if (not c->find_pdn_connection(it.eps_bearer_id, pdn)) {
          bearer_context_marked_for_removal_within_modify_bearer_response bm =
              {};
          bm.set(it.eps_bearer_id);
          cause_t cause     = {};
          cause.cause_value = CONTEXT_NOT_FOUND;
          bm.set(cause);
          bearer_contexts_marked_for_removal.push_back(bm);
        } else {
          // not necessary now, but should be later if S/P split
          bearer_context_to_be_removed_within_modify_bearer_request bm(it);

          bool pdn_registered = false;
          for (std::vector<std::shared_ptr<pdn_bearers_to_be_xied>>::iterator
                   it_pdns = pdn_bearers.begin();
               it_pdns != pdn_bearers.end(); ++it_pdns) {
            pdn_bearers_to_be_xied* px = it_pdns->get();
            if (px->pdn == pdn) {
              pdn_registered = true;
              // pdn_bearers.erase(it_pdns);
              px->bearer_contexts_to_be_removed.push_back(bm);
              break;
            }
          }

          if (not pdn_registered) {
            pdn_bearers_to_be_xied* px = new (pdn_bearers_to_be_xied);
            px->pdn                    = pdn;
            px->bearer_contexts_to_be_removed.push_back(bm);
            pdn_bearers.push_back(std::shared_ptr<pdn_bearers_to_be_xied>(px));
          }
        }
      }
    }

    if (pdn_bearers.empty()) {
      itti_s11_modify_bearer_response* s11_mbresp =
          new itti_s11_modify_bearer_response(TASK_SGWC_APP, TASK_SGWC_S11);
      s11_mbresp->gtpc_tx_id = get_trxn_id();
      s11_mbresp->r_endpoint = this->msg.r_endpoint;
      s11_mbresp->teid       = ebc->mme_fteid_s11.teid_gre_key;

      cause_t cause     = {};
      cause.cause_value = CONTEXT_NOT_FOUND;
      s11_mbresp->gtp_ies.set(cause);

      for (auto it_mod : bearer_contexts_modified) {
        s11_mbresp->gtp_ies.add_bearer_context_modified(it_mod);
      }
      for (auto it_rem : bearer_contexts_marked_for_removal) {
        s11_mbresp->gtp_ies.add_bearer_context_marked_for_removal(it_rem);
      }

      std::shared_ptr<itti_s11_modify_bearer_response> msg_send =
          std::shared_ptr<itti_s11_modify_bearer_response>(s11_mbresp);
      int ret = itti_inst->send_msg(msg_send);
      if (RETURNok != ret) {
        Logger::sgwc_app().error(
            "Could not send ITTI message %s to task TASK_SGWC_S11",
            s11_mbresp->get_msg_name());
        return RETURNerror;
      }
      return RETURNclear;
    } else {
      // Now we can send X modify bearer request to X GTPV2-C tunnels
      for (auto it_pdns : pdn_bearers) {
        pdn_bearers_to_be_xied* px = it_pdns.get();

        itti_s5s8_modify_bearer_request* s5s8_mbr =
            new itti_s5s8_modify_bearer_request(TASK_SGWC_APP, TASK_SGWC_S5S8);
        std::shared_ptr<itti_s5s8_modify_bearer_request> msg_s5s8 =
            std::shared_ptr<itti_s5s8_modify_bearer_request>(s5s8_mbr);
        // New gtpc_tx_id
        px->gtpc_tx_id =
            util::uint_uid_generator<uint64_t>::get_instance().get_uid();
        s5s8_mbr->gtpc_tx_id = px->gtpc_tx_id;
        s5s8_mbr->teid       = px->pdn->pgw_fteid_s5_s8_cp.teid_gre_key;
        s5s8_mbr->l_teid     = px->pdn->sgw_fteid_s5_s8_cp.teid_gre_key;
        s5s8_mbr->r_endpoint = endpoint(
            pgwc::pgw_config::pgw_s5s8_.iface.addr4,
            pgwc::pgw_config::gtpv2c_.port);

        mei_t mei;
        if (msg.gtp_ies.get(mei)) {
          msg_s5s8->gtp_ies.set(mei);
        }

        serving_network_t serving_network;
        if (msg.gtp_ies.get(serving_network)) {
          msg_s5s8->gtp_ies.set(serving_network);
        }

        ue_time_zone_t ue_time_zone;
        if (msg.gtp_ies.get(ue_time_zone)) {
          s5s8_mbr->gtp_ies.set(ue_time_zone);
        }

        for (auto it_mod : px->bearer_contexts_to_be_modified) {
          std::shared_ptr<sgw_eps_bearer> b = {};
          if (px->pdn->get_eps_bearer(it_mod.eps_bearer_id, b)) {
            if (it_mod.get_s1_u_enb_fteid(b->enb_fteid_s1u)) {
              s5s8_mbr->gtp_ies.add_bearer_context_to_be_modified(it_mod);
            }
          }
        }
        for (auto it_rem : px->bearer_contexts_to_be_removed) {
          s5s8_mbr->gtp_ies.add_bearer_context_to_be_removed(it_rem);
        }
        int ret = itti_inst->send_msg(msg_s5s8);
        if (RETURNok != ret) {
          Logger::sgwc_app().error(
              "Could not send ITTI message %s to task TASK_SGWC_S5S8",
              s5s8_mbr->get_msg_name());
        }
      }
    }
    return RETURNok;
  }
}
//------------------------------------------------------------------------------
void modify_bearer_request_procedure::handle_itti_msg(
    itti_s5s8_modify_bearer_response& s5resp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection> pdn) {
  itti_s11_modify_bearer_response* s11_resp =
      new itti_s11_modify_bearer_response(TASK_SGWC_APP, TASK_SGWC_S11);
  s11_resp->gtpc_tx_id = get_trxn_id();
  s11_resp->r_endpoint = this->msg.r_endpoint;
  s11_resp->teid       = this->ebc->mme_fteid_s11.teid_gre_key;

  // transfer IEs from S5 msg to S11 msg
  cause_t cause = {};
  if (s5resp.gtp_ies.get(cause)) {
    s11_resp->gtp_ies.set(cause);
  } else {
    Logger::sgwc_app().error(
        "Could not get CAUSE in S5S8 MODIFY_BEARER_RESPONSE");
    marked_for_removal = true;
  }

  // TODO Get PDN connection and fill the field
  if (nullptr == pdn.get()) {
    Logger::sgwc_app().error("Could not get sgw_pdn_connection object");
    return;
    marked_for_removal = true;
  }

  for (std::vector<std::shared_ptr<pdn_bearers_to_be_xied>>::iterator
           it_bearer = pdn_bearers.begin();
       it_bearer != pdn_bearers.end(); ++it_bearer) {
    pdn_bearers_to_be_xied* px = it_bearer->get();
    if (px->gtpc_tx_id == s5resp.gtpc_tx_id) {
      if (s5resp.gtp_ies.has_bearer_context_modified()) {
        for (auto it_modified :
             s5resp.gtp_ies.bearer_contexts_modified.second) {
          for (
              std::vector<
                  gtpv2c::
                      bearer_context_to_be_modified_within_modify_bearer_request>::
                  iterator it_to_be_modified =
                      px->bearer_contexts_to_be_modified.begin();
              it_to_be_modified != px->bearer_contexts_to_be_modified.end();
              ++it_to_be_modified) {
            if (it_to_be_modified->eps_bearer_id ==
                it_modified.eps_bearer_id.second) {
#define SPGW_PLIT 0
#if !SPGW_SPLIT
              it_modified.s1_u_sgw_fteid.second.interface_type = S1_U_SGW_GTP_U;
#endif
              bearer_contexts_modified.push_back(it_modified);
              px->bearer_contexts_to_be_modified.erase(it_to_be_modified);
              break;
            }
          }
        }
      }
      if (s5resp.gtp_ies.has_bearer_context_marked_for_removal()) {
        for (auto it_marked :
             s5resp.gtp_ies.bearer_contexts_marked_for_removal.second) {
          for (
              std::vector<
                  gtpv2c::
                      bearer_context_to_be_removed_within_modify_bearer_request>::
                  iterator it_to_be_removed =
                      px->bearer_contexts_to_be_removed.begin();
              it_to_be_removed != px->bearer_contexts_to_be_removed.end();
              ++it_to_be_removed) {
            if (it_to_be_removed->eps_bearer_id ==
                it_marked.eps_bearer_id.second) {
              bearer_contexts_marked_for_removal.push_back(it_marked);
              px->bearer_contexts_to_be_removed.erase(it_to_be_removed);
              break;
            }
          }
        }
      }
      std::shared_ptr<sgw_pdn_connection> pdn = px->pdn;
      pdn_bearers.erase(it_bearer);
      if (pdn_bearers.empty()) {
        // Send modify bearer response

        cause_t global_cause     = {};
        global_cause.cause_value = SYSTEM_FAILURE;

        marked_for_removal = true;

        for (auto it_to_be_modified :
             msg.gtp_ies.bearer_contexts_to_be_modified) {
          bool bearer_found = false;
          for (auto it_modified : bearer_contexts_modified) {
            if (it_modified.eps_bearer_id.second ==
                it_to_be_modified.eps_bearer_id) {
              pdn->update_eps_bearer(it_modified);
              s11_resp->gtp_ies.add_bearer_context_modified(it_modified);
              bearer_found = true;
              if (global_cause.cause_value != REQUEST_ACCEPTED_PARTIALLY) {
                global_cause.cause_value = REQUEST_ACCEPTED;
              }
              break;
            }
          }
          if (not bearer_found) {
            bearer_context_modified_within_modify_bearer_response b = {};
            b.set(it_to_be_modified.eps_bearer_id);
            cause             = {};
            cause.cause_value = CONTEXT_NOT_FOUND;
            b.set(cause);
            s11_resp->gtp_ies.add_bearer_context_modified(b);
            if (global_cause.cause_value == REQUEST_ACCEPTED) {
              global_cause.cause_value = REQUEST_ACCEPTED_PARTIALLY;
            }
          }
        }

        for (auto it_to_be_removed :
             msg.gtp_ies.bearer_contexts_to_be_removed) {
          bool bearer_found = false;
          for (auto it_marked : bearer_contexts_marked_for_removal) {
            if (it_marked.eps_bearer_id.second ==
                it_to_be_removed.eps_bearer_id) {
              pdn->remove_eps_bearer(it_marked.eps_bearer_id.second);
              s11_resp->gtp_ies.add_bearer_context_marked_for_removal(
                  it_marked);
              bearer_found = true;
              if (global_cause.cause_value != REQUEST_ACCEPTED_PARTIALLY) {
                global_cause.cause_value = REQUEST_ACCEPTED;
              }
              break;
            }
          }
          if (not bearer_found) {
            bearer_context_marked_for_removal_within_modify_bearer_response b =
                {};
            b.set(it_to_be_removed.eps_bearer_id);
            cause             = {};
            cause.cause_value = CONTEXT_NOT_FOUND;
            b.set(cause);
            s11_resp->gtp_ies.add_bearer_context_marked_for_removal(b);
          }
        }
        s11_resp->gtp_ies.set(global_cause);

        std::shared_ptr<itti_s11_modify_bearer_response> msg =
            std::shared_ptr<itti_s11_modify_bearer_response>(s11_resp);
        int ret = itti_inst->send_msg(msg);
        if (RETURNok != ret) {
          Logger::sgwc_app().error(
              "Could not send ITTI message %s to task TASK_SGWC_S11",
              s11_resp->get_msg_name());
        }
      }
      break;
    }
  }
}
//------------------------------------------------------------------------------
bool modify_bearer_request_procedure::has_trxn_id(const uint64_t trxn_id) {
  if (sebc_procedure::has_trxn_id(trxn_id)) {
    // Logger::sgwc_app().error(
    // "modify_bearer_request_procedure::has_trxn_id(%d) -> true", trxn_id);
    return true;
  }
  for (auto it_pdns : pdn_bearers) {
    // Logger::sgwc_app().error(
    // "modify_bearer_request_procedure::has_trxn_id(%d) -> check with %d",
    // trxn_id, it_pdns->gtpc_tx_id);
    if (it_pdns->gtpc_tx_id == trxn_id) {
      // Logger::sgwc_app().error(
      // "modify_bearer_request_procedure::has_trxn_id(%d) -> true", trxn_id);
      return true;
    }
  }
  // Logger::sgwc_app().error( "modify_bearer_request_procedure::has_trxn_id(%d)
  // -> false (is %d)", trxn_id, get_trxn_id());
  return false;
}
//------------------------------------------------------------------------------
bool release_access_bearers_request_procedure::has_trxn_id(
    const uint64_t trxn_id) {
  if (sebc_procedure::has_trxn_id(trxn_id)) {
    // Logger::sgwc_app().error(
    // "release_access_bearers_request_procedure::has_trxn_id(%d) -> true",
    // trxn_id);
    return true;
  }
  for (auto it : bearers) {
    // Logger::sgwc_app().error(
    // "release_access_bearers_request_procedure::has_trxn_id(%d) -> check with
    // %d", trxn_id, it_pdns->gtpc_tx_id);
    if (it->gtpc_tx_id == trxn_id) {
      // Logger::sgwc_app().error(
      // "release_access_bearers_request_procedure::has_trxn_id(%d) -> true",
      // trxn_id);
      return true;
    }
  }
  // Logger::sgwc_app().error(
  // "release_access_bearers_request_procedure::has_trxn_id(%d) -> false (is
  // %d)", trxn_id, get_trxn_id());
  return false;
}
//------------------------------------------------------------------------------
int release_access_bearers_request_procedure::run(
    shared_ptr<sgw_eps_bearer_context> c) {
  // Since SGW is not completely split with PGW, we have to always fw this req
  // to PGW
  if (nullptr == c.get()) {
    Logger::sgwc_app().error(
        "release_access_bearers_request_procedure Could not get "
        "sgw_eps_bearer_context");
    return RETURNerror;
  } else {
    ebc = c;

    for (std::map<kpdn_t, std::shared_ptr<sgw_pdn_connection>>::const_iterator
             it_pdn = ebc->pdn_connections.begin();
         it_pdn != ebc->pdn_connections.end(); ++it_pdn) {
      if (not it_pdn->second->is_released()) {
        bearers_to_be_released* breal = new bearers_to_be_released();

        breal->pdn        = it_pdn->second;
        breal->gtpc_tx_id = get_trxn_id();
        bearers.push_back(std::shared_ptr<bearers_to_be_released>(breal));

        itti_s5s8_release_access_bearers_request* s5s8 =
            new itti_s5s8_release_access_bearers_request(
                TASK_SGWC_APP, TASK_SGWC_S5S8);
        s5s8->gtpc_tx_id = breal->gtpc_tx_id;
        s5s8->teid       = it_pdn->second->pgw_fteid_s5_s8_cp.teid_gre_key;
        s5s8->r_endpoint = endpoint(
            it_pdn->second->pgw_fteid_s5_s8_cp.ipv4_address,
            pgwc::pgw_config::gtpv2c_.port);

        std::shared_ptr<itti_s5s8_release_access_bearers_request> msg =
            std::shared_ptr<itti_s5s8_release_access_bearers_request>(s5s8);
        // breal->msg = msg;

        int ret = itti_inst->send_msg(msg);
        if (RETURNok != ret) {
          Logger::sgwc_app().error(
              "Could not send ITTI message %s to task TASK_SGWC_S5S8",
              s5s8->get_msg_name());
        }
      }
    }

    if (bearers.empty()) {
      Logger::sgwc_app().error(
          "release_access_bearers_request_procedure all pdn connections "
          "already released");
      itti_s11_release_access_bearers_response* s11 =
          new itti_s11_release_access_bearers_response(
              TASK_SGWC_APP, TASK_SGWC_S11);
      s11->gtpc_tx_id = get_trxn_id();
      s11->r_endpoint = this->msg.r_endpoint;
      s11->teid       = ebc->mme_fteid_s11.teid_gre_key;

      cause_t cause     = {};
      cause.cause_value = REQUEST_ACCEPTED;
      s11->gtp_ies.set(cause);

      std::shared_ptr<itti_s11_release_access_bearers_response> msg_send =
          std::shared_ptr<itti_s11_release_access_bearers_response>(s11);
      int ret = itti_inst->send_msg(msg_send);
      if (RETURNok != ret) {
        Logger::sgwc_app().error(
            "Could not send ITTI message %s to task TASK_SGWC_S11",
            s11->get_msg_name());
        return RETURNerror;
      }
      return RETURNclear;
    }
    return RETURNok;
  }
}
//------------------------------------------------------------------------------
void release_access_bearers_request_procedure::handle_itti_msg(
    itti_s5s8_release_access_bearers_response& s5resp,
    std::shared_ptr<sgw_eps_bearer_context> ebc,
    std::shared_ptr<sgw_pdn_connection> pdn) {
  itti_s11_release_access_bearers_response* s11_resp =
      new itti_s11_release_access_bearers_response(
          TASK_SGWC_APP, TASK_SGWC_S11);
  s11_resp->gtpc_tx_id = get_trxn_id();
  s11_resp->r_endpoint = this->msg.r_endpoint;
  s11_resp->teid       = this->ebc->mme_fteid_s11.teid_gre_key;

  // transfer IEs from S5 msg to S11 msg

  // TODO Get PDN connection and fill the field
  if (nullptr == pdn.get()) {
    Logger::sgwc_app().error("Could not get sgw_pdn_connection object");
    return;
  }

  for (std::vector<std::shared_ptr<bearers_to_be_released>>::iterator
           it_bearer = bearers.begin();
       it_bearer != bearers.end(); ++it_bearer) {
    bearers_to_be_released* px = it_bearer->get();
    if (px->gtpc_tx_id == s5resp.gtpc_tx_id) {
      if (cause.cause_value == 0) {
        if (not s5resp.gtp_ies.get(cause)) {
          Logger::sgwc_app().info(
              "Could not get CAUSE in S5S8 RELEASE_ACCESS_BEARERS_RESPONSE");
          // TODO ?
          cause.cause_value = REQUEST_ACCEPTED;
        }
      } else {
        cause_t get_cause = {};
        if (s5resp.gtp_ies.get(get_cause)) {
          switch (get_cause.cause_value) {
            case REQUEST_ACCEPTED:
              if (cause.cause_value == CONTEXT_NOT_FOUND) {
                cause.cause_value = REQUEST_ACCEPTED_PARTIALLY;
              }
              break;
            case REQUEST_ACCEPTED_PARTIALLY:
              cause.cause_value = REQUEST_ACCEPTED_PARTIALLY;
              break;

            case CONTEXT_NOT_FOUND:
            default:
              if (cause.cause_value == REQUEST_ACCEPTED) {
                cause.cause_value = REQUEST_ACCEPTED_PARTIALLY;
              }
              break;
          }
          s11_resp->gtp_ies.set(cause);
        } else {
          cause.cause_value = REQUEST_ACCEPTED;
        }
      }
      s11_resp->gtp_ies.set(cause);

      std::shared_ptr<sgw_pdn_connection> pdn = px->pdn;
      bearers.erase(it_bearer);
      if (bearers.empty()) {
        // Send  response
        std::shared_ptr<itti_s11_release_access_bearers_response> msg =
            std::shared_ptr<itti_s11_release_access_bearers_response>(s11_resp);
        int ret = itti_inst->send_msg(msg);
        if (RETURNok != ret) {
          Logger::sgwc_app().error(
              "Could not send ITTI message %s to task TASK_SGWC_S11",
              s11_resp->get_msg_name());
        }
      }
      break;
    }
  }
}

//------------------------------------------------------------------------------
int downlink_data_notification_procedure::run(
    std::shared_ptr<sgw_eps_bearer_context> sebc,
    std::shared_ptr<sgw_pdn_connection> pdn) {
  if ((nullptr == sebc.get()) || (nullptr == pdn.get())) {
    return RETURNerror;
  } else {
    ebc            = sebc;
    pdn_connection = pdn;

    ebi_t ebi = {};
    if (not(msg.gtp_ies.get(ebi))) {
      Logger::sgwc_app().error(
          "downlink_data_notification_procedure: Could not get ebi in %s",
          msg.get_msg_name());
      return RETURNerror;
    }
    std::shared_ptr<sgw_eps_bearer> b = {};
    if (not pdn_connection->get_eps_bearer(ebi, b)) {
      Logger::sgwc_app().error(
          "downlink_data_notification_procedure: Could not get EPS bearer "
          "context %d",
          ebi.ebi);
      return RETURNerror;
    }

    itti_s11_downlink_data_notification* s11 =
        new itti_s11_downlink_data_notification(
            msg.gtp_ies, TASK_SGWC_APP, TASK_SGWC_S11);

    s11->teid       = ebc->mme_fteid_s11.teid_gre_key;
    s11->gtpc_tx_id = get_trxn_id();
    s11->r_endpoint =
        endpoint(ebc->mme_fteid_s11.ipv4_address, gtpv2c::default_port);
    s11_triggered = std::shared_ptr<itti_s11_downlink_data_notification>(s11);

    Logger::sgwc_app().info(
        "Sending ITTI message %s to task TASK_SGWC_S11",
        s11->gtp_ies.get_msg_name());
    int ret = itti_inst->send_msg(s11_triggered);
    if (RETURNok != ret) {
      Logger::sgwc_app().error(
          "Could not send ITTI message %s to task TASK_SGWC_S11",
          s11->gtp_ies.get_msg_name());
      return RETURNerror;
    }
    return RETURNok;
  }
}
//------------------------------------------------------------------------------
void downlink_data_notification_procedure::handle_itti_msg(
    itti_s11_downlink_data_notification_acknowledge& s11resp) {
  itti_s5s8_downlink_data_notification_acknowledge* s5 =
      new itti_s5s8_downlink_data_notification_acknowledge(
          s11resp.gtp_ies, TASK_SGWC_APP, TASK_SGWC_S5S8);
  s5->teid       = pdn_connection->pgw_fteid_s5_s8_cp.teid_gre_key;
  s5->gtpc_tx_id = get_trxn_id();
  s5->r_endpoint = endpoint(
      pdn_connection->pgw_fteid_s5_s8_cp.ipv4_address, gtpv2c::default_port);
  std::shared_ptr<itti_s5s8_downlink_data_notification_acknowledge>
      s5_response =
          std::shared_ptr<itti_s5s8_downlink_data_notification_acknowledge>(s5);

  Logger::sgwc_app().info(
      "Sending ITTI message %s to task TASK_SGWC_S5S8",
      s5->gtp_ies.get_msg_name());
  int ret = itti_inst->send_msg(s5_response);
  if (RETURNok != ret) {
    Logger::sgwc_app().error(
        "Could not send ITTI message %s to task TASK_SGWC_S5S8",
        s5->gtp_ies.get_msg_name());
  }
}
