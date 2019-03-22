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

/*! \file pgw_config.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "3gpp_29.274.hpp"
#include "common_defs.h"
#include "epc.h"
#include "if.hpp"
#include "logger.hpp"
#include "pgw_app.hpp"
#include "pgw_config.hpp"
#include "string.hpp"

#include <arpa/inet.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace libconfig;
using namespace oai::cn::core;
using namespace oai::cn::core::itti;
using namespace oai::cn::nf::pgwc;
using namespace oai::cn::util;
using namespace std;

// C includes
#include <errno.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

extern pgw_app *pgw_app_inst;
extern pgw_config pgw_cfg;

//------------------------------------------------------------------------------
int pgw_config::finalize ()
{
  Logger::pgwc_app().info("Finalize config...");

  for (int i = 0; i < num_ue_pool; i++) {
    uint32_t range_low_hbo = ntohl(ue_pool_range_low[i].s_addr);
    uint32_t range_high_hbo = ntohl(ue_pool_range_high[i].s_addr);
    uint32_t tmp_hbo = range_low_hbo ^ range_high_hbo;
    uint8_t  nbits = 32;
    while (tmp_hbo) {
      tmp_hbo = tmp_hbo >> 1;
      nbits -=1;
    }
    uint32_t network_hbo = range_high_hbo & (UINT32_MAX << (32-nbits));
    uint32_t netmask_hbo = 0xFFFFFFFF << (32-nbits);
    ue_pool_network[i].s_addr = htonl(network_hbo);
    ue_pool_netmask[i].s_addr = htonl(netmask_hbo);
  }
  // "TODO"
  //pgw_pcef_emulation_init(config_pP);
  Logger::pgwc_app().info("Finalized config");
  return 0;
}

//------------------------------------------------------------------------------
int pgw_config::load_interface(const Setting& if_cfg, interface_cfg_t & cfg)
{
  if_cfg.lookupValue(PGW_CONFIG_STRING_INTERFACE_NAME, cfg.if_name);
  util::trim(cfg.if_name);
  if (not boost::iequals(cfg.if_name, "none")) {
    std::string address = {};
    if_cfg.lookupValue(PGW_CONFIG_STRING_IPV4_ADDRESS, address);
    util::trim(address);
    if (boost::iequals(address, "read")) {
      if (get_inet_addr_infos_from_iface(cfg.if_name, cfg.addr4, cfg.network4, cfg.mtu)) {
        Logger::pgwc_app().error("Could not read %s network interface configuration", cfg.if_name);
        return RETURNerror;
      }
    } else {
      std::vector<std::string> words;
      boost::split(words, address, boost::is_any_of("/"), boost::token_compress_on);
      if (words.size() != 2) {
        Logger::pgwc_app().error("Bad value " PGW_CONFIG_STRING_IPV4_ADDRESS " = %s in config file", address.c_str());
        return RETURNerror;
      }
      unsigned char buf_in_addr[sizeof(struct in6_addr)]; // you never know...
      if (inet_pton (AF_INET, util::trim(words.at(0)).c_str(), buf_in_addr) == 1) {
        memcpy (&cfg.addr4, buf_in_addr, sizeof (struct in_addr));
      } else {
        Logger::pgwc_app().error("In conversion: Bad value " PGW_CONFIG_STRING_IPV4_ADDRESS " = %s in config file", util::trim(words.at(0)).c_str());
        return RETURNerror;
      }
      cfg.network4.s_addr = htons(ntohs(cfg.addr4.s_addr) & 0xFFFFFFFF << (32 - std::stoi (util::trim(words.at(1)))));
    }
    if_cfg.lookupValue(PGW_CONFIG_STRING_PORT, cfg.port);
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int pgw_config::load(const string& config_file)
{
  Config                 cfg;
  unsigned char          buf_in_addr[sizeof (struct in_addr)];
  unsigned char          buf_in6_addr[sizeof (struct in6_addr)];

  // Read the file. If there is an error, report it and exit.
  try
  {
    cfg.readFile(config_file.c_str());
  }
  catch(const FileIOException &fioex)
  {
    Logger::pgwc_app().error("I/O error while reading file %s - %s", config_file.c_str(), fioex.what());
    throw fioex;
  }
  catch(const ParseException &pex)
  {
    Logger::pgwc_app().error("Parse error at %s:%d - %s", pex.getFile(), pex.getLine(), pex.getError());
    throw pex;
  }

  const Setting& root = cfg.getRoot();

  try
  {
    string astring;
    const Setting &pgw_cfg = root[PGW_CONFIG_STRING_PGW_CONFIG];

    pgw_cfg.lookupValue(PGW_CONFIG_STRING_INSTANCE, instance);
    pgw_cfg.lookupValue(PGW_CONFIG_STRING_PID_DIRECTORY, pid_dir);

    const Setting &nw_if_cfg = pgw_cfg[PGW_CONFIG_STRING_INTERFACES];

    const Setting& s5s8_cp_cfg = nw_if_cfg[PGW_CONFIG_STRING_INTERFACE_S5_S8_CP];
    load_interface(s5s8_cp_cfg, s5s8_cp);

    const Setting& sx_cfg = nw_if_cfg[PGW_CONFIG_STRING_INTERFACE_SX];
    load_interface(sx_cfg, sx);


    const Setting &pool_cfg = pgw_cfg[PGW_CONFIG_STRING_IP_ADDRESS_POOL];
    arp_ue_oai = false;
    arp_ue_linux = false;
    pool_cfg.lookupValue(PGW_CONFIG_STRING_ARP_UE, astring);
    if (boost::iequals(astring, PGW_CONFIG_STRING_ARP_UE_CHOICE_LINUX)) {
      arp_ue_linux = true;
    } else if (boost::iequals(astring, PGW_CONFIG_STRING_ARP_UE_CHOICE_OAI)) {
      arp_ue_oai = true;
    }

    const Setting &ipv4_pool_cfg = pool_cfg[PGW_CONFIG_STRING_IPV4_ADDRESS_LIST];
    int count = ipv4_pool_cfg.getLength();
    for (int i = 0; i < count; i++) {
      const Setting &ipv4_cfg = ipv4_pool_cfg[i];
      string ipv4_range;
      ipv4_cfg.lookupValue(PGW_CONFIG_STRING_RANGE, ipv4_range);
      std::vector<std::string> ips;
      boost::split(ips, ipv4_range, boost::is_any_of(PGW_CONFIG_STRING_IPV4_ADDRESS_RANGE_DELIMITER), boost::token_compress_on);
      if (ips.size() != 2) {
        Logger::pgwc_app().error("Bad value %s : %s in config file %s", PGW_CONFIG_STRING_IPV4_ADDRESS_RANGE_DELIMITER, ipv4_range.c_str(), config_file.c_str());
        throw ("Bad value %s : %s in config file %s", PGW_CONFIG_STRING_IPV4_ADDRESS_RANGE_DELIMITER, ipv4_range.c_str(), config_file.c_str());
      }

      if (inet_pton (AF_INET, util::trim(ips.at(0)).c_str(), buf_in_addr) == 1) {
        memcpy (&ue_pool_range_low[num_ue_pool], buf_in_addr, sizeof (struct in_addr));
      } else {
        Logger::pgwc_app().error("CONFIG POOL ADDR IPV4: BAD LOWER ADDRESS in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST " pool %d", i);
        throw ("CONFIG POOL ADDR IPV4: BAD ADDRESS in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST);
      }

      if (inet_pton (AF_INET, util::trim(ips.at(1)).c_str(), buf_in_addr) == 1) {
        memcpy (&ue_pool_range_high[num_ue_pool], buf_in_addr, sizeof (struct in_addr));
      } else {
        Logger::pgwc_app().error("CONFIG POOL ADDR IPV4: BAD HIGHER ADDRESS in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST " pool %d", i);
        throw ("CONFIG POOL ADDR IPV4: BAD ADDRESS in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST);
      }
      if (htonl(ue_pool_range_low[num_ue_pool].s_addr) >=  htonl(ue_pool_range_high[num_ue_pool].s_addr)) {
        Logger::pgwc_app().error("CONFIG POOL ADDR IPV4: BAD RANGE in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST " pool %d", i);
        throw ("CONFIG POOL ADDR IPV4: BAD RANGE in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST);
      }
      num_ue_pool += 1;
    }

    const Setting &ipv6_pool_cfg = pool_cfg[PGW_CONFIG_STRING_IPV6_ADDRESS_LIST];
    int count6 = ipv6_pool_cfg.getLength();
    for (int i = 0; i < count6; i++) {
      const Setting &ipv6_cfg = ipv6_pool_cfg[i];
      string ipv6_prefix;
      ipv6_cfg.lookupValue(PGW_CONFIG_STRING_PREFIX, ipv6_prefix);
      std::vector<std::string> ips6;
      boost::split(ips6, ipv6_prefix, boost::is_any_of(PGW_CONFIG_STRING_IPV6_ADDRESS_PREFIX_DELIMITER), boost::token_compress_on);
      if (ips6.size() != 2) {
        Logger::pgwc_app().error("Bad value %s : %s in config file %s", PGW_CONFIG_STRING_PREFIX, ipv6_prefix.c_str(), config_file.c_str());
        throw ("Bad value %s : %s in config file %s", PGW_CONFIG_STRING_PREFIX, ipv6_prefix.c_str(), config_file.c_str());
      }

      std::string addr = ips6.at(0);
      util::trim(addr);
      if (inet_pton (AF_INET6, addr.c_str(), buf_in6_addr) == 1) {
        memcpy (&paa_pool6_prefix[num_paa6_pool], buf_in6_addr, sizeof (struct in6_addr));
      } else {
        Logger::pgwc_app().error("CONFIG POOL ADDR IPV6: BAD ADDRESS in " PGW_CONFIG_STRING_IPV6_ADDRESS_LIST " pool %d", i);
        throw ("CONFIG POOL ADDR IPV6: BAD ADDRESS in " PGW_CONFIG_STRING_IPV6_ADDRESS_LIST);
      }

      std::string prefix = ips6.at(1);
      util::trim(prefix);
      paa_pool6_prefix_len[num_paa6_pool] = std::stoi(prefix);
      num_paa6_pool += 1;
    }


    const Setting &apn_list_cfg = pgw_cfg[PGW_CONFIG_STRING_APN_LIST];
    count = apn_list_cfg.getLength();
    int apn_idx = 0;
    num_apn = 0;
    for (int i = 0; i < count; i++) {
      const Setting &apn_cfg = apn_list_cfg[i];
      apn_cfg.lookupValue(PGW_CONFIG_STRING_APN_NI, astring);
      apn[apn_idx].apn = astring;
      apn[apn_idx].apn_label = EPC::Utility::apn_label(astring);
      apn_cfg.lookupValue(PGW_CONFIG_STRING_PDN_TYPE, astring);
      if (boost::iequals(astring, "IPv4")) {
        apn[apn_idx].pdn_type.pdn_type = PDN_TYPE_E_IPV4;
      } else if (boost::iequals(astring, "IPv6") == 0) {
        apn[apn_idx].pdn_type.pdn_type = PDN_TYPE_E_IPV6;
      } else if (boost::iequals(astring, "IPv4IPv6") == 0) {
        apn[apn_idx].pdn_type.pdn_type = PDN_TYPE_E_IPV4V6;
      }  else if (boost::iequals(astring, "Non-IP") == 0) {
        apn[apn_idx].pdn_type.pdn_type = PDN_TYPE_E_NON_IP;
      } else {
        Logger::pgwc_app().error(" " PGW_CONFIG_STRING_PDN_TYPE " in %d'th APN :%s" , i+1, astring.c_str());
        throw ("Error PDN_TYPE in config file");
      }
      apn_cfg.lookupValue(PGW_CONFIG_STRING_IPV4_POOL, apn[apn_idx].pool_id_iv4);
      apn_cfg.lookupValue(PGW_CONFIG_STRING_IPV6_POOL, apn[apn_idx].pool_id_iv6);

      if ((0 <= apn[apn_idx].pool_id_iv4) && (apn[apn_idx].pdn_type.pdn_type == PDN_TYPE_E_IPV6)) {
        Logger::pgwc_app().error("PDN_TYPE versus pool identifier %d 'th APN in config file\n", i+1);
        throw ("PDN_TYPE versus pool identifier APN");
      }
      if ((0 <= apn[apn_idx].pool_id_iv6) && (apn[apn_idx].pdn_type.pdn_type == PDN_TYPE_E_IPV4)) {
        Logger::pgwc_app().error("PDN_TYPE versus pool identifier %d 'th APN in config file\n", i+1);
        throw ("PDN_TYPE versus pool identifier APN");
      }

      if (((0 <= apn[apn_idx].pool_id_iv4) || (0 <= apn[apn_idx].pool_id_iv6)) &&
          (not boost::iequals(apn[apn_idx].apn, "none"))) {
        bool doublon = false;
        for (int j = 0; j < apn_idx; j++) {
          if (boost::iequals(apn[j].apn, apn[apn_idx].apn)) {
            doublon = true;
            Logger::pgwc_app().info("%d'th APN %s already found in config file (%d 'th APN %s), bypassing\n", i+1, apn[apn_idx].apn.c_str(), j+1, apn[j].apn.c_str());
          }
        }
        if (not doublon) {
          apn_idx++;
          num_apn++;
        }
      } else {
        Logger::pgwc_app().error("Bypass %d'th APN %s in config file\n", i+1, apn[apn_idx].apn.c_str());
      }
    }
    pgw_cfg.lookupValue(PGW_CONFIG_STRING_DEFAULT_DNS_IPV4_ADDRESS, astring);
    IPV4_STR_ADDR_TO_INADDR (util::trim(astring).c_str(), default_dnsv4, "BAD IPv4 ADDRESS FORMAT FOR DEFAULT DNS !");

    pgw_cfg.lookupValue(PGW_CONFIG_STRING_DEFAULT_DNS_SEC_IPV4_ADDRESS, astring);
    IPV4_STR_ADDR_TO_INADDR (util::trim(astring).c_str(), default_dns_secv4, "BAD IPv4 ADDRESS FORMAT FOR DEFAULT DNS !");


    pgw_cfg.lookupValue(PGW_CONFIG_STRING_DEFAULT_DNS_IPV6_ADDRESS, astring);
    if (inet_pton (AF_INET6, util::trim(astring).c_str(), buf_in6_addr) == 1) {
      memcpy (&default_dnsv6, buf_in6_addr, sizeof (struct in6_addr));
    } else {
      Logger::pgwc_app().error("CONFIG : BAD ADDRESS in " PGW_CONFIG_STRING_DEFAULT_DNS_IPV6_ADDRESS " %s", astring.c_str());
      throw ("CONFIG : BAD ADDRESS in " PGW_CONFIG_STRING_DEFAULT_DNS_IPV6_ADDRESS " %s", astring.c_str());
    }
    pgw_cfg.lookupValue(PGW_CONFIG_STRING_DEFAULT_DNS_SEC_IPV6_ADDRESS, astring);
    if (inet_pton (AF_INET6, util::trim(astring).c_str(), buf_in6_addr) == 1) {
      memcpy (&default_dns_secv6, buf_in6_addr, sizeof (struct in6_addr));
    } else {
      Logger::pgwc_app().error("CONFIG : BAD ADDRESS in " PGW_CONFIG_STRING_DEFAULT_DNS_SEC_IPV6_ADDRESS " %s", astring.c_str());
      throw ("CONFIG : BAD ADDRESS in " PGW_CONFIG_STRING_DEFAULT_DNS_SEC_IPV6_ADDRESS " %s", astring.c_str());
    }

    pgw_cfg.lookupValue(PGW_CONFIG_STRING_NAS_FORCE_PUSH_PCO, astring);
    if (boost::iequals(astring, "yes")) {
      force_push_pco = true;
    } else {
      force_push_pco = false;
    }
    pgw_cfg.lookupValue(PGW_CONFIG_STRING_UE_MTU, ue_mtu);

    pgw_cfg.lookupValue(PGW_CONFIG_STRING_GTPV1U_REALIZATION, astring);
    if (boost::iequals (astring, PGW_CONFIG_STRING_NO_GTP_KERNEL_AVAILABLE)) {
      use_gtp_kernel_module = false;
      enable_loading_gtp_kernel_module = false;
    } else if (boost::iequals (astring, PGW_CONFIG_STRING_GTP_KERNEL_MODULE)) {
      use_gtp_kernel_module = true;
      enable_loading_gtp_kernel_module = true;
    } else if (boost::iequals (astring, PGW_CONFIG_STRING_GTP_KERNEL)) {
      use_gtp_kernel_module = true;
      enable_loading_gtp_kernel_module = false;
    }

    const Setting &pcef_cfg = pgw_cfg[PGW_CONFIG_STRING_PCEF];
    unsigned int apn_ambr = 0;
    if (!(pcef_cfg.lookupValue(PGW_CONFIG_STRING_APN_AMBR_UL, apn_ambr))) {
      Logger::pgwc_app().error(PGW_CONFIG_STRING_APN_AMBR_UL "failed");
      throw (PGW_CONFIG_STRING_APN_AMBR_UL "failed");
    }
    pcef.apn_ambr_ul = apn_ambr;
    if (!(pcef_cfg.lookupValue(PGW_CONFIG_STRING_APN_AMBR_DL, apn_ambr))) {
      Logger::pgwc_app().error(PGW_CONFIG_STRING_APN_AMBR_DL "failed");
      //throw (PGW_CONFIG_STRING_APN_AMBR_DL "failed");
    }
    pcef.apn_ambr_dl = apn_ambr;
  }
  catch(const SettingNotFoundException &nfex)
  {
    Logger::pgwc_app().error("%s : %s", nfex.what(), nfex.getPath());
    return RETURNerror;
  }
  return finalize();
}

//------------------------------------------------------------------------------
void pgw_config::display ()
{
  Logger::pgwc_app().info( "==== EURECOM %s v%s ====", PACKAGE_NAME, PACKAGE_VERSION);
  Logger::pgwc_app().info( "Configuration:");
  Logger::pgwc_app().info( "- Instance ..............: %d\n", instance);
  Logger::pgwc_app().info( "- PID dir ...............: %s\n", pid_dir.c_str());


#if ENABLE_OPENFLOW
  Logger::pgwc_app().info( "- OpenVSwitch:");
  Logger::pgwc_app().info( "    bridge_name .........: %s", ovs_config.bridge_name.c_str());
  Logger::pgwc_app().info( "    egress_port_num .....: %d", ovs_config.egress_port_num);
  Logger::pgwc_app().info( "    gtp_port_num ........: %d", ovs_config.gtp_port_num);
  Logger::pgwc_app().info( "    uplink_mac ..........: %s", ovs_config.uplink_mac.c_str());
  Logger::pgwc_app().info( "    l2_egress_port ......: %s", ovs_config.l2_egress_port.c_str());
  Logger::pgwc_app().info( "    ARP Cache:");
  for (int i = 0; i < ovs_config.sgi_arp_boot_cache.num_entries; i++) {
    Logger::pgwc_app().info( "      IP %s - MAC %s",  inet_ntoa (*((struct in_addr *)&ovs_config.sgi_arp_boot_cache.ip[i])), ovs_config.sgi_arp_boot_cache.mac[i].c_str());
  }
#endif

  Logger::pgwc_app().info( "- S5S8-C:");
  Logger::pgwc_app().info( "    iface ............: %s", s5s8_cp.if_name.c_str());
  Logger::sgwc_app().info( "    ipv4.addr ........: %s", inet_ntoa (s5s8_cp.addr4));
  if (s5s8_cp.network4.s_addr) {
    Logger::sgwc_app().info( "    ipv4.mask ........: %s", inet_ntoa (s5s8_cp.network4));
  }
  Logger::pgwc_app().info( "    port .............: %d", s5s8_cp.port);
  if (s5s8_cp.mtu) {
    Logger::pgwc_app().info( "    MTU ..............: %u", s5s8_cp.mtu);
  }
  Logger::pgwc_app().info( "- SX:");
  Logger::pgwc_app().info( "    iface ................: %s", sx.if_name.c_str());
  Logger::pgwc_app().info( "    ip ...................: %s", inet_ntoa (sx.addr4));
  Logger::pgwc_app().info( "    port .................: %d", sx.port);

  Logger::pgwc_app().info( "- " PGW_CONFIG_STRING_IP_ADDRESS_POOL ":");
  for (int i = 0; i < num_ue_pool; i++) {
    Logger::pgwc_app().info( "    IPv4 pool %d ..........: %s - %s", i, inet_ntoa (*((struct in_addr *)&ue_pool_range_low[i])), inet_ntoa (*((struct in_addr *)&ue_pool_range_high[i])));
  }
  char str_addr6[INET6_ADDRSTRLEN];
  for (int i = 0; i < num_paa6_pool; i++) {
    if (inet_ntop(AF_INET6, &paa_pool6_prefix[i], str_addr6, sizeof(str_addr6))) {
      Logger::pgwc_app().info( "    IPv6 pool %d ..........: %s / %d", i, str_addr6, paa_pool6_prefix_len[i]);
    }
  }
  Logger::pgwc_app().info( "- DEFAULT DNS:");
  Logger::pgwc_app().info( "    Primary DNS ..........: %s", inet_ntoa (*((struct in_addr *)&default_dnsv4)));
  Logger::pgwc_app().info( "    Secondary DNS ........: %s", inet_ntoa (*((struct in_addr *)&default_dns_secv4)));
  if (inet_ntop(AF_INET6, &default_dnsv6, str_addr6, sizeof(str_addr6))) {
    Logger::pgwc_app().info( "    Primary DNS v6........: %s", str_addr6);
  }
  if (inet_ntop(AF_INET6, &default_dns_secv6, str_addr6, sizeof(str_addr6))) {
    Logger::pgwc_app().info( "    Secondary DNS v6 .....: %s", str_addr6);
  }

  Logger::pgwc_app().info( "- " PGW_CONFIG_STRING_APN_LIST ":");
  for (int i = 0; i < num_apn; i++) {
    Logger::pgwc_app().info( "    APN %d:", i);
    Logger::pgwc_app().info( "        " PGW_CONFIG_STRING_APN_NI ":  %s", apn[i].apn.c_str());
    Logger::pgwc_app().info( "        " PGW_CONFIG_STRING_PDN_TYPE ":  %s", pdn_type_e2string(apn[i].pdn_type.pdn_type).c_str());
    if (apn[i].pool_id_iv4 >= 0) {
      Logger::pgwc_app().info( "        " PGW_CONFIG_STRING_IPV4_POOL ":  %d ( %s - %s)", apn[i].pool_id_iv4,
          inet_ntoa (*((struct in_addr *)&ue_pool_range_low[apn[i].pool_id_iv4])),
          inet_ntoa (*((struct in_addr *)&ue_pool_range_high[apn[i].pool_id_iv4])));
    }
    if (apn[i].pool_id_iv6 >= 0) {
      Logger::pgwc_app().info( "        " PGW_CONFIG_STRING_IPV6_POOL ":  %d", apn[i].pool_id_iv6);
    }
  }
  Logger::pgwc_app().info( "- PCEF support (in development)");
  Logger::pgwc_app().info( "    APN AMBR UL ..........: %lu  (Kilo bits/s)", pcef.apn_ambr_ul);
  Logger::pgwc_app().info( "    APN AMBR DL ..........: %lu  (Kilo bits/s)", pcef.apn_ambr_dl);
  Logger::pgwc_app().info( "- Helpers:");
  Logger::pgwc_app().info( "    Push PCO (DNS+MTU) ........: %s", force_push_pco == 0 ? "false" : "true");
}

//------------------------------------------------------------------------------
bool pgw_config::is_dotted_apn_handled(const string& apn, const core::pdn_type_t& pdn_type)
{
  for (int i = 0; i < pgw_cfg.num_apn; i++) {
    if (0 == apn.compare(pgw_cfg.apn[i].apn_label)) {
      // TODO refine
      if (pdn_type.pdn_type == pgw_cfg.apn[i].pdn_type.pdn_type) {
        return true;
      }
    }
  }
  return false;
}
//------------------------------------------------------------------------------
int pgw_config::get_pa_pool_id(const std::string& apn, int& pool_id_ipv4, int& pool_id_ipv6)
{
  for (int i = 0; i < pgw_cfg.num_apn; i++) {
    if (0 == apn.compare(pgw_cfg.apn[i].apn_label)) {
      pool_id_ipv4 = pgw_cfg.apn[i].pool_id_iv4;
      pool_id_ipv6 = pgw_cfg.apn[i].pool_id_iv6;
      return RETURNok;
    }
  }
  return RETURNerror;
}

//------------------------------------------------------------------------------
int pgw_config::get_pfcp_node_id(oai::cn::core::pfcp::node_id_t& node_id)
{
  node_id = {};
  if (sx.addr4.s_addr) {
    node_id.node_id_type = oai::cn::core::pfcp::NODE_ID_TYPE_IPV4_ADDRESS;
    node_id.u1.ipv4_address = sx.addr4;
    return RETURNok;
  }
  if (sx.addr6.s6_addr32[0] | sx.addr6.s6_addr32[1] | sx.addr6.s6_addr32[2] | sx.addr6.s6_addr32[3]) {
    node_id.node_id_type = oai::cn::core::pfcp::NODE_ID_TYPE_IPV6_ADDRESS;
    node_id.u1.ipv6_address = sx.addr6;
    return RETURNok;
  }
  return RETURNerror;
}
//------------------------------------------------------------------------------
int pgw_config::get_pfcp_fseid(oai::cn::core::pfcp::fseid_t& fseid)
{
  int rc = RETURNerror;
  fseid = {};
  if (sx.addr4.s_addr) {
    fseid.v4 = 1;
    fseid.ipv4_address = sx.addr4;
    rc = RETURNok;
  }
  if (sx.addr6.s6_addr32[0] | sx.addr6.s6_addr32[1] | sx.addr6.s6_addr32[2] | sx.addr6.s6_addr32[3]) {
    fseid.v6 = 1;
    fseid.ipv6_address = sx.addr6;
    rc = RETURNok;
  }
  return rc;
}

//------------------------------------------------------------------------------
pgw_config::~pgw_config()
{
  for (int i = 0; i < PGW_NUM_UE_POOL_MAX; i++) {
    ue_pool_excluded[i].erase(ue_pool_excluded[i].begin(),ue_pool_excluded[i].end());
  }
}

