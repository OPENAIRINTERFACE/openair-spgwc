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

/*! \file PfcpUpNodes.cpp
   \brief
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/
//--related header -------------------------------------------------------------
#include "PfcpUpNodes.hpp"
//--C includes -----------------------------------------------------------------
#include "common.h"
#include "common_defs.h"
#include <netdb.h>
//--C++ includes ---------------------------------------------------------------
//--Other includes -------------------------------------------------------------
#include "endpoint.hpp"
#include "itti.hpp"
#include "logger.hpp"
#include "pgw_app.hpp"
#include "pgw_config.hpp"
#include "pgw_pfcp_association.hpp"
#include "pgwc_procedure.hpp"
#include "pgwc_sxab.hpp"
#include "common.h"

extern pgwc::pgwc_sxab* pgwc_sxab_inst;
extern itti_mw* itti_inst;

//------------------------------------------------------------------------------
void pgwc::PfcpUpNode::TriggerAssociation() {
  if (association_state_ == kAssocNullState) {
    if (remote_endpoint_.family() == AF_UNSPEC) {
      unsigned char buf[sizeof(struct in6_addr)];
      if (inet_pton(AF_INET, id_.c_str(), buf) == 1) {
        node_id_.node_id_type = pfcp::NODE_ID_TYPE_IPV4_ADDRESS;
        memcpy(&node_id_.u1.ipv4_address, buf, sizeof(struct in_addr));
        hash_node_id_    = std::hash<pfcp::node_id_t>{}(node_id_);
        remote_endpoint_ = endpoint(
            node_id_.u1.ipv4_address,
            pgwc_sxab_inst->pfcp_registered_port_number());
      } else if (inet_pton(AF_INET6, id_.c_str(), buf) == 1) {
        node_id_.node_id_type = pfcp::NODE_ID_TYPE_IPV6_ADDRESS;
        memcpy(&node_id_.u1.ipv6_address, buf, sizeof(struct in6_addr));
        hash_node_id_    = std::hash<pfcp::node_id_t>{}(node_id_);
        remote_endpoint_ = endpoint(
            node_id_.u1.ipv6_address,
            pgwc_sxab_inst->pfcp_registered_port_number());
      } else {
        struct hostent* record = gethostbyname(id_.c_str());
        if (record == NULL) {
          association_state_ = kAssocNullState;
          Logger::pgwc_app().info(
              "Trigger association with %s: cannot resolve,"
              " retrying later",
              id_.c_str());
          return;
        }
        if (record->h_addrtype == AF_INET) {
          in_addr* address = (struct in_addr*) record->h_addr;
          remote_endpoint_ =
              endpoint(*address, pgwc_sxab_inst->pfcp_registered_port_number());
        } else if (record->h_addrtype == AF_INET6) {
          in_addr* address = (struct in_addr*) record->h_addr;
          remote_endpoint_ =
              endpoint(*address, pgwc_sxab_inst->pfcp_registered_port_number());
        } else {
          return;
        }
        node_id_.node_id_type = pfcp::NODE_ID_TYPE_FQDN;
        node_id_.fqdn         = id_;
        hash_node_id_         = std::hash<pfcp::node_id_t>{}(node_id_);
      }
    }
    association_state_ = kAssocInitiatedState;
    itti_sxab_association_setup_request itti_msg(TASK_PGWC_SX, TASK_PGWC_SX);
    itti_msg.trxn_id = 0;

    pfcp::node_id_t this_node_id = PfcpUpNodes::Instance().GetNodeId();
    itti_msg.pfcp_ies.set(this_node_id);
    pfcp::recovery_time_stamp_t r = {
        .recovery_time_stamp =
            (uint32_t) pgwc_sxab_inst->get_recovery_time_stamp()};
    itti_msg.pfcp_ies.set(r);
    itti_msg.pfcp_ies.set(PfcpUpNodes::Instance().GetCpFunctionFeatures());

    itti_msg.r_endpoint = remote_endpoint_;
    pgwc_sxab_inst->send_sx_msg(itti_msg);
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNode::NotifyNodeNotResponding() {
  Logger::pgwc_app().info(
      "User Plane Node %s detected not responding",
      node_id_.toString().c_str());
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNode::NotifyNodeNotReachable() {
  consecutive_failed_echo_proc++;
  failed_echo_proc++;
  consecutive_successful_echo_proc = 0;
  if (association_state_ == kAssocSetupState) {
    association_state_ = kAssocUnstable;
    Logger::pgwc_app().info(
        "User Plane Node %s detected unreachable: state kAssocUnstable",
        node_id_.toString().c_str());
  } else if (
      (association_state_ == kAssocUnstable) &&
      (consecutive_failed_echo_proc >=
       PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES)) {
    association_state_ = kAssocLost;
    Logger::pgwc_app().info(
        "User Plane Node %s detected unreachable: state kAssocLost",
        node_id_.toString().c_str());
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNode::NotifyNodeReachable() {
  if (association_state_ == kAssocLost) {
    association_state_ = kAssocUnstable;
  } else if (
      (association_state_ == kAssocUnstable) &&
      (consecutive_successful_echo_proc > 10)) {
    association_state_ = kAssocSetupState;
  }
  consecutive_failed_echo_proc = 0;
  consecutive_successful_echo_proc++;
  successful_echo_proc++;
  Logger::pgwc_app().debug(
      "User Plane Node %s is reachable", node_id_.toString().c_str());
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNode::NotifyNodeRestarted() {
  restarts++;
  Logger::pgwc_app().info(
      "User Plane Node %s detected restart", node_id_.toString().c_str());
  // TODO
}
//------------------------------------------------------------------------------
pgwc::PfcpUpNodes::PfcpUpNodes()
    : up_nodes_(64),
      m_pending_nodes(),
      pending_nodes_(),
      cp_function_features_() {
  std::set<std::string> s;
  for (auto it : pgw_config::cups_.nodes) {
    // get rid of duplicates
    s.insert(it.id);
  }
  std::unique_lock<std::mutex> l(m_pending_nodes);
  for (auto it : s) {
    auto sup = std::shared_ptr<PfcpUpNode>(new PfcpUpNode(it));
    pending_nodes_.push_back(sup);
  }
  //    up_nodes_.insert((int32_t) sup->hash_node_id_, sup);
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::TriggerAssociations() {
#define WAIT_FOR_UPF_CONVERGENCE 1
#if WAIT_FOR_UPF_CONVERGENCE
#else
  std::unique_lock<std::mutex> l(m_pending_nodes);
  for (auto it : pending_nodes_) {
    if (it->association_state_ == kAssocNullState) {
      it->TriggerAssociation();
    }
  }
  if (pending_nodes_.size()) {
    // trigger timer
    timer_id_t timer_association_ = itti_inst->timer_setup(
        pgw_config::cups_.first_association_retry_interval_ms / 1000,
        pgw_config::cups_.first_association_retry_interval_ms % 1000,
        TASK_PGWC_APP, (uint64_t) kTriggerAssociationUpNodes, 0);
  }
#endif
}

//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::AssociationSetupRequest(
    const uint64_t& trxn_id, const endpoint& remote_endpoint,
    pfcp::node_id_t& node_id, pfcp::recovery_time_stamp_t& recovery_time_stamp,
    std::pair<bool, pfcp::up_function_features_s>& up_function_features,
    std::pair<bool, pfcp::user_plane_ip_resource_information_t>&
        user_plane_ip_resource_information) {
  Logger::pgwc_app().info(
      "RX AssociationSetupRequest FQDN %s", node_id.fqdn.c_str());
  std::unique_lock<std::mutex> l(m_pending_nodes);
  if (pending_nodes_.size()) {
    for (auto it = pending_nodes_.begin(); it != pending_nodes_.end(); ++it) {
      // for (auto it : pending_nodes_) {
      if (((*it)->association_state_ == kAssocNullState) ||
          ((*it)->association_state_ == kAssocLost)) {
        if (((*it)->node_id_ == node_id) || (node_id == (*it)->id_)) {
          if (pfcp_associations::get_instance().add_association(
                  trxn_id, remote_endpoint, node_id, (*it)->id_,
                  recovery_time_stamp, up_function_features,
                  user_plane_ip_resource_information)) {
            (*it)->association_state_ = kAssocSetupState;
            (*it)->node_id_           = node_id;
            (*it)->hash_node_id_      = std::hash<pfcp::node_id_t>{}(node_id);
            (*it)->remote_endpoint_   = remote_endpoint;
            (*it)->peer_recovery_time_stamp_ = recovery_time_stamp;
            (*it)->peer_function_features_   = up_function_features;
            up_nodes_.insert((int32_t)(*it)->hash_node_id_, *it);
            pending_nodes_.erase(it);
          }
          return;
        }  // other cases TODO (ipv4, ipv6)
      }
    }
  } else {
    pfcp::node_id_t node_id = {};
    if (pgw_config::GetPfcpNodeId(node_id) == RETURNok) {
      itti_sxab_association_setup_response a(TASK_SPGWU_APP, TASK_SPGWU_SX);
      a.trxn_id           = trxn_id;
      pfcp::cause_t cause = {.cause_value = pfcp::CAUSE_VALUE_REQUEST_REJECTED};
      a.pfcp_ies.set(cause);
      a.pfcp_ies.set(node_id);
      a.r_endpoint = remote_endpoint;
      pgwc_sxab_inst->send_sx_msg(a);
    }
  }
}

//------------------------------------------------------------------------------
bool pgwc::PfcpUpNodes::GetUpNode(
    const pfcp::node_id_t& node_id, std::shared_ptr<PfcpUpNode>& su) const {
  std::size_t hash_node_id = std::hash<pfcp::node_id_t>{}(node_id);
  auto pit                 = up_nodes_.find((int32_t) hash_node_id);
  if (pit == up_nodes_.end())
    return false;
  else {
    su = pit->second;
    return true;
  }
}
//------------------------------------------------------------------------------
bool pgwc::PfcpUpNodes::GetUpNode(
    const std::size_t hash_node_id, std::shared_ptr<PfcpUpNode>& su) const {
  auto pit = up_nodes_.find((int32_t) hash_node_id);
  if (pit == up_nodes_.end())
    return false;
  else {
    su = pit->second;
    return true;
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::NotifyNodeNotResponding(
    const pfcp::node_id_t& node_id) {
  std::shared_ptr<PfcpUpNode> su = {};
  if (GetUpNode(node_id, su)) {
    su->NotifyNodeNotResponding();
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::NotifyNodeNotResponding(
    const std::size_t hash_node_id) {
  std::shared_ptr<PfcpUpNode> su = {};
  if (GetUpNode(hash_node_id, su)) {
    su->NotifyNodeNotResponding();
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::NotifyNodeNotReachable(const pfcp::node_id_t& node_id) {
  std::shared_ptr<PfcpUpNode> su = {};
  if (GetUpNode(node_id, su)) {
    su->NotifyNodeNotReachable();
    if (su->association_state_ == kAssocLost) {
      up_nodes_.erase((int32_t) su->hash_node_id_);
      std::unique_lock<std::mutex> l(m_pending_nodes);
      pending_nodes_.push_back(su);
    }
  } else {
    Logger::pgwc_app().error(
        "User Plane Node %s detected not reachable, unknown!",
        node_id_.toString().c_str());
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::NotifyNodeNotReachable(const std::size_t hash_node_id) {
  std::shared_ptr<PfcpUpNode> su = {};
  if (GetUpNode(hash_node_id, su)) {
    su->NotifyNodeNotReachable();
  } else {
    Logger::pgwc_app().error(
        "User Plane Node %ld detected not reachable, unknown!", hash_node_id);
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::NotifyNodeReachable(const pfcp::node_id_t& node_id) {
  std::shared_ptr<PfcpUpNode> su = {};
  if (GetUpNode(node_id, su)) {
    su->NotifyNodeReachable();
  } else {
    Logger::pgwc_app().error(
        "User Plane Node %s detected reachable, unknown!",
        node_id_.toString().c_str());
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::NotifyNodeReachable(const std::size_t hash_node_id) {
  std::shared_ptr<PfcpUpNode> su = {};
  if (GetUpNode(hash_node_id, su)) {
    su->NotifyNodeReachable();
  } else {
    Logger::pgwc_app().error(
        "User Plane Node %ld detected reachable, unknown!", hash_node_id);
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::NotifyNodeRestarted(const pfcp::node_id_t& node_id) {
  std::shared_ptr<PfcpUpNode> su = {};
  if (GetUpNode(node_id, su)) {
    su->NotifyNodeRestarted();
  } else {
    Logger::pgwc_app().error(
        "User Plane Node %s detected restarted, unknown!",
        node_id_.toString().c_str());
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::NotifyNodeRestarted(const std::size_t hash_node_id) {
  std::shared_ptr<PfcpUpNode> su = {};
  if (GetUpNode(hash_node_id, su)) {
    su->NotifyNodeRestarted();
  } else {
    Logger::pgwc_app().error(
        "User Plane Node %ld detected restarted, unknown!", hash_node_id);
  }
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::NotifyNodeAssociated(const pfcp::node_id_t& node_id) {
  std::unique_lock<std::mutex> l(m_pending_nodes);
  for (auto it = pending_nodes_.begin(); it != pending_nodes_.end(); ++it) {
    // for (auto it : pending_nodes_) {
    Logger::pgwc_app().error(
        "User Plane Node %ld detected associated, testing",
        (*it)->hash_node_id_);
    if (node_id.toString().compare((*it)->id_) == 0) {
      (*it)->node_id_                = node_id;
      (*it)->hash_node_id_           = std::hash<pfcp::node_id_t>{}(node_id);
      std::shared_ptr<PfcpUpNode> sp = *it;
      up_nodes_.insert((int32_t)(*it)->hash_node_id_, *it);
      pending_nodes_.erase(it);
      return;
    }
  }
  Logger::pgwc_app().error(
      "User Plane Node %ld detected associated, unknown!",
      node_id.toString().c_str());
}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::TimeOutNotification(
    const timer_id_t tid, uint64_t arg1_user, uint64_t arg2_user) {}
//------------------------------------------------------------------------------
void pgwc::PfcpUpNodes::Receive(
    std::unique_ptr<pfcp::pfcp_association_setup_response> t_msg,
    const endpoint& t_remote_endpoint, const uint64_t t_trxn_id) {}
