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

/*! \file sgw_pdn_connection.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "sgwc_app.hpp"
#include "sgwc_eps_bearer_context.hpp"

#include <algorithm>

using namespace sgwc;
using namespace std;

extern sgwc_app* sgwc_app_inst;

//------------------------------------------------------------------------------
void sgw_pdn_connection::add_eps_bearer(std::shared_ptr<sgw_eps_bearer> sb) {
  if (sb.get()) {
    if ((sb->ebi.ebi >= EPS_BEARER_IDENTITY_FIRST) and
        (sb->ebi.ebi <= EPS_BEARER_IDENTITY_LAST)) {
      sgw_eps_bearers.insert(
          std::pair<uint8_t, std::shared_ptr<sgw_eps_bearer>>(sb->ebi.ebi, sb));
      Logger::sgwc_app().trace(
          "sgw_pdn_connection::add_eps_bearer(%d) success", sb->ebi.ebi);
    } else {
      Logger::sgwc_app().error(
          "sgw_pdn_connection::add_eps_bearer(%d) failed, invalid EBI",
          sb->ebi.ebi);
    }
  }
}
//------------------------------------------------------------------------------
bool sgw_pdn_connection::get_eps_bearer(
    const ebi_t& ebi, std::shared_ptr<sgw_eps_bearer>& b) {
  if (sgw_eps_bearers.count(ebi.ebi)) {
    b = sgw_eps_bearers.at(ebi.ebi);
    return true;
  }
  b = {};
  return false;
}
//------------------------------------------------------------------------------
bool sgw_pdn_connection::update_eps_bearer(
    const gtpv2c::bearer_context_modified_within_modify_bearer_response& b) {
  ebi_t ebi = {};
  if (b.get(ebi)) {
    std::shared_ptr<sgw_eps_bearer> sb = {};
    if (get_eps_bearer(ebi, sb)) {
      return sb->update(b);
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void sgw_pdn_connection::remove_eps_bearer(const ebi_t& ebi) {
  std::shared_ptr<sgw_eps_bearer> sb = {};
  if (get_eps_bearer(ebi, sb)) {
    if (sb.get()) {
      sb->deallocate_ressources();
      sgw_eps_bearers.erase(ebi.ebi);
    }
  }
}
//------------------------------------------------------------------------------
void sgw_pdn_connection::remove_eps_bearer(std::shared_ptr<sgw_eps_bearer> sb) {
  if (sb.get()) {
    ebi_t ebi = {.ebi = sb->ebi.ebi};
    sb->deallocate_ressources();
    sgw_eps_bearers.erase(ebi.ebi);
  }
}
//------------------------------------------------------------------------------
void sgw_pdn_connection::delete_bearers() {
  sgw_eps_bearers.clear();
}

//------------------------------------------------------------------------------
void sgw_pdn_connection::deallocate_ressources() {
  Logger::sgwc_app().error("TODO sgw_pdn_connection::deallocate_ressources()");
  for (auto it : sgw_eps_bearers) {
    it.second->deallocate_ressources();
  }
}
//------------------------------------------------------------------------------
std::string sgw_pdn_connection::toString() const {
  std::string s = {};
  s.reserve(300);
  s.append("PDN CONNECTION:\n");
  s.append("\tAPN IN USE:\t\t").append(apn_in_use).append("\n");
  s.append("\tPDN TYPE:\t\t").append(pdn_type.toString()).append("\n");
  s.append("\tPGW FTEID S5S8 CP:\t")
      .append(pgw_fteid_s5_s8_cp.toString())
      .append("\n");
  // s.append("\tPGW ADDRESS IN USE
  // UP:\t").append(toString(pgw_address_in_use_up)).append("\n");
  s.append("\tSGW FTEID S5S8 CP:\t")
      .append(sgw_fteid_s5_s8_cp.toString())
      .append("\n");
  s.append("\tDEFAULT BEARER:\t\t")
      .append(std::to_string(default_bearer.ebi))
      .append("\n");
  for (auto it : sgw_eps_bearers) {
    s.append(it.second->toString());
  }
  return s;
}
