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

/*! \file spgwu_pfcp_association.hpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_SPGWU_PFCP_ASSOCIATION_HPP_SEEN
#define FILE_SPGWU_PFCP_ASSOCIATION_HPP_SEEN

#include "3gpp_29.244.h"
#include "itti.hpp"

#include <folly/AtomicHashMap.h>
#include <folly/AtomicLinkedList.h>
#include <mutex>
#include <vector>

namespace oai::cn::nf::spgwu {

#define PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES 5
  class pfcp_association {
  public:
    core::pfcp::node_id_t                              node_id;
    std::size_t                                                 hash_node_id;
    core::pfcp::recovery_time_stamp_t                  recovery_time_stamp;
    std::pair<bool,core::pfcp::cp_function_features_s> function_features;
    //
    mutable std::mutex                                          m_sessions;
    std::set<core::pfcp::fseid_t>                      sessions;
    //
    core::itti::timer_id_t                             timer_heartbeat;
    int                                                         num_retries_timer_heartbeat;
    uint64_t                                                    trxn_id_heartbeat;

    core::itti::timer_id_t                             timer_association;

    explicit pfcp_association(const core::pfcp::node_id_t& node_id) :
      node_id(node_id), recovery_time_stamp(), function_features(), m_sessions(), sessions() {
      hash_node_id = std::hash<core::pfcp::node_id_t>{}(node_id);
      timer_heartbeat = ITTI_INVALID_TIMER_ID;
      num_retries_timer_heartbeat = 0;
      trxn_id_heartbeat = 0;
    }
    pfcp_association(const core::pfcp::node_id_t& node_id, core::pfcp::recovery_time_stamp_t& recovery_time_stamp) :
      node_id(node_id), recovery_time_stamp(recovery_time_stamp), function_features(), m_sessions(), sessions() {
      hash_node_id = std::hash<core::pfcp::node_id_t>{}(node_id);
      timer_heartbeat = ITTI_INVALID_TIMER_ID;
      num_retries_timer_heartbeat = 0;
      trxn_id_heartbeat = 0;
    }
    pfcp_association(const core::pfcp::node_id_t& ni, core::pfcp::recovery_time_stamp_t& rts, core::pfcp::cp_function_features_s& uff):
      node_id(ni), recovery_time_stamp(rts), m_sessions(), sessions() {
      hash_node_id = std::hash<core::pfcp::node_id_t>{}(node_id);
      function_features.first = true;
      function_features.second = uff;
      timer_heartbeat = ITTI_INVALID_TIMER_ID;
      num_retries_timer_heartbeat = 0;
      trxn_id_heartbeat = 0;
      timer_association = {};
    }
//     pfcp_association(pfcp_association const & p)
//     {
//       node_id = p.node_id;
//       hash_node_id = p.hash_node_id;
//       recovery_time_stamp = p.recovery_time_stamp;
//       function_features = p.function_features;
//       sessions = p.sessions;
//       num_sessions = p.num_sessions;
//       timer_heartbeat = p.timer_heartbeat;
//       num_retries_timer_heartbeat = p.num_retries_timer_heartbeat;
//       trxn_id_heartbeat = p.trxn_id_heartbeat;
//     }
    void notify_add_session(const core::pfcp::fseid_t& cp_fseid);
    bool has_session(const core::pfcp::fseid_t& cp_fseid);
    void notify_del_session(const core::pfcp::fseid_t& cp_fseid);
    void del_sessions();
    void set(const core::pfcp::cp_function_features_s& ff) {function_features.first = true; function_features.second = ff;};
  };

#define PFCP_MAX_ASSOCIATIONS 16

  class pfcp_associations {
  private:
    std::vector<std::shared_ptr<pfcp_association>>   pending_associations;
    folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>> associations;

    pfcp_associations() : associations(PFCP_MAX_ASSOCIATIONS), pending_associations() {};
    void trigger_heartbeat_request_procedure(std::shared_ptr<pfcp_association>& s);
    bool remove_peer_candidate_node(core::pfcp::node_id_t& node_id, std::shared_ptr<pfcp_association>& s);

  public:
    static pfcp_associations& get_instance()
    {
      static pfcp_associations  instance;
      return instance;
    }

    pfcp_associations(pfcp_associations const&) = delete;
    void operator=(pfcp_associations const&) = delete;

    bool add_association(core::pfcp::node_id_t& node_id, core::pfcp::recovery_time_stamp_t& recovery_time_stamp);
    bool add_association(core::pfcp::node_id_t& node_id, core::pfcp::recovery_time_stamp_t& recovery_time_stamp, core::pfcp::cp_function_features_s& function_features);
    bool get_association(const core::pfcp::node_id_t& node_id, std::shared_ptr<pfcp_association>&  sa) const;
    bool get_association(const core::pfcp::fseid_t& cp_fseid, std::shared_ptr<pfcp_association>&  sa) const;

    void notify_add_session(const core::pfcp::node_id_t& node_id, const core::pfcp::fseid_t& cp_fseid);
    void notify_del_session(const core::pfcp::fseid_t& cp_fseid);

    bool add_peer_candidate_node(const core::pfcp::node_id_t& node_id);

    void restore_sx_sessions(const core::pfcp::node_id_t& node_id);

    void initiate_heartbeat_request(core::itti::timer_id_t timer_id, uint64_t arg2_user);
    void timeout_heartbeat_request(core::itti::timer_id_t timer_id, uint64_t arg2_user);

    void handle_receive_heartbeat_response(const uint64_t trxn_id);
  };
}

#endif /* FILE_SPGWU_PFCP_ASSOCIATION_HPP_SEEN */
