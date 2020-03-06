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

/*! \file SpgwcConfig.hpp
 * \brief
 * \author Lionel Gauthier
 * \date  2018-2020
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */

#ifndef FILE_SPGWC_CONFIG_HPP_SEEN
#define FILE_SPGWC_CONFIG_HPP_SEEN
// C includes ------------------------------------------------------------------
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <mutex>
//--C++ includes ---------------------------------------------------------------
#include <libconfig.h++>
//--Other includes -------------------------------------------------------------
#include "gtpv2c.hpp"
#include "pfcp.hpp"
#include "thread_sched.hpp"

namespace spgwc {
#define SPGWC_CONFIG_STRING_SGW_CONFIG "S-GW"
#define SPGWC_CONFIG_STRING_PID_DIRECTORY "PID_DIRECTORY"
#define SPGWC_CONFIG_STRING_INSTANCE "INSTANCE"
#define SPGWC_CONFIG_STRING_INTERFACES "INTERFACES"
#define SPGWC_CONFIG_STRING_INTERFACE_NAME "INTERFACE_NAME"
#define SPGWC_CONFIG_STRING_IPV4_ADDRESS "IPV4_ADDRESS"
#define SPGWC_CONFIG_STRING_PORT "PORT"
#define SPGWC_CONFIG_STRING_INTERFACE_S11_CP "S11_CP"
#define SPGWC_CONFIG_STRING_INTERFACE_S11_UP "S11_UP"
#define SPGW_CONFIG_STRING_INTERFACE_SX "SX"

#define SPGWC_CONFIG_STRING_ITTI_TASKS "ITTI_TASKS"
#define SPGWC_CONFIG_STRING_ITTI_TIMER_SCHED_PARAMS "ITTI_TIMER_SCHED_PARAMS"
#define SPGWC_CONFIG_STRING_S11_SCHED_PARAMS "S11_SCHED_PARAMS"
#define SPGWC_CONFIG_STRING_SX_SCHED_PARAMS "SX_SCHED_PARAMS"
#define SPGWC_CONFIG_STRING_SPGWC_APP_SCHED_PARAMS "SPGWC_APP_SCHED_PARAMS"

#define SPGWC_CONFIG_STRING_SCHED_PARAMS "SCHED_PARAMS"
#define SPGWC_CONFIG_STRING_THREAD_RD_CPU_ID "CPU_ID"
#define SPGWC_CONFIG_STRING_THREAD_RD_SCHED_POLICY "SCHED_POLICY"
#define SPGWC_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY "SCHED_PRIORITY"

#define SPGWC_CONFIG_STRING_PGW_MASQUERADE_SGI "PGW_MASQUERADE_SGI"
#define SPGWC_CONFIG_STRING_UE_TCP_MSS_CLAMPING "UE_TCP_MSS_CLAMPING"
#define SPGWC_CONFIG_STRING_NAS_FORCE_PUSH_PCO \
  "FORCE_PUSH_PROTOCOL_CONFIGURATION_OPTIONS"

#define SPGWC_CONFIG_STRING_IP_ADDRESS_POOL "IP_ADDRESS_POOL"
#define SPGWC_CONFIG_STRING_ARP_UE "ARP_UE"
#define SPGWC_CONFIG_STRING_ARP_UE_CHOICE_NO "NO"
#define SPGWC_CONFIG_STRING_ARP_UE_CHOICE_LINUX "LINUX"
#define SPGWC_CONFIG_STRING_ARP_UE_CHOICE_OAI "OAI"
#define SPGWC_CONFIG_STRING_IPV4_ADDRESS_LIST "IPV4_LIST"
#define SPGWC_CONFIG_STRING_IPV6_ADDRESS_LIST "IPV6_LIST"
#define SPGWC_CONFIG_STRING_RANGE "RANGE"
#define SPGWC_CONFIG_STRING_PREFIX "PREFIX"
#define SPGWC_CONFIG_STRING_IPV4_ADDRESS_RANGE_DELIMITER "-"
#define SPGWC_CONFIG_STRING_IPV6_ADDRESS_PREFIX_DELIMITER "/"
#define SPGWC_CONFIG_STRING_DEFAULT_DNS_IPV4_ADDRESS "DEFAULT_DNS_IPV4_ADDRESS"
#define SPGWC_CONFIG_STRING_DEFAULT_DNS_SEC_IPV4_ADDRESS \
  "DEFAULT_DNS_SEC_IPV4_ADDRESS"
#define SPGWC_CONFIG_STRING_DEFAULT_DNS_IPV6_ADDRESS "DEFAULT_DNS_IPV6_ADDRESS"
#define SPGWC_CONFIG_STRING_DEFAULT_DNS_SEC_IPV6_ADDRESS \
  "DEFAULT_DNS_SEC_IPV6_ADDRESS"
#define SPGWC_CONFIG_STRING_UE_MTU "UE_MTU"
#define SPGWC_CONFIG_STRING_GTPV1U_REALIZATION "GTPV1U_REALIZATION"
#define SPGWC_CONFIG_STRING_NO_GTP_KERNEL_AVAILABLE "NO_GTP_KERNEL_AVAILABLE"
#define SPGWC_CONFIG_STRING_GTP_KERNEL_MODULE "GTP_KERNEL_MODULE"
#define SPGWC_CONFIG_STRING_GTP_KERNEL "GTP_KERNEL"

#define SPGWC_CONFIG_STRING_INTERFACE_DISABLED "none"

#define SPGWC_CONFIG_STRING_APN_LIST "APN_LIST"
#define SPGWC_CONFIG_STRING_APN_NI "APN_NI"
#define SPGWC_CONFIG_STRING_PDN_TYPE "PDN_TYPE"
#define SPGWC_CONFIG_STRING_IPV4_POOL "IPV4_POOL"
#define SPGWC_CONFIG_STRING_IPV6_POOL "IPV6_POOL"

#define SPGWC_CONFIG_STRING_PCEF "PCEF"
#define SPGWC_CONFIG_STRING_PCEF_ENABLED "PCEF_ENABLED"
#define SPGWC_CONFIG_STRING_TCP_ECN_ENABLED "TCP_ECN_ENABLED"
#define SPGWC_CONFIG_STRING_APN_AMBR_UL "APN_AMBR_UL"
#define SPGWC_CONFIG_STRING_APN_AMBR_DL "APN_AMBR_DL"
#define SPGWC_ABORT_ON_ERROR true
#define SPGWC_WARN_ON_ERROR false

typedef struct interface_cfg_s {
  std::string if_name;
  struct in_addr addr4;
  struct in_addr network4;
  struct in6_addr addr6;
  unsigned int mtu;
  uint16_t port;
  util::ThreadSchedParams thread_rd_sched_params;
} interface_cfg_t;

typedef struct itti_cfg_s {
  util::ThreadSchedParams itti_timer_sched_params;
  util::ThreadSchedParams s11_sched_params;
  util::ThreadSchedParams sx_sched_params;
  util::ThreadSchedParams spgwc_app_sched_params;
} itti_cfg_t;

class SpgwcConfig {
 private:
  int LoadThreadSchedParams(const libconfig::Setting& thread_sched_params_cfg,
                            util::ThreadSchedParams& cfg);
  int LoadItti(const libconfig::Setting& itti_cfg, itti_cfg_t& cfg);
  int LoadInterface(const libconfig::Setting& if_cfg, interface_cfg_t& cfg);
  int Finalize();

 public:
  /* Reader/writer lock for this configuration */
  std::mutex m_rw_lock;
  std::string pid_dir;
  unsigned int instance;
  interface_cfg_t s11_cp;
  interface_cfg_t s11_up;
  interface_cfg_t sxab;
  itti_cfg_t itti;
  int max_cached_users;  // max_cached_users << max_users
  int max_users;         // max_users >> max_cached_users
  struct in_addr default_dnsv4;
  struct in_addr default_dns_secv4;
  struct in6_addr default_dnsv6;
  struct in6_addr default_dns_secv6;

#define PGW_NUM_APN_MAX 5
  int num_apn;
  struct {
    std::string apn;
    std::string apn_label;
    int pool_id_iv4;
    int pool_id_iv6;
    pdn_type_t pdn_type;
  } apn[PGW_NUM_APN_MAX];

  int num_ue_pool;
#define PGW_NUM_UE_POOL_MAX 96
  struct in_addr ue_pool_range_low[PGW_NUM_UE_POOL_MAX];
  struct in_addr ue_pool_range_high[PGW_NUM_UE_POOL_MAX];
  struct in_addr ue_pool_network[PGW_NUM_UE_POOL_MAX];
  struct in_addr ue_pool_netmask[PGW_NUM_UE_POOL_MAX];
  // computed from config, UE IP adresses that matches
  // ue_pool_network[]/ue_pool_netmask[] but do not match ue_pool_range_low[] -
  // ue_pool_range_high[]
  // The problem here is that OpenFlow do not deal with ip ranges but with
  // netmasks
  std::vector<struct in_addr> ue_pool_excluded[PGW_NUM_UE_POOL_MAX];

  int num_paa6_pool;
  struct in6_addr paa_pool6_prefix[PGW_NUM_UE_POOL_MAX];
  uint8_t paa_pool6_prefix_len[PGW_NUM_UE_POOL_MAX];

  bool force_push_pco;
  uint ue_mtu;

  struct {
    uint16_t max_concurrent_transactions_per_stack;
  } gtpv2c;

  struct {
    bool tcp_ecn_enabled = false;  // test for CoDel qdisc
    unsigned int apn_ambr_ul;
    unsigned int apn_ambr_dl;
  } pcef;

  SpgwcConfig()
      : m_rw_lock(),
        pid_dir(),
        instance(0),
        s11_cp(),
        s11_up(),
        sxab(),
        pcef(),
        num_apn(0) {
    for (int i = 0; i < PGW_NUM_APN_MAX; i++) {
      apn[i] = {};
    }
    default_dnsv4.s_addr = INADDR_ANY;
    default_dns_secv4.s_addr = INADDR_ANY;
    default_dnsv6 = in6addr_any;
    default_dns_secv6 = in6addr_any;

    num_ue_pool = 0;
    num_paa6_pool = 0;
    for (int i = 0; i < PGW_NUM_UE_POOL_MAX; i++) {
      ue_pool_range_low[i] = {};
      ue_pool_range_high[i] = {};
      ue_pool_network[i] = {};
      ue_pool_netmask[i] = {};
      paa_pool6_prefix[i] = {};
      paa_pool6_prefix_len[i] = {};
      ue_pool_excluded[i] = {};
    }
    force_push_pco = true;
    ue_mtu = 1500;

    itti.itti_timer_sched_params.sched_priority = 85;
    itti.s11_sched_params.sched_priority = 84;
    itti.sx_sched_params.sched_priority = 84;
    itti.spgwc_app_sched_params.sched_priority = 84;

    s11_cp.thread_rd_sched_params.sched_priority = 95;
    s11_cp.port = gtpv2c::kDefaultPort;

    s11_up.thread_rd_sched_params.sched_priority = 95;

    sxab.thread_rd_sched_params.sched_priority = 95;
    sxab.port = pfcp::kDefaultPort;
    max_cached_users = 256;
    max_users = max_cached_users;
#if CTXT_OFFLOAD
    max_users = 8192;
#endif
    gtpv2c.max_concurrent_transactions_per_stack = 128;
  };
  void Lock() { m_rw_lock.lock(); };
  void Unlock() { m_rw_lock.unlock(); };
  int Load(const std::string& config_file);
  int Execute();
  void Display();
  //------------------------------------------------------------------------------
  bool IsDottedApnHandled(const std::string& apn,
                          const pdn_type_t& pdn_type) const;
  //------------------------------------------------------------------------------
  int GetPfcpNodeId(pfcp::node_id_t& node_id) const;
  //------------------------------------------------------------------------------
  int GetPfcpFseid(pfcp::fseid_t& fseid) const;
};
}  // namespace spgwc

#endif /* FILE_SPGWC_CONFIG_HPP_SEEN */
