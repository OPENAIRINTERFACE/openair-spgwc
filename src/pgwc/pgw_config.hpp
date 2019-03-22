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

/*! \file pgw_config.hpp
* \brief
* \author Lionel Gauthier
* \company Eurecom
* \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_PGW_CONFIG_HPP_SEEN
#define FILE_PGW_CONFIG_HPP_SEEN

#include "3gpp_29.244.hpp"
#include "3gpp_29.274.hpp"

#include <arpa/inet.h>
#include <libconfig.h++>
#include <netinet/in.h>
#include <sys/socket.h>

#include <mutex>
#include <vector>

namespace oai::cn::nf::pgwc {


#define PGW_CONFIG_STRING_PGW_CONFIG                            "P-GW"
#define PGW_CONFIG_STRING_PID_DIRECTORY                         "PID_DIRECTORY"
#define PGW_CONFIG_STRING_INSTANCE                              "INSTANCE"
#define PGW_CONFIG_STRING_INTERFACES                            "INTERFACES"
#define PGW_CONFIG_STRING_INTERFACE_NAME                        "INTERFACE_NAME"
#define PGW_CONFIG_STRING_IPV4_ADDRESS                          "IPV4_ADDRESS"
#define PGW_CONFIG_STRING_PORT                                  "PORT"
#define PGW_CONFIG_STRING_INTERFACE_S5_S8_CP                    "S5_S8_CP"
#define PGW_CONFIG_STRING_INTERFACE_SX                          "SX"

#define PGW_CONFIG_STRING_PGW_MASQUERADE_SGI                    "PGW_MASQUERADE_SGI"
#define PGW_CONFIG_STRING_UE_TCP_MSS_CLAMPING                   "UE_TCP_MSS_CLAMPING"
#define PGW_CONFIG_STRING_NAS_FORCE_PUSH_PCO                    "FORCE_PUSH_PROTOCOL_CONFIGURATION_OPTIONS"

#define PGW_CONFIG_STRING_IP_ADDRESS_POOL                       "IP_ADDRESS_POOL"
#define PGW_CONFIG_STRING_ARP_UE                                "ARP_UE"
#define PGW_CONFIG_STRING_ARP_UE_CHOICE_NO                      "NO"
#define PGW_CONFIG_STRING_ARP_UE_CHOICE_LINUX                   "LINUX"
#define PGW_CONFIG_STRING_ARP_UE_CHOICE_OAI                     "OAI"
#define PGW_CONFIG_STRING_IPV4_ADDRESS_LIST                     "IPV4_LIST"
#define PGW_CONFIG_STRING_IPV6_ADDRESS_LIST                     "IPV6_LIST"
#define PGW_CONFIG_STRING_RANGE                                 "RANGE"
#define PGW_CONFIG_STRING_PREFIX                                "PREFIX"
#define PGW_CONFIG_STRING_IPV4_ADDRESS_RANGE_DELIMITER          "-"
#define PGW_CONFIG_STRING_IPV6_ADDRESS_PREFIX_DELIMITER         "/"
#define PGW_CONFIG_STRING_DEFAULT_DNS_IPV4_ADDRESS              "DEFAULT_DNS_IPV4_ADDRESS"
#define PGW_CONFIG_STRING_DEFAULT_DNS_SEC_IPV4_ADDRESS          "DEFAULT_DNS_SEC_IPV4_ADDRESS"
#define PGW_CONFIG_STRING_DEFAULT_DNS_IPV6_ADDRESS              "DEFAULT_DNS_IPV6_ADDRESS"
#define PGW_CONFIG_STRING_DEFAULT_DNS_SEC_IPV6_ADDRESS          "DEFAULT_DNS_SEC_IPV6_ADDRESS"
#define PGW_CONFIG_STRING_UE_MTU                                "UE_MTU"
#define PGW_CONFIG_STRING_GTPV1U_REALIZATION                    "GTPV1U_REALIZATION"
#define PGW_CONFIG_STRING_NO_GTP_KERNEL_AVAILABLE               "NO_GTP_KERNEL_AVAILABLE"
#define PGW_CONFIG_STRING_GTP_KERNEL_MODULE                     "GTP_KERNEL_MODULE"
#define PGW_CONFIG_STRING_GTP_KERNEL                            "GTP_KERNEL"

#define PGW_CONFIG_STRING_INTERFACE_DISABLED                    "none"

#define PGW_CONFIG_STRING_APN_LIST                              "APN_LIST"
#define PGW_CONFIG_STRING_APN_NI                                "APN_NI"
#define PGW_CONFIG_STRING_PDN_TYPE                              "PDN_TYPE"
#define PGW_CONFIG_STRING_IPV4_POOL                             "IPV4_POOL"
#define PGW_CONFIG_STRING_IPV6_POOL                             "IPV6_POOL"

#define PGW_CONFIG_STRING_PCEF                                  "PCEF"
#define PGW_CONFIG_STRING_PCEF_ENABLED                          "PCEF_ENABLED"
#define PGW_CONFIG_STRING_TCP_ECN_ENABLED                       "TCP_ECN_ENABLED"
#define PGW_CONFIG_STRING_AUTOMATIC_PUSH_DEDICATED_BEARER_PCC_RULE  "AUTOMATIC_PUSH_DEDICATED_BEARER_PCC_RULE"
#define PGW_CONFIG_STRING_DEFAULT_BEARER_STATIC_PCC_RULE        "DEFAULT_BEARER_STATIC_PCC_RULE"
#define PGW_CONFIG_STRING_PUSH_STATIC_PCC_RULES                 "PUSH_STATIC_PCC_RULES"
#define PGW_CONFIG_STRING_APN_AMBR_UL                           "APN_AMBR_UL"
#define PGW_CONFIG_STRING_APN_AMBR_DL                           "APN_AMBR_DL"
#define PGW_ABORT_ON_ERROR true
#define PGW_WARN_ON_ERROR  false

#define PGW_CONFIG_STRING_OVS_CONFIG                            "OVS"
#define PGW_CONFIG_STRING_OVS_BRIDGE_NAME                       "BRIDGE_NAME"
#define PGW_CONFIG_STRING_OVS_EGRESS_PORT_NUM                   "EGRESS_PORT_NUM"
#define PGW_CONFIG_STRING_OVS_GTP_PORT_NUM                      "GTP_PORT_NUM"
#define PGW_CONFIG_STRING_OVS_L2_EGRESS_PORT                    "L2_EGRESS_PORT"
#define PGW_CONFIG_STRING_OVS_UPLINK_MAC                        "UPLINK_MAC"
#define PGW_CONFIG_STRING_OVS_SGI_ARP_CACHE                     "SGI_ARP_CACHE"
#define PGW_CONFIG_STRING_IP                                    "IP"
#define PGW_CONFIG_STRING_MAC                                   "MAC"

// may be more
#define PGW_MAX_ALLOCATED_PDN_ADDRESSES 1024

typedef struct sgi_arp_boot_cache_s {
#define PGW_ARP_BOOT_CACHE_NUM_ENTRIES_MAX 16
  struct in_addr    ip[PGW_ARP_BOOT_CACHE_NUM_ENTRIES_MAX];
  std::string       mac[PGW_ARP_BOOT_CACHE_NUM_ENTRIES_MAX];
  int num_entries;
} sgi_arp_boot_cache_t;

typedef struct spgw_ovs_config_s {
  std::string    bridge_name;
  int            egress_port_num;
  std::string    l2_egress_port;
  int            gtp_port_num;
  std::string    uplink_mac; // next (first) hop
  sgi_arp_boot_cache_t sgi_arp_boot_cache;
} spgw_ovs_config_t;

typedef struct interface_cfg_s {
  std::string     if_name;
  struct in_addr  addr4;
  struct in_addr  network4;
  struct in6_addr addr6;
  unsigned int    mtu;
  unsigned int    port;
} interface_cfg_t;

class pgw_config {
private:
  int load_interface(const libconfig::Setting& if_cfg, interface_cfg_t & cfg);

public:
  /* Reader/writer lock for this configuration */
  std::mutex        m_rw_lock;
  std::string       pid_dir;
  unsigned int      instance = 0;

  interface_cfg_t s5s8_cp;
  interface_cfg_t sx;

  struct in_addr default_dnsv4;
  struct in_addr default_dns_secv4;
  struct in6_addr default_dnsv6;
  struct in6_addr default_dns_secv6;


#if ENABLE_LIBGTPNL
  bool      ue_tcp_mss_clamp; // for UE TCP traffic
  bool      masquerade_SGI;
#endif

#define PGW_NUM_APN_MAX 5
  int              num_apn;
  struct {
    std::string    apn;
    std::string    apn_label;
    int            pool_id_iv4;
    int            pool_id_iv6;
    core::pdn_type_t     pdn_type;
  } apn[PGW_NUM_APN_MAX];

  int              num_ue_pool;
#define PGW_NUM_UE_POOL_MAX 96
  struct in_addr   ue_pool_range_low[PGW_NUM_UE_POOL_MAX];
  struct in_addr   ue_pool_range_high[PGW_NUM_UE_POOL_MAX];
  struct in_addr   ue_pool_network[PGW_NUM_UE_POOL_MAX];
  struct in_addr   ue_pool_netmask[PGW_NUM_UE_POOL_MAX];
  //computed from config, UE IP adresses that matches ue_pool_network[]/ue_pool_netmask[] but do not match ue_pool_range_low[] - ue_pool_range_high[]
  // The problem here is that OpenFlow do not deal with ip ranges but with netmasks
  std::vector<struct in_addr> ue_pool_excluded[PGW_NUM_UE_POOL_MAX];

  int              num_paa6_pool;
  struct in6_addr  paa_pool6_prefix[PGW_NUM_UE_POOL_MAX];
  uint8_t          paa_pool6_prefix_len[PGW_NUM_UE_POOL_MAX];

  bool             arp_ue_linux;
  bool             arp_ue_oai;

  bool             force_push_pco;
  uint             ue_mtu;
  bool             use_gtp_kernel_module;
  bool             enable_loading_gtp_kernel_module;

  struct {
    bool      tcp_ecn_enabled = false;           // test for CoDel qdisc
    unsigned int  apn_ambr_ul;
    unsigned int  apn_ambr_dl;
  } pcef;

#if ENABLE_OPENFLOW
  spgw_ovs_config_t ovs_config;
#endif

  pgw_config() : m_rw_lock(), pcef(), num_apn(0), pid_dir(), instance(0), s5s8_cp(), sx() {
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
    arp_ue_linux = false;
    arp_ue_oai = false;

    force_push_pco = true;
    ue_mtu = 1500;
    use_gtp_kernel_module = true;
    enable_loading_gtp_kernel_module = true;
#if ENABLE_LIBGTPNL
    ue_tcp_mss_clamp = true;
    masquerade_SGI = true;
#endif
#if ENABLE_OPENFLOW
    ovs_config = {};
#endif
  };
  ~pgw_config();
  void lock() {m_rw_lock.lock();};
  void unlock() {m_rw_lock.unlock();};
  int load(const std::string& config_file);
  int finalize();
  void display();
  bool is_dotted_apn_handled(const std::string& apn, const core::pdn_type_t& pdn_type);
  int get_pa_pool_id(const std::string& apn, int& pool_id_ipv4, int& pool_id_ipv6);
  int get_pfcp_node_id(oai::cn::core::pfcp::node_id_t& node_id);
  int get_pfcp_fseid(oai::cn::core::pfcp::fseid_t& fseid);
};

} // namespace pgw


#endif /* FILE_PGW_CONFIG_HPP_SEEN */
