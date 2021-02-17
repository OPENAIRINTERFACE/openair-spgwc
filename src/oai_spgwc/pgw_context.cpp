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

/*! \file pgw_context.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "pgw_context.hpp"
#include "itti.hpp"
#include "logger.hpp"
#include "pgw_app.hpp"
#include "pgw_config.hpp"
#include "pgw_paa_dynamic.hpp"
#include "pgwc_procedure.hpp"

#include <algorithm>

using namespace pgwc;

extern itti_mw* itti_inst;
extern pgwc::pgw_app* pgw_app_inst;

//------------------------------------------------------------------------------
void pgw_eps_bearer::release_access_bearer() {
  released = true;
}
//------------------------------------------------------------------------------
std::string pgw_eps_bearer::toString() const {
  std::string s = {};
  s.append("EPS BEARER:\n");
  s.append("\tEBI:\t\t\t\t").append(std::to_string(ebi.ebi)).append("\n");
  s.append("\tTFT:\t\t\t\tTODO tft").append("\n");
  s.append("\tSGW FTEID S5S8 UP:\t\t")
      .append(sgw_fteid_s5_s8_up.toString())
      .append("\n");
  s.append("\tPGW FTEID S5S8 UP:\t\t")
      .append(pgw_fteid_s5_s8_up.toString())
      .append("\n");
  s.append("\tBearer QOS:\t\t\t")
      .append(eps_bearer_qos.toString())
      .append("\n");
  s.append("\tPDR ID UL:\t\t\t")
      .append(std::to_string(pdr_id_ul.rule_id))
      .append("\n");
  s.append("\tPDR ID DL:\t\t\t")
      .append(std::to_string(pdr_id_dl.rule_id))
      .append("\n");
  s.append("\tPRECEDENCE:\t\t\t")
      .append(std::to_string(precedence.precedence))
      .append("\n");
  if (far_id_ul.first) {
    s.append("\tFAR ID UL:\t\t\t")
        .append(std::to_string(far_id_ul.second.far_id))
        .append("\n");
  }
  if (far_id_dl.first) {
    s.append("\tFAR ID DL:\t\t\t")
        .append(std::to_string(far_id_dl.second.far_id))
        .append("\n");
  }
  return s;
}
//------------------------------------------------------------------------------
void pgw_eps_bearer::deallocate_ressources() {
  Logger::pgwc_app().info("pgw_eps_bearer::deallocate_ressources(%d)", ebi.ebi);
  // if (not is_fteid_zero(pgw_fteid_s5_s8_up))
  // pgw_app_inst->free_s5s8_up_fteid(pgw_fteid_s5_s8_up);
  clear();
}
//------------------------------------------------------------------------------
void pgw_pdn_connection::set(const paa_t& paa) {
  switch (paa.pdn_type.pdn_type) {
    case PDN_TYPE_E_IPV4:
      ipv4         = true;
      ipv6         = false;
      ipv4_address = paa.ipv4_address;
      break;
    case PDN_TYPE_E_IPV6:
      ipv4         = false;
      ipv6         = true;
      ipv6_address = paa.ipv6_address;
      break;
    case PDN_TYPE_E_IPV4V6:
      ipv4         = true;
      ipv6         = true;
      ipv4_address = paa.ipv4_address;
      ipv6_address = paa.ipv6_address;
      break;
    case PDN_TYPE_E_NON_IP:
      ipv4 = false;
      ipv6 = false;
      break;
    default:
      Logger::pgwc_app().error(
          "pgw_pdn_connection::set(paa_t) Unknown PDN type %d",
          paa.pdn_type.pdn_type);
  }
}
//------------------------------------------------------------------------------
void pgw_pdn_connection::add_eps_bearer(pgw_eps_bearer& bearer) {
  if ((bearer.ebi.ebi >= EPS_BEARER_IDENTITY_FIRST) and
      (bearer.ebi.ebi <= EPS_BEARER_IDENTITY_LAST)) {
    eps_bearers.erase(bearer.ebi.ebi);
    eps_bearers.insert(
        std::pair<uint8_t, pgw_eps_bearer>(bearer.ebi.ebi, bearer));
    Logger::pgwc_app().trace(
        "pgw_pdn_connection::add_eps_bearer(%d) success", bearer.ebi.ebi);
  } else {
    Logger::pgwc_app().error(
        "pgw_pdn_connection::add_eps_bearer(%d) failed, invalid EBI",
        bearer.ebi.ebi);
  }
}

//------------------------------------------------------------------------------
pgw_eps_bearer& pgw_pdn_connection::get_eps_bearer(const ebi_t& ebi) {
  return eps_bearers[ebi.ebi];
}

//------------------------------------------------------------------------------
bool pgw_pdn_connection::find_eps_bearer(
    const pfcp::pdr_id_t& pdr_id, pgw_eps_bearer& bearer) {
  for (std::map<uint8_t, pgw_eps_bearer>::iterator it = eps_bearers.begin();
       it != eps_bearers.end(); ++it) {
    if ((it->second.pdr_id_ul == pdr_id) || (it->second.pdr_id_dl == pdr_id)) {
      bearer = it->second;
      return true;
    }
  }
  return false;
}
//------------------------------------------------------------------------------
bool pgw_pdn_connection::has_eps_bearer(
    const pfcp::pdr_id_t& pdr_id, ebi_t& ebi) {
  for (std::map<uint8_t, pgw_eps_bearer>::iterator it = eps_bearers.begin();
       it != eps_bearers.end(); ++it) {
    if ((it->second.pdr_id_ul == pdr_id) || (it->second.pdr_id_dl == pdr_id)) {
      ebi = it->second.ebi;
      return true;
    }
  }
  return false;
}
//------------------------------------------------------------------------------
void pgw_pdn_connection::remove_eps_bearer(const ebi_t& ebi) {
  pgw_eps_bearer& bearer = eps_bearers[ebi.ebi];
  bearer.deallocate_ressources();
  eps_bearers.erase(ebi.ebi);
}
//------------------------------------------------------------------------------
void pgw_pdn_connection::remove_eps_bearer(pgw_eps_bearer& bearer) {
  ebi_t ebi = {.ebi = bearer.ebi.ebi};
  bearer.deallocate_ressources();
  eps_bearers.erase(ebi.ebi);
}

//------------------------------------------------------------------------------
void pgw_pdn_connection::deallocate_ressources(const std::string& apn) {
  for (std::map<uint8_t, pgw_eps_bearer>::iterator it = eps_bearers.begin();
       it != eps_bearers.end(); ++it) {
    it->second.deallocate_ressources();
  }
  eps_bearers.clear();
  if (ipv4) {
    paa_dynamic::get_instance().release_paa(apn, ipv4_address);
  }
  pgw_app_inst->free_s5s8_cp_fteid(pgw_fteid_s5_s8_cp);
  clear();
}
//------------------------------------------------------------------------------
void pgw_pdn_connection::generate_seid() {
  // DO it simple now:
  seid = pgw_fteid_s5_s8_cp.teid_gre_key |
         (((uint64_t) pgwc::pgw_config::instance_) << 32);
}
//------------------------------------------------------------------------------
// TODO check if prd_id should be uniq in the (S)PGW-U or in the context of a
// pdn connection
void pgw_pdn_connection::generate_far_id(pfcp::far_id_t& far_id) {
  far_id.far_id = far_id_generator.get_uid();
}
//------------------------------------------------------------------------------
// TODO check if prd_id should be uniq in the (S)PGW-U or in the context of a
// pdn connection
void pgw_pdn_connection::release_far_id(const pfcp::far_id_t& far_id) {
  far_id_generator.free_uid(far_id.far_id);
}
//------------------------------------------------------------------------------
// TODO check if prd_id should be uniq in the (S)PGW-U or in the context of a
// pdn connection
void pgw_pdn_connection::generate_pdr_id(pfcp::pdr_id_t& pdr_id) {
  pdr_id.rule_id = pdr_id_generator.get_uid();
}
//------------------------------------------------------------------------------
// TODO check if prd_id should be uniq in the (S)PGW-U or in the context of a
// pdn connection
void pgw_pdn_connection::release_pdr_id(const pfcp::pdr_id_t& pdr_id) {
  pdr_id_generator.free_uid(pdr_id.rule_id);
}

//------------------------------------------------------------------------------
std::string pgw_pdn_connection::toString() const {
  std::string s = {};
  s.append("PDN CONNECTION:\n");
  s.append("\tPDN type:\t\t\t").append(pdn_type.toString()).append("\n");
  if (ipv4)
    s.append("\tPAA IPv4:\t\t\t")
        .append(conv::toString(ipv4_address))
        .append("\n");
  if (ipv6)
    s.append("\tPAA IPv6:\t\t\t")
        .append(conv::toString(ipv6_address))
        .append("\n");
  s.append("\tSGW FTEID S5S8 CP:\t\t")
      .append(sgw_fteid_s5_s8_cp.toString())
      .append("\n");
  s.append("\tPGW FTEID S5S8 CP:\t\t")
      .append(pgw_fteid_s5_s8_cp.toString())
      .append("\n");
  s.append("\tDefault EBI:\t\t\t")
      .append(std::to_string(default_bearer.ebi))
      .append("\n");
  s.append("\tSEID:\t\t\t").append(std::to_string(seid)).append("\n");
  for (auto it : eps_bearers) {
    s.append(it.second.toString());
  }
  return s;
}

//------------------------------------------------------------------------------
void apn_context::insert_pdn_connection(
    std::shared_ptr<pgw_pdn_connection>& sp) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  pdn_connections.push_back(sp);
}
//------------------------------------------------------------------------------
bool apn_context::find_pdn_connection(
    const teid_t xgw_s5s8c_teid, const bool is_local_teid,
    std::shared_ptr<pgw_pdn_connection>& pdn) {
  pdn = {};
  if (is_local_teid) {
    std::unique_lock<std::recursive_mutex> lock(m_context);
    for (auto it : pdn_connections) {
      if (xgw_s5s8c_teid == it->pgw_fteid_s5_s8_cp.teid_gre_key) {
        pdn = it;
        return true;
      }
    }
    return false;
  } else {
    std::unique_lock<std::recursive_mutex> lock(m_context);
    for (auto it : pdn_connections) {
      if (xgw_s5s8c_teid == it->sgw_fteid_s5_s8_cp.teid_gre_key) {
        pdn = it;
        return true;
      }
    }
    return false;
  }
}

//------------------------------------------------------------------------------
bool apn_context::find_pdn_connection(
    const pfcp::pdr_id_t& pdr_id, std::shared_ptr<pgw_pdn_connection>& pdn,
    ebi_t& ebi) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  for (auto pit : pdn_connections) {
    if (pit->has_eps_bearer(pdr_id, ebi)) {
      pdn = pit;  // May make pair
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void apn_context::delete_pdn_connection(
    std::shared_ptr<pgw_pdn_connection>& pdn_connection) {
  if (pdn_connection.get()) {
    pdn_connection->deallocate_ressources(apn_in_use);
    // remove it from collection
    std::unique_lock<std::recursive_mutex> lock(m_context);
    for (std::vector<std::shared_ptr<pgw_pdn_connection>>::iterator it =
             pdn_connections.begin();
         it != pdn_connections.end(); ++it) {
      if (pdn_connection.get() == (*it).get()) {
        pdn_connection->deallocate_ressources(apn_in_use);
        pdn_connections.erase(it);
        return;
      }
    }
  }
}
//------------------------------------------------------------------------------
void apn_context::deallocate_ressources() {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  for (std::vector<std::shared_ptr<pgw_pdn_connection>>::iterator it =
           pdn_connections.begin();
       it != pdn_connections.end(); ++it) {
    (*it)->deallocate_ressources(apn_in_use);
  }
  pdn_connections.clear();
  in_use   = false;
  apn_ambr = {0};
}
//------------------------------------------------------------------------------
std::string apn_context::toString() const {
  std::string s = {};
  s.append("APN CONTEXT:\n");
  s.append("\tIn use:\t\t\t\t").append(std::to_string(in_use)).append("\n");
  s.append("\tAPN:\t\t\t\t").append(apn_in_use).append("\n");
  s.append("\tAPN AMBR Bitrate Uplink:\t")
      .append(std::to_string(apn_ambr.br_ul))
      .append("\n");
  s.append("\tAPN AMBR Bitrate Downlink:\t")
      .append(std::to_string(apn_ambr.br_dl))
      .append("\n");
  for (auto it : pdn_connections) {
    s.append(it->toString());
  }
  return s;
}

//------------------------------------------------------------------------------
bool pgw_context::find_pdn_connection(
    const teid_t xgw_s5s8c_teid, const bool is_local_teid,
    pdn_duo_t& pdn_connection) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  for (auto ait : apns) {
    std::shared_ptr<pgw_pdn_connection> sp;
    if (ait->find_pdn_connection(xgw_s5s8c_teid, is_local_teid, sp)) {
      pdn_connection = make_pair(ait, sp);
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
bool pgw_context::find_pdn_connection(
    const pfcp::pdr_id_t& pdr_id, std::shared_ptr<pgw_pdn_connection>& pdn,
    ebi_t& ebi) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  for (auto ait : apns) {
    std::shared_ptr<pgw_pdn_connection> sp;
    if (ait->find_pdn_connection(pdr_id, sp, ebi)) {
      pdn = sp;  // May make pair
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
bool pgw_context::find_pdn_connection(
    const std::string& apn, const teid_t xgw_s5s8c_teid,
    const bool is_local_teid, pdn_duo_t& pdn_connection) {
  pdn_connection = {};
  std::unique_lock<std::recursive_mutex> lock(m_context);
  std::shared_ptr<apn_context> sa;
  if (find_apn_context(apn, sa)) {
    std::shared_ptr<pgw_pdn_connection> sp;
    if (sa.get()) {
      if (sa->find_pdn_connection(xgw_s5s8c_teid, is_local_teid, sp)) {
        // would need to make a pair of mutexes ?
        pdn_connection = make_pair(sa, sp);
        return true;
      }
    }
  }
  return false;
}
//------------------------------------------------------------------------------
void pgw_context::delete_apn_context(std::shared_ptr<apn_context>& sa) {
  if (sa.get()) {
    std::unique_lock<std::recursive_mutex> lock(m_context);
    for (std::vector<std::shared_ptr<apn_context>>::iterator ait = apns.begin();
         ait != apns.end(); ++ait) {
      // for (auto ait : apns) {
      if ((*ait).get() == sa.get()) {
        (*ait)->deallocate_ressources();
        apns.erase(ait);
        return;
      }
    }
  }
}
//------------------------------------------------------------------------------
void pgw_context::delete_pdn_connection(
    std::shared_ptr<apn_context>& sa, std::shared_ptr<pgw_pdn_connection>& sp) {
  if (sa.get()) {
    sa->delete_pdn_connection(sp);
    if (sa->get_num_pdn_connections() == 0) {
      delete_apn_context(sa);
    }
  }
}
//------------------------------------------------------------------------------
void pgw_context::insert_apn(std::shared_ptr<apn_context>& sa) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  apns.push_back(sa);
}
//------------------------------------------------------------------------------
bool pgw_context::find_apn_context(
    const std::string& apn, std::shared_ptr<apn_context>& apn_context) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  for (auto it : apns) {
    if (0 == apn.compare(it->apn_in_use)) {
      apn_context = it;
      return true;
    }
  }
  return false;
}
//------------------------------------------------------------------------------
void pgw_context::insert_procedure(std::shared_ptr<pgw_procedure>& sproc) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  pending_procedures.push_back(sproc);
}
//------------------------------------------------------------------------------
bool pgw_context::find_procedure(
    const uint64_t& trxn_id, std::shared_ptr<pgw_procedure>& proc) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  auto found = std::find_if(
      pending_procedures.begin(), pending_procedures.end(),
      [trxn_id](std::shared_ptr<pgw_procedure> const& i) -> bool {
        return i->trxn_id == trxn_id;
      });
  if (found != pending_procedures.end()) {
    proc = *found;
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
void pgw_context::remove_procedure(pgw_procedure* proc) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  auto found = std::find_if(
      pending_procedures.begin(), pending_procedures.end(),
      [proc](std::shared_ptr<pgw_procedure> const& i) {
        return i.get() == proc;
      });
  if (found != pending_procedures.end()) {
    pending_procedures.erase(found);
  }
}

//------------------------------------------------------------------------------
void pgw_context::handle_itti_msg(
    std::shared_ptr<itti_s5s8_create_session_request> s5_trigger) {
  itti_s5s8_create_session_request* csreq = s5_trigger.get();
  // If PCEF integrated in PGW, TODO create a procedure
  pdn_duo_t apn_pdn;
  bool found_pdn = find_pdn_connection(
      csreq->gtp_ies.apn.access_point_name, csreq->teid,
      IS_FIND_PDN_WITH_LOCAL_TEID, apn_pdn);
  std::shared_ptr<apn_context> sa        = apn_pdn.first;
  std::shared_ptr<pgw_pdn_connection> sp = apn_pdn.second;

  // The itti_s5s8_create_session_response will be built completly after PFCP
  // exchange (U plane may alloc U-FTEIDs)
  cause_t cause = {
      .cause_value = REQUEST_ACCEPTED, .pce = 0, .bce = 0, .cs = 0};
  itti_s5s8_create_session_response* s5s8 =
      new itti_s5s8_create_session_response(TASK_PGWC_APP, TASK_PGWC_S5S8);
  std::shared_ptr<itti_s5s8_create_session_response> s5_triggered_pending =
      std::shared_ptr<itti_s5s8_create_session_response>(s5s8);

  csreq->gtp_ies.get(imsi);
  indication_t indication = {};
  if (csreq->gtp_ies.get(indication)) {
    if (indication.uimsi) {
      imsi_unauthenticated_indicator = true;
    }
  }

  if (nullptr == sa.get()) {
    apn_context* a = new (apn_context);
    a->in_use      = true;
    a->apn_in_use  = csreq->gtp_ies.apn.access_point_name;
    if (csreq->gtp_ies.ie_presence_mask &
        GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN_AMBR) {
      a->apn_ambr = csreq->gtp_ies.ambr;
    }
    sa = std::shared_ptr<apn_context>(a);
    insert_apn(sa);
  } else {
    // TODO update ambr ?
  }
  //------
  // BEARER_CONTEXTS_TO_BE_CREATED
  //------
  if (nullptr == sp.get()) {
    pgw_pdn_connection* p = new (pgw_pdn_connection);
    if (not csreq->gtp_ies.get(p->pdn_type)) {
      // default
      p->pdn_type.pdn_type = PDN_TYPE_E_IPV4;
    }
    p->default_bearer =
        csreq->gtp_ies.bearer_contexts_to_be_created.at(0).eps_bearer_id;
    p->sgw_fteid_s5_s8_cp = csreq->gtp_ies.sender_fteid_for_cp;
    p->pgw_fteid_s5_s8_cp = pgw_app_inst->generate_s5s8_cp_fteid(
        pgwc::pgw_config::pgw_s5s8_.iface.addr4);
    pgw_app_inst->set_s5s8cpgw_fteid_2_pgw_context(
        p->pgw_fteid_s5_s8_cp, shared_from_this());
    sp = std::shared_ptr<pgw_pdn_connection>(p);
    sa->insert_pdn_connection(sp);
    // Ignore bearer context to be removed
  } else {
    // TODO bearer context to be removed
  }
  //  for (auto it : csreq->gtp_ies.bearer_contexts_to_be_created) {
  //    pgw_eps_bearer& eps_bearer = sp->get_eps_bearer(it.eps_bearer_id);
  //    eps_bearer.ebi = it.eps_bearer_id;
  //    eps_bearer.tft = it.tft;
  //    //eps_bearer.pgw_fteid_s5_s8_up =
  //    pgw_app_inst->generate_s5s8_up_fteid(pgw_cfg.s5s8_up.addr4);
  //    // Not now (no split SGW-PGW)
  //    //eps_bearer.sgw_fteid_s5_s8_up = it.s5_s8_u_sgw_fteid;
  //    eps_bearer.eps_bearer_qos = it.bearer_level_qos;
  //    sp->add_eps_bearer(eps_bearer);
  //
  //    bearer_context_created_within_create_session_response bcc = {};
  //    cause_t bcc_cause = {.cause_value = REQUEST_ACCEPTED, .pce = 0, .bce =
  //    0, .cs = 0}; bcc.set(eps_bearer.ebi); bcc.set(bcc_cause);
  //    bcc.set(eps_bearer.pgw_fteid_s5_s8_up, 2);
  //    // only if modified bcc.set(bearer_level_qos);
  //    s5s8->gtp_ies.add_bearer_context_created(bcc);
  //  }

  //  // TODO: remove this code if SPGW split (only for TAU/HO procedures)
  //  for (auto it : csreq->gtp_ies.bearer_contexts_to_be_removed) {
  //    pgw_eps_bearer&  eps_bearer = sp->get_eps_bearer(it.eps_bearer_id);
  //    if (eps_bearer == it.eps_bearer_id) {
  //      cause_t bcc_cause = {.cause_value = REQUEST_ACCEPTED, .pce = 0, .bce =
  //      0, .cs = 0};
  //      bearer_context_marked_for_removal_within_create_session_response bcc =
  //      {}; bcc.set(eps_bearer.ebi); bcc.set(bcc_cause);
  //      s5s8->gtp_ies.add_bearer_context_marked_for_removal(bcc);
  //      // remove the bearer
  //      sp->remove_eps_bearer(it.eps_bearer_id);
  //    }
  //
  //    bearer_context_marked_for_removal_within_create_session_response bcc =
  //    {}; cause_t bcc_cause = {.cause_value = REQUEST_ACCEPTED, .pce = 0, .bce
  //    = 0, .cs = 0}; bcc.set(it.eps_bearer_id); bcc.set(bcc_cause);
  //    s5s8->gtp_ies.add_bearer_context_marked_for_removal(bcc);
  //  }

  //------
  // PAA
  //------
  bool set_paa = false;
  paa_t paa    = {};
  //  if (cause.cause_value == REQUEST_ACCEPTED) {
  //    paa.pdn_type = sp->pdn_type;
  //    bool paa_res = csreq->gtp_ies.get(paa);
  //    if ((not paa_res) || (not paa.is_ip_assigned())) {
  //      bool success = paa_dynamic::get_instance().get_free_paa
  //      (sa->apn_in_use, paa);
  //      if (success) {
  //        set_paa = true;
  //      } else {
  //        cause.cause_value = PREFERRED_PDN_TYPE_NOT_SUPPORTED;
  //        cause.pce = 1;
  //      }
  //    }
  //  }

  //------
  // PCO
  //------
  protocol_configuration_options_t pco_resp    = {};
  protocol_configuration_options_ids_t pco_ids = {
      .pi_ipcp                                     = 0,
      .ci_dns_server_ipv4_address_request          = 0,
      .ci_ip_address_allocation_via_nas_signalling = 0,
      .ci_ipv4_address_allocation_via_dhcpv4       = 0,
      .ci_ipv4_link_mtu_request                    = 0};

  pgw_app_inst->process_pco_request(csreq->gtp_ies.pco, pco_resp, pco_ids);
  switch (sp->pdn_type.pdn_type) {
    case PDN_TYPE_E_IPV4: {
      // Use NAS by default if no preference is set.
      //
      // For context, the protocol configuration options (PCO) section of the
      // packet from the UE is optional, which means that it is perfectly valid
      // for a UE to send no PCO preferences at all. The previous logic only
      // allocates an IPv4 address if the UE has explicitly set the PCO
      // parameter for allocating IPv4 via NAS signaling (as opposed to via
      // DHCPv4). This means that, in the absence of either parameter being set,
      // the does not know what to do, so we need a default option as well.
      //
      // Since we only support the NAS signaling option right now, we will
      // default to using NAS signaling UNLESS we see a preference for DHCPv4.
      // This means that all IPv4 addresses are now allocated via NAS signaling
      // unless specified otherwise.
      //
      // In the long run, we will want to evolve the logic to use whatever
      // information we have to choose the ``best" allocation method. This means
      // adding new bitfields to pco_ids in pgw_pco.h, setting them in pgw_pco.c
      // and using them here in conditional logic. We will also want to
      // implement different logic between the PDN types.
      if (!pco_ids.ci_ipv4_address_allocation_via_dhcpv4) {
        bool paa_res = csreq->gtp_ies.get(paa);
        if ((not paa_res) || (not paa.is_ip_assigned())) {
          bool success =
              paa_dynamic::get_instance().get_free_paa(sa->apn_in_use, paa);
          if (success) {
            set_paa = true;
          } else {
            cause.cause_value = ALL_DYNAMIC_ADDRESSES_ARE_OCCUPIED;
            cause.pce         = 1;
          }
          // Static IP address allocation
        } else if ((paa_res) && (paa.is_ip_assigned())) {
          set_paa = true;
        }
      } else {
        // TODO allocation via DHCP
        Logger::pgwc_app().info("TODO PAA via DHCP");
      }
    } break;

    case PDN_TYPE_E_IPV6: {
      bool paa_res = csreq->gtp_ies.get(paa);
      if ((not paa_res) || (not paa.is_ip_assigned())) {
        bool success =
            paa_dynamic::get_instance().get_free_paa(sa->apn_in_use, paa);
        if (success) {
          set_paa = true;
        } else {
          cause.cause_value = ALL_DYNAMIC_ADDRESSES_ARE_OCCUPIED;
          cause.pce         = 1;
        }
        // Static IP address allocation
      } else if ((paa_res) && (paa.is_ip_assigned())) {
        set_paa = true;
      }
    } break;

    case PDN_TYPE_E_IPV4V6: {
      bool paa_res = csreq->gtp_ies.get(paa);
      if ((not paa_res) || (not paa.is_ip_assigned())) {
        bool success =
            paa_dynamic::get_instance().get_free_paa(sa->apn_in_use, paa);
        if (success) {
          set_paa = true;
        } else {
          cause.cause_value = ALL_DYNAMIC_ADDRESSES_ARE_OCCUPIED;
          cause.pce         = 1;
        }
      } else if ((paa_res) && (paa.is_ip_assigned())) {
        set_paa = true;
      }
    } break;

    case PDN_TYPE_E_NON_IP:
      cause.cause_value = PREFERRED_PDN_TYPE_NOT_SUPPORTED;
      cause.pce         = 1;
      break;

    default:
      Logger::pgwc_app().error("Unknown PDN type %d", sp->pdn_type.pdn_type);
      cause.cause_value = PREFERRED_PDN_TYPE_NOT_SUPPORTED;
      cause.pce         = 1;
      break;
  }

  //------
  // GTPV2C-Stack
  //------
  s5s8->gtpc_tx_id = csreq->gtpc_tx_id;
  s5s8->teid       = sp->sgw_fteid_s5_s8_cp.teid_gre_key;
  s5s8->r_endpoint = csreq->r_endpoint;

  //------
  // PAA, PCO, s5_s8_pgw_fteid
  //------
  s5s8->gtp_ies.set(cause);
  if (cause.cause_value == REQUEST_ACCEPTED) {
    if (set_paa) {
      s5s8->gtp_ies.set(paa);
      sp->set(paa);
    } else {
      // Valid PAA sent in CSR ?
      bool paa_res = csreq->gtp_ies.get(paa);
      if ((paa_res) && (paa.is_ip_assigned())) {
        sp->set(paa);
      }
    }
    s5s8->gtp_ies.set(pco_resp);
    s5s8->gtp_ies.set_s5_s8_pgw_fteid(sp->pgw_fteid_s5_s8_cp);
    // apn_restriction
    s5s8->gtp_ies.set(sa->apn_ambr);
    //  pgw_fq_csid = {};
    //  sgw_fq_csid = {};

    session_establishment_procedure* proc =
        new session_establishment_procedure(sp);
    std::shared_ptr<pgw_procedure> sproc = std::shared_ptr<pgw_procedure>(proc);
    insert_procedure(sproc);
    if (proc->run(s5_trigger, s5_triggered_pending, shared_from_this())) {
      // error !
      Logger::pgwc_app().info("S5S8 CREATE_SESSION_REQUEST procedure failed");
      remove_procedure(proc);
    }
  }

  // finally
  // cause was set in both cases
  if (s5s8->gtp_ies.cause.second.cause_value != REQUEST_ACCEPTED) {
    s5s8->gtp_ies.clear_pco();
    s5s8->gtp_ies.clear_apn_ambr();
    paa_t free_paa = {};
    if (s5s8->gtp_ies.get(free_paa)) {
      switch (sp->pdn_type.pdn_type) {
        case PDN_TYPE_E_IPV4:
        case PDN_TYPE_E_IPV4V6:
          paa_dynamic::get_instance().release_paa(
              sa->apn_in_use, free_paa.ipv4_address);
          break;

        case PDN_TYPE_E_IPV6:
        case PDN_TYPE_E_NON_IP:
        default:;
      }
      s5s8->gtp_ies.clear_paa();
    }
    for (auto it : csreq->gtp_ies.bearer_contexts_to_be_created) {
      gtpv2c::bearer_context_created_within_create_session_response bcc = {};
      cause_t bcc_cause                                                 = {
          .cause_value = NO_RESOURCES_AVAILABLE, .pce = 0, .bce = 0, .cs = 0};
      bcc.set(it.eps_bearer_id);
      bcc.set(bcc_cause);
      s5s8->gtp_ies.add_bearer_context_created(bcc);
    }

    Logger::pgwc_app().info(
        "Sending ITTI message %s to task TASK_PGWC_S5S8",
        s5_triggered_pending->get_msg_name());
    int ret = itti_inst->send_msg(s5_triggered_pending);
    if (RETURNok != ret) {
      Logger::pgwc_app().error(
          "Could not send ITTI message %s to task TASK_PGWC_S5S8",
          s5_triggered_pending->get_msg_name());
    }
  }
}
//------------------------------------------------------------------------------
void pgw_context::handle_itti_msg(
    std::shared_ptr<itti_s5s8_delete_session_request> s5_trigger) {
  itti_s5s8_delete_session_request* dsreq = s5_trigger.get();
  fteid_t sender_fteid                    = {};
  bool sender_fteid_present               = dsreq->gtp_ies.get(sender_fteid);

  pdn_duo_t apn_pdn                      = {};
  std::shared_ptr<apn_context> sa        = {};
  std::shared_ptr<pgw_pdn_connection> sp = {};

  bool found = false;
  if (sender_fteid_present) {
    found = find_pdn_connection(
        sender_fteid.teid_gre_key, IS_FIND_PDN_WITH_PEER_TEID, apn_pdn);
  } else {
    found =
        find_pdn_connection(dsreq->teid, IS_FIND_PDN_WITH_LOCAL_TEID, apn_pdn);
  }
  if (found) {
    sa = apn_pdn.first;
    sp = apn_pdn.second;

    if (sp.get()) {
      delete_session_procedure* proc =
          new delete_session_procedure(shared_from_this(), sp);
      std::shared_ptr<pgw_procedure> sproc =
          std::shared_ptr<pgw_procedure>(proc);
      insert_procedure(sproc);

      itti_s5s8_delete_session_response* s5s8 =
          new itti_s5s8_delete_session_response(TASK_PGWC_APP, TASK_PGWC_S5S8);
      std::shared_ptr<itti_s5s8_delete_session_response> s5_triggered_pending =
          std::shared_ptr<itti_s5s8_delete_session_response>(s5s8);
      //------
      // GTPV2C-Stack
      //------
      s5s8->gtpc_tx_id = s5_trigger->gtpc_tx_id;
      s5s8->teid       = sp->sgw_fteid_s5_s8_cp.teid_gre_key;
      s5s8->r_endpoint = s5_trigger->r_endpoint;

      if (proc->run(s5_trigger, s5_triggered_pending, shared_from_this())) {
        // error !
        Logger::pgwc_app().info("S5S8 DELETE_SESSION_REQUEST procedure failed");
        remove_procedure(proc);
      }
    }
  } else {
    if (sender_fteid_present) {
      pgw_app_inst->send_delete_session_response_cause_context_not_found(
          dsreq->gtpc_tx_id, sender_fteid.teid_gre_key, dsreq->r_endpoint);
    }
  }
  std::cout << toString() << std::endl;
}
//------------------------------------------------------------------------------
void pgw_context::handle_itti_msg(
    itti_s5s8_downlink_data_notification_acknowledge& ack) {
  std::shared_ptr<pgw_procedure> proc = {};
  if (find_procedure(ack.gtpc_tx_id, proc)) {
    Logger::pgwc_app().debug(
        "Received S5S8 DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE sender "
        "teid " TEID_FMT "  gtpc_tx_id " PROC_ID_FMT " ",
        ack.teid, ack.gtpc_tx_id);
    proc->handle_itti_msg(ack);
    remove_procedure(proc.get());
  } else {
    Logger::pgwc_app().debug(
        "Received S5S8 DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE sender "
        "teid " TEID_FMT "  gtpc_tx_id " PROC_ID_FMT
        ", pgw_procedure not found, discarded!",
        ack.teid, ack.gtpc_tx_id);
  }
}

//------------------------------------------------------------------------------
void pgw_context::handle_itti_msg(
    std::shared_ptr<itti_s5s8_modify_bearer_request> s5_trigger) {
  std::cout << toString() << std::endl;

  itti_s5s8_modify_bearer_request* mbreq = s5_trigger.get();
  fteid_t sender_fteid                   = {};
  bool sender_fteid_present              = mbreq->gtp_ies.get(sender_fteid);

  pdn_duo_t apn_pdn                      = {};
  std::shared_ptr<apn_context> sa        = {};
  std::shared_ptr<pgw_pdn_connection> sp = {};

  bool found = false;
  if (sender_fteid_present) {
    found = find_pdn_connection(
        sender_fteid.teid_gre_key, IS_FIND_PDN_WITH_PEER_TEID, apn_pdn);
  } else {
    found =
        find_pdn_connection(mbreq->teid, IS_FIND_PDN_WITH_LOCAL_TEID, apn_pdn);
  }
  if (found) {
    sa = apn_pdn.first;
    sp = apn_pdn.second;

    if (sp.get()) {
      modify_bearer_procedure* proc = new modify_bearer_procedure(sp);
      std::shared_ptr<pgw_procedure> sproc =
          std::shared_ptr<pgw_procedure>(proc);
      insert_procedure(sproc);

      itti_s5s8_modify_bearer_response* s5s8 =
          new itti_s5s8_modify_bearer_response(TASK_PGWC_APP, TASK_PGWC_S5S8);
      std::shared_ptr<itti_s5s8_modify_bearer_response> s5_triggered_pending =
          std::shared_ptr<itti_s5s8_modify_bearer_response>(s5s8);
      //------
      // GTPV2C-Stack
      //------
      s5s8->gtpc_tx_id = s5_trigger->gtpc_tx_id;
      s5s8->teid       = sp->sgw_fteid_s5_s8_cp.teid_gre_key;
      s5s8->r_endpoint = s5_trigger->r_endpoint;

      if (proc->run(s5_trigger, s5_triggered_pending, shared_from_this())) {
        // error !
        Logger::pgwc_app().info("S5S8 MODIFY_BEARER_REQUEST procedure failed");
        remove_procedure(proc);
      }
    }
  } else {
    if (sender_fteid_present) {
      pgw_app_inst->send_modify_bearer_response_cause_context_not_found(
          mbreq->gtpc_tx_id, sender_fteid.teid_gre_key, mbreq->r_endpoint);
    }
  }
  std::cout << toString() << std::endl;
}
//------------------------------------------------------------------------------
void pgw_context::handle_itti_msg(
    std::shared_ptr<itti_s5s8_release_access_bearers_request> s5_trigger) {
  pdn_duo_t apn_pdn                      = {};
  std::shared_ptr<apn_context> sa        = {};
  std::shared_ptr<pgw_pdn_connection> sp = {};

  bool found = find_pdn_connection(
      s5_trigger->teid, IS_FIND_PDN_WITH_LOCAL_TEID, apn_pdn);
  if (found) {
    sa = apn_pdn.first;
    sp = apn_pdn.second;

    if (sp) {
      if (not sp->released) {
        release_access_bearers_procedure* proc =
            new release_access_bearers_procedure(sp);
        std::shared_ptr<pgw_procedure> sproc =
            std::shared_ptr<pgw_procedure>(proc);
        insert_procedure(sproc);

        itti_s5s8_release_access_bearers_response* s5s8 =
            new itti_s5s8_release_access_bearers_response(
                TASK_PGWC_APP, TASK_PGWC_S5S8);
        std::shared_ptr<itti_s5s8_release_access_bearers_response>
            s5_triggered_pending =
                std::shared_ptr<itti_s5s8_release_access_bearers_response>(
                    s5s8);
        //------
        // GTPV2C-Stack
        //------
        s5s8->gtpc_tx_id = s5_trigger->gtpc_tx_id;
        s5s8->teid       = sp->sgw_fteid_s5_s8_cp.teid_gre_key;
        s5s8->r_endpoint = s5_trigger->r_endpoint;

        if (proc->run(s5_trigger, s5_triggered_pending, shared_from_this())) {
          // error !
          Logger::pgwc_app().info(
              "S5S8 RELEASE_ACCESS_BEARERS_REQUEST procedure failed");
          remove_procedure(proc);
        }
      } else {
        Logger::pgwc_app().warn(
            "S5S8 RELEASE_ACCESS_BEARERS_REQUEST procedure TODO");
        pgw_app_inst
            ->send_release_access_bearers_response_cause_request_accepted(
                s5_trigger->gtpc_tx_id, sp->sgw_fteid_s5_s8_cp.teid_gre_key,
                s5_trigger->r_endpoint);
      }
    }
  } else {
    Logger::pgwc_app().info(
        "S5S8 RELEASE_ACCESS_BEARERS_REQUEST procedure failed, context not "
        "found");
    pgw_app_inst->send_release_access_bearers_response_cause_context_not_found(
        s5_trigger->gtpc_tx_id, sp->sgw_fteid_s5_s8_cp.teid_gre_key,
        s5_trigger->r_endpoint);
  }
}

//------------------------------------------------------------------------------
void pgw_context::handle_itti_msg(
    itti_sxab_session_establishment_response& seresp) {
  std::shared_ptr<pgw_procedure> proc = {};
  if (find_procedure(seresp.trxn_id, proc)) {
    Logger::pgwc_app().debug(
        "Received SXAB SESSION ESTABLISHMENT RESPONSE sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 "\n",
        seresp.seid, seresp.trxn_id);
    proc->handle_itti_msg(seresp);
    remove_procedure(proc.get());
  } else {
    Logger::pgwc_app().debug(
        "Received SXAB SESSION ESTABLISHMENT RESPONSE sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", pgw_procedure not found, discarded!",
        seresp.seid, seresp.trxn_id);
  }
}
//------------------------------------------------------------------------------
void pgw_context::handle_itti_msg(
    itti_sxab_session_modification_response& smresp) {
  std::shared_ptr<pgw_procedure> proc = {};
  if (find_procedure(smresp.trxn_id, proc)) {
    Logger::pgwc_app().debug(
        "Received SXAB SESSION MODIFICATION RESPONSE sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 "\n",
        smresp.seid, smresp.trxn_id);
    proc->handle_itti_msg(smresp);
    remove_procedure(proc.get());
  } else {
    Logger::pgwc_app().debug(
        "Received SXAB SESSION MODIFICATION RESPONSE sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", pgw_procedure not found, discarded!",
        smresp.seid, smresp.trxn_id);
  }
  std::cout << toString() << std::endl;
}
//------------------------------------------------------------------------------
void pgw_context::handle_itti_msg(itti_sxab_session_deletion_response& sdresp) {
  std::shared_ptr<pgw_procedure> proc = {};
  if (find_procedure(sdresp.trxn_id, proc)) {
    Logger::pgwc_app().debug(
        "Received SXAB SESSION DELETION RESPONSE sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 "\n",
        sdresp.seid, sdresp.trxn_id);
    proc->handle_itti_msg(sdresp);
    remove_procedure(proc.get());
  } else {
    Logger::pgwc_app().debug(
        "Received SXAB SESSION DELETION RESPONSE sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", pgw_procedure not found, discarded!",
        sdresp.seid, sdresp.trxn_id);
  }
  std::cout << toString() << std::endl;
}
//------------------------------------------------------------------------------
void pgw_context::handle_itti_msg(
    std::shared_ptr<itti_sxab_session_report_request>& req) {
  pfcp::report_type_t report_type;
  if (req->pfcp_ies.get(report_type)) {
    pfcp::pdr_id_t pdr_id;
    // Downlink Data Report
    if (report_type.dldr) {
      pfcp::downlink_data_report data_report;
      if (req->pfcp_ies.get(data_report)) {
        pfcp::pdr_id_t pdr_id;
        if (data_report.get(pdr_id)) {
          std::shared_ptr<pgw_pdn_connection> ppc = {};
          ebi_t ebi;
          if (find_pdn_connection(pdr_id, ppc, ebi)) {
            downlink_data_report_procedure* p =
                new downlink_data_report_procedure(req);
            std::shared_ptr<pgw_procedure> sproc =
                std::shared_ptr<pgw_procedure>(p);
            insert_procedure(sproc);
            if (p->run(shared_from_this(), ppc, ebi)) {
              // TODO handle error code
              Logger::pgwc_app().info(
                  "S5S8 DOWNLINK_DATA_REPORT procedure failed");
              remove_procedure(p);
            } else {
            }
          }
        }
      }
    }
    // Usage Report
    if (report_type.usar) {
      Logger::pgwc_app().debug("TODO PFCP_SESSION_REPORT_REQUEST/Usage Report");
    }
    // Error Indication Report
    if (report_type.erir) {
      Logger::pgwc_app().debug(
          "TODO PFCP_SESSION_REPORT_REQUEST/Error Indication Report");
    }
    // User Plane Inactivity Report
    if (report_type.upir) {
      Logger::pgwc_app().debug(
          "TODO PFCP_SESSION_REPORT_REQUEST/User Plane Inactivity Report");
    }
  }
}

//------------------------------------------------------------------------------
std::string pgw_context::toString() const {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  std::string s = {};
  s.append("PGW CONTEXT:\n");
  s.append("\tIMSI:\t\t\t\t").append(imsi.toString()).append("\n");
  s.append("\tIMSI UNAUTHENTICATED:\t\t")
      .append(std::to_string(imsi_unauthenticated_indicator))
      .append("\n");
  for (auto it : apns) {
    s.append(it->toString());
  }

  // s.append("\tIMSI:\t"+toString(p.msisdn));
  // apns.reserve(MAX_APN_PER_UE);
  return s;
}
