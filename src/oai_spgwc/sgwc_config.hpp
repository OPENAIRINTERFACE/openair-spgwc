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

/*! \file sgwc_config.hpp
 * \brief
 * \author Lionel Gauthier
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */

#ifndef FILE_SGWC_CONFIG_HPP_SEEN
#define FILE_SGWC_CONFIG_HPP_SEEN

#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <libconfig.h++>
#include <mutex>
#include "gtpv2c.hpp"
#include "thread_sched.hpp"

namespace sgwc {

#define SGWC_CONFIG_STRING_SGW_CONFIG "S-GW"
#define SGWC_CONFIG_STRING_PID_DIRECTORY "PID_DIRECTORY"
#define SGWC_CONFIG_STRING_INSTANCE "INSTANCE"
#define SGWC_CONFIG_STRING_INTERFACES "INTERFACES"
#define SGWC_CONFIG_STRING_INTERFACE_NAME "INTERFACE_NAME"
#define SGWC_CONFIG_STRING_IPV4_ADDRESS "IPV4_ADDRESS"
#define SGWC_CONFIG_STRING_PORT "PORT"
#define SGWC_CONFIG_STRING_INTERFACE_S11_CP "S11_CP"
#define SGWC_CONFIG_STRING_INTERFACE_S11_UP "S11_UP"
#define SGWC_CONFIG_STRING_INTERFACE_S5_S8_CP "S5_S8_CP"

#define SGWC_CONFIG_STRING_ITTI_TASKS "ITTI_TASKS"
#define SGWC_CONFIG_STRING_ITTI_TIMER_SCHED_PARAMS "ITTI_TIMER_SCHED_PARAMS"
#define SGWC_CONFIG_STRING_S11_SCHED_PARAMS "S11_SCHED_PARAMS"
#define SGWC_CONFIG_STRING_S5S8_SCHED_PARAMS "S5S8_SCHED_PARAMS"
#define SGWC_CONFIG_STRING_SX_SCHED_PARAMS "SX_SCHED_PARAMS"
#define SGWC_CONFIG_STRING_SGW_APP_SCHED_PARAMS "SGW_APP_SCHED_PARAMS"
#define SGWC_CONFIG_STRING_ASYNC_CMD_SCHED_PARAMS "ASYNC_CMD_SCHED_PARAMS"

#define SGWC_CONFIG_STRING_SCHED_PARAMS "SCHED_PARAMS"
#define SGWC_CONFIG_STRING_THREAD_RD_CPU_ID "CPU_ID"
#define SGWC_CONFIG_STRING_THREAD_RD_SCHED_POLICY "SCHED_POLICY"
#define SGWC_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY "SCHED_PRIORITY"

#define SPGW_ABORT_ON_ERROR true
#define SPGW_WARN_ON_ERROR false

typedef struct interface_cfg_s {
  std::string if_name;
  struct in_addr addr4;
  struct in_addr network4;
  struct in6_addr addr6;
  unsigned int mtu;
  unsigned int port;
  util::thread_sched_params thread_rd_sched_params;
} interface_cfg_t;

typedef struct itti_cfg_s {
  util::thread_sched_params itti_timer_sched_params;
  util::thread_sched_params s11_sched_params;
  util::thread_sched_params s5s8_sched_params;
  // util::thread_sched_params sx_sched_params;
  util::thread_sched_params sgw_app_sched_params;
  util::thread_sched_params async_cmd_sched_params;
} itti_cfg_t;

class sgwc_config {
 private:
  int load_thread_sched_params(
      const libconfig::Setting& thread_sched_params_cfg,
      util::thread_sched_params& cfg);
  int load_itti(const libconfig::Setting& itti_cfg, itti_cfg_t& cfg);
  int load_interface(const libconfig::Setting& if_cfg, interface_cfg_t& cfg);

 public:
  /* Reader/writer lock for this configuration */
  std::mutex m_rw_lock;
  std::string pid_dir;
  unsigned int instance;
  interface_cfg_t s11_cp;
  interface_cfg_t s11_up;
  interface_cfg_t s5s8_cp;
  // interface_cfg_t sxa;
  itti_cfg_t itti;

  sgwc_config()
      : m_rw_lock(), pid_dir(), instance(0), s11_cp(), s11_up(), s5s8_cp() {
    itti.itti_timer_sched_params.sched_priority = 85;
    itti.s11_sched_params.sched_priority        = 84;
    itti.s5s8_sched_params.sched_priority       = 84;
    itti.sgw_app_sched_params.sched_priority    = 84;
    itti.async_cmd_sched_params.sched_priority  = 84;

    s11_cp.thread_rd_sched_params.sched_priority = 95;
    s11_cp.port                                  = gtpv2c::default_port;

    s11_up.thread_rd_sched_params.sched_priority = 95;

    s5s8_cp.thread_rd_sched_params.sched_priority = 95;
    s5s8_cp.port                                  = gtpv2c::default_port;
  };
  void lock() { m_rw_lock.lock(); };
  void unlock() { m_rw_lock.unlock(); };
  int load(const std::string& config_file);
  int execute();
  void display();
};
}  // namespace sgwc

#endif /* FILE_SGWC_CONFIG_HPP_SEEN */
