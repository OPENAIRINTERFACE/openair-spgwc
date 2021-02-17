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

/*! \file sgw_s5s8.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "sgwc_s5s8.hpp"
#include "common_defs.h"
#include "itti.hpp"
#include "logger.hpp"
#include "pgw_config.hpp"

#include <stdexcept>

using namespace gtpv2c;
using namespace sgwc;
using namespace std;

extern itti_mw* itti_inst;
extern sgw_s5s8* sgw_s5s8_inst;

void sgw_s5s8_task(void*);

//------------------------------------------------------------------------------

void sgw_s5s8_task(void* args_p) {
  const task_id_t task_id = TASK_SGWC_S5S8;
  itti_inst->notify_task_ready(task_id);

  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case S5S8_CREATE_SESSION_REQUEST:
        if (itti_s5s8_create_session_request* m =
                dynamic_cast<itti_s5s8_create_session_request*>(msg)) {
          sgw_s5s8_inst->send_msg(ref(*m));
        }
        break;

      case S5S8_MODIFY_BEARER_REQUEST:
        if (itti_s5s8_modify_bearer_request* m =
                dynamic_cast<itti_s5s8_modify_bearer_request*>(msg)) {
          sgw_s5s8_inst->send_msg(ref(*m));
        }
        break;

      case S5S8_RELEASE_ACCESS_BEARERS_REQUEST:
        if (itti_s5s8_release_access_bearers_request* m =
                dynamic_cast<itti_s5s8_release_access_bearers_request*>(msg)) {
          sgw_s5s8_inst->send_msg(ref(*m));
        }
        break;

      case S5S8_DELETE_SESSION_REQUEST:
        if (itti_s5s8_delete_session_request* m =
                dynamic_cast<itti_s5s8_delete_session_request*>(msg)) {
          sgw_s5s8_inst->send_msg(ref(*m));
        }
        break;

      case S5S8_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE:
        if (itti_s5s8_downlink_data_notification_acknowledge* m =
                dynamic_cast<itti_s5s8_downlink_data_notification_acknowledge*>(
                    msg)) {
          sgw_s5s8_inst->send_msg(ref(*m));
        }
        break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          Logger::sgwc_s5s8().debug("TIME-OUT event timer id %d", to->timer_id);
          sgw_s5s8_inst->time_out_itti_event(to->timer_id);
        }
        break;

      case TERMINATE:
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::sgwc_s5s8().info("Received terminate message");
          return;
        }
        break;

      case HEALTH_PING:
        break;

      default:
        Logger::sgwc_s5s8().info("no handler for msg type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
sgw_s5s8::sgw_s5s8()
    : gtpv2c_stack(
          string(inet_ntoa(pgwc::pgw_config::sgw_s5s8_.iface.addr4)),
          pgwc::pgw_config::gtpv2c_.port,
          pgwc::pgw_config::gtpv2c_.sched_params) {
  Logger::sgwc_s5s8().startup("Starting...");
  if (itti_inst->create_task(TASK_SGWC_S5S8, sgw_s5s8_task, nullptr)) {
    Logger::sgwc_s5s8().error("Cannot create task TASK_SGWC_S5S8");
    throw std::runtime_error("Cannot create task TASK_SGWC_S5S8");
  }
  Logger::sgwc_s5s8().startup("Started");
}
//------------------------------------------------------------------------------
void sgw_s5s8::send_msg(itti_s5s8_create_session_request& i) {
  send_initial_message(
      i.r_endpoint, i.teid, i.l_teid, i.gtp_ies, TASK_SGWC_S5S8, i.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgw_s5s8::send_msg(itti_s5s8_delete_session_request& i) {
  send_initial_message(
      i.r_endpoint, i.teid, i.l_teid, i.gtp_ies, TASK_SGWC_S5S8, i.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgw_s5s8::send_msg(itti_s5s8_modify_bearer_request& i) {
  send_initial_message(
      i.r_endpoint, i.teid, i.l_teid, i.gtp_ies, TASK_SGWC_S5S8, i.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgw_s5s8::send_msg(itti_s5s8_release_access_bearers_request& i) {
  send_initial_message(
      i.r_endpoint, i.teid, i.l_teid, i.gtp_ies, TASK_SGWC_S5S8, i.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgw_s5s8::send_msg(itti_s5s8_downlink_data_notification_acknowledge& i) {
  send_triggered_message(i.r_endpoint, i.teid, i.gtp_ies, i.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgw_s5s8::handle_receive_create_session_response(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                                       = true;
  uint64_t gtpc_tx_id                              = 0;
  gtpv2c_create_session_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S5S8, error, gtpc_tx_id);
  if (!error) {
    itti_s5s8_create_session_response* itti_msg =
        new itti_s5s8_create_session_response(TASK_SGWC_S5S8, TASK_SGWC_APP);
    itti_msg->gtp_ies    = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->gtpc_tx_id = gtpc_tx_id;
    itti_msg->teid       = msg.get_teid();
    std::shared_ptr<itti_s5s8_create_session_response> i =
        std::shared_ptr<itti_s5s8_create_session_response>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::sgwc_s5s8().error(
          "Could not send ITTI message %s to task TASK_SGWC_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void sgw_s5s8::handle_receive_modify_bearer_response(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                                      = true;
  uint64_t gtpc_tx_id                             = 0;
  gtpv2c_modify_bearer_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S5S8, error, gtpc_tx_id);
  if (!error) {
    itti_s5s8_modify_bearer_response* itti_msg =
        new itti_s5s8_modify_bearer_response(TASK_SGWC_S5S8, TASK_SGWC_APP);
    itti_msg->gtp_ies    = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->gtpc_tx_id = gtpc_tx_id;
    itti_msg->teid       = msg.get_teid();
    std::shared_ptr<itti_s5s8_modify_bearer_response> i =
        std::shared_ptr<itti_s5s8_modify_bearer_response>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::sgwc_s5s8().error(
          "Could not send ITTI message %s to task TASK_SGWC_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void sgw_s5s8::handle_receive_release_access_bearers_response(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                                               = true;
  uint64_t gtpc_tx_id                                      = 0;
  gtpv2c_release_access_bearers_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S5S8, error, gtpc_tx_id);
  if (!error) {
    itti_s5s8_release_access_bearers_response* itti_msg =
        new itti_s5s8_release_access_bearers_response(
            TASK_SGWC_S5S8, TASK_SGWC_APP);
    itti_msg->gtp_ies    = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->gtpc_tx_id = gtpc_tx_id;
    itti_msg->teid       = msg.get_teid();
    std::shared_ptr<itti_s5s8_release_access_bearers_response> i =
        std::shared_ptr<itti_s5s8_release_access_bearers_response>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::sgwc_s5s8().error(
          "Could not send ITTI message %s to task TASK_SGWC_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void sgw_s5s8::handle_receive_delete_session_response(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                                       = true;
  uint64_t gtpc_tx_id                              = 0;
  gtpv2c_delete_session_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S5S8, error, gtpc_tx_id);
  if (!error) {
    itti_s5s8_delete_session_response* itti_msg =
        new itti_s5s8_delete_session_response(TASK_SGWC_S5S8, TASK_SGWC_APP);
    itti_msg->gtp_ies    = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->gtpc_tx_id = gtpc_tx_id;
    itti_msg->teid       = msg.get_teid();
    std::shared_ptr<itti_s5s8_delete_session_response> i =
        std::shared_ptr<itti_s5s8_delete_session_response>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::sgwc_s5s8().error(
          "Could not send ITTI message %s to task TASK_SGWC_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void sgw_s5s8::handle_receive_downlink_data_notification(
    gtpv2c::gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                                          = true;
  uint64_t gtpc_tx_id                                 = 0;
  gtpv2c_downlink_data_notification msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S5S8, error, gtpc_tx_id);
  if (!error) {
    itti_s5s8_downlink_data_notification* itti_msg =
        new itti_s5s8_downlink_data_notification(TASK_SGWC_S5S8, TASK_SGWC_APP);
    itti_msg->gtp_ies    = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->gtpc_tx_id = gtpc_tx_id;
    itti_msg->teid       = msg.get_teid();
    std::shared_ptr<itti_s5s8_downlink_data_notification> i =
        std::shared_ptr<itti_s5s8_downlink_data_notification>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::sgwc_s5s8().error(
          "Could not send ITTI message %s to task TASK_SGWC_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}

//------------------------------------------------------------------------------
void sgw_s5s8::handle_receive_gtpv2c_msg(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  // Logger::sgwc_s5s8().trace( "handle_receive_gtpv2c_msg msg type %d length
  // %d", msg.get_message_type(), msg.get_message_length());
  switch (msg.get_message_type()) {
    case GTP_CREATE_SESSION_REQUEST: {
    } break;
    case GTP_ECHO_REQUEST:
    case GTP_ECHO_RESPONSE:
    case GTP_VERSION_NOT_SUPPORTED_INDICATION:
    case GTP_CREATE_SESSION_RESPONSE:
      handle_receive_create_session_response(msg, remote_endpoint);
      break;
    case GTP_MODIFY_BEARER_REQUEST: {
    } break;
    case GTP_MODIFY_BEARER_RESPONSE:
      handle_receive_modify_bearer_response(msg, remote_endpoint);
      break;
    case GTP_DELETE_SESSION_REQUEST: {
      // handle_receive_delete_session_request(msg, remote_endpoint);
    } break;
    case GTP_DELETE_SESSION_RESPONSE: {
      handle_receive_delete_session_response(msg, remote_endpoint);
    } break;
    case GTP_RELEASE_ACCESS_BEARERS_RESPONSE: {
      handle_receive_release_access_bearers_response(msg, remote_endpoint);
    } break;
    case GTP_DOWNLINK_DATA_NOTIFICATION: {
      handle_receive_downlink_data_notification(msg, remote_endpoint);
    } break;

    case GTP_CHANGE_NOTIFICATION_REQUEST:
    case GTP_CHANGE_NOTIFICATION_RESPONSE:
    case GTP_REMOTE_UE_REPORT_NOTIFICATION:
    case GTP_REMOTE_UE_REPORT_ACKNOWLEDGE:
    case GTP_MODIFY_BEARER_COMMAND:
    case GTP_MODIFY_BEARER_FAILURE_INDICATION:
    case GTP_DELETE_BEARER_COMMAND:
    case GTP_DELETE_BEARER_FAILURE_INDICATION:
    case GTP_BEARER_RESOURCE_COMMAND:
    case GTP_BEARER_RESOURCE_FAILURE_INDICATION:
    case GTP_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION:
    case GTP_TRACE_SESSION_ACTIVATION:
    case GTP_TRACE_SESSION_DEACTIVATION:
    case GTP_STOP_PAGING_INDICATION:
    case GTP_CREATE_BEARER_REQUEST:
    case GTP_CREATE_BEARER_RESPONSE:
    case GTP_UPDATE_BEARER_REQUEST:
    case GTP_UPDATE_BEARER_RESPONSE:
    case GTP_DELETE_BEARER_REQUEST:
    case GTP_DELETE_BEARER_RESPONSE:
    case GTP_DELETE_PDN_CONNECTION_SET_REQUEST:
    case GTP_DELETE_PDN_CONNECTION_SET_RESPONSE:
    case GTP_PGW_DOWNLINK_TRIGGERING_NOTIFICATION:
    case GTP_PGW_DOWNLINK_TRIGGERING_ACKNOWLEDGE:
    case GTP_IDENTIFICATION_REQUEST:
    case GTP_IDENTIFICATION_RESPONSE:
    case GTP_CONTEXT_REQUEST:
    case GTP_CONTEXT_RESPONSE:
    case GTP_CONTEXT_ACKNOWLEDGE:
    case GTP_FORWARD_RELOCATION_REQUEST:
    case GTP_FORWARD_RELOCATION_RESPONSE:
    case GTP_FORWARD_RELOCATION_COMPLETE_NOTIFICATION:
    case GTP_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE:
    case GTP_FORWARD_ACCESS_CONTEXT_NOTIFICATION:
    case GTP_FORWARD_ACCESS_CONTEXT_ACKNOWLEDGE:
    case GTP_RELOCATION_CANCEL_REQUEST:
    case GTP_RELOCATION_CANCEL_RESPONSE:
    case GTP_CONFIGURATION_TRANSFER_TUNNEL_MESSAGE:
    case GTP_DETACH_NOTIFICATION:
    case GTP_DETACH_ACKNOWLEDGE:
    case GTP_CS_PAGING_INDICATION:
    case GTP_RAN_INFORMATION_RELAY:
    case GTP_ALERT_MME_NOTIFICATION:
    case GTP_ALERT_MME_ACKNOWLEDGE:
    case GTP_UE_ACTIVITY_NOTIFICATION:
    case GTP_UE_ACTIVITY_ACKNOWLEDGE:
    case GTP_ISR_STATUS_INDICATION:
    case GTP_UE_REGISTRATION_QUERY_REQUEST:
    case GTP_UE_REGISTRATION_QUERY_RESPONSE:
    case GTP_CREATE_FORWARDING_TUNNEL_REQUEST:
    case GTP_CREATE_FORWARDING_TUNNEL_RESPONSE:
    case GTP_SUSPEND_NOTIFICATION:
    case GTP_SUSPEND_ACKNOWLEDGE:
    case GTP_RESUME_NOTIFICATION:
    case GTP_RESUME_ACKNOWLEDGE:
    case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST:
    case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE:
    case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST:
    case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_RESPONSE:
    case GTP_RELEASE_ACCESS_BEARERS_REQUEST:
    case GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE:
    case GTP_PGW_RESTART_NOTIFICATION:
    case GTP_PGW_RESTART_NOTIFICATION_ACKNOWLEDGE:
    case GTP_UPDATE_PDN_CONNECTION_SET_REQUEST:
    case GTP_UPDATE_PDN_CONNECTION_SET_RESPONSE:
    case GTP_MODIFY_ACCESS_BEARERS_REQUEST:
    case GTP_MODIFY_ACCESS_BEARERS_RESPONSE:
    case GTP_MBMS_SESSION_START_REQUEST:
    case GTP_MBMS_SESSION_START_RESPONSE:
    case GTP_MBMS_SESSION_UPDATE_REQUEST:
    case GTP_MBMS_SESSION_UPDATE_RESPONSE:
    case GTP_MBMS_SESSION_STOP_RESPONSE:
      break;
    default:
      Logger::sgwc_s5s8().error(
          "handle_receive_gtpv2c_msg msg length %d", msg.get_message_length());
  }
}
//------------------------------------------------------------------------------
void sgw_s5s8::handle_receive(
    char* recv_buffer, const std::size_t bytes_transferred,
    const endpoint& remote_endpoint) {
  // Logger::sgwc_s5s8().info( "handle_receive(%d bytes)", bytes_transferred);
  // std::cout << string_to_hex(recv_buffer, bytes_transferred) << std::endl;
  std::istringstream iss(std::istringstream::binary);
  iss.rdbuf()->pubsetbuf(recv_buffer, bytes_transferred);
  gtpv2c_msg msg  = {};
  msg.remote_port = remote_endpoint.port();
  try {
    msg.load_from(iss);
    handle_receive_gtpv2c_msg(msg, remote_endpoint);
  } catch (gtpc_exception& e) {
    Logger::sgwc_s5s8().info("handle_receive exception %s", e.what());
  }
}
//------------------------------------------------------------------------------
void sgw_s5s8::notify_ul_error(
    const endpoint& r_endpoint, const teid_t l_teid, const cause_value_e cause,
    const uint64_t gtpc_tx_id) {
  switch (cause) {
    case cause_value_e::REMOTE_PEER_NOT_RESPONDING: {
      itti_s5s8_remote_peer_not_responding* itti_msg =
          new itti_s5s8_remote_peer_not_responding(
              TASK_SGWC_S5S8, TASK_SGWC_APP);
      itti_msg->r_endpoint = r_endpoint;
      itti_msg->gtpc_tx_id = gtpc_tx_id;
      itti_msg->l_teid     = l_teid;
      std::shared_ptr<itti_s5s8_remote_peer_not_responding> i =
          std::shared_ptr<itti_s5s8_remote_peer_not_responding>(itti_msg);
      int ret = itti_inst->send_msg(i);
      if (RETURNok != ret) {
        Logger::sgwc_s5s8().error(
            "Could not send ITTI message %s to task TASK_SGWC_APP",
            i->get_msg_name());
      }
    } break;
    default:
      Logger::sgwc_s5s8().warn("notify_ul_error cause %d not handled", cause);
  }
}
//------------------------------------------------------------------------------
void sgw_s5s8::time_out_itti_event(const uint32_t timer_id) {
  bool handled = false;
  time_out_event(timer_id, TASK_SGWC_S5S8, handled);
  if (!handled) {
    Logger::sgwc_s5s8().warn("Timer %d not Found", timer_id);
  }
}
