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

/*! \file pgw_pfcp_association.hpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_PGW_PFCP_ASSOCIATION_HPP_SEEN
#define FILE_PGW_PFCP_ASSOCIATION_HPP_SEEN

//--C includes -----------------------------------------------------------------
#include "3gpp_29.244.h"
//--C++ includes ---------------------------------------------------------------
#include <mutex>
#include <vector>
//--Other includes -------------------------------------------------------------
#include <folly/AtomicHashMap.h>
#include <folly/AtomicLinkedList.h>
#include "endpoint.hpp"
#include "itti.hpp"

namespace pgwc {

enum node_selection_criteria_e {
  kNodeSelectionCriteriaBestMaxHeartBeatRtt = 0,
  kNodeSelectionCriteriaMinPfcpSessions     = 1,
  kNodeSelectionCriteriaMinUpTime           = 2,
  kNodeSelectionCriteriaMaxUpTime           = 3,
  kNodeSelectionCriteriaMinRestart          = 4,
  kNodeSelectionCriteriaMaxAvailableBw      = 5,
  kNodeSelectionCriteriaNone                = 6
};

// Not PFCP retries, just Application retry over PFCP retries
#define PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES 2
class pfcp_association {
 public:
  pfcp::node_id_t node_id;
  std::size_t hash_node_id;
  std::string id;
  pfcp::recovery_time_stamp_t recovery_time_stamp;
  std::pair<bool, pfcp::up_function_features_s> function_features;
  std::pair<bool, pfcp::user_plane_ip_resource_information_t>
      user_plane_ip_resource_information;
  endpoint remote_endpoint;
  //
  mutable std::mutex m_sessions;
  std::set<pfcp::fseid_t> sessions;
  //
  timer_id_t timer_heartbeat;
  int num_retries_timer_heartbeat;
  uint64_t trxn_id_heartbeat;

  bool is_restore_sessions_pending;

  timer_id_t timer_association;

  explicit pfcp_association(const pfcp::node_id_t& node_id)
      : node_id(node_id),
        id(),
        recovery_time_stamp(),
        function_features(),
        user_plane_ip_resource_information(),
        m_sessions(),
        sessions() {
    hash_node_id                = std::hash<pfcp::node_id_t>{}(node_id);
    timer_heartbeat             = ITTI_INVALID_TIMER_ID;
    num_retries_timer_heartbeat = 0;
    trxn_id_heartbeat           = 0;
    is_restore_sessions_pending = false;
    timer_association           = ITTI_INVALID_TIMER_ID;
  }

  pfcp_association(
      const pfcp::node_id_t& ni, pfcp::recovery_time_stamp_t& rts,
      std::pair<bool, pfcp::up_function_features_s>& uff,
      std::pair<bool, pfcp::user_plane_ip_resource_information_t>& upiri)
      : node_id(ni), recovery_time_stamp(rts), m_sessions(), sessions() {
    hash_node_id                       = std::hash<pfcp::node_id_t>{}(node_id);
    function_features                  = uff;
    user_plane_ip_resource_information = upiri;
    timer_heartbeat                    = ITTI_INVALID_TIMER_ID;
    num_retries_timer_heartbeat        = 0;
    trxn_id_heartbeat                  = 0;
    is_restore_sessions_pending        = false;
    timer_association                  = ITTI_INVALID_TIMER_ID;
  }
  pfcp_association(pfcp_association const& p)
      : node_id(p.node_id),
        hash_node_id(p.hash_node_id),
        id(p.id),
        recovery_time_stamp(p.recovery_time_stamp),
        function_features(p.function_features),
        user_plane_ip_resource_information(
            p.user_plane_ip_resource_information),
        timer_heartbeat(p.timer_heartbeat),
        num_retries_timer_heartbeat(p.num_retries_timer_heartbeat),
        trxn_id_heartbeat(p.trxn_id_heartbeat),
        is_restore_sessions_pending(p.is_restore_sessions_pending),
        timer_association(0) {}

  void notify_add_session(const pfcp::fseid_t& cp_fseid);
  bool has_session(const pfcp::fseid_t& cp_fseid);
  void notify_del_session(const pfcp::fseid_t& cp_fseid);
  // void del_sessions();
  void restore_sx_sessions();
  void set(const pfcp::up_function_features_s& ff) {
    function_features.first  = true;
    function_features.second = ff;
  };
};

#define PFCP_MAX_ASSOCIATIONS 16

class pfcp_associations {
 private:
  std::vector<std::shared_ptr<pfcp_association>> pending_associations;
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>> associations;

  pfcp_associations()
      : associations(PFCP_MAX_ASSOCIATIONS), pending_associations(){};
  void trigger_heartbeat_request_procedure(
      std::shared_ptr<pfcp_association>& s);

 public:
  static pfcp_associations& get_instance() {
    static pfcp_associations instance;
    return instance;
  }

  pfcp_associations(pfcp_associations const&) = delete;
  void operator=(pfcp_associations const&) = delete;

  bool add_association(
      const uint64_t& trxn_id, const endpoint& remote_endpoint,
      pfcp::node_id_t& node_id, std::string& id,
      pfcp::recovery_time_stamp_t& recovery_time_stamp,
      std::pair<bool, pfcp::up_function_features_s>& up_function_features,
      std::pair<bool, pfcp::user_plane_ip_resource_information_t>&
          user_plane_ip_resource_information);
  bool get_association(
      const pfcp::node_id_t& node_id,
      std::shared_ptr<pfcp_association>& sa) const;
  bool get_association(
      const pfcp::fseid_t& cp_fseid,
      std::shared_ptr<pfcp_association>& sa) const;

  void notify_add_session(
      const pfcp::node_id_t& node_id, const pfcp::fseid_t& cp_fseid);
  void notify_del_session(const pfcp::fseid_t& cp_fseid);

  void restore_sx_sessions(const pfcp::node_id_t& node_id);

  void initiate_heartbeat_request(timer_id_t timer_id, uint64_t arg2_user);
  void timeout_heartbeat_request(timer_id_t timer_id, uint64_t arg2_user);
  void handle_receive_heartbeat_response(
      const uint64_t trxn_id, const endpoint& remote_endpoint,
      const pfcp::recovery_time_stamp_t& recovery_time_stamp);
  void handle_receive_heartbeat_request(
      const uint64_t trxn_id, const endpoint& remote_endpoint,
      const pfcp::recovery_time_stamp_t& recovery_time_stamp);

  bool select_up_node(
      const apn_t& apn, const uli_t& uli,
      const serving_network_t& serving_network, const rat_type_t& rat_type,
      const pdn_type_t& pdn_type, const paa_t& paa, pfcp::node_id_t& node_id,
      const int node_selection_criteria);
};
}  // namespace pgwc

#endif /* FILE_PGW_PFCP_ASSOCIATION_HPP_SEEN */
