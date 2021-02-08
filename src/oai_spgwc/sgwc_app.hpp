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

/*! \file sgwc_app.hpp
   \author  Lionel GAUTHIER
   \date 2018
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_SGWC_APP_HPP_SEEN
#define FILE_SGWC_APP_HPP_SEEN

#include "common_root_types.h"

#include "itti_msg_s11.hpp"
#include "itti_msg_s5s8.hpp"
#include "sgwc_eps_bearer_context.hpp"

#include <boost/atomic.hpp>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <thread>

namespace sgwc {

class sgwc_app {
 private:
  std::thread::id thread_id;
  std::thread thread;

  // teid generators (linear)
  boost::atomic<teid_t> teid_s11_cp;
  boost::atomic<teid_t> teid_s5s8_cp;
  /* There shall be only one pair of TEID-C per UE over the S11 and the S4
     interfaces. The same tunnel shall be shared for the control messages
     related to the same UE operation. A TEID-C on the S11/S4 interface shall be
     released after all its associated EPS bearers are deleted.*/
  std::map<imsi64_t, std::shared_ptr<sgw_eps_bearer_context>>
      imsi2sgw_eps_bearer_context;
  std::map<teid_t, std::shared_ptr<sgw_eps_bearer_context>>
      s11lteid2sgw_eps_bearer_context;

  std::map<
      teid_t, std::pair<
                  std::shared_ptr<sgw_eps_bearer_context>,
                  std::shared_ptr<sgw_pdn_connection>>>
      s5s8lteid2sgw_contexts;

  teid_t generate_s11_cp_teid();
  bool is_s11c_teid_exist(const teid_t& teid_s11_cp) const;

  bool is_s5s8sgw_teid_2_sgw_contexts(const teid_t& sgw_teid) const;

  // s11lteid2sgw_eps_bearer_context collection
  bool is_s11sgw_teid_2_sgw_eps_bearer_context(const teid_t& sgw_teid) const;

  bool is_imsi64_2_sgw_eps_bearer_context(const imsi64_t& imsi64) const;
  void set_imsi64_2_sgw_eps_bearer_context(
      const imsi64_t& imsi64, std::shared_ptr<sgw_eps_bearer_context> sebc);

  teid_t generate_s5s8_cp_teid();
  bool is_s5s8c_teid_exist(const teid_t& teid_s5s8_cp) const;

  bool is_s5s8u_teid_exist(const teid_t& teid_s5s8_up) const;

 public:
  // key is S11 S-GW local teid, value is S11 tunnel id pair
  // map<teid_t, int> s11teid2mme_hashtable;

  // key is paa, value is S11 s-gw local teid
  // obj_hash_table_uint64_t *ip2s11teid;

  // key is S1-U S-GW local teid
  ////hash_table_t *s1uteid2enb_hashtable;

  // the key of this hashtable is the S11 s-gw local teid.
  // hash_table_ts_t *s11_bearer_context_information_hashtable;

  /* The TEID-C shall be unique per PDN-Connection on GTP based S2a, S2b, S5 and
     S8 interfaces. The same tunnel shall be shared for the control messages
     related to all bearers associated to the PDN-Connection. A TEID-C on the
     S2a/S2b/S5/S8 interface shall be released after all its associated EPS
     bearers are deleted. */

  // gtpv1u_data_t    gtpv1u_data;

  explicit sgwc_app(const std::string& config_file);
  ~sgwc_app();
  sgwc_app(sgwc_app const&) = delete;
  void operator=(sgwc_app const&) = delete;

  void send_create_session_response_cause(
      const uint64_t gtpc_tx_id, const teid_t teid, const endpoint& r_endpoint,
      const cause_t& cause) const;

  fteid_t generate_s5s8_cp_fteid(const struct in_addr ipv4_address);
  fteid_t generate_s11_cp_fteid(const struct in_addr ipv4_address);
  std::pair<
      std::shared_ptr<sgw_eps_bearer_context>,
      std::shared_ptr<sgw_pdn_connection>>
  s5s8sgw_teid_2_sgw_contexts(const teid_t& sgw_teid) const;
  std::shared_ptr<sgw_eps_bearer_context> s11sgw_teid_2_sgw_eps_bearer_context(
      const teid_t& sgw_teid) const;
  void set_s11sgw_teid_2_sgw_eps_bearer_context(
      const teid_t& sgw_teid, std::shared_ptr<sgw_eps_bearer_context> sebc);
  void delete_sgw_eps_bearer_context(
      std::shared_ptr<sgw_eps_bearer_context> sebc);
  std::shared_ptr<sgw_eps_bearer_context> imsi64_2_sgw_eps_bearer_context(
      const imsi64_t& imsi64) const;
  void set_s5s8sgw_teid_2_sgw_contexts(
      const teid_t& sgw_teid, std::shared_ptr<sgw_eps_bearer_context> sebc,
      std::shared_ptr<sgw_pdn_connection> spc);
  void delete_s5s8sgw_teid_2_sgw_contexts(const teid_t& sgw_teid);

  void handle_itti_msg(itti_s11_create_session_request& m);
  void handle_itti_msg(itti_s11_delete_session_request& m);
  void handle_itti_msg(itti_s11_modify_bearer_request& m);
  void handle_itti_msg(itti_s11_release_access_bearers_request& m);
  void handle_itti_msg(itti_s11_remote_peer_not_responding& m);
  void handle_itti_msg(itti_s11_downlink_data_notification_acknowledge& m);
  void handle_itti_msg(itti_s5s8_create_session_response& m);
  void handle_itti_msg(itti_s5s8_delete_session_response& m);
  void handle_itti_msg(itti_s5s8_modify_bearer_response& m);
  void handle_itti_msg(itti_s5s8_release_access_bearers_response& m);
  void handle_itti_msg(itti_s5s8_remote_peer_not_responding& m);
  void handle_itti_msg(itti_s5s8_downlink_data_notification& m);
};
}  // namespace sgwc
#endif /* FILE_SGWC_APP_HPP_SEEN */
