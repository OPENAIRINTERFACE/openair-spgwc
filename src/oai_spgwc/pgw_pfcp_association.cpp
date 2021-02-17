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
//--related header -------------------------------------------------------------
#include "pgw_pfcp_association.hpp"
//--C includes -----------------------------------------------------------------
//--C++ includes ---------------------------------------------------------------
//--Other includes -------------------------------------------------------------
#include "common_defs.h"
#include "logger.hpp"
#include "pgw_config.hpp"
#include "pgwc_procedure.hpp"
#include "pgwc_sxab.hpp"
#include "PfcpUpNodes.hpp"

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
//------------------------------------------------------------------------------
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
    const uint64_t& trxn_id, const endpoint& remote_endpoint,
    pfcp::node_id_t& node_id, std::string& id,
    pfcp::recovery_time_stamp_t& recovery_time_stamp,
    std::pair<bool, pfcp::up_function_features_s>& up_function_features,
    std::pair<bool, pfcp::user_plane_ip_resource_information_t>&
        user_plane_ip_resource_information) {
  std::shared_ptr<pfcp_association> sa =
      std::shared_ptr<pfcp_association>(nullptr);
  bool is_restore_sx_sessions                 = false;
  bool is_trigger_heartbeat_request_procedure = false;
  if (get_association(node_id, sa)) {
    itti_inst->timer_remove(sa->timer_heartbeat);
    if (sa->recovery_time_stamp == recovery_time_stamp) {
      is_restore_sx_sessions = false;
      return true;
    } else {
      is_restore_sx_sessions = true;
    }
    sa->recovery_time_stamp                = recovery_time_stamp;
    sa->function_features                  = up_function_features;
    sa->user_plane_ip_resource_information = user_plane_ip_resource_information;
  } else {
    is_restore_sx_sessions        = false;
    pfcp_association* association = new pfcp_association(
        node_id, recovery_time_stamp, up_function_features,
        user_plane_ip_resource_information);
    sa     = std::shared_ptr<pfcp_association>(association);
    sa->id = id;
    associations.insert((int32_t) association->hash_node_id, sa);
    is_trigger_heartbeat_request_procedure = true;
  }
  sa->remote_endpoint = remote_endpoint;
  // always yes (for the time being)
  itti_sxab_association_setup_response a(TASK_SPGWU_SX, TASK_SPGWU_SX);
  a.trxn_id           = trxn_id;
  pfcp::cause_t cause = {.cause_value = pfcp::CAUSE_VALUE_REQUEST_ACCEPTED};
  a.pfcp_ies.set(cause);
  pfcp::node_id_t this_node_id = {};
  if (pgw_config::GetPfcpNodeId(this_node_id) == RETURNok) {
    a.pfcp_ies.set(this_node_id);
    a.r_endpoint = remote_endpoint;
    pgwc_sxab_inst->send_sx_msg(a);
    // !deadlock design
    // PfcpUpNodes::Instance().NotifyNodeAssociated(node_id);
  } else {
    Logger::pgwc_sx().warn(
        "Received SX ASSOCIATION SETUP REQUEST could not set node id!, "
        "ignore message");
    return false;
  }

  if (is_trigger_heartbeat_request_procedure) {
    trigger_heartbeat_request_procedure(sa);
  }
  if (is_restore_sx_sessions) {
    pfcp_associations::get_instance().restore_sx_sessions(node_id);
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
      pgw_config::cups_.association_heartbeat_period_ms / 1000,
      (pgw_config::cups_.association_heartbeat_period_ms % 1000) * 1000,
      TASK_PGWC_SX, TASK_PGWC_SX_TRIGGER_HEARTBEAT_REQUEST, s->hash_node_id);
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
    //    if (pit->second->num_retries_timer_heartbeat <
    //        PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES) {
    //      Logger::pgwc_sx().info(
    //          "PFCP HEARTBEAT PROCEDURE hash %u TIMED OUT (retry %d)",
    //          hash_node_id, pit->second->num_retries_timer_heartbeat);
    //      pit->second->num_retries_timer_heartbeat++;
    //      pgwc_sxab_inst->send_heartbeat_request(pit->second);
    //    } else {
    //      Logger::pgwc_sx().warn(
    //          "PFCP HEARTBEAT PROCEDURE FAILED after %d retries!",
    //          PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES);
    //      PfcpUpNodes::Instance().NotifyNodeNotReachable(pit->second->node_id);
    //    }
  }
}
//------------------------------------------------------------------------------
void pfcp_associations::handle_receive_heartbeat_response(
    const uint64_t trxn_id, const endpoint& remote_endpoint,
    const pfcp::recovery_time_stamp_t& recovery_time_stamp) {
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>>::iterator it;
  FOR_EACH(it, associations) {
    std::shared_ptr<pfcp_association> pit = it->second;
    if (it->second->trxn_id_heartbeat == trxn_id) {
      it->second->trxn_id_heartbeat = 0;
      itti_inst->timer_remove(it->second->timer_heartbeat);
      if (pit->recovery_time_stamp == recovery_time_stamp) {
        trigger_heartbeat_request_procedure(it->second);
        PfcpUpNodes::Instance().NotifyNodeReachable(pit->hash_node_id);
      } else {
        PfcpUpNodes::Instance().NotifyNodeRestarted(pit->hash_node_id);
      }
      return;
    }
  }
  Logger::pgwc_sx().warn("PFCP HEARTBEAT PROCEDURE mismatch trxn id");
}
//------------------------------------------------------------------------------
void pfcp_associations::handle_receive_heartbeat_request(
    const uint64_t trxn_id, const endpoint& remote_endpoint,
    const pfcp::recovery_time_stamp_t& recovery_time_stamp) {
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>>::iterator it;
  FOR_EACH(it, associations) {
    std::shared_ptr<pfcp_association> pit = it->second;
    if (it->second->remote_endpoint == remote_endpoint) {
      if (pit->recovery_time_stamp == recovery_time_stamp) {
        PfcpUpNodes::Instance().NotifyNodeReachable(pit->hash_node_id);
      } else {
        PfcpUpNodes::Instance().NotifyNodeRestarted(pit->hash_node_id);
      }
      return;
    }
  }
}

//------------------------------------------------------------------------------
bool pfcp_associations::select_up_node(
    const apn_t& apn, const uli_t& uli,
    const serving_network_t& serving_network, const rat_type_t& rat_type,
    const pdn_type_t& pdn_type, const paa_t& paa, pfcp::node_id_t& node_id,
    const int node_selection_criteria) {
  node_id = {};
  if (associations.empty()) {
    Logger::pgwc_app().warn("No available UP node for selection");
    return false;
  }
  // filter by apn, location, paa if already set
  std::vector<up_node_cfg_t> up_nodes = {};
  if (pgw_config::GetUpNodes(apn, uli, paa, up_nodes)) {
    std::set<std::shared_ptr<pfcp_association>> up_nodes_associated = {};
    for (auto node_it : up_nodes) {
      folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>>::iterator
          it;
      FOR_EACH(it, associations) {
        std::shared_ptr<pfcp_association> a = it->second;
        if (a->id.compare(node_it.id) == 0) {
          up_nodes_associated.insert(a);
        }
      }
    }
    if (up_nodes_associated.size()) {
      // TODO
      switch (node_selection_criteria) {
        case kNodeSelectionCriteriaBestMaxHeartBeatRtt:
        case kNodeSelectionCriteriaMinPfcpSessions:
        case kNodeSelectionCriteriaMinUpTime:
        case kNodeSelectionCriteriaMaxUpTime:
        case kNodeSelectionCriteriaMinRestart:
        case kNodeSelectionCriteriaMaxAvailableBw:
        case kNodeSelectionCriteriaNone:
        default:
          node_id = up_nodes_associated.begin()->get()->node_id;
          Logger::pgwc_app().debug(
              "Select UP node: %s",
              up_nodes_associated.begin()->get()->node_id.toString().c_str());
          return true;
          break;
      }
    }
  }
  Logger::pgwc_app().warn("No suitable UP node found for selection");
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
