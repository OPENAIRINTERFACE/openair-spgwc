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

/*! \file sgw_s11.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "sgwc_s11.hpp"
#include "common_defs.h"
#include "itti.hpp"
#include "logger.hpp"
#include "pgw_config.hpp"

#include <stdexcept>

using namespace gtpv2c;
using namespace sgwc;
using namespace std;

extern itti_mw* itti_inst;
extern sgw_s11* sgw_s11_inst;

void sgw_s11_task(void*);

static std::string string_to_hex(
    const char* const input, const std::size_t bytes_transferred) {
  static const char* const lut = "0123456789ABCDEF";
  size_t len                   = bytes_transferred;

  std::string output;
  output.reserve(2 * len);
  for (size_t i = 0; i < len; ++i) {
    const unsigned char c = input[i];
    output.push_back(lut[c >> 4]);
    output.push_back(lut[c & 15]);
  }
  return output;
}

//------------------------------------------------------------------------------

void sgw_s11_task(void* args_p) {
  const task_id_t task_id = TASK_SGWC_S11;
  itti_inst->notify_task_ready(task_id);

  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case S11_CREATE_SESSION_RESPONSE:
        if (itti_s11_create_session_response* m =
                dynamic_cast<itti_s11_create_session_response*>(msg)) {
          sgw_s11_inst->send_msg(ref(*m));
        }
        break;

      case S11_DELETE_SESSION_RESPONSE:
        if (itti_s11_delete_session_response* m =
                dynamic_cast<itti_s11_delete_session_response*>(msg)) {
          sgw_s11_inst->send_msg(ref(*m));
        }
        break;

      case S11_MODIFY_BEARER_RESPONSE:
        if (itti_s11_modify_bearer_response* m =
                dynamic_cast<itti_s11_modify_bearer_response*>(msg)) {
          sgw_s11_inst->send_msg(ref(*m));
        }
        break;

      case S11_RELEASE_ACCESS_BEARERS_RESPONSE:
        if (itti_s11_release_access_bearers_response* m =
                dynamic_cast<itti_s11_release_access_bearers_response*>(msg)) {
          sgw_s11_inst->send_msg(ref(*m));
        }
        break;

      case S11_DOWNLINK_DATA_NOTIFICATION:
        if (itti_s11_downlink_data_notification* m =
                dynamic_cast<itti_s11_downlink_data_notification*>(msg)) {
          sgw_s11_inst->send_msg(ref(*m));
        }
        break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          Logger::sgwc_s11().debug("TIME-OUT event timer id %d", to->timer_id);
          sgw_s11_inst->time_out_itti_event(to->timer_id);
        }
        break;

      case TERMINATE:
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::sgwc_s11().info("Received terminate message");
          return;
        }
        break;

      case HEALTH_PING:
        break;

      default:
        Logger::sgwc_s11().info("no handler for msg type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
sgw_s11::sgw_s11()
    : gtpv2c_stack(
          string(inet_ntoa(pgwc::pgw_config::s11_.iface.addr4)),
          pgwc::pgw_config::gtpv2c_.port,
          pgwc::pgw_config::gtpv2c_.sched_params) {
  Logger::sgwc_s11().startup("Starting...");
  if (itti_inst->create_task(TASK_SGWC_S11, sgw_s11_task, nullptr)) {
    Logger::sgwc_s11().error("Cannot create task TASK_SGWC_S11");
    throw std::runtime_error("Cannot create task TASK_SGWC_S11");
  }
  Logger::sgwc_s11().startup("Started");
}

//------------------------------------------------------------------------------
void sgw_s11::send_msg(itti_s11_create_session_response& i) {
  send_triggered_message(i.r_endpoint, i.teid, i.gtp_ies, i.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgw_s11::send_msg(itti_s11_delete_session_response& i) {
  send_triggered_message(i.r_endpoint, i.teid, i.gtp_ies, i.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgw_s11::send_msg(itti_s11_modify_bearer_response& i) {
  send_triggered_message(i.r_endpoint, i.teid, i.gtp_ies, i.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgw_s11::send_msg(itti_s11_release_access_bearers_response& i) {
  send_triggered_message(i.r_endpoint, i.teid, i.gtp_ies, i.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgw_s11::send_msg(itti_s11_downlink_data_notification& i) {
  send_initial_message(
      i.r_endpoint, i.teid, i.l_teid, i.gtp_ies, TASK_SGWC_S11, i.gtpc_tx_id);
}
//------------------------------------------------------------------------------
void sgw_s11::handle_receive_create_session_request(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                                      = true;
  uint64_t gtpc_tx_id                             = 0;
  gtpv2c_create_session_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S11, error, gtpc_tx_id);
  if (!error) {
    itti_s11_create_session_request* itti_msg =
        new itti_s11_create_session_request(TASK_SGWC_S11, TASK_SGWC_APP);
    itti_msg->gtp_ies    = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->gtpc_tx_id = gtpc_tx_id;
    itti_msg->teid       = msg.get_teid();
    std::shared_ptr<itti_s11_create_session_request> i =
        std::shared_ptr<itti_s11_create_session_request>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::sgwc_s11().error(
          "Could not send ITTI message %s to task TASK_SGWC_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void sgw_s11::handle_receive_delete_session_request(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                                      = true;
  uint64_t gtpc_tx_id                             = 0;
  gtpv2c_delete_session_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S11, error, gtpc_tx_id);
  if (!error) {
    itti_s11_delete_session_request* itti_msg =
        new itti_s11_delete_session_request(TASK_SGWC_S11, TASK_SGWC_APP);
    itti_msg->gtp_ies    = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->gtpc_tx_id = gtpc_tx_id;
    itti_msg->teid       = msg.get_teid();
    std::shared_ptr<itti_s11_delete_session_request> i =
        std::shared_ptr<itti_s11_delete_session_request>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::sgwc_s11().error(
          "Could not send ITTI message %s to task TASK_SGWC_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void sgw_s11::handle_receive_modify_bearer_request(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                                     = true;
  uint64_t gtpc_tx_id                            = 0;
  gtpv2c_modify_bearer_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S11, error, gtpc_tx_id);
  if (!error) {
    itti_s11_modify_bearer_request* itti_msg =
        new itti_s11_modify_bearer_request(TASK_SGWC_S11, TASK_SGWC_APP);
    itti_msg->gtp_ies    = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->gtpc_tx_id = gtpc_tx_id;
    itti_msg->teid       = msg.get_teid();
    std::shared_ptr<itti_s11_modify_bearer_request> i =
        std::shared_ptr<itti_s11_modify_bearer_request>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::sgwc_s11().error(
          "Could not send ITTI message %s to task TASK_SGWC_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void sgw_s11::handle_receive_release_access_bearers_request(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                                              = true;
  uint64_t gtpc_tx_id                                     = 0;
  gtpv2c_release_access_bearers_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S11, error, gtpc_tx_id);
  if (!error) {
    itti_s11_release_access_bearers_request* itti_msg =
        new itti_s11_release_access_bearers_request(
            TASK_SGWC_S11, TASK_SGWC_APP);
    itti_msg->gtp_ies    = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->gtpc_tx_id = gtpc_tx_id;
    itti_msg->teid       = msg.get_teid();
    std::shared_ptr<itti_s11_release_access_bearers_request> i =
        std::shared_ptr<itti_s11_release_access_bearers_request>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::sgwc_s11().error(
          "Could not send ITTI message %s to task TASK_SGWC_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void sgw_s11::handle_receive_downlink_data_notification_acknowledge(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                                                      = true;
  uint64_t gtpc_tx_id                                             = 0;
  gtpv2c_downlink_data_notification_acknowledge msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S11, error, gtpc_tx_id);
  if (!error) {
    itti_s11_downlink_data_notification_acknowledge* itti_msg =
        new itti_s11_downlink_data_notification_acknowledge(
            TASK_SGWC_S11, TASK_SGWC_APP);
    itti_msg->gtp_ies    = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->gtpc_tx_id = gtpc_tx_id;
    itti_msg->teid       = msg.get_teid();
    std::shared_ptr<itti_s11_downlink_data_notification_acknowledge> i =
        std::shared_ptr<itti_s11_downlink_data_notification_acknowledge>(
            itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::sgwc_s11().error(
          "Could not send ITTI message %s to task TASK_SGWC_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}

//------------------------------------------------------------------------------
void sgw_s11::handle_receive_echo_request(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                            = true;
  uint64_t gtpc_tx_id                   = 0;
  gtpv2c_echo_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S11, error, gtpc_tx_id);
  if (!error) {
    if (not msg_ies_container.recovery_restart_counter.first) {
      // Should be detected by lower layers
      Logger::sgwc_s11().warn(
          "Received S11 ECHO REQUEST without recovery restart counter IE!, "
          "ignore message");
      return;
    }
    Logger::sgwc_s11().info("Received S11 ECHO REQUEST");
    send_echo_response(remote_endpoint, gtpc_tx_id);
  }
}
//------------------------------------------------------------------------------
void sgw_s11::handle_receive_echo_response(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  bool error                             = true;
  uint64_t gtpc_tx_id                    = 0;
  gtpv2c_echo_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SGWC_S11, error, gtpc_tx_id);
  if (!error) {
    if (not msg_ies_container.recovery_restart_counter.first) {
      // Should be detected by lower layers
      Logger::sgwc_s11().warn(
          "Received S11 ECHO RESPONSE without recovery restart counter IE!, "
          "ignore message");
      return;
    }
    Logger::sgwc_s11().info("Received S11 ECHO RESPONSE");
    // TODO
  }
}
//------------------------------------------------------------------------------
void sgw_s11::send_echo_response(
    const endpoint& r_endpoint, const uint64_t trxn_id) {
  gtpv2c_echo_response h = {};
  recovery_t r           = {.restart_counter = 0};
  h.set(r);
  send_triggered_message(r_endpoint, h, trxn_id);
}

//------------------------------------------------------------------------------
void sgw_s11::handle_receive_gtpv2c_msg(
    gtpv2c_msg& msg, const endpoint& remote_endpoint) {
  // Logger::sgwc_s11().trace( "handle_receive_gtpv2c_msg msg type %d length
  // %d", msg.get_message_type(), msg.get_message_length());
  switch (msg.get_message_type()) {
    case GTP_CREATE_SESSION_REQUEST: {
      handle_receive_create_session_request(msg, remote_endpoint);
    } break;
    case GTP_MODIFY_BEARER_REQUEST: {
      handle_receive_modify_bearer_request(msg, remote_endpoint);
    } break;
    case GTP_DELETE_SESSION_REQUEST: {
      handle_receive_delete_session_request(msg, remote_endpoint);
    } break;
    case GTP_RELEASE_ACCESS_BEARERS_REQUEST: {
      handle_receive_release_access_bearers_request(msg, remote_endpoint);
    } break;
    case GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE: {
      handle_receive_downlink_data_notification_acknowledge(
          msg, remote_endpoint);
    } break;
    case GTP_ECHO_REQUEST: {
      handle_receive_echo_request(msg, remote_endpoint);
    } break;
    case GTP_ECHO_RESPONSE: {
      handle_receive_echo_response(msg, remote_endpoint);
    } break;
    case GTP_VERSION_NOT_SUPPORTED_INDICATION:
    case GTP_CREATE_SESSION_RESPONSE:
    case GTP_MODIFY_BEARER_RESPONSE:
    case GTP_DELETE_SESSION_RESPONSE:
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
    case GTP_RELEASE_ACCESS_BEARERS_RESPONSE:
    case GTP_DOWNLINK_DATA_NOTIFICATION:
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
      Logger::sgwc_s11().error(
          "handle_receive_gtpv2c_msg msg length %d", msg.get_message_length());
  }
}
//------------------------------------------------------------------------------
void sgw_s11::handle_receive(
    char* recv_buffer, const std::size_t bytes_transferred,
    const endpoint& remote_endpoint) {
  // Logger::sgwc_s11().info( "handle_receive(%d bytes)", bytes_transferred);
  // std::cout << string_to_hex(recv_buffer, bytes_transferred) << std::endl;
  std::istringstream iss(std::istringstream::binary);
  iss.rdbuf()->pubsetbuf(recv_buffer, bytes_transferred);
  gtpv2c_msg msg  = {};
  msg.remote_port = remote_endpoint.port();
  try {
    msg.load_from(iss);
    handle_receive_gtpv2c_msg(msg, remote_endpoint);
  } catch (gtpc_exception& e) {
    Logger::sgwc_s11().info("handle_receive exception %s", e.what());
  }
}
//------------------------------------------------------------------------------
void sgw_s11::notify_ul_error(
    const endpoint& r_endpoint, const teid_t l_teid, const cause_value_e cause,
    const uint64_t gtpc_tx_id) {
  switch (cause) {
    case cause_value_e::REMOTE_PEER_NOT_RESPONDING: {
      itti_s11_remote_peer_not_responding* itti_msg =
          new itti_s11_remote_peer_not_responding(TASK_SGWC_S11, TASK_SGWC_APP);
      itti_msg->r_endpoint = r_endpoint;
      itti_msg->gtpc_tx_id = gtpc_tx_id;
      itti_msg->teid       = l_teid;
      std::shared_ptr<itti_s11_remote_peer_not_responding> i =
          std::shared_ptr<itti_s11_remote_peer_not_responding>(itti_msg);
      int ret = itti_inst->send_msg(i);
      if (RETURNok != ret) {
        Logger::sgwc_s11().error(
            "Could not send ITTI message %s to task TASK_SGWC_APP",
            i->get_msg_name());
      }
    } break;
    default:
      Logger::sgwc_s11().warn("notify_ul_error cause %d not handled", cause);
  }
}
//------------------------------------------------------------------------------
void sgw_s11::time_out_itti_event(const uint32_t timer_id) {
  bool handled = false;
  time_out_event(timer_id, TASK_SGWC_S11, handled);
  if (!handled) {
    Logger::sgwc_s11().warn("Timer %d not Found", timer_id);
  }
}
