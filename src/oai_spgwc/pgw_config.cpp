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

/*! \file pgw_config.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "pgw_config.hpp"
#include "3gpp_29.274.hpp"
#include "common_defs.h"
#include "epc.h"
#include "if.hpp"
#include "logger.hpp"
#include "pgw_app.hpp"
#include "string.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace libconfig;
using namespace pgwc;

// C includes
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

extern pgw_app* pgw_app_inst;
extern pgw_config pgw_cfg;

//------------------------------------------------------------------------------
int pgw_config::finalize() {
  Logger::pgwc_app().info("Finalize config...");

  for (int i = 0; i < num_ue_pool; i++) {
    uint32_t range_low_hbo  = ntohl(ue_pool_range_low[i].s_addr);
    uint32_t range_high_hbo = ntohl(ue_pool_range_high[i].s_addr);
    uint32_t tmp_hbo        = range_low_hbo ^ range_high_hbo;
    uint8_t nbits           = 32;
    while (tmp_hbo) {
      tmp_hbo = tmp_hbo >> 1;
      nbits -= 1;
    }
    uint32_t network_hbo      = range_high_hbo & (UINT32_MAX << (32 - nbits));
    uint32_t netmask_hbo      = 0xFFFFFFFF << (32 - nbits);
    ue_pool_network[i].s_addr = htonl(network_hbo);
    ue_pool_netmask[i].s_addr = htonl(netmask_hbo);
  }
  // "TODO"
  // pgw_pcef_emulation_init(config_pP);
  Logger::pgwc_app().info("Finalized config");
  return 0;
}

//------------------------------------------------------------------------------
int pgw_config::load_thread_sched_params(
    const Setting& thread_sched_params_cfg, util::thread_sched_params& cfg) {
  try {
    thread_sched_params_cfg.lookupValue(
        PGW_CONFIG_STRING_THREAD_RD_CPU_ID, cfg.cpu_id);
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    std::string thread_rd_sched_policy;
    thread_sched_params_cfg.lookupValue(
        PGW_CONFIG_STRING_THREAD_RD_SCHED_POLICY, thread_rd_sched_policy);
    util::trim(thread_rd_sched_policy);
    if (boost::iequals(thread_rd_sched_policy, "SCHED_OTHER")) {
      cfg.sched_policy = SCHED_OTHER;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_IDLE")) {
      cfg.sched_policy = SCHED_IDLE;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_BATCH")) {
      cfg.sched_policy = SCHED_BATCH;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_FIFO")) {
      cfg.sched_policy = SCHED_FIFO;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_RR")) {
      cfg.sched_policy = SCHED_RR;
    } else {
      Logger::pgwc_app().error(
          "thread_rd_sched_policy: %s, unknown in config file",
          thread_rd_sched_policy.c_str());
      return RETURNerror;
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    thread_sched_params_cfg.lookupValue(
        PGW_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY, cfg.sched_priority);
    if ((cfg.sched_priority > 99) || (cfg.sched_priority < 1)) {
      Logger::pgwc_app().error(
          "thread_rd_sched_priority: %d, must be in interval [1..99] in config "
          "file",
          cfg.sched_priority);
      return RETURNerror;
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  return RETURNok;
}
//------------------------------------------------------------------------------
int pgw_config::load_itti(const Setting& itti_cfg, itti_cfg_t& cfg) {
  try {
    const Setting& itti_timer_sched_params_cfg =
        itti_cfg[PGW_CONFIG_STRING_ITTI_TIMER_SCHED_PARAMS];
    load_thread_sched_params(
        itti_timer_sched_params_cfg, cfg.itti_timer_sched_params);
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& s5s8_sched_params_cfg =
        itti_cfg[PGW_CONFIG_STRING_S5S8_SCHED_PARAMS];
    load_thread_sched_params(s5s8_sched_params_cfg, cfg.s5s8_sched_params);
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& sx_sched_params_cfg =
        itti_cfg[PGW_CONFIG_STRING_SX_SCHED_PARAMS];
    load_thread_sched_params(sx_sched_params_cfg, cfg.sx_sched_params);
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& pgw_app_sched_params_cfg =
        itti_cfg[PGW_CONFIG_STRING_PGW_APP_SCHED_PARAMS];
    load_thread_sched_params(
        pgw_app_sched_params_cfg, cfg.pgw_app_sched_params);
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& async_cmd_sched_params_cfg =
        itti_cfg[PGW_CONFIG_STRING_ASYNC_CMD_SCHED_PARAMS];
    load_thread_sched_params(
        async_cmd_sched_params_cfg, cfg.async_cmd_sched_params);
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  return RETURNok;
}

//------------------------------------------------------------------------------
int pgw_config::load_interface(const Setting& if_cfg, interface_cfg_t& cfg) {
  if_cfg.lookupValue(PGW_CONFIG_STRING_INTERFACE_NAME, cfg.if_name);
  util::trim(cfg.if_name);
  if (not boost::iequals(cfg.if_name, "none")) {
    std::string address = {};
    if_cfg.lookupValue(PGW_CONFIG_STRING_IPV4_ADDRESS, address);
    util::trim(address);
    if (boost::iequals(address, "read")) {
      if (get_inet_addr_infos_from_iface(
              cfg.if_name, cfg.addr4, cfg.network4, cfg.mtu)) {
        Logger::pgwc_app().error(
            "Could not read %s network interface configuration", cfg.if_name);
        return RETURNerror;
      }
    } else {
      std::vector<std::string> words;
      boost::split(
          words, address, boost::is_any_of("/"), boost::token_compress_on);
      if (words.size() != 2) {
        Logger::pgwc_app().error(
            "Bad value " PGW_CONFIG_STRING_IPV4_ADDRESS " = %s in config file",
            address.c_str());
        return RETURNerror;
      }
      unsigned char buf_in_addr[sizeof(struct in6_addr)];  // you never know...
      if (inet_pton(AF_INET, util::trim(words.at(0)).c_str(), buf_in_addr) ==
          1) {
        memcpy(&cfg.addr4, buf_in_addr, sizeof(struct in_addr));
      } else {
        Logger::pgwc_app().error(
            "In conversion: Bad value " PGW_CONFIG_STRING_IPV4_ADDRESS
            " = %s in config file",
            util::trim(words.at(0)).c_str());
        return RETURNerror;
      }
      cfg.network4.s_addr = htons(
          ntohs(cfg.addr4.s_addr) &
          0xFFFFFFFF << (32 - std::stoi(util::trim(words.at(1)))));
    }
    if_cfg.lookupValue(PGW_CONFIG_STRING_PORT, cfg.port);

    try {
      const Setting& sched_params_cfg = if_cfg[PGW_CONFIG_STRING_SCHED_PARAMS];
      load_thread_sched_params(sched_params_cfg, cfg.thread_rd_sched_params);
    } catch (const SettingNotFoundException& nfex) {
      Logger::pgwc_app().info(
          "%s : %s, using defaults", nfex.what(), nfex.getPath());
    }
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int pgw_config::load(const string& config_file) {
  Config cfg;
  unsigned char buf_in6_addr[sizeof(struct in6_addr)];

  // Read the file. If there is an error, report it and exit.
  try {
    cfg.readFile(config_file.c_str());
  } catch (const FileIOException& fioex) {
    Logger::pgwc_app().error(
        "I/O error while reading file %s - %s", config_file.c_str(),
        fioex.what());
    throw;
  } catch (const ParseException& pex) {
    Logger::pgwc_app().error(
        "Parse error at %s:%d - %s", pex.getFile(), pex.getLine(),
        pex.getError());
    throw;
  }

  const Setting& root = cfg.getRoot();

  try {
    const Setting& pgw_cfg = root[PGW_CONFIG_STRING_PGW_CONFIG];
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().error("%s : %s", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  const Setting& pgw_cfg = root[PGW_CONFIG_STRING_PGW_CONFIG];

  try {
    pgw_cfg.lookupValue(PGW_CONFIG_STRING_INSTANCE, instance);
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    pgw_cfg.lookupValue(PGW_CONFIG_STRING_PID_DIRECTORY, pid_dir);
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& itti_cfg = pgw_cfg[PGW_CONFIG_STRING_ITTI_TASKS];
    load_itti(itti_cfg, itti);
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& nw_if_cfg = pgw_cfg[PGW_CONFIG_STRING_INTERFACES];

    const Setting& s5s8_cp_cfg =
        nw_if_cfg[PGW_CONFIG_STRING_INTERFACE_S5_S8_CP];
    load_interface(s5s8_cp_cfg, s5s8_cp);

    const Setting& sx_cfg = nw_if_cfg[PGW_CONFIG_STRING_INTERFACE_SX];
    load_interface(sx_cfg, sx);
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().error("%s : %s", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  try {
    string astring;

    const Setting& pool_cfg = pgw_cfg[PGW_CONFIG_STRING_IP_ADDRESS_POOL];

    const Setting& ipv4_pool_cfg =
        pool_cfg[PGW_CONFIG_STRING_IPV4_ADDRESS_LIST];
    int count = ipv4_pool_cfg.getLength();
    for (int i = 0; i < count; i++) {
      const Setting& ipv4_cfg = ipv4_pool_cfg[i];
      string ipv4_range;
      unsigned char buf_in_addr[sizeof(struct in_addr)];

      ipv4_cfg.lookupValue(PGW_CONFIG_STRING_RANGE, ipv4_range);
      std::vector<std::string> ips;
      boost::split(
          ips, ipv4_range,
          boost::is_any_of(PGW_CONFIG_STRING_IPV4_ADDRESS_RANGE_DELIMITER),
          boost::token_compress_on);
      if (ips.size() != 2) {
        Logger::pgwc_app().error(
            "Bad value %s : %s in config file %s",
            PGW_CONFIG_STRING_IPV4_ADDRESS_RANGE_DELIMITER, ipv4_range.c_str(),
            config_file.c_str());
        throw(
            "Bad value %s : %s in config file %s",
            PGW_CONFIG_STRING_IPV4_ADDRESS_RANGE_DELIMITER, ipv4_range.c_str(),
            config_file.c_str());
      }

      memset(buf_in_addr, 0, sizeof(buf_in_addr));
      if (inet_pton(AF_INET, util::trim(ips.at(0)).c_str(), buf_in_addr) == 1) {
        memcpy(
            &ue_pool_range_low[num_ue_pool], buf_in_addr,
            sizeof(struct in_addr));
      } else {
        Logger::pgwc_app().error(
            "CONFIG POOL ADDR IPV4: BAD LOWER ADDRESS "
            "in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST " pool %d",
            i);
        throw(
            "CONFIG POOL ADDR IPV4: BAD ADDRESS "
            "in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST);
      }

      memset(buf_in_addr, 0, sizeof(buf_in_addr));
      if (inet_pton(AF_INET, util::trim(ips.at(1)).c_str(), buf_in_addr) == 1) {
        memcpy(
            &ue_pool_range_high[num_ue_pool], buf_in_addr,
            sizeof(struct in_addr));
      } else {
        Logger::pgwc_app().error(
            "CONFIG POOL ADDR IPV4: BAD HIGHER ADDRESS "
            "in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST " pool %d",
            i);
        throw(
            "CONFIG POOL ADDR IPV4: BAD ADDRESS "
            "in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST);
      }
      if (htonl(ue_pool_range_low[num_ue_pool].s_addr) >=
          htonl(ue_pool_range_high[num_ue_pool].s_addr)) {
        Logger::pgwc_app().error(
            "CONFIG POOL ADDR IPV4: BAD RANGE "
            "in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST " pool %d",
            i);
        throw(
            "CONFIG POOL ADDR IPV4: BAD RANGE "
            "in " PGW_CONFIG_STRING_IPV4_ADDRESS_LIST);
      }
      num_ue_pool += 1;
    }

    const Setting& ipv6_pool_cfg =
        pool_cfg[PGW_CONFIG_STRING_IPV6_ADDRESS_LIST];
    int count6 = ipv6_pool_cfg.getLength();
    for (int i = 0; i < count6; i++) {
      const Setting& ipv6_cfg = ipv6_pool_cfg[i];
      string ipv6_prefix;
      ipv6_cfg.lookupValue(PGW_CONFIG_STRING_PREFIX, ipv6_prefix);
      std::vector<std::string> ips6;
      boost::split(
          ips6, ipv6_prefix,
          boost::is_any_of(PGW_CONFIG_STRING_IPV6_ADDRESS_PREFIX_DELIMITER),
          boost::token_compress_on);
      if (ips6.size() != 2) {
        Logger::pgwc_app().error(
            "Bad value %s : %s in config file %s", PGW_CONFIG_STRING_PREFIX,
            ipv6_prefix.c_str(), config_file.c_str());
        throw(
            "Bad value %s : %s in config file %s", PGW_CONFIG_STRING_PREFIX,
            ipv6_prefix.c_str(), config_file.c_str());
      }

      std::string addr = ips6.at(0);
      util::trim(addr);
      if (inet_pton(AF_INET6, addr.c_str(), buf_in6_addr) == 1) {
        memcpy(
            &paa_pool6_prefix[num_paa6_pool], buf_in6_addr,
            sizeof(struct in6_addr));
      } else {
        Logger::pgwc_app().error(
            "CONFIG POOL ADDR IPV6: BAD ADDRESS "
            "in " PGW_CONFIG_STRING_IPV6_ADDRESS_LIST " pool %d",
            i);
        throw(
            "CONFIG POOL ADDR IPV6: BAD ADDRESS "
            "in " PGW_CONFIG_STRING_IPV6_ADDRESS_LIST);
      }

      std::string prefix = ips6.at(1);
      util::trim(prefix);
      paa_pool6_prefix_len[num_paa6_pool] = std::stoi(prefix);
      num_paa6_pool += 1;
    }

    const Setting& apn_list_cfg = pgw_cfg[PGW_CONFIG_STRING_APN_LIST];
    count                       = apn_list_cfg.getLength();
    int apn_idx                 = 0;
    num_apn                     = 0;
    for (int i = 0; i < count; i++) {
      const Setting& apn_cfg = apn_list_cfg[i];
      apn_cfg.lookupValue(PGW_CONFIG_STRING_APN_NI, astring);
      apn[apn_idx].apn       = astring;
      apn[apn_idx].apn_label = EPC::Utility::apn_label(astring);
      apn_cfg.lookupValue(PGW_CONFIG_STRING_PDN_TYPE, astring);
      if (boost::iequals(astring, "IPv4")) {
        apn[apn_idx].pdn_type.pdn_type = PDN_TYPE_E_IPV4;
      } else if (boost::iequals(astring, "IPv6") == 0) {
        apn[apn_idx].pdn_type.pdn_type = PDN_TYPE_E_IPV6;
      } else if (boost::iequals(astring, "IPv4IPv6") == 0) {
        apn[apn_idx].pdn_type.pdn_type = PDN_TYPE_E_IPV4V6;
      } else if (boost::iequals(astring, "Non-IP") == 0) {
        apn[apn_idx].pdn_type.pdn_type = PDN_TYPE_E_NON_IP;
      } else {
        Logger::pgwc_app().error(
            " " PGW_CONFIG_STRING_PDN_TYPE " in %d'th APN :%s", i + 1,
            astring.c_str());
        throw("Error PDN_TYPE in config file");
      }
      apn_cfg.lookupValue(
          PGW_CONFIG_STRING_IPV4_POOL, apn[apn_idx].pool_id_iv4);
      apn_cfg.lookupValue(
          PGW_CONFIG_STRING_IPV6_POOL, apn[apn_idx].pool_id_iv6);

      if ((0 <= apn[apn_idx].pool_id_iv4) &&
          (apn[apn_idx].pdn_type.pdn_type == PDN_TYPE_E_IPV6)) {
        Logger::pgwc_app().error(
            "PDN_TYPE versus pool identifier %d 'th APN in config file\n",
            i + 1);
        throw("PDN_TYPE versus pool identifier APN");
      }
      if ((0 <= apn[apn_idx].pool_id_iv6) &&
          (apn[apn_idx].pdn_type.pdn_type == PDN_TYPE_E_IPV4)) {
        Logger::pgwc_app().error(
            "PDN_TYPE versus pool identifier %d 'th APN in config file\n",
            i + 1);
        throw("PDN_TYPE versus pool identifier APN");
      }

      if (((0 <= apn[apn_idx].pool_id_iv4) ||
           (0 <= apn[apn_idx].pool_id_iv6)) &&
          (not boost::iequals(apn[apn_idx].apn, "none"))) {
        bool doublon = false;
        for (int j = 0; j < apn_idx; j++) {
          if (boost::iequals(apn[j].apn, apn[apn_idx].apn)) {
            doublon = true;
            Logger::pgwc_app().info(
                "%d'th APN %s already found in config file (%d 'th APN %s), "
                "bypassing\n",
                i + 1, apn[apn_idx].apn.c_str(), j + 1, apn[j].apn.c_str());
          }
        }
        if (not doublon) {
          apn_idx++;
          num_apn++;
        }
      } else {
        Logger::pgwc_app().error(
            "Bypass %d'th APN %s in config file\n", i + 1,
            apn[apn_idx].apn.c_str());
      }
    }
    pgw_cfg.lookupValue(PGW_CONFIG_STRING_DEFAULT_DNS_IPV4_ADDRESS, astring);
    IPV4_STR_ADDR_TO_INADDR(
        util::trim(astring).c_str(), default_dnsv4,
        "BAD IPv4 ADDRESS FORMAT FOR DEFAULT DNS !");

    pgw_cfg.lookupValue(
        PGW_CONFIG_STRING_DEFAULT_DNS_SEC_IPV4_ADDRESS, astring);
    IPV4_STR_ADDR_TO_INADDR(
        util::trim(astring).c_str(), default_dns_secv4,
        "BAD IPv4 ADDRESS FORMAT FOR DEFAULT DNS !");

    pgw_cfg.lookupValue(PGW_CONFIG_STRING_DEFAULT_DNS_IPV6_ADDRESS, astring);
    if (inet_pton(AF_INET6, util::trim(astring).c_str(), buf_in6_addr) == 1) {
      memcpy(&default_dnsv6, buf_in6_addr, sizeof(struct in6_addr));
    } else {
      Logger::pgwc_app().error(
          "CONFIG : BAD ADDRESS in " PGW_CONFIG_STRING_DEFAULT_DNS_IPV6_ADDRESS
          " %s",
          astring.c_str());
      throw(
          "CONFIG : BAD ADDRESS in " PGW_CONFIG_STRING_DEFAULT_DNS_IPV6_ADDRESS
          " %s",
          astring.c_str());
    }
    pgw_cfg.lookupValue(
        PGW_CONFIG_STRING_DEFAULT_DNS_SEC_IPV6_ADDRESS, astring);
    if (inet_pton(AF_INET6, util::trim(astring).c_str(), buf_in6_addr) == 1) {
      memcpy(&default_dns_secv6, buf_in6_addr, sizeof(struct in6_addr));
    } else {
      Logger::pgwc_app().error(
          "CONFIG : BAD ADDRESS "
          "in " PGW_CONFIG_STRING_DEFAULT_DNS_SEC_IPV6_ADDRESS " %s",
          astring.c_str());
      throw(
          "CONFIG : BAD ADDRESS "
          "in " PGW_CONFIG_STRING_DEFAULT_DNS_SEC_IPV6_ADDRESS " %s",
          astring.c_str());
    }

    pgw_cfg.lookupValue(PGW_CONFIG_STRING_NAS_FORCE_PUSH_PCO, astring);
    if (boost::iequals(astring, "yes")) {
      force_push_pco = true;
    } else {
      force_push_pco = false;
    }
    pgw_cfg.lookupValue(PGW_CONFIG_STRING_UE_MTU, ue_mtu);

    const Setting& pcef_cfg = pgw_cfg[PGW_CONFIG_STRING_PCEF];
    unsigned int apn_ambr   = 0;
    if (!(pcef_cfg.lookupValue(PGW_CONFIG_STRING_APN_AMBR_UL, apn_ambr))) {
      Logger::pgwc_app().error(PGW_CONFIG_STRING_APN_AMBR_UL "failed");
      throw(PGW_CONFIG_STRING_APN_AMBR_UL "failed");
    }
    pcef.apn_ambr_ul = apn_ambr;
    if (!(pcef_cfg.lookupValue(PGW_CONFIG_STRING_APN_AMBR_DL, apn_ambr))) {
      Logger::pgwc_app().error(PGW_CONFIG_STRING_APN_AMBR_DL "failed");
      // throw (PGW_CONFIG_STRING_APN_AMBR_DL "failed");
    }
    pcef.apn_ambr_dl = apn_ambr;
  } catch (const SettingNotFoundException& nfex) {
    Logger::pgwc_app().error("%s : %s", nfex.what(), nfex.getPath());
    return RETURNerror;
  }
  return finalize();
}

//------------------------------------------------------------------------------
void pgw_config::display() {
  Logger::pgwc_app().info(
      "==== EURECOM %s v%s ====", PACKAGE_NAME, PACKAGE_VERSION);
  Logger::pgwc_app().info("Configuration PGW-C:");
  Logger::pgwc_app().info("- Instance ..............: %d\n", instance);
  Logger::pgwc_app().info("- PID dir ...............: %s\n", pid_dir.c_str());

  Logger::pgwc_app().info("- S5S8-C Networking:");
  Logger::pgwc_app().info(
      "    iface ............: %s", s5s8_cp.if_name.c_str());
  Logger::pgwc_app().info(
      "    ipv4.addr ........: %s", inet_ntoa(s5s8_cp.addr4));
  if (s5s8_cp.network4.s_addr) {
    Logger::pgwc_app().info(
        "    ipv4.mask ........: %s", inet_ntoa(s5s8_cp.network4));
  }
  Logger::pgwc_app().info("    port .............: %d", s5s8_cp.port);
  if (s5s8_cp.mtu) {
    Logger::pgwc_app().info("    MTU ..............: %u", s5s8_cp.mtu);
  }
  Logger::pgwc_app().info("- SX Networking:");
  Logger::pgwc_app().info("    iface ................: %s", sx.if_name.c_str());
  Logger::pgwc_app().info(
      "    ip ...................: %s", inet_ntoa(sx.addr4));
  Logger::pgwc_app().info("    port .................: %d", sx.port);
  Logger::pgwc_app().info("- S5S8 Threading:");
  Logger::pgwc_app().info(
      "    CPU id............: %d", s5s8_cp.thread_rd_sched_params.cpu_id);
  Logger::pgwc_app().info(
      "    Scheduling policy : %d",
      s5s8_cp.thread_rd_sched_params.sched_policy);
  Logger::pgwc_app().info(
      "    Scheduling prio  .: %d",
      s5s8_cp.thread_rd_sched_params.sched_priority);
  Logger::pgwc_app().info("- SX Threading:");
  Logger::pgwc_app().info(
      "    CPU id............: %d", sx.thread_rd_sched_params.cpu_id);
  Logger::pgwc_app().info(
      "    Scheduling policy : %d", sx.thread_rd_sched_params.sched_policy);
  Logger::pgwc_app().info(
      "    Scheduling prio  .: %d", sx.thread_rd_sched_params.sched_priority);
  Logger::pgwc_app().info("- ITTI Timer Task Threading:");
  Logger::pgwc_app().info(
      "    CPU id............: %d", itti.itti_timer_sched_params.cpu_id);
  Logger::pgwc_app().info(
      "    Scheduling policy : %d", itti.itti_timer_sched_params.sched_policy);
  Logger::pgwc_app().info(
      "    Scheduling prio  .: %d",
      itti.itti_timer_sched_params.sched_priority);
  Logger::pgwc_app().info("- ITTI S5S8 Task Threading:");
  Logger::pgwc_app().info(
      "    CPU id............: %d", itti.s5s8_sched_params.cpu_id);
  Logger::pgwc_app().info(
      "    Scheduling policy : %d", itti.s5s8_sched_params.sched_policy);
  Logger::pgwc_app().info(
      "    Scheduling prio  .: %d", itti.s5s8_sched_params.sched_priority);
  Logger::pgwc_app().info("- ITTI Sx Task Threading:");
  Logger::pgwc_app().info(
      "    CPU id............: %d", itti.sx_sched_params.cpu_id);
  Logger::pgwc_app().info(
      "    Scheduling policy : %d", itti.sx_sched_params.sched_policy);
  Logger::pgwc_app().info(
      "    Scheduling prio  .: %d", itti.sx_sched_params.sched_priority);
  Logger::pgwc_app().info("- ITTI PGW_APP task Threading:");
  Logger::pgwc_app().info(
      "    CPU id............: %d", itti.pgw_app_sched_params.cpu_id);
  Logger::pgwc_app().info(
      "    Scheduling policy : %d", itti.pgw_app_sched_params.sched_policy);
  Logger::pgwc_app().info(
      "    Scheduling prio  .: %d", itti.pgw_app_sched_params.sched_priority);
  Logger::pgwc_app().info("- ITTI ASYNC_CMD task Threading:");
  Logger::pgwc_app().info(
      "    CPU id............: %d", itti.async_cmd_sched_params.cpu_id);
  Logger::pgwc_app().info(
      "    Scheduling policy : %d", itti.async_cmd_sched_params.sched_policy);
  Logger::pgwc_app().info(
      "    Scheduling prio  .: %d", itti.async_cmd_sched_params.sched_priority);
  Logger::pgwc_app().info("- " PGW_CONFIG_STRING_IP_ADDRESS_POOL ":");
  for (int i = 0; i < num_ue_pool; i++) {
    std::string range_low(inet_ntoa(ue_pool_range_low[apn[i].pool_id_iv4]));
    std::string range_high(inet_ntoa(ue_pool_range_high[apn[i].pool_id_iv4]));
    Logger::pgwc_app().info(
        "    IPv4 pool %d ..........: %s - %s", i, range_low.c_str(),
        range_high.c_str());
  }
  char str_addr6[INET6_ADDRSTRLEN];
  for (int i = 0; i < num_paa6_pool; i++) {
    if (inet_ntop(
            AF_INET6, &paa_pool6_prefix[i], str_addr6, sizeof(str_addr6))) {
      Logger::pgwc_app().info(
          "    IPv6 pool %d ..........: %s / %d", i, str_addr6,
          paa_pool6_prefix_len[i]);
    }
  }
  Logger::pgwc_app().info("- DEFAULT DNS:");
  Logger::pgwc_app().info(
      "    Primary DNS ..........: %s",
      inet_ntoa(*((struct in_addr*) &default_dnsv4)));
  Logger::pgwc_app().info(
      "    Secondary DNS ........: %s",
      inet_ntoa(*((struct in_addr*) &default_dns_secv4)));
  if (inet_ntop(AF_INET6, &default_dnsv6, str_addr6, sizeof(str_addr6))) {
    Logger::pgwc_app().info("    Primary DNS v6........: %s", str_addr6);
  }
  if (inet_ntop(AF_INET6, &default_dns_secv6, str_addr6, sizeof(str_addr6))) {
    Logger::pgwc_app().info("    Secondary DNS v6 .....: %s", str_addr6);
  }

  Logger::pgwc_app().info("- " PGW_CONFIG_STRING_APN_LIST ":");
  for (int i = 0; i < num_apn; i++) {
    Logger::pgwc_app().info("    APN %d:", i);
    Logger::pgwc_app().info(
        "        " PGW_CONFIG_STRING_APN_NI ":  %s", apn[i].apn.c_str());
    Logger::pgwc_app().info(
        "        " PGW_CONFIG_STRING_PDN_TYPE ":  %s",
        apn[i].pdn_type.toString().c_str());
    if (apn[i].pool_id_iv4 >= 0) {
      std::string range_low(inet_ntoa(ue_pool_range_low[apn[i].pool_id_iv4]));
      std::string range_high(inet_ntoa(ue_pool_range_high[apn[i].pool_id_iv4]));
      Logger::pgwc_app().info(
          "        " PGW_CONFIG_STRING_IPV4_POOL ":  %d ( %s - %s)",
          apn[i].pool_id_iv4, range_low.c_str(), range_high.c_str());
    }
    if (apn[i].pool_id_iv6 >= 0) {
      Logger::pgwc_app().info(
          "        " PGW_CONFIG_STRING_IPV6_POOL ":  %d", apn[i].pool_id_iv6);
    }
  }
  Logger::pgwc_app().info("- PCEF support (in development)");
  Logger::pgwc_app().info(
      "    APN AMBR UL ..........: %lu  (Kilo bits/s)", pcef.apn_ambr_ul);
  Logger::pgwc_app().info(
      "    APN AMBR DL ..........: %lu  (Kilo bits/s)", pcef.apn_ambr_dl);
  Logger::pgwc_app().info("- Helpers:");
  Logger::pgwc_app().info(
      "    Push PCO (DNS+MTU) ........: %s",
      force_push_pco == 0 ? "false" : "true");
}

//------------------------------------------------------------------------------
bool pgw_config::is_dotted_apn_handled(
    const string& apn, const pdn_type_t& pdn_type) {
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
int pgw_config::get_pfcp_node_id(pfcp::node_id_t& node_id) {
  node_id = {};
  if (sx.addr4.s_addr) {
    node_id.node_id_type    = pfcp::NODE_ID_TYPE_IPV4_ADDRESS;
    node_id.u1.ipv4_address = sx.addr4;
    return RETURNok;
  }
  if (sx.addr6.s6_addr32[0] | sx.addr6.s6_addr32[1] | sx.addr6.s6_addr32[2] |
      sx.addr6.s6_addr32[3]) {
    node_id.node_id_type    = pfcp::NODE_ID_TYPE_IPV6_ADDRESS;
    node_id.u1.ipv6_address = sx.addr6;
    return RETURNok;
  }
  return RETURNerror;
}
//------------------------------------------------------------------------------
int pgw_config::get_pfcp_fseid(pfcp::fseid_t& fseid) {
  int rc = RETURNerror;
  fseid  = {};
  if (sx.addr4.s_addr) {
    fseid.v4           = 1;
    fseid.ipv4_address = sx.addr4;
    rc                 = RETURNok;
  }
  if (sx.addr6.s6_addr32[0] | sx.addr6.s6_addr32[1] | sx.addr6.s6_addr32[2] |
      sx.addr6.s6_addr32[3]) {
    fseid.v6           = 1;
    fseid.ipv6_address = sx.addr6;
    rc                 = RETURNok;
  }
  return rc;
}

//------------------------------------------------------------------------------
pgw_config::~pgw_config() {}
