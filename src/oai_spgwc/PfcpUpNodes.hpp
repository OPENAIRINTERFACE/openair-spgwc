/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

/*! \file PfcpUpNode.hpp
   \author  Lionel GAUTHIER
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_PFCP_UP_NODES_HPP_SEEN
#define FILE_PFCP_UP_NODES_HPP_SEEN
//--C includes -----------------------------------------------------------------
#include "3gpp_29.244.h"
//--C++ includes ---------------------------------------------------------------
#include <map>
#include <memory>
#include <mutex>
#include <list>
//--Other includes -------------------------------------------------------------
#include <folly/AtomicHashMap.h>
#include "itti.hpp"
#include "msg_pfcp.hpp"
#include "endpoint.hpp"

namespace pgwc {

#define PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES 2

enum AssociationState {
  kAssocNullState = 0,
  kAssocInitiatedState,
  kAssocSetupState,
  kAssocUnstable,
  kAssocLost
};

class PfcpUpNode {
 public:
  std::string id_;
  pfcp::node_id_t node_id_;
  std::size_t hash_node_id_;
  pfcp::recovery_time_stamp_t peer_recovery_time_stamp_;
  std::pair<bool, pfcp::up_function_features_s> peer_function_features_;
  endpoint remote_endpoint_;
  //
  // mutable std::mutex m_sessions_;
  // std::set<pfcp::fseid_t> sessions_;
  //
  timer_id_t timer_heartbeat_;
  int num_retries_timer_heartbeat_;
  uint64_t trxn_id_heartbeat_;

  bool is_restore_sessions_pending_;

  int association_state_;
  timer_id_t timer_association_;
  uint consecutive_failed_echo_proc;
  uint consecutive_successful_echo_proc;
  uint64_t failed_echo_proc;
  uint64_t successful_echo_proc;
  uint64_t restarts;

  explicit PfcpUpNode(const std::string& id)
      : id_(id),
        peer_recovery_time_stamp_(),
        peer_function_features_(),
        timer_heartbeat_(ITTI_INVALID_TIMER_ID),
        num_retries_timer_heartbeat_(0),
        trxn_id_heartbeat_(0),
        is_restore_sessions_pending_(false),
        association_state_(kAssocNullState),
        timer_association_(ITTI_INVALID_TIMER_ID),
        hash_node_id_(0),
        consecutive_failed_echo_proc(0),
        consecutive_successful_echo_proc(0),
        failed_echo_proc(0),
        successful_echo_proc(0),
        restarts(0) {}

  void TriggerAssociation();
  void NotifyAddSession(const pfcp::fseid_t& cp_fseid);
  bool HasSession(const pfcp::fseid_t& cp_fseid);
  void NotifyDelSession(const pfcp::fseid_t& cp_fseid);
  // void del_sessions();
  void NotifyNodeNotResponding();
  void NotifyNodeNotReachable();
  void NotifyNodeReachable();
  void NotifyNodeRestarted();
  void RestoreSxSessions();
  void Set(const pfcp::up_function_features_s& ff) {
    peer_function_features_.first  = true;
    peer_function_features_.second = ff;
  };
};

class PfcpUpNodes {
 private:
  pfcp::node_id_t node_id_;
  pfcp::cp_function_features_t cp_function_features_;

  mutable std::mutex m_pending_nodes;
  std::list<std::shared_ptr<PfcpUpNode>> pending_nodes_;
  folly::AtomicHashMap<int32_t, std::shared_ptr<PfcpUpNode>> up_nodes_;

  PfcpUpNodes();

 public:
  static PfcpUpNodes& Instance() {
    static PfcpUpNodes instance;
    return instance;
  }

  PfcpUpNodes(PfcpUpNodes const&) = delete;
  void operator=(PfcpUpNodes const&) = delete;

  pfcp::node_id_t GetNodeId() const { return node_id_; };
  pfcp::cp_function_features_t GetCpFunctionFeatures() const {
    return cp_function_features_;
  };

  void TriggerAssociations();

  void AssociationSetupRequest(
      const uint64_t& trxn_id, const endpoint& remote_endpoint,
      pfcp::node_id_t& node_id,
      pfcp::recovery_time_stamp_t& recovery_time_stamp,
      std::pair<bool, pfcp::up_function_features_s>& up_function_features,
      std::pair<bool, pfcp::user_plane_ip_resource_information_t>&
          user_plane_ip_resource_information);

  bool AddUpNode(const std::string t_up_ip, const std::string t_apn);
  bool GetUpNode(
      const pfcp::node_id_t& node_id, std::shared_ptr<PfcpUpNode>& su) const;
  bool GetUpNode(
      const std::size_t hash_node_id, std::shared_ptr<PfcpUpNode>& su) const;
  void NotifyNodeNotResponding(const pfcp::node_id_t& node_id);
  void NotifyNodeNotResponding(const std::size_t hash_node_id);
  void NotifyNodeNotReachable(const pfcp::node_id_t& node_id);
  void NotifyNodeNotReachable(const std::size_t hash_node_id);
  void NotifyNodeReachable(const pfcp::node_id_t& node_id);
  void NotifyNodeReachable(const std::size_t hash_node_id);
  void NotifyNodeRestarted(const pfcp::node_id_t& node_id);
  void NotifyNodeRestarted(const std::size_t hash_node_id);
  void NotifyNodeAssociated(const pfcp::node_id_t& node_id);

  // implement TimerApplication interface
  void TimeOutNotification(
      const timer_id_t tid, uint64_t arg1_user = 0, uint64_t arg2_user = 0);

  void Receive(
      std::unique_ptr<pfcp::pfcp_association_setup_response> t_msg,
      const endpoint& t_remote_endpoint, const uint64_t t_trxn_id);
};
}  // namespace pgwc

#endif /* FILE_PFCP_UP_NODES_HPP_SEEN */
