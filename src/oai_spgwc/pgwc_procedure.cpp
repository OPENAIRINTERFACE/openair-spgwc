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

#include "pgwc_procedure.hpp"
#include "3gpp_29.244.h"
#include "3gpp_29.274.h"
#include "3gpp_conversions.hpp"
#include "common_defs.h"
#include "conversions.hpp"
#include "itti.hpp"
#include "itti_msg_sx_restore.hpp"
#include "logger.hpp"
#include "msg_gtpv2c.hpp"
#include "pgw_app.hpp"
#include "pgw_config.hpp"
#include "pgw_context.hpp"
#include "pgw_pfcp_association.hpp"

#include <algorithm>  // std::search

using namespace pfcp;
using namespace pgwc;
using namespace std;

extern itti_mw* itti_inst;
extern pgwc::pgw_app* pgw_app_inst;
extern pgwc::pgw_config pgw_cfg;

//------------------------------------------------------------------------------
int sx_session_restore_procedure::run() {
  if (pending_sessions.size()) {
    itti_sx_restore* itti_msg = nullptr;
    for (std::set<pfcp::fseid_t>::iterator it = pending_sessions.begin();
         it != pending_sessions.end(); ++it) {
      if (!itti_msg) {
        itti_msg = new itti_sx_restore(TASK_PGWC_SX, TASK_PGWC_APP);
      }
      itti_msg->sessions.insert(*it);
      if (itti_msg->sessions.size() >= 64) {
        std::shared_ptr<itti_sx_restore> i =
            std::shared_ptr<itti_sx_restore>(itti_msg);
        int ret = itti_inst->send_msg(i);
        if (RETURNok != ret) {
          Logger::pgwc_sx().error(
              "Could not send ITTI message %s to task TASK_PGWC_APP",
              i->get_msg_name());
        }
        itti_msg = nullptr;
      }
    }
    if (itti_msg) {
      std::shared_ptr<itti_sx_restore> i =
          std::shared_ptr<itti_sx_restore>(itti_msg);
      int ret = itti_inst->send_msg(i);
      if (RETURNok != ret) {
        Logger::pgwc_sx().error(
            "Could not send ITTI message %s to task TASK_PGWC_APP",
            i->get_msg_name());
        return RETURNerror;
      }
    }
  }
  return RETURNok;
}
//------------------------------------------------------------------------------
int session_establishment_procedure::run(
    std::shared_ptr<itti_s5s8_create_session_request>& req,
    std::shared_ptr<itti_s5s8_create_session_response>& resp,
    std::shared_ptr<pgwc::pgw_context> pc) {
  // TODO check if compatible with ongoing procedures if any
  pfcp::node_id_t up_node_id = {};
  if (not pfcp_associations::get_instance().select_up_node(
          req->gtp_ies.apn, req->gtp_ies.uli, req->gtp_ies.serving_network,
          req->gtp_ies.rat_type, req->gtp_ies.pdn_type, req->gtp_ies.paa,
          up_node_id, kNodeSelectionCriteriaMinPfcpSessions)) {
    // TODO
    ::cause_t cause   = {};
    cause.pce         = 1;
    cause.cause_value = NO_RESOURCES_AVAILABLE;
    resp->gtp_ies.set(cause);
    return RETURNerror;
  }
  std::shared_ptr<pfcp_association> sa = {};
  if (not pfcp_associations::get_instance().get_association(up_node_id, sa)) {
    ::cause_t cause   = {};
    cause.pce         = 1;
    cause.cause_value = SYSTEM_FAILURE;
    resp->gtp_ies.set(cause);
    return RETURNerror;
  }
  //-------------------
  s5_trigger           = req;
  s5_triggered_pending = resp;
  ppc->generate_seid();
  itti_sxab_session_establishment_request* sx_ser =
      new itti_sxab_session_establishment_request(TASK_PGWC_APP, TASK_PGWC_SX);
  sx_ser->seid    = 0;
  sx_ser->trxn_id = this->trxn_id;

  sx_ser->r_endpoint = sa->remote_endpoint;
  // sx_ser->r_endpoint =
  //                   endpoint(up_node_id.u1.ipv4_address, pfcp::default_port);
  sx_triggered =
      std::shared_ptr<itti_sxab_session_establishment_request>(sx_ser);

  //-------------------
  // IE node_id_t
  //-------------------
  pfcp::node_id_t node_id = {};
  pgw_cfg.GetPfcpNodeId(node_id);
  sx_ser->pfcp_ies.set(node_id);

  //-------------------
  // IE fseid_t
  //-------------------
  pfcp::fseid_t cp_fseid = {};
  pgw_cfg.GetPfcpFseid(cp_fseid);
  cp_fseid.seid = ppc->seid;
  sx_ser->pfcp_ies.set(cp_fseid);

  for (auto it : s5_trigger->gtp_ies.bearer_contexts_to_be_created) {
    //*******************
    // UPLINK
    //*******************
    //-------------------
    // IE create_far
    //-------------------
    pfcp::create_far create_far                       = {};
    pfcp::far_id_t far_id                             = {};
    pfcp::apply_action_t apply_action                 = {};
    pfcp::forwarding_parameters forwarding_parameters = {};
    //    pfcp::duplicating_parameters      duplicating_parameters = {};
    //    pfcp::bar_id_t                    bar_id = {};

    // forwarding_parameters IEs
    pfcp::destination_interface_t destination_interface = {};
    // pfcp::network_instance_t          network_instance = {};
    // pfcp::redirect_information_t      redirect_information = {};
    // pfcp::outer_header_creation_t     outer_header_creation = {};
    // pfcp::transport_level_marking_t   transport_level_marking = {};
    // pfcp::forwarding_policy_t         forwarding_policy = {};
    // pfcp::header_enrichment_t         header_enrichment = {};
    // pfcp::traffic_endpoint_id_t       linked_traffic_endpoint_id_t = {};
    // pfcp::proxying_t                  proxying = {};

    ppc->generate_far_id(far_id);
    apply_action.forw = 1;

    destination_interface.interface_value =
        pfcp::INTERFACE_VALUE_CORE;  // ACCESS is for downlink, CORE for uplink
    forwarding_parameters.set(destination_interface);

    create_far.set(far_id);
    create_far.set(apply_action);
    create_far.set(forwarding_parameters);
    //-------------------
    // IE create_pdr
    //-------------------
    pfcp::create_pdr create_pdr                       = {};
    pfcp::pdr_id_t pdr_id                             = {};
    pfcp::precedence_t precedence                     = {};
    pfcp::pdi pdi                                     = {};
    pfcp::outer_header_removal_t outer_header_removal = {};
    //    pfcp::far_id_t                    far_id;
    //    pfcp::urr_id_t                    urr_id;
    //    pfcp::qer_id_t                    qer_id;
    //    pfcp::activate_predefined_rules_t activate_predefined_rules;
    // pdi IEs
    pfcp::source_interface_t source_interface = {};
    pfcp::fteid_t local_fteid                 = {};
    // pfcp::network_instance_t         network_instance = {};
    pfcp::ue_ip_address_t ue_ip_address = {};
    // pfcp::traffic_endpoint_id_t      traffic_endpoint_id = {};
    pfcp::sdf_filter_t sdf_filter         = {};
    pfcp::application_id_t application_id = {};
    // pfcp::ethernet_packet_filter     ethernet_packet_filter = {};
    pfcp::qfi_t qfi = {};
    // pfcp::framed_route_t             framed_route = {};
    // pfcp::framed_routing_t           framed_routing = {};
    // pfcp::framed_ipv6_route_t        framed_ipv6_route = {};
    source_interface.interface_value = pfcp::INTERFACE_VALUE_ACCESS;
    local_fteid.ch                   = 1;
    // local_fteid.chid = 1;
    xgpp_conv::paa_to_pfcp_ue_ip_address(
        s5_triggered_pending->gtp_ies.paa.second, ue_ip_address);

    // DOIT simple
    // shall uniquely identify the PDR among all the PDRs configured for that
    // PFCP session.
    ppc->generate_pdr_id(pdr_id);
    precedence.precedence = it.bearer_level_qos.pl;

    pdi.set(source_interface);
    pdi.set(local_fteid);
    pdi.set(ue_ip_address);

    outer_header_removal.outer_header_removal_description =
        OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4;

    create_pdr.set(pdr_id);
    create_pdr.set(precedence);
    create_pdr.set(pdi);
    create_pdr.set(outer_header_removal);
    create_pdr.set(far_id);

    //-------------------
    // ADD IEs to message
    //-------------------
    sx_ser->pfcp_ies.set(create_pdr);
    sx_ser->pfcp_ies.set(create_far);

    // Have to backup far id and pdr id
    pgw_eps_bearer b   = {};
    b.far_id_ul.first  = true;
    b.far_id_ul.second = far_id;
    b.pdr_id_ul        = pdr_id;
    b.ebi              = it.eps_bearer_id;
    pgw_eps_bearer b2  = b;
    ppc->add_eps_bearer(b2);
  }

  // for finding procedure when receiving response
  pgw_app_inst->set_seid_2_pgw_context(cp_fseid.seid, pc);

  Logger::pgwc_app().info(
      "Sending ITTI message %s to task TASK_PGWC_SX", sx_ser->get_msg_name());
  int ret = itti_inst->send_msg(sx_triggered);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_SX",
        sx_ser->get_msg_name());
    return RETURNerror;
  }
  return RETURNok;
}
//------------------------------------------------------------------------------
void session_establishment_procedure::handle_itti_msg(
    itti_sxab_session_establishment_response& resp) {
  pfcp::cause_t cause = {};
  resp.pfcp_ies.get(cause);
  if (cause.cause_value == pfcp::CAUSE_VALUE_REQUEST_ACCEPTED) {
    resp.pfcp_ies.get(ppc->up_fseid);
  }

  for (auto it : resp.pfcp_ies.created_pdrs) {
    pfcp::pdr_id_t pdr_id = {};
    pfcp::far_id_t far_id = {};
    if (it.get(pdr_id)) {
      pgw_eps_bearer b = {};
      if (ppc->get_eps_bearer(pdr_id, b)) {
        pfcp::fteid_t local_up_fteid = {};
        if (it.get(local_up_fteid)) {
          xgpp_conv::pfcp_to_core_fteid(local_up_fteid, b.pgw_fteid_s5_s8_up);
          b.pgw_fteid_s5_s8_up.interface_type = S5_S8_PGW_GTP_U;
          // comment if SPGW-C allocate up fteid
          pgw_eps_bearer b2 = b;
          ppc->add_eps_bearer(b2);
        }
        // uncomment if SPGW-C allocate up fteid
        // ppc->add_eps_bearer(b);
      } else {
        Logger::pgwc_app().error(
            "Could not get EPS bearer for created_pdr %d", pdr_id.rule_id);
      }
    } else {
      Logger::pgwc_app().error(
          "Could not get pdr_id for created_pdr in %s",
          resp.pfcp_ies.get_msg_name());
    }
  }

  for (auto it : s5_trigger->gtp_ies.bearer_contexts_to_be_created) {
    pgw_eps_bearer b                                                  = {};
    gtpv2c::bearer_context_created_within_create_session_response bcc = {};
    ::cause_t bcc_cause                                               = {
        .cause_value = REQUEST_ACCEPTED, .pce = 0, .bce = 0, .cs = 0};
    if (not ppc->get_eps_bearer(it.eps_bearer_id, b)) {
      bcc_cause.cause_value = SYSTEM_FAILURE;
    } else {
      if (b.pgw_fteid_s5_s8_up.is_zero()) {
        bcc_cause.cause_value = SYSTEM_FAILURE;
      } else {
        bcc.set(b.pgw_fteid_s5_s8_up, 2);
      }
    }
    bcc.set(b.ebi);
    bcc.set(bcc_cause);
    // only if modified bcc.set(bearer_level_qos);
    s5_triggered_pending->gtp_ies.add_bearer_context_created(bcc);
  }

  Logger::pgwc_app().info(
      "Sending ITTI message %s to task TASK_PGWC_S5S8",
      s5_triggered_pending->gtp_ies.get_msg_name());
  int ret = itti_inst->send_msg(s5_triggered_pending);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5_triggered_pending->gtp_ies.get_msg_name());
  }
}

//------------------------------------------------------------------------------
int modify_bearer_procedure::run(
    std::shared_ptr<itti_s5s8_modify_bearer_request>& req,
    std::shared_ptr<itti_s5s8_modify_bearer_response>& resp,
    std::shared_ptr<pgwc::pgw_context> pc) {
  bool send_sx = false;

  //-------------------
  s5_trigger                       = req;
  s5_triggered_pending             = resp;
  s5_triggered_pending->gtpc_tx_id = req->gtpc_tx_id;
  itti_sxab_session_modification_request* sx_smr =
      new itti_sxab_session_modification_request(TASK_PGWC_APP, TASK_PGWC_SX);
  sx_smr->seid       = ppc->up_fseid.seid;
  sx_smr->trxn_id    = this->trxn_id;
  sx_smr->r_endpoint = endpoint(ppc->up_fseid.ipv4_address, pgw_cfg.pfcp_.port);
  sx_triggered =
      std::shared_ptr<itti_sxab_session_modification_request>(sx_smr);

  //-------------------
  // IE fseid_t
  //-------------------
  // Unchanged so do not set it.
  // pfcp::fseid_t cp_fseid = {};
  // pgw_cfg.get_pfcp_fseid(cp_fseid);
  // cp_fseid.seid = ppc->seid;
  // sx_smr->pfcp_ies.set(cp_fseid);

  for (auto it : s5_trigger->gtp_ies.bearer_contexts_to_be_modified) {
    ::fteid_t v         = {};
    pgw_eps_bearer& peb = ppc->get_eps_bearer(it.eps_bearer_id);
    if (peb.ebi !=
        it.eps_bearer_id) {  // target is to test EPS_BEARER_IDENTITY_UNASSIGNED
      // TODO something (bearer not there)
      Logger::pgwc_app().error(
          "modify_bearer_procedure: missing pgw_eps_bearer ebi %d",
          it.eps_bearer_id.ebi);
      ::cause_t cause = {};
      cause.pce       = 1;
      // TODO cause
      cause.cause_value = SYSTEM_FAILURE;
      resp->gtp_ies.set(cause);

      gtpv2c::bearer_context_modified_within_modify_bearer_response bcc = {};
      ::cause_t bcc_cause                                               = {
          .cause_value = SYSTEM_FAILURE, .pce = 0, .bce = 0, .cs = 0};
      bcc.set(peb.ebi);
      bcc.set(bcc_cause);
      // only if modified bcc.set(bearer_level_qos);
      s5_triggered_pending->gtp_ies.add_bearer_context_modified(bcc);

      continue;
      //    } else if (it.get_s5_s8_u_sgw_fteid(v)) {get_s1_u_enb_fteid
    } else if (it.get_s1_u_enb_fteid(v)) {
      pfcp::far_id_t far_id = {};
      pfcp::pdr_id_t pdr_id = {};
      if ((v == peb.sgw_fteid_s5_s8_up) && (not peb.released)) {
        Logger::pgwc_app().debug(
            "modify_bearer_procedure: ebi %d sgw_fteid_s5_s8_up unchanged",
            it.eps_bearer_id.ebi);
        ::cause_t cause = {};
        cause.pce       = 1;
        // TODO CAUSE
        cause.cause_value = REQUEST_ACCEPTED;
        resp->gtp_ies.set(cause);

        gtpv2c::bearer_context_modified_within_modify_bearer_response bcc = {};
        ::cause_t bcc_cause                                               = {
            .cause_value = REQUEST_ACCEPTED, .pce = 0, .bce = 0, .cs = 0};
        bcc.set(peb.ebi);
        bcc.set(bcc_cause);
        // only if modified bcc.set(bearer_level_qos);
        s5_triggered_pending->gtp_ies.add_bearer_context_modified(bcc);
        continue;
      } else if ((peb.far_id_dl.first) && (peb.far_id_dl.second.far_id)) {
        // Update FAR
        far_id.far_id                     = peb.far_id_dl.second.far_id;
        pfcp::update_far update_far       = {};
        pfcp::apply_action_t apply_action = {};
        pfcp::outer_header_creation_t outer_header_creation             = {};
        pfcp::update_forwarding_parameters update_forwarding_parameters = {};

        update_far.set(peb.far_id_dl.second);
        outer_header_creation.outer_header_creation_description =
            OUTER_HEADER_CREATION_GTPU_UDP_IPV4;
        outer_header_creation.teid                = v.teid_gre_key;
        outer_header_creation.ipv4_address.s_addr = v.ipv4_address.s_addr;
        update_forwarding_parameters.set(outer_header_creation);
        update_far.set(update_forwarding_parameters);
        apply_action.forw = 1;
        update_far.set(apply_action);

        sx_smr->pfcp_ies.set(update_far);

        send_sx = true;

        peb.far_id_dl.first = true;
      } else {
        // Create FAR
        pfcp::create_far create_far                       = {};
        pfcp::apply_action_t apply_action                 = {};
        pfcp::forwarding_parameters forwarding_parameters = {};
        //    pfcp::duplicating_parameters      duplicating_parameters = {};
        //    pfcp::bar_id_t                    bar_id = {};

        // forwarding_parameters IEs
        pfcp::destination_interface_t destination_interface = {};
        // pfcp::network_instance_t          network_instance = {};
        // pfcp::redirect_information_t      redirect_information = {};
        pfcp::outer_header_creation_t outer_header_creation = {};
        // pfcp::transport_level_marking_t   transport_level_marking = {};
        // pfcp::forwarding_policy_t         forwarding_policy = {};
        // pfcp::header_enrichment_t         header_enrichment = {};
        // pfcp::traffic_endpoint_id_t       linked_traffic_endpoint_id_t = {};
        // pfcp::proxying_t                  proxying = {};

        ppc->generate_far_id(far_id);
        apply_action.forw = 1;

        destination_interface.interface_value =
            pfcp::INTERFACE_VALUE_ACCESS;  // ACCESS is for downlink, CORE for
                                           // uplink
        forwarding_parameters.set(destination_interface);
        outer_header_creation.outer_header_creation_description =
            OUTER_HEADER_CREATION_GTPU_UDP_IPV4;
        outer_header_creation.teid = it.s1_u_enb_fteid.teid_gre_key;
        outer_header_creation.ipv4_address.s_addr =
            it.s1_u_enb_fteid.ipv4_address.s_addr;
        forwarding_parameters.set(outer_header_creation);

        create_far.set(far_id);
        create_far.set(apply_action);
        create_far.set(forwarding_parameters);
        //-------------------
        // ADD IEs to message
        //-------------------
        sx_smr->pfcp_ies.set(create_far);

        send_sx = true;

        peb.far_id_dl.first  = true;
        peb.far_id_dl.second = far_id;
      }

      if (not peb.pdr_id_dl.rule_id) {
        //-------------------
        // IE create_pdr
        //-------------------
        pfcp::create_pdr create_pdr   = {};
        pfcp::precedence_t precedence = {};
        pfcp::pdi pdi                 = {};
        //    pfcp::far_id_t                    far_id;
        //    pfcp::urr_id_t                    urr_id;
        //    pfcp::qer_id_t                    qer_id;
        //    pfcp::activate_predefined_rules_t activate_predefined_rules;
        // pdi IEs
        pfcp::source_interface_t source_interface = {};
        // pfcp::fteid_t                    local_fteid = {};
        // pfcp::network_instance_t         network_instance = {};
        pfcp::ue_ip_address_t ue_ip_address = {};
        // pfcp::traffic_endpoint_id_t      traffic_endpoint_id = {};
        pfcp::sdf_filter_t sdf_filter         = {};
        pfcp::application_id_t application_id = {};
        // pfcp::ethernet_packet_filter     ethernet_packet_filter = {};
        pfcp::qfi_t qfi = {};
        // pfcp::framed_route_t             framed_route = {};
        // pfcp::framed_routing_t           framed_routing = {};
        // pfcp::framed_ipv6_route_t        framed_ipv6_route = {};
        source_interface.interface_value = pfcp::INTERFACE_VALUE_CORE;

        // local_fteid.from_core_fteid(peb.sgw_fteid_s5_s8_up);
        if (ppc->ipv4) {
          ue_ip_address.v4                  = 1;
          ue_ip_address.ipv4_address.s_addr = ppc->ipv4_address.s_addr;
        }
        if (ppc->ipv6) {
          ue_ip_address.v6           = 1;
          ue_ip_address.ipv6_address = ppc->ipv6_address;
        }

        // DOIT simple
        // shall uniquely identify the PDR among all the PDRs configured for
        // that PFCP session.
        ppc->generate_pdr_id(pdr_id);
        precedence.precedence = peb.eps_bearer_qos.pl;

        pdi.set(source_interface);
        // pdi.set(local_fteid);
        pdi.set(ue_ip_address);

        create_pdr.set(pdr_id);
        create_pdr.set(precedence);
        create_pdr.set(pdi);
        create_pdr.set(far_id);
        //-------------------
        // ADD IEs to message
        //-------------------
        sx_smr->pfcp_ies.set(create_pdr);

        send_sx = true;

        peb.pdr_id_dl = pdr_id;
      } else {
        // Update FAR
        far_id.far_id                     = peb.far_id_ul.second.far_id;
        pfcp::update_far update_far       = {};
        pfcp::apply_action_t apply_action = {};

        update_far.set(peb.far_id_ul.second);
        apply_action.forw = 1;
        update_far.set(apply_action);

        sx_smr->pfcp_ies.set(update_far);

        send_sx = true;

        peb.far_id_dl.first = true;
      }
    }

    // after a release bearers
    if (not peb.pgw_fteid_s5_s8_up.is_zero()) {
      pfcp::far_id_t far_id = {};
      pfcp::pdr_id_t pdr_id = {};
      if ((not peb.far_id_ul.first) || (not peb.far_id_ul.second.far_id)) {
        //-------------------
        // IE create_far
        //-------------------
        pfcp::create_far create_far                       = {};
        pfcp::apply_action_t apply_action                 = {};
        pfcp::forwarding_parameters forwarding_parameters = {};
        //    pfcp::duplicating_parameters      duplicating_parameters = {};
        //    pfcp::bar_id_t                    bar_id = {};

        // forwarding_parameters IEs
        pfcp::destination_interface_t destination_interface = {};
        // pfcp::network_instance_t          network_instance = {};
        // pfcp::redirect_information_t      redirect_information = {};
        // pfcp::outer_header_creation_t     outer_header_creation = {};
        // pfcp::transport_level_marking_t   transport_level_marking = {};
        // pfcp::forwarding_policy_t         forwarding_policy = {};
        // pfcp::header_enrichment_t         header_enrichment = {};
        // pfcp::traffic_endpoint_id_t       linked_traffic_endpoint_id_t = {};
        // pfcp::proxying_t                  proxying = {};

        ppc->generate_far_id(far_id);
        apply_action.forw = 1;

        destination_interface.interface_value =
            pfcp::INTERFACE_VALUE_CORE;  // ACCESS is for downlink, CORE for
                                         // uplink
        forwarding_parameters.set(destination_interface);

        create_far.set(far_id);
        create_far.set(apply_action);
        create_far.set(forwarding_parameters);

        peb.far_id_ul.first  = true;
        peb.far_id_ul.second = far_id;

        //-------------------
        // ADD IEs to message
        //-------------------
        sx_smr->pfcp_ies.set(create_far);
        send_sx = true;
      } else {
        far_id.far_id = peb.far_id_ul.second.far_id;
      }

      if (not peb.pdr_id_ul.rule_id) {
        pfcp::create_pdr create_pdr                       = {};
        pfcp::precedence_t precedence                     = {};
        pfcp::pdi pdi                                     = {};
        pfcp::outer_header_removal_t outer_header_removal = {};
        //    pfcp::far_id_t                    far_id;
        //    pfcp::urr_id_t                    urr_id;
        //    pfcp::qer_id_t                    qer_id;
        //    pfcp::activate_predefined_rules_t activate_predefined_rules;
        // pdi IEs
        pfcp::source_interface_t source_interface = {};
        pfcp::fteid_t local_fteid                 = {};
        // pfcp::network_instance_t         network_instance = {};
        pfcp::ue_ip_address_t ue_ip_address = {};
        // pfcp::traffic_endpoint_id_t      traffic_endpoint_id = {};
        pfcp::sdf_filter_t sdf_filter         = {};
        pfcp::application_id_t application_id = {};
        // pfcp::ethernet_packet_filter     ethernet_packet_filter = {};
        pfcp::qfi_t qfi = {};
        // pfcp::framed_route_t             framed_route = {};
        // pfcp::framed_routing_t           framed_routing = {};
        // pfcp::framed_ipv6_route_t        framed_ipv6_route = {};
        source_interface.interface_value = pfcp::INTERFACE_VALUE_ACCESS;
        local_fteid.ch                   = 1;
        // local_fteid.chid = 1;
        xgpp_conv::pdn_ip_to_pfcp_ue_ip_address(
            ppc->pdn_type, ppc->ipv4_address, ppc->ipv6_address, ue_ip_address);

        // DOIT simple
        // shall uniquely identify the PDR among all the PDRs configured for
        // that PFCP session.
        ppc->generate_pdr_id(pdr_id);
        precedence.precedence = peb.eps_bearer_qos.pl;

        pdi.set(source_interface);
        pdi.set(local_fteid);
        pdi.set(ue_ip_address);

        outer_header_removal.outer_header_removal_description =
            OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4;

        create_pdr.set(pdr_id);
        create_pdr.set(precedence);
        create_pdr.set(pdi);
        create_pdr.set(outer_header_removal);
        create_pdr.set(far_id);

        //-------------------
        // ADD IEs to message
        //-------------------
        sx_smr->pfcp_ies.set(create_pdr);

        send_sx = true;

        peb.pdr_id_ul = pdr_id;
      }
    }
    // may be modified
    pgw_eps_bearer peb2 = peb;
    ppc->add_eps_bearer(peb2);
  }

  if (send_sx) {
    Logger::pgwc_app().info(
        "Sending ITTI message %s to task TASK_PGWC_SX", sx_smr->get_msg_name());
    int ret = itti_inst->send_msg(sx_triggered);
    if (RETURNok != ret) {
      Logger::pgwc_app().error(
          "Could not send ITTI message %s to task TASK_PGWC_SX",
          sx_smr->get_msg_name());
      return RETURNerror;
    }
  } else {
    // send s5
    Logger::pgwc_app().info(
        "Sending ITTI message %s to task TASK_PGWC_S5S8",
        s5_triggered_pending->gtp_ies.get_msg_name());
    int ret = itti_inst->send_msg(s5_triggered_pending);
    if (RETURNok != ret) {
      Logger::pgwc_app().error(
          "Could not send ITTI message %s to task TASK_PGWC_S5S8",
          s5_triggered_pending->gtp_ies.get_msg_name());
    }
    return RETURNclear;
  }
  return RETURNok;
}
//------------------------------------------------------------------------------
void modify_bearer_procedure::handle_itti_msg(
    itti_sxab_session_modification_response& resp) {
  pfcp::cause_t cause = {};
  ::cause_t cause_gtp = {.cause_value = REQUEST_ACCEPTED};

  // must be there
  if (resp.pfcp_ies.get(cause)) {
    xgpp_conv::pfcp_cause_to_core_cause(cause, cause_gtp);
  }

  bool bearer_context_found = false;
  for (auto it_created_pdr : resp.pfcp_ies.created_pdrs) {
    bearer_context_found  = false;
    pfcp::pdr_id_t pdr_id = {};
    if (it_created_pdr.get(pdr_id)) {
      pgw_eps_bearer b = {};
      if (ppc->get_eps_bearer(pdr_id, b)) {
        for (
            std::vector<
                gtpv2c::
                    bearer_context_to_be_modified_within_modify_bearer_request>::
                const_iterator it_to_be_modified =
                    s5_trigger->gtp_ies.bearer_contexts_to_be_modified.begin();
            it_to_be_modified !=
            s5_trigger->gtp_ies.bearer_contexts_to_be_modified.end();
            ++it_to_be_modified) {
          // pfcp::pdr_id_t conv_pdr_id = {};
          // ebi2pdr_id(it_to_be_modified.eps_bearer_id, conv_pdr_id,
          // EBI2PDR_ID_DL_BEARER);

          // False can be UL Bearer also
          // if (conv_pdr_id.rule_id == pdr_id.rule_id) {
          bearer_context_found = true;
          // can try the two till SGW not split (depends on developer convention
          // to fake PGW)
          it_to_be_modified->get_s5_s8_u_sgw_fteid(b.sgw_fteid_s5_s8_up);
          it_to_be_modified->get_s1_u_enb_fteid(b.sgw_fteid_s5_s8_up);

          pfcp::fteid_t local_up_fteid = {};
          if (it_created_pdr.get(local_up_fteid)) {
            xgpp_conv::pfcp_to_core_fteid(local_up_fteid, b.pgw_fteid_s5_s8_up);
            b.pgw_fteid_s5_s8_up.interface_type = S5_S8_PGW_GTP_U;
            // comment if SPGW-C allocate up fteid
            Logger::pgwc_app().error(
                "got local_up_fteid from created_pdr %s",
                b.pgw_fteid_s5_s8_up.toString().c_str());
          } else {
            Logger::pgwc_app().error(
                "Could not get local_up_fteid from created_pdr");
          }
          b.released        = false;
          pgw_eps_bearer b2 = b;
          ppc->add_eps_bearer(b2);

          gtpv2c::bearer_context_modified_within_modify_bearer_response bcc =
              {};
          ::cause_t bcc_cause = {
              .cause_value = REQUEST_ACCEPTED, .pce = 0, .bce = 0, .cs = 0};
          bcc.set_s1_u_sgw_fteid(b.pgw_fteid_s5_s8_up);
          bcc.set(b.ebi);
          bcc.set(bcc_cause);
          s5_triggered_pending->gtp_ies.add_bearer_context_modified(bcc);

          // Avoid duplicate with update fars
          s5_trigger->gtp_ies.bearer_contexts_to_be_modified.erase(
              it_to_be_modified);
          break;
          //}
        }
      }
    } else {
      Logger::pgwc_app().error(
          "Could not get pdr_id for created_pdr in %s",
          resp.pfcp_ies.get_msg_name());
    }
  }
  if (cause.cause_value == CAUSE_VALUE_REQUEST_ACCEPTED) {
    // TODO failed rule id
    for (auto it_update_far : sx_triggered->pfcp_ies.update_fars) {
      pfcp::far_id_t far_id = {};
      if (it_update_far.get(far_id)) {
        pgw_eps_bearer b = {};
        if (ppc->get_eps_bearer(far_id, b)) {
          for (
              std::vector<
                  gtpv2c::
                      bearer_context_to_be_modified_within_modify_bearer_request>::
                  const_iterator it_to_be_mod =
                      s5_trigger->gtp_ies.bearer_contexts_to_be_modified
                          .begin();
              it_to_be_mod !=
              s5_trigger->gtp_ies.bearer_contexts_to_be_modified.end();
              ++it_to_be_mod) {
            if (it_to_be_mod->eps_bearer_id == b.ebi) {
              it_to_be_mod->get_s5_s8_u_sgw_fteid(b.sgw_fteid_s5_s8_up);
              it_to_be_mod->get_s1_u_enb_fteid(b.sgw_fteid_s5_s8_up);
              pgw_eps_bearer b2 = b;
              ppc->add_eps_bearer(b2);

              gtpv2c::bearer_context_modified_within_modify_bearer_response
                  bcc             = {};
              ::cause_t bcc_cause = {
                  .cause_value = REQUEST_ACCEPTED, .pce = 0, .bce = 0, .cs = 0};
              bcc.set(b.ebi);
              bcc.set_s1_u_sgw_fteid(b.pgw_fteid_s5_s8_up);
              bcc.set(bcc_cause);
              s5_triggered_pending->gtp_ies.add_bearer_context_modified(bcc);
              break;
            }
          }
        } else {
          Logger::pgwc_app().error(
              "Could not get eps bearer for far_id for update_far in %s",
              resp.pfcp_ies.get_msg_name());
        }
      } else {
        Logger::pgwc_app().error(
            "Could not get far_id for update_far in %s",
            resp.pfcp_ies.get_msg_name());
      }
    }
  }

  // TODO NOT GOOD since we removed the bearer from
  // s5_trigger->gtp_ies.bearer_contexts_to_be_modified check we got all
  // responses
  for (auto it : s5_trigger->gtp_ies.bearer_contexts_to_be_modified) {
    ebi_t ebi_tobe;
    if ((it.get(ebi_tobe)) &&
        (s5_triggered_pending->gtp_ies.bearer_contexts_modified.first)) {
      for (auto it_modified :
           s5_triggered_pending->gtp_ies.bearer_contexts_modified.second) {
        ebi_t ebi_done;
        if ((it_modified.get(ebi_done)) && (ebi_done == ebi_tobe)) {
          if ((cause_gtp.cause_value == REQUEST_ACCEPTED) &&
              (it_modified.cause.second.cause_value != REQUEST_ACCEPTED)) {
            cause_gtp.cause_value = REQUEST_ACCEPTED_PARTIALLY;
          } else if (
              (cause_gtp.cause_value != REQUEST_ACCEPTED) &&
              (it_modified.cause.second.cause_value == REQUEST_ACCEPTED)) {
            cause_gtp.cause_value = REQUEST_ACCEPTED_PARTIALLY;
          }
          // found = true;
          break;
        }
      }
      //      if (not found) {
      //        // TODO ?
      //      }
    }
  }
  s5_triggered_pending->gtp_ies.set(cause_gtp);
  Logger::pgwc_app().info(
      "Sending ITTI message %s to task TASK_PGWC_S5S8",
      s5_triggered_pending->gtp_ies.get_msg_name());
  int ret = itti_inst->send_msg(s5_triggered_pending);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5_triggered_pending->gtp_ies.get_msg_name());
  }
}

//------------------------------------------------------------------------------
int release_access_bearers_procedure::run(
    std::shared_ptr<itti_s5s8_release_access_bearers_request>& req,
    std::shared_ptr<itti_s5s8_release_access_bearers_response>& resp,
    std::shared_ptr<pgwc::pgw_context> pc) {
  //-------------------
  s5_trigger                       = req;
  s5_triggered_pending             = resp;
  s5_triggered_pending->gtpc_tx_id = req->gtpc_tx_id;
  itti_sxab_session_modification_request* sx_smr =
      new itti_sxab_session_modification_request(TASK_PGWC_APP, TASK_PGWC_SX);
  sx_smr->seid       = ppc->up_fseid.seid;
  sx_smr->trxn_id    = this->trxn_id;
  sx_smr->r_endpoint = endpoint(ppc->up_fseid.ipv4_address, pgw_cfg.pfcp_.port);
  sx_triggered =
      std::shared_ptr<itti_sxab_session_modification_request>(sx_smr);

  //-------------------
  // IE fseid_t
  //-------------------
  // Unchanged so do not set it.
  // pfcp::fseid_t cp_fseid = {};
  // pgw_cfg.get_pfcp_fseid(cp_fseid);
  // cp_fseid.seid = ppc->seid;
  // sx_smr->pfcp_ies.set(cp_fseid);

  for (auto it : ppc->eps_bearers) {
    pgw_eps_bearer& peb = it.second;

    //*******************
    // DOWNLINK
    //*******************
    //-------------------
    // IE create_far
    //-------------------
    pfcp::update_far far  = {};
    pfcp::far_id_t far_id = {};
    //    pfcp::update_forwarding_parameters forwarding_parameters = {};
    //    pfcp::update_duplicating_parameters      duplicating_parameters = {};
    //    pfcp::bar_id_t                    bar_id = {};

    // forwarding_parameters IEs
    // pfcp::destination_interface_t     destination_interface = {};
    // pfcp::network_instance_t          network_instance = {};
    // pfcp::redirect_information_t      redirect_information = {};
    // pfcp::outer_header_creation_t     outer_header_creation = {};
    // pfcp::transport_level_marking_t   transport_level_marking = {};
    // pfcp::forwarding_policy_t         forwarding_policy = {};
    // pfcp::header_enrichment_t         header_enrichment = {};
    // pfcp::traffic_endpoint_id_t       linked_traffic_endpoint_id_t = {};
    // pfcp::proxying_t                  proxying = {};

    // DOIT simple ?
    if (peb.far_id_dl.first) {
      far_id.far_id = peb.far_id_dl.second.far_id;
      // apply_action.buff = 1;
      pfcp::apply_action_t apply_action = {};
      apply_action.nocp                 = 1;

      far.set(far_id);
      far.set(apply_action);
      //-------------------
      // ADD IEs to message
      //-------------------
      sx_smr->pfcp_ies.set(far);
    } else {
      Logger::pgwc_app().info(
          "release_access_bearers_procedure , could not get FAR ID of EPS "
          "bearer %d",
          peb.ebi.ebi);
    }
    if (peb.far_id_ul.first) {
      pfcp::update_far far              = {};
      pfcp::far_id_t far_id             = {};
      far_id.far_id                     = peb.far_id_ul.second.far_id;
      pfcp::apply_action_t apply_action = {};
      apply_action.drop                 = 1;

      far.set(far_id);
      far.set(apply_action);
      sx_smr->pfcp_ies.set(far);
    }

    peb.release_access_bearer();
    pgw_eps_bearer b2 = peb;
    ppc->add_eps_bearer(b2);
  }

  Logger::pgwc_app().info(
      "Sending ITTI message %s to task TASK_PGWC_SX", sx_smr->get_msg_name());
  int ret = itti_inst->send_msg(sx_triggered);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_SX",
        sx_smr->get_msg_name());
    return RETURNerror;
  }
  return RETURNok;
}
//------------------------------------------------------------------------------
void release_access_bearers_procedure::handle_itti_msg(
    itti_sxab_session_modification_response& resp) {
  ::cause_t gtp_cause      = {};
  pfcp::cause_t pfcp_cause = {};

  resp.pfcp_ies.get(pfcp_cause);
  xgpp_conv::pfcp_cause_to_core_cause(pfcp_cause, gtp_cause);

  s5_triggered_pending->gtp_ies.set(gtp_cause);
  // for now we do not need more IEs
  Logger::pgwc_app().info(
      "Sending ITTI message %s to task TASK_PGWC_S5S8",
      s5_triggered_pending->gtp_ies.get_msg_name());
  int ret = itti_inst->send_msg(s5_triggered_pending);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5_triggered_pending->gtp_ies.get_msg_name());
  }
}
//------------------------------------------------------------------------------
int delete_session_procedure::run(
    std::shared_ptr<itti_s5s8_delete_session_request>& req,
    std::shared_ptr<itti_s5s8_delete_session_response>& resp,
    std::shared_ptr<pgwc::pgw_context> pc) {
  // TODO check if compatible with ongoing procedures if any

  //-------------------
  s5_trigger                       = req;
  s5_triggered_pending             = resp;
  s5_triggered_pending->gtpc_tx_id = req->gtpc_tx_id;
  itti_sxab_session_deletion_request* sx =
      new itti_sxab_session_deletion_request(TASK_PGWC_APP, TASK_PGWC_SX);
  sx->seid       = ppc->up_fseid.seid;
  sx->trxn_id    = this->trxn_id;
  sx->r_endpoint = endpoint(ppc->up_fseid.ipv4_address, pgw_cfg.pfcp_.port);
  sx_triggered   = std::shared_ptr<itti_sxab_session_deletion_request>(sx);

  Logger::pgwc_app().info(
      "Sending ITTI message %s to task TASK_PGWC_SX",
      sx->pfcp_ies.get_msg_name());
  int ret = itti_inst->send_msg(sx_triggered);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_SX",
        sx->pfcp_ies.get_msg_name());
    return RETURNerror;
  }
  return RETURNok;
}
//------------------------------------------------------------------------------
void delete_session_procedure::handle_itti_msg(
    itti_sxab_session_deletion_response& resp) {
  ::cause_t gtp_cause = {
      .cause_value = REQUEST_ACCEPTED, .pce = 0, .bce = 0, .cs = 0};
  pfcp::cause_t cause = {.cause_value = pfcp::CAUSE_VALUE_REQUEST_ACCEPTED};
  if (resp.pfcp_ies.get(cause)) {
    switch (cause.cause_value) {
      case CAUSE_VALUE_REQUEST_ACCEPTED:
        break;
      case CAUSE_VALUE_REQUEST_REJECTED:
      case CAUSE_VALUE_SESSION_CONTEXT_NOT_FOUND:
      case CAUSE_VALUE_MANDATORY_IE_MISSING:
      case CAUSE_VALUE_CONDITIONAL_IE_MISSING:
      case CAUSE_VALUE_INVALID_LENGTH:
      case CAUSE_VALUE_MANDATORY_IE_INCORRECT:
      case CAUSE_VALUE_INVALID_FORWARDING_POLICY:
      case CAUSE_VALUE_INVALID_FTEID_ALLOCATION_OPTION:
      case CAUSE_VALUE_NO_ESTABLISHED_PFCP_ASSOCIATION:
      case CAUSE_VALUE_RULE_CREATION_MODIFICATION_FAILURE:
      case CAUSE_VALUE_PFCP_ENTITY_IN_CONGESTION:
      case CAUSE_VALUE_NO_RESOURCES_AVAILABLE:
      case CAUSE_VALUE_SERVICE_NOT_SUPPORTED:
      case CAUSE_VALUE_SYSTEM_FAILURE:
      default:
        gtp_cause.cause_value = CONTEXT_NOT_FOUND;
    }
  }

  s5_triggered_pending->gtp_ies.set(gtp_cause);

  Logger::pgwc_app().info(
      "Sending ITTI message %s to task TASK_PGWC_S5S8",
      s5_triggered_pending->gtp_ies.get_msg_name());
  int ret = itti_inst->send_msg(s5_triggered_pending);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5_triggered_pending->gtp_ies.get_msg_name());
  }

  // find APN context
  pdn_duo_t pdn_duo = {};
  if (pc->find_pdn_connection(
          ppc->pgw_fteid_s5_s8_cp.teid_gre_key, IS_FIND_PDN_WITH_LOCAL_TEID,
          pdn_duo)) {
    pc->delete_pdn_connection(pdn_duo.first, pdn_duo.second);
  } else {
    Logger::pgwc_app().error(
        "Could not delete PDN connection (APN context not found)");
  }
}
//------------------------------------------------------------------------------
int downlink_data_report_procedure::run(
    std::shared_ptr<pgwc::pgw_context> context,
    std::shared_ptr<pgwc::pgw_pdn_connection> pdn, const ebi_t& e) {
  ppc = pdn;
  pc  = context;
  ebi = e;

  itti_s5s8_downlink_data_notification* s5 =
      new itti_s5s8_downlink_data_notification(TASK_PGWC_APP, TASK_PGWC_S5S8);
  s5->teid       = ppc->sgw_fteid_s5_s8_cp.teid_gre_key;
  s5->gtpc_tx_id = this->trxn_id;
  s5->r_endpoint =
      endpoint(ppc->sgw_fteid_s5_s8_cp.ipv4_address, pgw_cfg.gtpv2c_.port);
  s5->gtp_ies.set(e);
  s5_triggered = std::shared_ptr<itti_s5s8_downlink_data_notification>(s5);

  Logger::pgwc_app().info(
      "Sending ITTI message %s to task TASK_PGWC_S5S8",
      s5->gtp_ies.get_msg_name());
  int ret = itti_inst->send_msg(s5_triggered);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_S5S8",
        s5->gtp_ies.get_msg_name());
    return RETURNerror;
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
void downlink_data_report_procedure::handle_itti_msg(
    itti_s5s8_downlink_data_notification_acknowledge& ack) {
  ::cause_t gtp_cause = {};

  pfcp::cause_t pfcp_cause = {.cause_value =
                                  pfcp::CAUSE_VALUE_REQUEST_ACCEPTED};
  if (ack.gtp_ies.get(gtp_cause)) {
    switch (gtp_cause.cause_value) {
      case REQUEST_ACCEPTED:
        break;
      default:
        pfcp_cause.cause_value = CAUSE_VALUE_REQUEST_REJECTED;
    }
  } else {
    Logger::pgwc_app().error(
        "downlink_data_report_procedure: Could not get cause from "
        "itti_s5s8_downlink_data_notification_acknowledge");
    return;
  }

  itti_sxab_session_report_response* sx =
      new itti_sxab_session_report_response(TASK_PGWC_APP, TASK_PGWC_SX);
  sx->seid       = ppc->up_fseid.seid;
  sx->trxn_id    = this->trxn_id;
  sx->r_endpoint = endpoint(ppc->up_fseid.ipv4_address, pgw_cfg.pfcp_.port);
  std::shared_ptr<itti_sxab_session_report_response> sx_triggered =
      std::shared_ptr<itti_sxab_session_report_response>(sx);
  sx->pfcp_ies.set(pfcp_cause);
  int ret = itti_inst->send_msg(sx_triggered);
  if (RETURNok != ret) {
    Logger::pgwc_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_SX",
        sx->pfcp_ies.get_msg_name());
  }
}
