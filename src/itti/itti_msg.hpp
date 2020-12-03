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
/*! \file itti_msg.hpp
   \brief
   \author  Lionel GAUTHIER
   \date 2018
   \email: lionel.gauthier@eurecom.fr
*/
#ifndef SRC_ITTI_ITTI_MSG_HPP_INCLUDED_
#define SRC_ITTI_ITTI_MSG_HPP_INCLUDED_

#include <stdint.h>
#include <utility>

typedef enum {
  TASK_FIRST      = 0,
  TASK_ITTI_TIMER = TASK_FIRST,
  TASK_ASYNC_SHELL_CMD,
  TASK_ENB_S1U,
  TASK_GTPV1_U,
  TASK_GTPV2_C,
  TASK_MME_S11,
  TASK_PGWC_APP,
  TASK_PGWU_APP,
  TASK_SPGWU_APP,
  TASK_PGWC_S5S8,
  TASK_PGWC_SX,
  TASK_PGWU_SX,
  TASK_PGW_UDP,
  TASK_SGWC_APP,
  TASK_SGWC_S11,
  TASK_SGWC_S5S8,
  TASK_SGWC_SXA,
  TASK_SGWU_SXA,
  TASK_SPGWU_SX,
  TASK_SPGWU_S1U,
  TASK_SGW_UDP,
  TASK_MAX,
  TASK_NONE,
  TASK_ALL = 255
} task_id_t;

typedef enum message_priorities_e {
  MESSAGE_PRIORITY_MAX       = 100,
  MESSAGE_PRIORITY_MAX_LEAST = 85,
  MESSAGE_PRIORITY_MED_PLUS  = 70,
  MESSAGE_PRIORITY_MED       = 55,
  MESSAGE_PRIORITY_MED_LEAST = 40,
  MESSAGE_PRIORITY_MIN_PLUS  = 25,
  MESSAGE_PRIORITY_MIN       = 10,
} message_priorities_t;

typedef enum {
  ITTI_MSG_TYPE_NONE  = -1,
  ITTI_MSG_TYPE_FIRST = 0,
  ASYNC_SHELL_CMD     = ITTI_MSG_TYPE_FIRST,
  RESTORE_SX_SESSIONS,
  S11_REMOTE_PEER_NOT_RESPONDING,
  S11_CREATE_SESSION_REQUEST,
  S11_CREATE_SESSION_RESPONSE,
  S11_CREATE_BEARER_REQUEST,
  S11_CREATE_BEARER_RESPONSE,
  S11_MODIFY_BEARER_REQUEST,
  S11_MODIFY_BEARER_RESPONSE,
  S11_DELETE_BEARER_COMMAND,
  S11_DELETE_BEARER_FAILURE_INDICATION,
  S11_DELETE_SESSION_REQUEST,
  S11_DELETE_SESSION_RESPONSE,
  S11_RELEASE_ACCESS_BEARERS_REQUEST,
  S11_RELEASE_ACCESS_BEARERS_RESPONSE,
  S11_DOWNLINK_DATA_NOTIFICATION,
  S11_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE,
  S11_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION,
  S1U_ECHO_REQUEST,
  S1U_ECHO_RESPONSE,
  S1U_ERROR_INDICATION,
  S1U_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION,
  S1U_END_MARKER,
  S1U_G_PDU,  // UNUSED
  S5S8_REMOTE_PEER_NOT_RESPONDING,
  S5S8_CREATE_SESSION_REQUEST,
  S5S8_CREATE_SESSION_RESPONSE,
  S5S8_CREATE_BEARER_REQUEST,
  S5S8_CREATE_BEARER_RESPONSE,
  S5S8_MODIFY_BEARER_REQUEST,
  S5S8_MODIFY_BEARER_RESPONSE,
  S5S8_DELETE_BEARER_COMMAND,
  S5S8_DELETE_BEARER_FAILURE_INDICATION,
  S5S8_DELETE_SESSION_REQUEST,
  S5S8_DELETE_SESSION_RESPONSE,
  S5S8_RELEASE_ACCESS_BEARERS_REQUEST,
  S5S8_RELEASE_ACCESS_BEARERS_RESPONSE,
  S5S8_DOWNLINK_DATA_NOTIFICATION,
  S5S8_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE,
  S5S8_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION,
  SXAB_HEARTBEAT_REQUEST,
  SXAB_HEARTBEAT_RESPONSE,
  SXAB_PFCP_PFD_MANAGEMENT_REQUEST,
  SXAB_PFCP_PFD_MANAGEMENT_RESPONSE,
  SXAB_ASSOCIATION_SETUP_REQUEST,
  SXAB_ASSOCIATION_SETUP_RESPONSE,
  SXAB_ASSOCIATION_UPDATE_REQUEST,
  SXAB_ASSOCIATION_UPDATE_RESPONSE,
  SXAB_ASSOCIATION_RELEASE_REQUEST,
  SXAB_ASSOCIATION_RELEASE_RESPONSE,
  SXAB_VERSION_NOT_SUPPORTED_RESPONSE,
  SXAB_NODE_REPORT_REQUEST,
  SXAB_NODE_REPORT_RESPONSE,
  SXAB_SESSION_SET_DELETION_REQUEST,
  SXAB_SESSION_SET_DELETION_RESPONSE,
  SXAB_SESSION_ESTABLISHMENT_REQUEST,
  SXAB_SESSION_ESTABLISHMENT_RESPONSE,
  SXAB_SESSION_MODIFICATION_REQUEST,
  SXAB_SESSION_MODIFICATION_RESPONSE,
  SXAB_SESSION_DELETION_REQUEST,
  SXAB_SESSION_DELETION_RESPONSE,
  SXAB_SESSION_REPORT_REQUEST,
  SXAB_SESSION_REPORT_RESPONSE,
  UDP_INIT,
  UDP_DATA_REQ,
  UDP_DATA_IND,
  TIME_OUT,
  HEALTH_PING,
  TERMINATE,
  ITTI_MSG_TYPE_MAX
} itti_msg_type_t;

typedef unsigned long message_number_t;

class itti_msg {
 public:
  itti_msg();
  itti_msg(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination);
  itti_msg(const itti_msg& i);

  itti_msg& operator=(itti_msg other) {
    std::swap(msg_num, other.msg_num);
    std::swap(origin, other.origin);
    std::swap(destination, other.destination);
    std::swap(msg_type, other.msg_type);
    return *this;
  }

  virtual ~itti_msg() = default;
  static const char* get_msg_name();

  message_number_t msg_num;
  task_id_t origin;
  task_id_t destination;
  itti_msg_type_t msg_type;
};

class itti_msg_timeout : public itti_msg {
 public:
  itti_msg_timeout(
      const task_id_t origin, const task_id_t destination, uint32_t timer_id,
      uint64_t arg1_user, uint64_t arg2_user)
      : itti_msg(TIME_OUT, origin, destination),
        timer_id(timer_id),
        arg1_user(arg1_user),
        arg2_user(arg2_user) {}
  itti_msg_timeout(const itti_msg_timeout& i)
      : itti_msg(i),
        timer_id(i.timer_id),
        arg1_user(i.arg1_user),
        arg2_user(i.arg2_user) {}
  static const char* get_msg_name() { return "TIME_OUT"; };
  uint32_t timer_id;
  uint64_t arg1_user;
  uint64_t arg2_user;
};

class itti_msg_ping : public itti_msg {
 public:
  itti_msg_ping(
      const task_id_t origin, const task_id_t destination, uint32_t seq)
      : itti_msg(HEALTH_PING, origin, destination), seq(seq) {}
  itti_msg_ping(const itti_msg_ping& i) : itti_msg(i), seq(i.seq) {}
  static const char* get_msg_name() { return "HEALTH_PING"; };
  uint32_t seq;
};

class itti_msg_terminate : public itti_msg {
 public:
  itti_msg_terminate(const task_id_t origin, const task_id_t destination)
      : itti_msg(TERMINATE, origin, destination) {}
  itti_msg_terminate(const itti_msg_terminate& i) : itti_msg(i) {}
  static const char* get_msg_name() { return "TERMINATE"; };
};

#endif /* SRC_ITTI_ITTI_MSG_HPP_INCLUDED_ */
