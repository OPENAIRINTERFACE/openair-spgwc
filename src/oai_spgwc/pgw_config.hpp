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

/*! \file pgw_config.hpp
 * \brief
 * \author Lionel Gauthier
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */

#ifndef FILE_PGW_CONFIG_HPP_SEEN
#define FILE_PGW_CONFIG_HPP_SEEN
// C includes ------------------------------------------------------------------
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/socket.h>
//--C++ includes ---------------------------------------------------------------
#include <mutex>
#include <vector>
//--Other includes -------------------------------------------------------------
#include "3gpp_29.244.h"
#include "3gpp_29.274.h"
#include "gtpv2c.hpp"
#include "pfcp.hpp"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "thread_sched.hpp"

#define PGW_MAX_ALLOCATED_PDN_ADDRESSES 1024

namespace pgwc {

typedef struct timer_cfg_s {
  util::thread_sched_params sched_params;
} timer_cfg_t;

typedef struct gtpv2c_cfg_s {
  uint16_t port;
  uint16_t n3;
  uint16_t t3_ms;
  uint16_t worker_threads;
  util::thread_sched_params sched_params;
  uint16_t max_concurrent_procedures;
} gtpv2c_cfg_t;

typedef struct pfcp_cfg_s {
  uint16_t port;
  uint16_t n1;
  uint16_t t1_ms;
  uint16_t worker_threads;
  util::thread_sched_params sched_params;
  uint16_t max_concurrent_procedures;
} pfcp_cfg_t;

typedef struct interface_cfg_s {
  std::string if_name;
  struct in_addr addr4;
  struct in_addr network4;
  struct in6_addr addr6;
  uint mtu;
} interface_cfg_t;

typedef struct s11_cfg_s {
  interface_cfg_t iface;
} s11_cfg_t;

typedef struct s5s8_cfg_s {
  interface_cfg_t iface;
} s5s8_cfg_t;

typedef struct sx_cfg_s {
  interface_cfg_t iface;
} sx_cfg_t;

class PdnCfg {
 public:
  std::string apn;
  std::string apn_label;
  struct in_addr ue_pool_range_low;
  struct in_addr ue_pool_range_high;
  struct in_addr ue_pool_network;
  struct in_addr ue_pool_netmask;
  struct in6_addr paa_pool6_prefix;
  uint8_t paa_pool6_prefix_len;
  pdn_type_t pdn_type;
  uint ue_mtu;

  bool is_in_pool(const paa_t& paa) const {
    switch (paa.pdn_type.pdn_type) {
      case PDN_TYPE_E_IPV4:
        return (paa.ipv4_address.s_addr >= ue_pool_range_low.s_addr) and
               (paa.ipv4_address.s_addr <= ue_pool_range_high.s_addr);
      case PDN_TYPE_E_IPV6:
      case PDN_TYPE_E_IPV4V6:
      default:
        return false;
    }
  }
};

typedef struct itti_cfg_s {
  util::thread_sched_params itti_timer_sched_params;
  util::thread_sched_params sx_sched_params;
  util::thread_sched_params s5s8_sched_params;
  util::thread_sched_params pgw_app_sched_params;
  util::thread_sched_params async_cmd_sched_params;
} itti_cfg_t;

typedef struct pgw_app_cfg_s {
  util::thread_sched_params sched_params;
  struct in_addr default_dnsv4;
  struct in_addr default_dns_secv4;
  struct in6_addr default_dnsv6;
  struct in6_addr default_dns_secv6;
  uint default_ue_mtu;
  bool force_push_pco;
  std::vector<PdnCfg> pdns;
  uint32_t max_cached_users;

  PdnCfg& GetPdnCfg(const uint pdn_index) { return pdns.at(pdn_index); }
} pgw_app_cfg_t;

typedef struct up_node_cfg_s {
  std::string mcc;
  std::string mnc;
  uint16_t tac;
  tai_field_t tai;
  uint pdn_index;
  std::string id;  // FQDN, IP address
  std::string toString() const {
    std::string str = {};
    str.append(id).append(" <-> mcc:").append(mcc).append(" mnc:").append(mnc);
    str.append(" tac:").append(std::to_string(tac));
    str.append(" pdn_index:").append(std::to_string(pdn_index));
    return str;
  }

} up_node_cfg_t;

typedef struct cups_cfg_s {
  uint16_t max_associations;
  uint32_t association_retry_period_ms;
  uint32_t association_heartbeat_period_ms;
  std::vector<up_node_cfg_t> nodes;
  bool feature_overload_control;
  bool feature_load_control;
} cups_cfg_t;

class pgw_config {
 private:
  static const bool ParseSchedParams(
      const RAPIDJSON_NAMESPACE::Value& conf, util::thread_sched_params& cfg);
  static const bool ParseTimer(
      const RAPIDJSON_NAMESPACE::Value& conf, timer_cfg_t& cfg);
  static const bool ParseInterface(
      const RAPIDJSON_NAMESPACE::Value& conf, interface_cfg_t& cfg);
  static const bool Finalize();
  static const bool ParseItti(
      const RAPIDJSON_NAMESPACE::Value& conf, itti_cfg_t& cfg);

 public:
  /* Reader/writer lock for this configuration */
  static const int kJsonFileBuffer;
  static std::mutex rw_lock_;
  static timer_cfg_t timer_;
  static gtpv2c_cfg_t gtpv2c_;
  static pfcp_cfg_t pfcp_;
  static s11_cfg_t s11_;
  static s5s8_cfg_t sgw_s5s8_;
  static s5s8_cfg_t pgw_s5s8_;
  static sx_cfg_t sx_;
  static pgw_app_cfg_t spgw_app_;
  static PdnCfg pdn_;
  static cups_cfg_t cups_;
  static std::string jsoncfg_;
  static std::string pid_dir_;
  static unsigned int instance_;

  itti_cfg_t itti;

  static void Default() {
    pid_dir_  = "/var/run";
    instance_ = 0;

    timer_.sched_params.cpu_id         = -1;
    timer_.sched_params.sched_policy   = SCHED_FIFO;
    timer_.sched_params.sched_priority = 46;

    gtpv2c_.port                        = gtpv2c::default_port;
    gtpv2c_.n3                          = 3;
    gtpv2c_.t3_ms                       = 1000;
    gtpv2c_.worker_threads              = 1;
    gtpv2c_.sched_params.cpu_id         = -1;
    gtpv2c_.sched_params.sched_policy   = SCHED_FIFO;
    gtpv2c_.sched_params.sched_priority = 40;
    gtpv2c_.max_concurrent_procedures   = 256;

    pfcp_.port                        = pfcp::default_port;
    pfcp_.n1                          = 3;
    pfcp_.t1_ms                       = 1000;
    pfcp_.worker_threads              = 1;
    pfcp_.sched_params.cpu_id         = -1;
    pfcp_.sched_params.sched_policy   = SCHED_FIFO;
    pfcp_.sched_params.sched_priority = 42;
    pfcp_.max_concurrent_procedures   = 256;

    s11_.iface.if_name         = "lo";
    s11_.iface.addr4.s_addr    = INADDR_ANY;
    s11_.iface.network4.s_addr = INADDR_ANY;
    s11_.iface.addr6           = in6addr_any;

    sgw_s5s8_.iface.if_name         = "lo";
    sgw_s5s8_.iface.addr4.s_addr    = INADDR_ANY;
    sgw_s5s8_.iface.network4.s_addr = INADDR_ANY;
    sgw_s5s8_.iface.addr6           = in6addr_any;

    pgw_s5s8_.iface.if_name         = "lo";
    pgw_s5s8_.iface.addr4.s_addr    = INADDR_ANY;
    pgw_s5s8_.iface.network4.s_addr = INADDR_ANY;
    pgw_s5s8_.iface.addr6           = in6addr_any;

    sx_.iface.if_name         = "lo";
    sx_.iface.addr4.s_addr    = INADDR_ANY;
    sx_.iface.network4.s_addr = INADDR_ANY;
    sx_.iface.addr6           = in6addr_any;

    spgw_app_.sched_params.cpu_id                      = -1;
    spgw_app_.sched_params.sched_policy                = SCHED_FIFO;
    spgw_app_.sched_params.sched_priority              = 44;
    spgw_app_.default_dnsv4.s_addr                     = htonl(0x08080808);
    spgw_app_.default_dns_secv4.s_addr                 = htonl(0x08080404);
    spgw_app_.default_dnsv6.__in6_u.__u6_addr16[0]     = htonl(0x2001);
    spgw_app_.default_dnsv6.__in6_u.__u6_addr16[1]     = htonl(0x4860);
    spgw_app_.default_dnsv6.__in6_u.__u6_addr16[2]     = htonl(0x4860);
    spgw_app_.default_dnsv6.__in6_u.__u6_addr16[3]     = htonl(0x0000);
    spgw_app_.default_dnsv6.__in6_u.__u6_addr16[4]     = htonl(0x0000);
    spgw_app_.default_dnsv6.__in6_u.__u6_addr16[5]     = htonl(0x0000);
    spgw_app_.default_dnsv6.__in6_u.__u6_addr16[6]     = htonl(0x0000);
    spgw_app_.default_dnsv6.__in6_u.__u6_addr16[7]     = htonl(0x8888);
    spgw_app_.default_dns_secv6.__in6_u.__u6_addr16[0] = htonl(0x2001);
    spgw_app_.default_dns_secv6.__in6_u.__u6_addr16[1] = htonl(0x4860);
    spgw_app_.default_dns_secv6.__in6_u.__u6_addr16[2] = htonl(0x4860);
    spgw_app_.default_dns_secv6.__in6_u.__u6_addr16[3] = htonl(0x0000);
    spgw_app_.default_dns_secv6.__in6_u.__u6_addr16[4] = htonl(0x0000);
    spgw_app_.default_dns_secv6.__in6_u.__u6_addr16[5] = htonl(0x0000);
    spgw_app_.default_dns_secv6.__in6_u.__u6_addr16[6] = htonl(0x0000);
    spgw_app_.default_dns_secv6.__in6_u.__u6_addr16[7] = htonl(0x8844);
    spgw_app_.default_ue_mtu                           = 1358;
    spgw_app_.force_push_pco                           = false;
    spgw_app_.pdns.clear();

    cups_.nodes.clear();
    cups_.association_retry_period_ms     = 10000;
    cups_.association_heartbeat_period_ms = 10000;
    cups_.max_associations                = 8;
    cups_.feature_overload_control        = false;
    cups_.feature_load_control            = false;
  };
  static bool ParseJson();

  static void Lock() { rw_lock_.lock(); };
  static void Unlock() { rw_lock_.unlock(); };
  static int Load(const std::string& config_file);
  static int Execute();
  static void Display();
  //------------------------------------------------------------------------------
  static bool IsDottedApnHandled(
      const std::string& apn, const pdn_type_t& pdn_type);
  //------------------------------------------------------------------------------
  static int GetPfcpNodeId(pfcp::node_id_t& node_id);
  //------------------------------------------------------------------------------
  static int GetPfcpFseid(pfcp::fseid_t& fseid);
  static bool GetUpNodes(
      const apn_t& apn, const uli_t& uli, const paa_t& paa,
      std::vector<up_node_cfg_t>& up_nodes);
};

}  // namespace pgwc

#endif /* FILE_PGW_CONFIG_HPP_SEEN */
