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

/*! \file sgw_eps_bearer_context.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "sgwc_eps_bearer_context.hpp"
#include "3gpp_29.274.h"
#include "sgwc_app.hpp"
#include "pgw_config.hpp"

#include <algorithm>

using namespace sgwc;
using namespace std;

extern sgwc_app* sgwc_app_inst;
extern pgwc::pgw_config pgw_cfg;

//------------------------------------------------------------------------------
std::string sgw_eps_bearer::toString() const {
  std::string s = {};
  s.append("EPS BEARER:\n");
  s.append("\tEBI:\t\t\t\t").append(std::to_string(ebi.ebi)).append("\n");
  s.append("\tTFT:\t\t\t\tTODO tft").append("\n");
  s.append("\tPGW FTEID S5S8 UP:\t\t")
      .append(pgw_fteid_s5_s8_up.toString())
      .append("\n");
  s.append("\tSGW FTEID S5S8 UP:\t\t")
      .append(sgw_fteid_s5_s8_up.toString())
      .append("\n");
  s.append("\tSGW FTEID S1S12S4 UP:\t\t")
      .append(sgw_fteid_s1u_s12_s4u_s11u.toString())
      .append("\n");
  s.append("\tSGW FTEID S11 UP:\t\t")
      .append(sgw_fteid_s11u.toString())
      .append("\n");
  s.append("\tMME FTEID S11 UP:\t\t")
      .append(mme_fteid_s11u.toString())
      .append("\n");
  s.append("\teNB FTEID S1 UP:\t\t")
      .append(enb_fteid_s1u.toString())
      .append("\n");
  s.append("\tBearer QOS:\t\t\t")
      .append(eps_bearer_qos.toString())
      .append("\n");
  return s;
}
//------------------------------------------------------------------------------
void sgw_eps_bearer::deallocate_ressources() {
  //  Logger::sgwc_app().info( "TODO remove_eps_bearer(%d) OpenFlow", ebi.ebi);
  //  if (not is_fteid_zero(sgw_fteid_s5_s8_up))
  //    sgwc_app_inst->free_s5s8_up_fteid(sgw_fteid_s5_s8_up);
  // if (not is_fteid_zero(sgw_fteid_s1u_s12_s4u_s11u))
  //  sgwc_app_inst->free_s1s12s4s11_up_fteid(sgw_fteid_s1u_s12_s4u_s11u);
}

//------------------------------------------------------------------------------
void sgw_eps_bearer_context::release() {
  // pending_procedures
  pending_procedures.clear();
  // pdn connections
  for (auto it = pdn_connections.begin(); it != pdn_connections.end(); ++it) {
    std::shared_ptr<sgw_pdn_connection> sp = it->second;
    delete_pdn_connection(sp);
  }
}

//------------------------------------------------------------------------------
void sgw_eps_bearer_context::erase_pdn_connection(
    std::shared_ptr<sgw_pdn_connection> spc) {
  kpdn_t k = std::make_pair<std::string, uint8_t>(
      std::string(spc.get()->apn_in_use),
      (uint8_t) spc.get()->pdn_type.pdn_type);
  std::size_t size = pdn_connections.erase(k);
  Logger::sgwc_app().trace(
      "erase_pdn_connection(%s,%d) %d erased", spc.get()->apn_in_use.c_str(),
      spc.get()->pdn_type.pdn_type, size);
}

//------------------------------------------------------------------------------
shared_ptr<sgw_pdn_connection> sgw_eps_bearer_context::insert_pdn_connection(
    sgw_pdn_connection* p) {
  kpdn_t k(p->apn_in_use, (uint8_t) p->pdn_type.pdn_type);
  shared_ptr<sgw_pdn_connection> s = shared_ptr<sgw_pdn_connection>(p);
  std::pair<std::map<kpdn_t, shared_ptr<sgw_pdn_connection>>::iterator, bool>
      ret;
  ret = pdn_connections.insert(
      std::pair<kpdn_t, shared_ptr<sgw_pdn_connection>>(k, s));
  if (ret.second == false) {
    Logger::sgwc_app().error(
        "insert_pdn_connection(%s,%d) failed", ret.first->first.first.c_str(),
        (int) ret.first->first.second);
  } else {
    Logger::sgwc_app().trace(
        "insert_pdn_connection(%s,%d) succeed key(%s,%d)",
        ret.first->first.first.c_str(), (int) ret.first->first.second,
        k.first.c_str(), (int) k.second);
  }
  return s;
}
//------------------------------------------------------------------------------
bool sgw_eps_bearer_context::find_pdn_connection(
    const std::string& apn, const pdn_type_t pdn_type,
    std::shared_ptr<sgw_pdn_connection>& sp) {
  kpdn_t k(apn, pdn_type.pdn_type);
  std::map<kpdn_t, shared_ptr<sgw_pdn_connection>>::iterator it;

  it = pdn_connections.find(k);
  if (it != pdn_connections.end()) {
    sp = it->second;
    return true;
  }
  sp = {};
  return false;
}
//------------------------------------------------------------------------------
bool sgw_eps_bearer_context::find_pdn_connection(
    const ebi_t& ebi, std::shared_ptr<sgw_pdn_connection>& sp) {
  for (auto it = pdn_connections.begin(); it != pdn_connections.end(); ++it) {
    if (it->second.get()->default_bearer == ebi) {
      sp = it->second;
      return true;
    }
    std::shared_ptr<sgw_eps_bearer> seb;
    if (it->second.get()->get_eps_bearer(ebi, seb)) {
      if (seb.get()) {
        sp = it->second;
        return true;
      }
    }
  }
  sp = {};
  return false;
}

//------------------------------------------------------------------------------
void sgw_eps_bearer_context::create_procedure(
    itti_s11_create_session_request& csreq) {
  create_session_request_procedure* p =
      new create_session_request_procedure(csreq);
  insert_procedure(p);
  if (p->run(sgwc_app_inst->s11sgw_teid_2_sgw_eps_bearer_context(
          this->sgw_fteid_s11_s4_cp.teid_gre_key))) {
    // TODO handle error code
    Logger::sgwc_app().info("S11 CREATE_SESSION_REQUEST procedure failed");
    remove_procedure(p);
  } else {
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::create_procedure(
    itti_s11_modify_bearer_request& mbreq) {
  // Assuming actually that only one pdn_connection per modify bearer request
  if (mbreq.gtp_ies.has_bearer_context_to_be_modified() ||
      mbreq.gtp_ies.has_bearer_context_to_be_removed()) {
    modify_bearer_request_procedure* p =
        new modify_bearer_request_procedure(mbreq);
    insert_procedure(p);
    int rc = p->run(sgwc_app_inst->s11sgw_teid_2_sgw_eps_bearer_context(
        this->sgw_fteid_s11_s4_cp.teid_gre_key));
    switch (rc) {
      case RETURNerror:
        // TODO handle error code
        Logger::sgwc_app().info("S11 MODIFY_BEARER_REQUEST procedure failed");
      case RETURNclear:
        remove_procedure(p);
        break;
      case RETURNok:
      default:;
    }
  } else {
    Logger::sgwc_app().error(
        "S11 MODIFY_BEARER_REQUEST not bearer context to be modified or to be "
        "removed found, should be handled by GTPV2-C stack, silently "
        "discarded!");
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::create_procedure(
    itti_s11_release_access_bearers_request& rabreq) {
  // Assuming actually that only one pdn_connection per modify bearer request
  release_access_bearers_request_procedure* p =
      new release_access_bearers_request_procedure(rabreq);
  insert_procedure(p);
  int rc = p->run(sgwc_app_inst->s11sgw_teid_2_sgw_eps_bearer_context(
      this->sgw_fteid_s11_s4_cp.teid_gre_key));
  switch (rc) {
    case RETURNerror:
      // TODO handle error code
      Logger::sgwc_app().info(
          "S11 RELEASE_ACCESS_BEARERS_REQUEST procedure failed");
    case RETURNclear:
      remove_procedure(p);
      break;
    case RETURNok:
    default:;
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::create_procedure(
    itti_s11_delete_session_request& dsreq) {
  std::vector<shared_ptr<sgw_pdn_connection>> to_delete =
      {};  // list of PDN connections to delete
  ebi_t default_bearer = {};
  if (dsreq.gtp_ies.get(default_bearer)) {
    shared_ptr<sgw_pdn_connection> pdn;
    if (find_pdn_connection(default_bearer, pdn)) {
      if (pdn.get()) {
        to_delete.push_back(pdn);
      } else {
        // TODO return error
        Logger::sgwc_app().info(
            "S11 DELETE_SESSION_REQUEST PDN connection null, discarded!");
      }
    } else {
      Logger::sgwc_app().info(
          "S11 DELETE_SESSION_REQUEST PDN connection not found, discarded!");
    }
  } else {
    // Normally should be only one PDN connection
    // TODO decide what todo (reject request, delete all, ?)
    for (auto it = pdn_connections.begin(); it != pdn_connections.end(); ++it) {
      to_delete.push_back(it->second);
    }
  }

  for (auto it = to_delete.begin(); it != to_delete.end(); ++it) {
    delete_session_request_procedure* p =
        new delete_session_request_procedure(dsreq, *it);
    insert_procedure(p);
    if (p->run(shared_from_this())) {
      Logger::sgwc_app().info("S11 DELETE_SESSION_REQUEST procedure failed");
      // TODO handle error code
      remove_procedure(p);
    }
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::create_procedure(
    itti_s5s8_downlink_data_notification& ddn,
    std::shared_ptr<sgw_pdn_connection> spc) {
  downlink_data_notification_procedure* p =
      new downlink_data_notification_procedure(ddn);
  insert_procedure(p);
  int rc = p->run(shared_from_this(), spc);
  switch (rc) {
    case RETURNerror:
      // TODO handle error code
      Logger::sgwc_app().info(
          "S5S8 DOWNLINK_DATA_NOTIFICATION procedure failed");
    case RETURNclear:
      remove_procedure(p);
      break;
    case RETURNok:
    default:;
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::insert_procedure(sebc_procedure* proc) {
  pending_procedures.push_back(shared_ptr<sebc_procedure>(proc));
}
//------------------------------------------------------------------------------
shared_ptr<sebc_procedure> sgw_eps_bearer_context::find_procedure(
    const uint64_t& gtpc_tx_id) {
  auto found = std::find_if(
      pending_procedures.begin(), pending_procedures.end(),
      [gtpc_tx_id](std::shared_ptr<sebc_procedure> const& i) -> bool {
        return i.get()->has_trxn_id(gtpc_tx_id);
      });
  if (found != pending_procedures.end()) {
    return *found;
  }
  return shared_ptr<sebc_procedure>(nullptr);
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::remove_procedure(sebc_procedure* proc) {
  auto found = std::find_if(
      pending_procedures.begin(), pending_procedures.end(),
      [proc](std::shared_ptr<sebc_procedure> const& i) {
        return i.get() == proc;
      });
  if (found != pending_procedures.end()) {
    pending_procedures.erase(found);
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::delete_pdn_connection(
    std::shared_ptr<sgw_pdn_connection> spc) {
  if (spc.get()) {
    Logger::sgwc_app().debug(
        "sgw_eps_bearer_context::delete_pdn_connection() OK doing it");
    sgwc_app_inst->delete_s5s8sgw_teid_2_sgw_contexts(
        spc->sgw_fteid_s5_s8_cp.teid_gre_key);
    erase_pdn_connection(spc);
    spc.get()->deallocate_ressources();
    spc.get()->delete_bearers();
  }
}

//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s11_create_session_request& csreq) {
  if (sgw_fteid_s11_s4_cp.teid_gre_key == UNASSIGNED_TEID) {
    sgw_fteid_s11_s4_cp =
        sgwc_app_inst->generate_s11_cp_fteid(pgw_cfg.s11_.iface.addr4);
    sgwc_app_inst->set_s11sgw_teid_2_sgw_eps_bearer_context(
        sgw_fteid_s11_s4_cp.teid_gre_key, shared_from_this());
    mme_fteid_s11 = csreq.gtp_ies.sender_fteid_for_cp;
    imsi          = csreq.gtp_ies.imsi;
  } else {
    if (not(mme_fteid_s11 == csreq.gtp_ies.sender_fteid_for_cp)) {
      Logger::sgwc_app().debug(
          "S11 CREATE_SESSION_REQUEST MME S11 FTEID changed");
      mme_fteid_s11 = csreq.gtp_ies.sender_fteid_for_cp;
    }
  }
  create_procedure(csreq);
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s11_modify_bearer_request& mbreq) {
  shared_ptr<sebc_procedure> sp = find_procedure(mbreq.gtpc_tx_id);
  if (sp.get()) {
    Logger::sgwc_app().error(
        "S11 MODIFY_BEARER_REQUEST ignored, existing procedure found "
        "gtpc_tx_id %d!",
        mbreq.gtpc_tx_id);
    return;
  } else {
    create_procedure(mbreq);
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s11_release_access_bearers_request& rabreq) {
  shared_ptr<sebc_procedure> sp = find_procedure(rabreq.gtpc_tx_id);
  if (sp.get()) {
    Logger::sgwc_app().error(
        "S11 RELEASE_ACCESS_BEARERS_REQUEST ignored, existing procedure found "
        "gtpc_tx_id " PROC_ID_FMT "",
        rabreq.gtpc_tx_id);
    return;
  } else {
    create_procedure(rabreq);
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s11_delete_session_request& dsreq) {
  shared_ptr<sebc_procedure> sp = find_procedure(dsreq.gtpc_tx_id);
  if (sp.get()) {
    Logger::sgwc_app().error(
        "S11 DELETE_SESSION_REQUEST ignored, existing procedure found "
        "gtpc_tx_id " PROC_ID_FMT "!",
        dsreq.gtpc_tx_id);
    return;
  } else {
    indication_t indication = {};
    if (dsreq.gtp_ies.get(indication)) {
      if (indication.oi) {
        create_procedure(dsreq);
        return;
      }
    } else {
      Logger::sgwc_app().trace(
          "S11 DELETE_SESSION_REQUEST indication.u1.b = %d");
    }
    // TODO DELETE SESSION locally
    Logger::sgwc_app().info(
        "S11 DELETE_SESSION_REQUEST TODO delete session locally");
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s11_downlink_data_notification_acknowledge& ddn) {
  shared_ptr<sebc_procedure> sp = find_procedure(ddn.gtpc_tx_id);
  if (sp.get()) {
    sp->handle_itti_msg(ddn);
    return;
  } else {
    Logger::sgwc_app().error(
        "S11 DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE ignored, no procedure "
        "found gtpc_tx_id " PROC_ID_FMT "!",
        ddn.gtpc_tx_id);
  }
}

//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s5s8_create_session_response& csresp,
    std::shared_ptr<sgw_pdn_connection> spc) {
  shared_ptr<sebc_procedure> sp = find_procedure(csresp.gtpc_tx_id);
  if (sp.get()) {
    dynamic_pointer_cast<create_session_request_procedure>(sp)->handle_itti_msg(
        csresp, shared_from_this(), spc);
    if (sp.get()->marked_for_removal) {
      remove_procedure(sp.get());
    }
  } else {
    Logger::sgwc_app().debug(
        "S5S8 CREATE_SESSION_RESPONSE ignored, no procedure found "
        "gtpc_tx_id " PROC_ID_FMT "!",
        csresp.gtpc_tx_id);
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s5s8_modify_bearer_response& resp,
    std::shared_ptr<sgw_pdn_connection> spc) {
  shared_ptr<sebc_procedure> sp = find_procedure(resp.gtpc_tx_id);
  if (sp.get()) {
    dynamic_pointer_cast<modify_bearer_request_procedure>(sp)->handle_itti_msg(
        resp, shared_from_this(), spc);
    if (sp.get()->marked_for_removal) {
      remove_procedure(sp.get());
    }
  } else {
    Logger::sgwc_app().debug(
        "S5S8 MODIFY_BEARER_RESPONSE ignored, no procedure found "
        "gtpc_tx_id " PROC_ID_FMT "!",
        resp.gtpc_tx_id);
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s5s8_release_access_bearers_response& resp,
    std::shared_ptr<sgw_pdn_connection> spc) {
  shared_ptr<sebc_procedure> sp = find_procedure(resp.gtpc_tx_id);
  if (sp.get()) {
    dynamic_pointer_cast<release_access_bearers_request_procedure>(sp)
        ->handle_itti_msg(resp, shared_from_this(), spc);
    if (sp.get()->marked_for_removal) {
      remove_procedure(sp.get());
    }
  } else {
    Logger::sgwc_app().debug(
        "S5S8 RELEASE_ACCESS_BEARERS_RESPONSE ignored, no procedure found "
        "gtpc_tx_id " PROC_ID_FMT "!",
        resp.gtpc_tx_id);
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s5s8_remote_peer_not_responding& resp,
    std::shared_ptr<sgw_pdn_connection> spc) {
  shared_ptr<sebc_procedure> sp = find_procedure(resp.gtpc_tx_id);
  if (sp.get()) {
    sp->handle_itti_msg(resp, shared_from_this(), spc);
    remove_procedure(sp.get());
  } else {
    Logger::sgwc_app().debug(
        "S5S8 RELEASE_ACCESS_BEARERS_RESPONSE ignored, no procedure found "
        "gtpc_tx_id " PROC_ID_FMT "!",
        resp.gtpc_tx_id);
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s5s8_delete_session_response& dsresp,
    std::shared_ptr<sgw_pdn_connection> spc) {
  shared_ptr<sebc_procedure> sp = find_procedure(dsresp.gtpc_tx_id);
  if (sp.get()) {
    Logger::sgwc_app().debug(
        "S5S8 DELETE_SESSION_RESPONSE , procedure gtpc_tx_id %d found ",
        dsresp.gtpc_tx_id);
    dynamic_pointer_cast<delete_session_request_procedure>(sp)->handle_itti_msg(
        dsresp, shared_from_this(), spc);
    if (sp.get()->marked_for_removal) {
      remove_procedure(sp.get());
    }
  } else {
    Logger::sgwc_app().debug(
        "S5S8 DELETE_SESSION_RESPONSE ignored, no procedure found "
        "gtpc_tx_id " PROC_ID_FMT "!",
        dsresp.gtpc_tx_id);
  }
}
//------------------------------------------------------------------------------
void sgw_eps_bearer_context::handle_itti_msg(
    itti_s5s8_downlink_data_notification& ddn,
    std::shared_ptr<sgw_pdn_connection> spc) {
  shared_ptr<sebc_procedure> sp = find_procedure(ddn.gtpc_tx_id);
  if (sp.get()) {
    Logger::sgwc_app().error(
        "S5S8 DOWNLINK_DATA_NOTIFICATION ignored, existing procedure found "
        "gtpc_tx_id %d!",
        ddn.gtpc_tx_id);
    return;
  } else {
    create_procedure(ddn, spc);
  }
}
//------------------------------------------------------------------------------
std::string sgw_eps_bearer_context::toString() const {
  std::string s = {};
  s.append("SGW EPS BEARER CONTEXT:\n");
  s.append("\tIMSI:\t\t\t").append(imsi.toString()).append("\n");
  s.append("\tIMSI UNAUTHENTICATED:\t")
      .append(std::to_string(imsi_unauthenticated_indicator))
      .append("\n");
  s.append("\tMME FTEID S11 CP:\t")
      .append(mme_fteid_s11.toString())
      .append("\n");
  s.append("\tSGW FTEID S11 CP:\t")
      .append(sgw_fteid_s11_s4_cp.toString())
      .append("\n");
  // s.append("\tSGSN FTEID S4
  // CP:\t").append(toString(sgsn_fteid_s4_cp)).append("\n");
  s.append("\tLAST KNOWN CELL ID:\t")
      .append(last_known_cell_Id.toString())
      .append("\n");
  for (auto it : pdn_connections) {
    s.append(it.second.get()->toString());
  }
  return s;
}
