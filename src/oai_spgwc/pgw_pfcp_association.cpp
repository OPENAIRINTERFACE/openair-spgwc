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

/*! \file spgwu_pfcp_association.cpp
   \brief
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/
#include "pgw_pfcp_association.hpp"
#include "common_defs.h"
#include "logger.hpp"
#include "pgwc_procedure.hpp"
#include "pgwc_sxab.hpp"

using namespace pgwc;
using namespace std;

extern itti_mw* itti_inst;
extern pgwc_sxab* pgwc_sxab_inst;
//------------------------------------------------------------------------------
void pfcp_association::notify_add_session(const pfcp::fseid_t& cp_fseid) {
  std::unique_lock<std::mutex> l(m_sessions);
  sessions.insert(cp_fseid);
}
//------------------------------------------------------------------------------
bool pfcp_association::has_session(const pfcp::fseid_t& cp_fseid) {
  std::unique_lock<std::mutex> l(m_sessions);
  auto it = sessions.find(cp_fseid);
  if (it != sessions.end()) {
    return true;
  } else {
    return false;
  }
}
//------------------------------------------------------------------------------
void pfcp_association::notify_del_session(const pfcp::fseid_t& cp_fseid) {
  std::unique_lock<std::mutex> l(m_sessions);
  sessions.erase(cp_fseid);
}
// //------------------------------------------------------------------------------
// void pfcp_association::del_sessions()
// {
//   std::unique_lock<std::mutex> l(m_sessions);
//   for (std::set<pfcp::fseid_t>::iterator it=sessions.begin();
//   it!=sessions.end();) {
//     ???->remove_pfcp_session(*it);
//     sessions.erase(it++);
//   }
// }
//------------------------------------------------------------------------------
void pfcp_association::restore_sx_sessions() {
  std::unique_lock<std::mutex> l(m_sessions);
  if (sessions.size()) {
    is_restore_sessions_pending = true;
    sx_session_restore_procedure* restore_proc =
        new sx_session_restore_procedure(sessions);
    restore_proc->run();
  }
}
//------------------------------------------------------------------------------
bool pfcp_associations::add_association(
    pfcp::node_id_t& node_id, pfcp::recovery_time_stamp_t& recovery_time_stamp,
    bool& restore_sx_sessions) {
  std::shared_ptr<pfcp_association> sa =
      std::shared_ptr<pfcp_association>(nullptr);
  if (get_association(node_id, sa)) {
    itti_inst->timer_remove(sa->timer_heartbeat);
    if (sa->recovery_time_stamp == recovery_time_stamp) {
      restore_sx_sessions = false;
    } else {
      restore_sx_sessions = true;
    }
    sa->recovery_time_stamp = recovery_time_stamp;
    sa->function_features   = {};
  } else {
    restore_sx_sessions = false;
    pfcp_association* association =
        new pfcp_association(node_id, recovery_time_stamp);
    sa                       = std::shared_ptr<pfcp_association>(association);
    sa->recovery_time_stamp  = recovery_time_stamp;
    std::size_t hash_node_id = std::hash<pfcp::node_id_t>{}(node_id);
    associations.insert((int32_t) hash_node_id, sa);
    trigger_heartbeat_request_procedure(sa);
  }
  return true;
}
//------------------------------------------------------------------------------
bool pfcp_associations::add_association(
    pfcp::node_id_t& node_id, pfcp::recovery_time_stamp_t& recovery_time_stamp,
    pfcp::up_function_features_s& function_features,
    bool& restore_sx_sessions) {
  std::shared_ptr<pfcp_association> sa =
      std::shared_ptr<pfcp_association>(nullptr);
  if (get_association(node_id, sa)) {
    itti_inst->timer_remove(sa->timer_heartbeat);
    if (sa->recovery_time_stamp == recovery_time_stamp) {
      restore_sx_sessions = false;
    } else {
      restore_sx_sessions = true;
    }
    sa->recovery_time_stamp      = recovery_time_stamp;
    sa->function_features.first  = true;
    sa->function_features.second = function_features;
  } else {
    restore_sx_sessions = false;
    pfcp_association* association =
        new pfcp_association(node_id, recovery_time_stamp, function_features);
    sa                      = std::shared_ptr<pfcp_association>(association);
    sa->recovery_time_stamp = recovery_time_stamp;
    sa->function_features.first  = true;
    sa->function_features.second = function_features;
    std::size_t hash_node_id     = std::hash<pfcp::node_id_t>{}(node_id);
    associations.insert((int32_t) hash_node_id, sa);
    trigger_heartbeat_request_procedure(sa);
  }
  return true;
}
//------------------------------------------------------------------------------
bool pfcp_associations::get_association(
    const pfcp::node_id_t& node_id,
    std::shared_ptr<pfcp_association>& sa) const {
  std::size_t hash_node_id = std::hash<pfcp::node_id_t>{}(node_id);
  auto pit                 = associations.find((int32_t) hash_node_id);
  if (pit == associations.end())
    return false;
  else {
    sa = pit->second;
    return true;
  }
}
//------------------------------------------------------------------------------
bool pfcp_associations::get_association(
    const pfcp::fseid_t& cp_fseid,
    std::shared_ptr<pfcp_association>& sa) const {
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>>::iterator it;

  FOR_EACH(it, associations) {
    std::shared_ptr<pfcp_association> a = it->second;
    if (it->second->has_session(cp_fseid)) {
      sa = it->second;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void pfcp_associations::restore_sx_sessions(const pfcp::node_id_t& node_id) {
  std::shared_ptr<pfcp_association> sa = {};
  if (get_association(node_id, sa)) {
    sa->restore_sx_sessions();
  }
}
//------------------------------------------------------------------------------
void pfcp_associations::trigger_heartbeat_request_procedure(
    std::shared_ptr<pfcp_association>& s) {
  s->timer_heartbeat = itti_inst->timer_setup(
      PFCP_ASSOCIATION_HEARTBEAT_INTERVAL_SEC, 0, TASK_PGWC_SX,
      TASK_PGWC_SX_TRIGGER_HEARTBEAT_REQUEST, s->hash_node_id);
}
//------------------------------------------------------------------------------
void pfcp_associations::initiate_heartbeat_request(
    timer_id_t timer_id, uint64_t arg2_user) {
  size_t hash_node_id = (size_t) arg2_user;
  auto pit            = associations.find((int32_t) hash_node_id);
  if (pit == associations.end())
    return;
  else {
    Logger::pgwc_sx().info(
        "PFCP HEARTBEAT PROCEDURE hash %u starting", hash_node_id);
    pit->second->num_retries_timer_heartbeat = 0;
    pgwc_sxab_inst->send_heartbeat_request(pit->second);
  }
}
//------------------------------------------------------------------------------
void pfcp_associations::timeout_heartbeat_request(
    timer_id_t timer_id, uint64_t arg2_user) {
  size_t hash_node_id = (size_t) arg2_user;
  auto pit            = associations.find((int32_t) hash_node_id);
  if (pit == associations.end())
    return;
  else {
    if (pit->second->num_retries_timer_heartbeat <
        PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES) {
      Logger::pgwc_sx().info(
          "PFCP HEARTBEAT PROCEDURE hash %u TIMED OUT (retrie %d)",
          hash_node_id, pit->second->num_retries_timer_heartbeat);
      pit->second->num_retries_timer_heartbeat++;
      pgwc_sxab_inst->send_heartbeat_request(pit->second);
    } else {
      Logger::pgwc_sx().warn(
          "PFCP HEARTBEAT PROCEDURE FAILED after %d retries! TODO",
          PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES);
    }
  }
}
//------------------------------------------------------------------------------
void pfcp_associations::handle_receive_heartbeat_response(
    const uint64_t trxn_id) {
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>>::iterator it;

  FOR_EACH(it, associations) {
    std::shared_ptr<pfcp_association> a = it->second;
    if (it->second->trxn_id_heartbeat == trxn_id) {
      itti_inst->timer_remove(it->second->timer_heartbeat);
      trigger_heartbeat_request_procedure(it->second);
      return;
    }
  }
}

//------------------------------------------------------------------------------
bool pfcp_associations::select_up_node(
    pfcp::node_id_t& node_id, const int node_selection_criteria) {
  node_id = {};
  if (associations.empty()) {
    return false;
  }
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>>::iterator it;
  FOR_EACH(it, associations) {
    std::shared_ptr<pfcp_association> a = it->second;
    // TODO
    switch (node_selection_criteria) {
      case NODE_SELECTION_CRITERIA_BEST_MAX_HEARBEAT_RTT:
      case NODE_SELECTION_CRITERIA_MIN_PFCP_SESSIONS:
      case NODE_SELECTION_CRITERIA_MIN_UP_TIME:
      case NODE_SELECTION_CRITERIA_MAX_AVAILABLE_BW:
      case NODE_SELECTION_CRITERIA_NONE:
      default:
        node_id = it->second->node_id;
        return true;
        break;
    }
  }
  return false;
}
//------------------------------------------------------------------------------
void pfcp_associations::notify_add_session(
    const pfcp::node_id_t& node_id, const pfcp::fseid_t& cp_fseid) {
  std::shared_ptr<pfcp_association> sa = {};
  if (get_association(node_id, sa)) {
    sa->notify_add_session(cp_fseid);
  }
}
//------------------------------------------------------------------------------
void pfcp_associations::notify_del_session(const pfcp::fseid_t& cp_fseid) {
  std::shared_ptr<pfcp_association> sa = {};
  if (get_association(cp_fseid, sa)) {
    sa->notify_del_session(cp_fseid);
  }
}
