/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
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

/*! \file spgwu_app.hpp
   \author  Lionel GAUTHIER
   \date 2018
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_SPGWU_APP_HPP_SEEN
#define FILE_SPGWU_APP_HPP_SEEN

#include "common_root_types.h"

#include "itti_msg_sxab.hpp"

#include <boost/atomic.hpp>

#include <map>
#include <string>
#include <thread>
#include <memory>
#include <map>
#include <set>

namespace oai::cn::nf::spgwu {

class spgwu_app {
private:
  std::thread::id                      thread_id;
  std::thread                          thread;

  // teid generators (linear)
//  boost::atomic<teid_t>                 teid_sxab_cp;
//  boost::atomic<teid_t>                 teid_s5s8_up;
//  /* There shall be only one pair of TEID-C per UE over the S11 and the S4 interfaces. The same tunnel shall be
//     shared for the control messages related to the same UE operation. A TEID-C on the S11/S4 interface shall be
//     released after all its associated EPS bearers are deleted.*/
//  std::map<imsi64_t, std::shared_ptr<sgwu_eps_bearer_context>> imsi2sgwu_eps_bearer_context;
//  std::map<teid_t, std::shared_ptr<sgwu_eps_bearer_context>> s11lteid2sgwu_eps_bearer_context;
//
//  std::map<teid_t, std::pair<std::shared_ptr<sgwu_eps_bearer_context>, std::shared_ptr<sgwu_pdn_connection>>> s5s8lteid2sgwu_contexts;
//  std::set<teid_t> s5s8uplteid; // In case of overflow of generator of teid_t
//
//  teid_t generate_s11_cp_teid();
//  bool is_s11c_teid_exist(const teid_t& teid_s11_cp) const;
//
//  bool is_s5s8sgwu_teid_2_sgwu_contexts(const teid_t& sgwu_teid) const;
//
//  // s11lteid2sgwu_eps_bearer_context collection
//  bool is_s11sgwu_teid_2_sgwu_eps_bearer_context(const teid_t& sgwu_teid) const;
//
//  bool is_imsi64_2_sgwu_eps_bearer_context(const imsi64_t& imsi64) const;
//  void set_imsi64_2_sgwu_eps_bearer_context(const imsi64_t& imsi64, std::shared_ptr<sgwu_eps_bearer_context> sebc);
//
//  teid_t generate_s5s8_cp_teid();
//  bool is_s5s8c_teid_exist(const teid_t& teid_s5s8_cp)const;
//
//  bool is_s5s8u_teid_exist(const teid_t& teid_s5s8_up) const;

public:
  // key is S11 S-GW local teid, value is S11 tunnel id pair
 // map<teid_t, int> s11teid2mme_hashtable;

  // key is paa, value is S11 s-gw local teid
  //obj_hash_table_uint64_t *ip2s11teid;

  // key is S1-U S-GW local teid
  ////hash_table_t *s1uteid2enb_hashtable;

  // the key of this hashtable is the S11 s-gw local teid.
  //hash_table_ts_t *s11_bearer_context_information_hashtable;

  /* The TEID-C shall be unique per PDN-Connection on GTP based S2a, S2b, S5 and S8 interfaces. The same
     tunnel shall be shared for the control messages related to all bearers associated to
     the PDN-Connection. A TEID-C on the S2a/S2b/S5/S8 interface shall be released after all its associated EPS bearers are deleted. */

  //gtpv1u_data_t    gtpv1u_data;

  explicit spgwu_app(const std::string& config_file);
  ~spgwu_app();
  spgwu_app(spgwu_app const&) = delete;
  void operator=(spgwu_app const&) = delete;

  teid_t generate_s5s8_up_teid();



//  void handle_itti_msg core::(itti::itti_sxab_heartbeat_request& m);
//  void handle_itti_msg (core::itti::itti_sxab_heartbeat_response& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_pfd_management_request& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_pfd_management_response& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_association_setup_request& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_association_setup_response& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_association_update_request& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_association_update_response& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_association_release_request& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_association_release_response& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_version_not_supported_response& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_node_report_request& m);
//  void handle_itti_msg (core::itti::itti_sxab_pfcp_node_report_response& m);
  void handle_itti_msg (std::shared_ptr<core::itti::itti_sxab_session_establishment_request> m);
  void handle_itti_msg (std::shared_ptr<core::itti::itti_sxab_session_modification_request> m);
  void handle_itti_msg (std::shared_ptr<core::itti::itti_sxab_session_deletion_request> m);
//  void handle_itti_msg (core::itti::itti_sxab_session_deletion_response& m);
//  void handle_itti_msg (core::itti::itti_sxab_session_report_request& m);
//  void handle_itti_msg (core::itti::itti_sxab_session_report_response& m);
};
}
#endif /* FILE_SPGWU_APP_HPP_SEEN */
