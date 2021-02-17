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
//--related header -------------------------------------------------------------
#include "pgw_config.hpp"
// C includes ------------------------------------------------------------------
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
//--C++ includes ---------------------------------------------------------------
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cstdlib>
#include <iomanip>
#include <iostream>
//--Other includes -------------------------------------------------------------
#include "3gpp_29.274.hpp"
#include "common_defs.h"
#include "epc.h"
#include "if.hpp"
#include "logger.hpp"
#include "pgw_app.hpp"
#include "string.hpp"
//------------------------------------------------------------------------------
using namespace std;
using namespace pgwc;

extern pgw_app* pgw_app_inst;

const int pgw_config::kJsonFileBuffer = 1024;
std::string pgw_config::jsoncfg_;
std::mutex pgw_config::rw_lock_;
timer_cfg_t pgw_config::timer_;
gtpv2c_cfg_t pgw_config::gtpv2c_;
pfcp_cfg_t pgw_config::pfcp_;
s11_cfg_t pgw_config::s11_;
s5s8_cfg_t pgw_config::sgw_s5s8_;
s5s8_cfg_t pgw_config::pgw_s5s8_;
sx_cfg_t pgw_config::sx_;
pgw_app_cfg_t pgw_config::spgw_app_;
PdnCfg pgw_config::pdn_;
cups_cfg_t pgw_config::cups_;
std::string pgw_config::pid_dir_;
unsigned int pgw_config::instance_;
//------------------------------------------------------------------------------
const bool pgw_config::Finalize() {
  Logger::pgwc_app().info("Finalize config...");
  std::vector<PdnCfg> pdns = {};
  for (auto p : spgw_app_.pdns) {
    uint32_t range_low_hbo  = ntohl(p.ue_pool_range_low.s_addr);
    uint32_t range_high_hbo = ntohl(p.ue_pool_range_high.s_addr);
    uint32_t tmp_hbo        = range_low_hbo ^ range_high_hbo;
    uint8_t nbits           = 32;
    while (tmp_hbo) {
      tmp_hbo = tmp_hbo >> 1;
      nbits -= 1;
    }
    uint32_t network_hbo     = range_high_hbo & (UINT32_MAX << (32 - nbits));
    uint32_t netmask_hbo     = 0xFFFFFFFF << (32 - nbits);
    p.ue_pool_network.s_addr = htonl(network_hbo);
    p.ue_pool_netmask.s_addr = htonl(netmask_hbo);
    // TODO
    p.apn_label = p.apn;
    pdns.push_back(p);
  }
  spgw_app_.pdns = pdns;
  // "TODO"
  // pgw_pcef_emulation_init(config_pP);
  Logger::pgwc_app().info("Finalized config");
  return true;
}
//------------------------------------------------------------------------------
const bool pgw_config::ParseSchedParams(
    const RAPIDJSON_NAMESPACE::Value& conf, util::thread_sched_params& cfg) {
  if (conf.HasMember("cpu_id")) {
    if (!conf["cpu_id"].IsInt()) {
      Logger::pgwc_app().error("Error parsing json value: [cpu_id]");
      return false;
    }
    cfg.cpu_id = conf["cpu_id"].GetUint();
  }
  if (conf.HasMember("sched_priority")) {
    if (!conf["sched_priority"].IsInt()) {
      Logger::pgwc_app().error("Error parsing json value: [sched_priority]");
      return false;
    }
    cfg.sched_priority = conf["sched_priority"].GetUint();
  }
  if (conf.HasMember("sched_policy")) {
    if (!conf["sched_policy"].IsString()) {
      Logger::pgwc_app().error("Error parsing json value: [sched_policy]");
      return false;
    }
    std::string sched_policy = conf["sched_policy"].GetString();
    util::trim(sched_policy);
    if (boost::iequals(sched_policy, "sched_other")) {
      cfg.sched_policy = SCHED_OTHER;
    } else if (boost::iequals(sched_policy, "sched_idle")) {
      cfg.sched_policy = SCHED_IDLE;
    } else if (boost::iequals(sched_policy, "sched_batch")) {
      cfg.sched_policy = SCHED_BATCH;
    } else if (boost::iequals(sched_policy, "sched_fifo")) {
      cfg.sched_policy = SCHED_FIFO;
    } else if (boost::iequals(sched_policy, "sched_rr")) {
      cfg.sched_policy = SCHED_RR;
    } else {
      Logger::pgwc_app().error(
          "sched_policy: %s, unknown, allowed values [sched_other, sched_idle"
          ", sched_batch, sched_fifo, sched_rr",
          sched_policy.c_str());
      return false;
    }
  }
  return true;
}
//------------------------------------------------------------------------------
const bool pgw_config::ParseTimer(
    const RAPIDJSON_NAMESPACE::Value& conf, timer_cfg_t& cfg) {
  if (conf.HasMember("itti")) {
    const RAPIDJSON_NAMESPACE::Value& itti_section = conf["itti"];
    if (itti_section.HasMember("sched_params")) {
      const RAPIDJSON_NAMESPACE::Value& sched_section =
          itti_section["sched_params"];
      return ParseSchedParams(sched_section, cfg.sched_params);
    }
  }
  return true;
}
//------------------------------------------------------------------------------
const bool pgw_config::ParseInterface(
    const RAPIDJSON_NAMESPACE::Value& conf, interface_cfg_t& cfg) {
  if (conf.HasMember("interface_name")) {
    if (!conf["interface_name"].IsString()) {
      Logger::pgwc_app().error("Error parsing json value: [interface_name]");
      return false;
    }
    cfg.if_name = conf["interface_name"].GetString();
    util::trim(cfg.if_name);
    if (not boost::iequals(cfg.if_name, "none")) {
      std::string address = {};
      if (conf.HasMember("ipv4_address")) {
        if (!conf["ipv4_address"].IsString()) {
          Logger::pgwc_app().error("Error parsing json value: [ipv4_address]");
          return false;
        }
        address = conf["ipv4_address"].GetString();
        if (boost::iequals(address, "read")) {
          if (get_inet_addr_infos_from_iface(
                  cfg.if_name, cfg.addr4, cfg.network4, cfg.mtu)) {
            Logger::pgwc_app().error(
                "Could not read %s network interface configuration",
                cfg.if_name);
            return false;
          }
        } else {
          unsigned char buf_in_addr[sizeof(struct in6_addr)];
          memset(buf_in_addr, 0, sizeof(buf_in_addr));
          if (inet_pton(AF_INET, util::trim(address).c_str(), buf_in_addr) ==
              1) {
            memcpy(&cfg.addr4, buf_in_addr, sizeof(struct in_addr));
          } else {
            Logger::pgwc_app().error(
                "Could not read %s IP address configuration", address.c_str());
            return false;
          }
        }
      } else {
        if (get_inet_addr_infos_from_iface(
                cfg.if_name, cfg.addr4, cfg.network4, cfg.mtu)) {
          Logger::pgwc_app().error(
              "Could not read %s network interface configuration", cfg.if_name);
          return false;
        }
      }
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool pgw_config::ParseJson() {
  FILE* fp = fopen(jsoncfg_.c_str(), "r");
  if (fp == NULL) {
    std::cout << "The json config file specified does not exists" << std::endl;
    return false;
  }

  char readBuffer[kJsonFileBuffer];
  RAPIDJSON_NAMESPACE::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  RAPIDJSON_NAMESPACE::Document doc;
  doc.ParseStream(is);
  fclose(fp);

  if (!doc.IsObject()) {
    std::cout << "Error parsing the json config file" << std::endl;
    return false;
  }

  if (doc.HasMember("timer")) {
    const RAPIDJSON_NAMESPACE::Value& timer_section = doc["timer"];
    if (timer_section.HasMember("timer")) {
      if (!ParseTimer(doc["timer"], timer_)) {
        Logger::pgwc_app().error("Failed to parse json timer");
        return false;
      }
    }
  }
  if (doc.HasMember("gtpv2c")) {
    const RAPIDJSON_NAMESPACE::Value& gtpv2c_section = doc["gtpv2c"];
    if (gtpv2c_section.HasMember("port")) {
      if (!gtpv2c_section["port"].IsInt()) {
        Logger::pgwc_app().error("Error parsing json value: gtpv2c/port");
        return false;
      }
      gtpv2c_.port = gtpv2c_section["port"].GetUint();
    }
    if (gtpv2c_section.HasMember("n3")) {
      if (!gtpv2c_section["n3"].IsInt()) {
        Logger::pgwc_app().error("Error parsing json value: gtpv2c/n3");
        return false;
      }
      gtpv2c_.n3 = gtpv2c_section["n3"].GetUint();
    }
    if (gtpv2c_section.HasMember("t3_ms")) {
      if (!gtpv2c_section["t3_ms"].IsInt()) {
        Logger::pgwc_app().error("Error parsing json value: gtpv2c/t3_ms");
        return false;
      }
      gtpv2c_.t3_ms = gtpv2c_section["t3_ms"].GetUint();
    }
    if (gtpv2c_section.HasMember("worker_threads")) {
      if (!gtpv2c_section["worker_threads"].IsInt()) {
        Logger::pgwc_app().error(
            "Error parsing json value: gtpv2c/worker_threads");
        return false;
      }
      gtpv2c_.worker_threads = gtpv2c_section["worker_threads"].GetUint();
    }
    if (gtpv2c_section.HasMember("max_concurrent_procedures")) {
      if (!gtpv2c_section["max_concurrent_procedures"].IsInt()) {
        Logger::pgwc_app().error(
            "Error parsing json value: gtpv2c/max_concurrent_procedures");
        return false;
      }
      gtpv2c_.max_concurrent_procedures =
          gtpv2c_section["max_concurrent_procedures"].GetUint();
    }

    if (gtpv2c_section.HasMember("sched_params")) {
      const RAPIDJSON_NAMESPACE::Value& sched_section =
          gtpv2c_section["sched_params"];
      if (!ParseSchedParams(sched_section, gtpv2c_.sched_params)) {
        Logger::pgwc_app().error(
            "Error parsing json section: gtpv2c/sched_params");
        return false;
      }
    }
  }
  if (doc.HasMember("pfcp")) {
    const RAPIDJSON_NAMESPACE::Value& pfcp_section = doc["pfcp"];
    if (pfcp_section.HasMember("port")) {
      if (!pfcp_section["port"].IsInt()) {
        Logger::pgwc_app().error("Error parsing json value: pfcp/port");
        return false;
      }
      pfcp_.port = pfcp_section["port"].GetUint();
    }
    if (pfcp_section.HasMember("n1")) {
      if (!pfcp_section["n1"].IsInt()) {
        Logger::pgwc_app().error("Error parsing json value: pfcp/n1");
        return false;
      }
      pfcp_.n1 = pfcp_section["n1"].GetUint();
    }
    if (pfcp_section.HasMember("t1_ms")) {
      if (!pfcp_section["t1_ms"].IsInt()) {
        Logger::pgwc_app().error("Error parsing json value: pfcp/t1_ms");
        return false;
      }
      pfcp_.t1_ms = pfcp_section["t1_ms"].GetUint();
    }
    if (pfcp_section.HasMember("worker_threads")) {
      if (!pfcp_section["worker_threads"].IsInt()) {
        Logger::pgwc_app().error(
            "Error parsing json value: pfcp/worker_threads");
        return false;
      }
      pfcp_.worker_threads = pfcp_section["worker_threads"].GetUint();
    }
    if (pfcp_section.HasMember("max_concurrent_procedures")) {
      if (!pfcp_section["max_concurrent_procedures"].IsInt()) {
        Logger::pgwc_app().error(
            "Error parsing json value: pfcp/max_concurrent_procedures");
        return false;
      }
      pfcp_.max_concurrent_procedures =
          pfcp_section["max_concurrent_procedures"].GetUint();
    }
    if (pfcp_section.HasMember("sched_params")) {
      const RAPIDJSON_NAMESPACE::Value& sched_section =
          pfcp_section["sched_params"];
      if (!ParseSchedParams(sched_section, pfcp_.sched_params)) {
        Logger::pgwc_app().error(
            "Error parsing json section: pfcp/sched_params");
        return false;
      }
    }
  }
  if (doc.HasMember("s11")) {
    const RAPIDJSON_NAMESPACE::Value& iface_section = doc["s11"];
    if (!ParseInterface(iface_section, s11_.iface)) {
      Logger::pgwc_app().error("Error parsing json section: s11");
      return false;
    }
  }
  if (doc.HasMember("sgws5s8")) {
    const RAPIDJSON_NAMESPACE::Value& iface_section = doc["sgws5s8"];
    if (!ParseInterface(iface_section, sgw_s5s8_.iface)) {
      Logger::pgwc_app().error("Error parsing json section: sgws5s8");
      return false;
    }
  }
  if (doc.HasMember("pgws5s8")) {
    const RAPIDJSON_NAMESPACE::Value& iface_section = doc["pgws5s8"];
    if (!ParseInterface(iface_section, pgw_s5s8_.iface)) {
      Logger::pgwc_app().error("Error parsing json section: pgws5s8");
      return false;
    }
  }
  if (doc.HasMember("sx")) {
    const RAPIDJSON_NAMESPACE::Value& iface_section = doc["sx"];
    if (!ParseInterface(iface_section, sx_.iface)) {
      Logger::pgwc_app().error("Error parsing json section: sx");
      return false;
    }
  }
  if (doc.HasMember("spgw_app")) {
    const RAPIDJSON_NAMESPACE::Value& spgw_app_section = doc["spgw_app"];
    if (spgw_app_section.HasMember("worker_threads")) {
      if (!spgw_app_section["worker_threads"].IsInt()) {
        Logger::pgwc_app().error(
            "Error parsing json value: spgw_app/worker_threads");
        return false;
      }
    }
    if (spgw_app_section.HasMember("sched_params")) {
      const RAPIDJSON_NAMESPACE::Value& sched_section =
          spgw_app_section["sched_params"];
      if (!ParseSchedParams(sched_section, spgw_app_.sched_params)) {
        Logger::pgwc_app().error(
            "Error parsing json section: spgw_app/sched_params");
        return false;
      }
    }
    if (spgw_app_section.HasMember("default_dns_ipv4_address")) {
      if (!spgw_app_section["default_dns_ipv4_address"].IsString()) {
        Logger::pgwc_app().error(
            "Error parsing json value: spgw_app/default_dns_ipv4_address");
        return false;
      }
      std::string dns =
          spgw_app_section["default_dns_ipv4_address"].GetString();
      IPV4_STR_ADDR_TO_INADDR(
          util::trim(dns).c_str(), spgw_app_.default_dnsv4,
          "BAD IPv4 ADDRESS FORMAT FOR DEFAULT DNS !");
    }
    if (spgw_app_section.HasMember("default_dns_sec_ipv4_address")) {
      if (!spgw_app_section["default_dns_sec_ipv4_address"].IsString()) {
        Logger::pgwc_app().error(
            "Error parsing json value: spgw_app/default_dns_sec_ipv4_address");
        return false;
      }
      std::string dns =
          spgw_app_section["default_dns_sec_ipv4_address"].GetString();
      IPV4_STR_ADDR_TO_INADDR(
          util::trim(dns).c_str(), spgw_app_.default_dns_secv4,
          "BAD IPv4 ADDRESS FORMAT FOR DEFAULT SEC DNS !");
    }
    if (spgw_app_section.HasMember("default_dns_ipv6_address")) {
      if (!spgw_app_section["default_dns_ipv6_address"].IsString()) {
        Logger::pgwc_app().error(
            "Error parsing json value: spgw_app/default_dns_ipv6_address");
        return false;
      }
      unsigned char buf_in6_addr[sizeof(struct in6_addr)];
      std::string dns =
          spgw_app_section["default_dns_ipv6_address"].GetString();
      if (inet_pton(AF_INET6, util::trim(dns).c_str(), buf_in6_addr) == 1) {
        memcpy(&spgw_app_.default_dnsv6, buf_in6_addr, sizeof(struct in6_addr));
      } else {
        Logger::pgwc_app().error(
            "CONFIG : BAD ADDRESS in default_dns_ipv6_address %s", dns.c_str());
        return false;
      }
    }
    if (spgw_app_section.HasMember("default_dns_sec_ipv6_address")) {
      if (!spgw_app_section["default_dns_sec_ipv6_address"].IsString()) {
        Logger::pgwc_app().error(
            "Error parsing json value: spgw_app/default_dns_sec_ipv6_address");
        return false;
      }
      unsigned char buf_in6_addr[sizeof(struct in6_addr)];
      std::string dns =
          spgw_app_section["default_dns_sec_ipv6_address"].GetString();
      if (inet_pton(AF_INET6, util::trim(dns).c_str(), buf_in6_addr) == 1) {
        memcpy(
            &spgw_app_.default_dns_secv6, buf_in6_addr,
            sizeof(struct in6_addr));
      } else {
        Logger::pgwc_app().error(
            "CONFIG : BAD ADDRESS in default_dns_sec_ipv6_address %s",
            dns.c_str());
        return false;
      }
    }
    if (spgw_app_section.HasMember(
            "force_push_protocol_configuration_options")) {
      if (!spgw_app_section["force_push_protocol_configuration_options"]
               .IsBool()) {
        Logger::pgwc_app().error(
            "Error parsing json value: "
            "spgw_app/force_push_protocol_configuration_options");
        return false;
      }
      spgw_app_.force_push_pco =
          spgw_app_section["force_push_protocol_configuration_options"]
              .GetBool();
    }
    if (doc.HasMember("pdns")) {
      const RAPIDJSON_NAMESPACE::Value& pdns_section = doc["pdns"];
      if (!pdns_section.IsArray()) {
        Logger::pgwc_app().error("Error parsing json value: spgw_app/pdns");
        return false;
      }
      for (RAPIDJSON_NAMESPACE::SizeType i = 0; i < pdns_section.Size(); i++) {
        PdnCfg pdn_cfg = {};
        if (pdns_section[i].HasMember("apn_ni")) {
          if (!pdns_section[i]["apn_ni"].IsString()) {
            Logger::pgwc_app().error("Error parsing json value: pdns/[apn_ni]");
            return false;
          }
          pdn_cfg.apn = pdns_section[i]["apn_ni"].GetString();
          // std::vector<PdnCfg>::iterator it = std::find_if(
          //    spgw_app_.pdns.begin(), spgw_app_.pdns.end(),
          //    [pdn_cfg](const PdnCfg& p) {
          //      return p.apn.compare(pdn_cfg.apn) == 0;
          //    });
          // if (it != spgw_app_.pdns.end()) {
          //  pdn_cfg = *it;
          //  spgw_app_.pdns.erase(it);
          //}
          if (pdns_section[i].HasMember("dyn_ue_ipv4_range")) {
            if (!pdns_section[i]["dyn_ue_ipv4_range"].IsString()) {
              Logger::pgwc_app().error(
                  "Error parsing json value: pdns/[dyn_ue_ipv4_range]");
              return false;
            }
            unsigned char buf_in_addr[sizeof(struct in_addr)];
            std::vector<std::string> ips;
            std::string ipv4_range =
                pdns_section[i]["dyn_ue_ipv4_range"].GetString();
            struct in_addr in = {};
            boost::split(
                ips, ipv4_range, boost::is_any_of("-"),
                boost::token_compress_on);
            if (ips.size() != 2) {
              Logger::pgwc_app().error(
                  "Bad value %s : %s pdns/[dyn_ue_ipv4_range]",
                  ipv4_range.c_str());
              return false;
            }
            memset(buf_in_addr, 0, sizeof(buf_in_addr));
            if (inet_pton(
                    AF_INET, util::trim(ips.at(0)).c_str(), buf_in_addr) == 1) {
              memcpy(&in, buf_in_addr, sizeof(struct in_addr));
              // pdn_cfg.ue_pool_range_low.push_back(in);
              pdn_cfg.ue_pool_range_low = in;
            } else {
              Logger::pgwc_app().error(
                  "Config pool addr ipv4: bad lower address in "
                  "pdns/[dyn_ue_ipv4_range] %d nth item",
                  i);
              return false;
            }
            memset(buf_in_addr, 0, sizeof(buf_in_addr));
            if (inet_pton(
                    AF_INET, util::trim(ips.at(1)).c_str(), buf_in_addr) == 1) {
              memcpy(&in, buf_in_addr, sizeof(struct in_addr));
              // pdn_cfg.ue_pool_range_high.push_back(in);
              pdn_cfg.ue_pool_range_high = in;
            } else {
              Logger::pgwc_app().error(
                  "Config pool addr ipv4: bad high address in "
                  "pdns/[dyn_ue_ipv4_range] %d nth item",
                  i);
              return false;
            }
            if (htonl(pdn_cfg.ue_pool_range_low.s_addr) >=
                htonl(pdn_cfg.ue_pool_range_high.s_addr)) {
              Logger::pgwc_app().error(
                  "config pool addr ipv4: bad range in "
                  "pdns/[dyn_ue_ipv4_range] %d nth item",
                  i);
              return false;
            }
            if (pdn_cfg.pdn_type == PDN_TYPE_E_IPV6) {
              pdn_cfg.pdn_type = PDN_TYPE_E_IPV4V6;
            } else if (pdn_cfg.pdn_type.pdn_type != PDN_TYPE_E_IPV4V6) {
              pdn_cfg.pdn_type = PDN_TYPE_E_IPV4;
            }
          } else if (pdns_section[i].HasMember("ipv6_prefix")) {
            if (!pdns_section[i]["ipv6_prefix"].IsString()) {
              Logger::pgwc_app().error(
                  "Error parsing json value: pdns/[ipv6_prefix]");
              return false;
            }
            std::string ipv6_prefix =
                pdns_section[i]["ipv6_prefix"].GetString();
            std::vector<std::string> ips6;
            boost::split(
                ips6, ipv6_prefix, boost::is_any_of("/"),
                boost::token_compress_on);
            if (ips6.size() != 2) {
              Logger::pgwc_app().error(
                  "Bad value ipv6_prefix : %s in pdns/[ipv6_prefix] %d nth "
                  "item",
                  ipv6_prefix.c_str(), i);
              return false;
            }
            unsigned char buf_in6_addr[sizeof(struct in6_addr)];
            struct in6_addr in6 = {};
            std::string addr    = ips6.at(0);
            util::trim(addr);
            if (inet_pton(AF_INET6, addr.c_str(), buf_in6_addr) == 1) {
              memcpy(&in6, buf_in6_addr, sizeof(struct in6_addr));
              // pdn_cfg.paa_pool6_prefix.push_back(in6);
              pdn_cfg.paa_pool6_prefix = in6;
            } else {
              Logger::pgwc_app().error(
                  "config pool addr ipv4: bad address in pdns/[ipv6_prefix]"
                  "  %d nth item",
                  i);
              return false;
            }
            std::string prefix = ips6.at(1);
            util::trim(prefix);
            // pdn_cfg.paa_pool6_prefix_len.push_back(std::stoi(prefix));
            pdn_cfg.paa_pool6_prefix_len = std::stoi(prefix);
            if (pdn_cfg.pdn_type == PDN_TYPE_E_IPV4) {
              pdn_cfg.pdn_type = PDN_TYPE_E_IPV4V6;
            } else if (pdn_cfg.pdn_type.pdn_type != PDN_TYPE_E_IPV4V6) {
              pdn_cfg.pdn_type = PDN_TYPE_E_IPV6;
            }
          } else {
            pdn_cfg.pdn_type = PDN_TYPE_E_NON_IP;
          }
          spgw_app_.pdns.push_back(pdn_cfg);
        }
      }
    }
  }
  if (doc.HasMember("cups")) {
    const RAPIDJSON_NAMESPACE::Value& cups_section = doc["cups"];
    if (cups_section.HasMember("association_retry_period_ms")) {
      if (!cups_section["association_retry_period_ms"].IsInt()) {
        Logger::pgwc_app().error(
            "Error parsing json value: "
            "cups/association_retry_period_ms");
        return false;
      }
      cups_.association_retry_period_ms =
          cups_section["association_retry_period_ms"].GetInt();
    }
    if (cups_section.HasMember("association_heartbeat_period_ms")) {
      if (!cups_section["association_heartbeat_period_ms"].IsInt()) {
        Logger::pgwc_app().error(
            "Error parsing json value: cups/association_heartbeat_period_ms");
        return false;
      }
      cups_.association_heartbeat_period_ms =
          cups_section["association_heartbeat_period_ms"].GetInt();
    }
    if (cups_section.HasMember("feature_overload_control")) {
      if (!cups_section["feature_overload_control"].IsBool()) {
        Logger::pgwc_app().error(
            "Error parsing json value: cups/feature_overload_control");
        return false;
      }
      cups_.feature_overload_control =
          cups_section["feature_overload_control"].GetBool();
    }
    if (cups_section.HasMember("feature_load_control")) {
      if (!cups_section["feature_load_control"].IsBool()) {
        Logger::pgwc_app().error(
            "Error parsing json value: cups/feature_load_control");
        return false;
      }
      cups_.feature_load_control =
          cups_section["feature_load_control"].GetBool();
    }
    if (cups_section.HasMember("up_nodes_selection")) {
      const RAPIDJSON_NAMESPACE::Value& nodes_section =
          cups_section["up_nodes_selection"];
      if (!nodes_section.IsArray()) {
        Logger::pgwc_app().error(
            "Error parsing json value: up_nodes_selection");
        return false;
      }
      for (RAPIDJSON_NAMESPACE::SizeType i = 0; i < nodes_section.Size(); i++) {
        if (nodes_section[i].HasMember("mcc")) {
          if (!nodes_section[i]["mcc"].IsString()) {
            Logger::pgwc_app().error(
                "Error parsing json value: up_nodes_selection/[mcc]");
            return false;
          }
          if (nodes_section[i].HasMember("mnc")) {
            if (!nodes_section[i]["mnc"].IsString()) {
              Logger::pgwc_app().error(
                  "Error parsing json value: cup_nodes_selection/[mnc]");
              return false;
            }
          }
          if (nodes_section[i].HasMember("tac")) {
            if (!nodes_section[i]["tac"].IsInt()) {
              Logger::pgwc_app().error(
                  "Error parsing json value: cup_nodes_selection/[tac]");
              return false;
            }
          }
          if (nodes_section[i].HasMember("pdn_idx")) {
            if (!nodes_section[i]["pdn_idx"].IsInt()) {
              Logger::pgwc_app().error(
                  "Error parsing json value: cup_nodes_selection/[pdn_idx]");
              return false;
            }
          }
          if (nodes_section[i].HasMember("id")) {
            if (!nodes_section[i]["id"].IsString()) {
              Logger::pgwc_app().error(
                  "Error parsing json value: cup_nodes_selection/[id]");
              return false;
            }
            /*unsigned char buf_in_addr[sizeof(struct in6_addr)];
            std::string up_ip_addr = nodes_section[i]["id"].GetString();
            memset(buf_in_addr, 0, sizeof(buf_in_addr));
            pfcp::node_id_t node = {};
            if (inet_pton(AF_INET, up_ip_addr.c_str(), buf_in_addr) == 1) {
              memcpy(
                  &node.u1.ipv4_address, buf_in_addr, sizeof(struct in_addr));

            } else {
              Logger::pgwc_app().error(
                  "Config : bad SPGW-U IP address  "
                  "cups_up_nodes/[ip_address] %d nth item",
                  i);
              return false;
            }*/
            up_node_cfg_t up_node = {};
            up_node.mcc           = nodes_section[i]["mcc"].GetString();
            up_node.mnc           = nodes_section[i]["mnc"].GetString();
            up_node.tac           = nodes_section[i]["tac"].GetInt();
            up_node.pdn_index     = nodes_section[i]["pdn_idx"].GetUint();
            up_node.id            = nodes_section[i]["id"].GetString();
            up_node.tai.from_items(up_node.mcc, up_node.mnc, up_node.tac);
            cups_.nodes.push_back(up_node);
          }
        }
      }
    }
  }
  return Finalize();
}

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "UNKNOWN PACKAGE NAME"
#endif
#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "UNKNOWN PACKAGE VERSION"
#endif
//------------------------------------------------------------------------------
void pgw_config::Display() {
  Logger::pgwc_app().info(
      "==== EURECOM %s v%s ====", PACKAGE_NAME, PACKAGE_VERSION);
  Logger::pgwc_app().info("Configuration SPGW-C:");
  Logger::pgwc_app().info("- S11-C Networking:");
  Logger::pgwc_app().info(
      "    iface ............: %s", s11_.iface.if_name.c_str());
  Logger::pgwc_app().info(
      "    ipv4.addr ........: %s", inet_ntoa(s11_.iface.addr4));
  Logger::pgwc_app().info(
      "    ipv4.mask ........: %s", inet_ntoa(s11_.iface.network4));
  Logger::pgwc_app().info("- SGW S5S8-C Networking:");
  Logger::pgwc_app().info(
      "    iface ............: %s", sgw_s5s8_.iface.if_name.c_str());
  Logger::pgwc_app().info(
      "    ipv4.addr ........: %s", inet_ntoa(sgw_s5s8_.iface.addr4));
  Logger::pgwc_app().info(
      "    ipv4.mask ........: %s", inet_ntoa(sgw_s5s8_.iface.network4));
  Logger::pgwc_app().info("- PGW S5S8-C Networking:");
  Logger::pgwc_app().info(
      "    iface ............: %s", pgw_s5s8_.iface.if_name.c_str());
  Logger::pgwc_app().info(
      "    ipv4.addr ........: %s", inet_ntoa(pgw_s5s8_.iface.addr4));
  Logger::pgwc_app().info(
      "    ipv4.mask ........: %s", inet_ntoa(pgw_s5s8_.iface.network4));
  Logger::pgwc_app().info("- SXab Networking:");
  Logger::pgwc_app().info(
      "    iface ............: %s", sx_.iface.if_name.c_str());
  Logger::pgwc_app().info(
      "    ipv4.addr ........: %s", inet_ntoa(sx_.iface.addr4));
  Logger::pgwc_app().info(
      "    ipv4.mask ........: %s", inet_ntoa(sx_.iface.network4));
  Logger::pgwc_app().info("- GTPv2-C:");
  Logger::pgwc_app().info("    port .............: %u", gtpv2c_.port);
  Logger::pgwc_app().info("    N3 ...............: %u", gtpv2c_.n3);
  Logger::pgwc_app().info("    T3 ...............: %u ms", gtpv2c_.t3_ms);
  Logger::pgwc_app().info("    workers ..........: %u", gtpv2c_.worker_threads);
  Logger::pgwc_app().info(
      "    max procedures ...: %u", gtpv2c_.max_concurrent_procedures);
  Logger::pgwc_app().info("    Threading:");
  Logger::pgwc_app().info(
      "        CPU id .......: %d", gtpv2c_.sched_params.cpu_id);
  Logger::pgwc_app().info(
      "        Sched policy .: %d", gtpv2c_.sched_params.sched_policy);
  Logger::pgwc_app().info(
      "        Sched prio ...: %d", gtpv2c_.sched_params.sched_priority);
  Logger::pgwc_app().info("- PFCP:");
  Logger::pgwc_app().info("    port .............: %u", pfcp_.port);
  Logger::pgwc_app().info("    N1 ...............: %u", pfcp_.n1);
  Logger::pgwc_app().info("    T1 ...............: %u ms", pfcp_.t1_ms);
  Logger::pgwc_app().info("    workers ..........: %u", pfcp_.worker_threads);
  Logger::pgwc_app().info(
      "    max procedures ...: %u", pfcp_.max_concurrent_procedures);
  Logger::pgwc_app().info("    Threading:");
  Logger::pgwc_app().info(
      "        CPU id .......: %d", pfcp_.sched_params.cpu_id);
  Logger::pgwc_app().info(
      "        Sched policy .: %d", pfcp_.sched_params.sched_policy);
  Logger::pgwc_app().info(
      "        Sched prio ...: %d", pfcp_.sched_params.sched_priority);
  Logger::pgwc_app().info("- Timers :");
  Logger::pgwc_app().info("    ITTI implementation:");
  Logger::pgwc_app().info(
      "        CPU id .......: %d", timer_.sched_params.cpu_id);
  Logger::pgwc_app().info(
      "        Sched policy .: %d", timer_.sched_params.sched_policy);
  Logger::pgwc_app().info(
      "        Sched prio ...: %d", timer_.sched_params.sched_priority);
  Logger::pgwc_app().info("- SPGW-C APP :");
  Logger::pgwc_app().info("    Threading:");
  Logger::pgwc_app().info(
      "        CPU id .......: %d", spgw_app_.sched_params.cpu_id);
  Logger::pgwc_app().info(
      "        Sched policy .: %d", spgw_app_.sched_params.sched_policy);
  Logger::pgwc_app().info(
      "        Sched prio ...: %d", spgw_app_.sched_params.sched_priority);
  Logger::pgwc_app().info("- PDNs :");
  Logger::pgwc_app().info("    - DEFAULT DNS:");
  Logger::pgwc_app().info(
      "        Primary DNS ..: %s",
      inet_ntoa(*((struct in_addr*) &spgw_app_.default_dnsv4)));
  Logger::pgwc_app().info(
      "        Secondary DNS : %s",
      inet_ntoa(*((struct in_addr*) &spgw_app_.default_dns_secv4)));
  char str_addr6[INET6_ADDRSTRLEN];
  if (inet_ntop(
          AF_INET6, &spgw_app_.default_dnsv6, str_addr6, sizeof(str_addr6))) {
    Logger::pgwc_app().info("        Primary DNS v6: %s", str_addr6);
  }
  if (inet_ntop(
          AF_INET6, &spgw_app_.default_dns_secv6, str_addr6,
          sizeof(str_addr6))) {
    Logger::pgwc_app().info("        Second. DNS v6: %s", str_addr6);
  }
  uint index = 0;
  Logger::pgwc_app().info("    - PDNs :");
  for (const PdnCfg& pdn : spgw_app_.pdns) {
    std::string range_low(inet_ntoa(pdn.ue_pool_range_low));
    std::string range_high(inet_ntoa(pdn.ue_pool_range_high));
    std::string network(inet_ntoa(pdn.ue_pool_network));
    std::string netmask(inet_ntoa(pdn.ue_pool_netmask));
    inet_ntop(AF_INET6, &pdn.paa_pool6_prefix, str_addr6, sizeof(str_addr6));
    Logger::pgwc_app().info(
        "    - PDN %02u.: apn ni %s label %s "
        " IPv4 pool: %s - %s <=> %s/%s"
        "  IPv6 pool: %s / %u",
        index++, pdn.apn.c_str(), pdn.apn_label.c_str(), range_low.c_str(),
        range_high.c_str(), network.c_str(), netmask.c_str(), str_addr6,
        pdn.paa_pool6_prefix_len);
  }
  //  Logger::pgwc_app().info("- PCEF support (in development)");
  //  Logger::pgwc_app().info("    APN AMBR UL ..........: %lu  (Kilo bits/s)",
  //                          pcef.apn_ambr_ul);
  //  Logger::pgwc_app().info("    APN AMBR DL ..........: %lu  (Kilo bits/s)",
  //                          pcef.apn_ambr_dl);
  Logger::pgwc_app().info("- CUPS:");
  Logger::pgwc_app().info(
      "    Node association retry : %u ms", cups_.association_retry_period_ms);
  Logger::pgwc_app().info(
      "    Echo Node period: %u ms", cups_.association_heartbeat_period_ms);
  Logger::pgwc_app().info("    User Plane Nodes, network planning:");
  for (auto it : cups_.nodes) {
    Logger::pgwc_app().info("        %s", it.toString().c_str());
  }
  Logger::pgwc_app().info("- Helpers:");
  Logger::pgwc_app().info(
      "    Push PCO (DNS+MTU) ........: %s",
      spgw_app_.force_push_pco == 0 ? "false" : "true");
}

//------------------------------------------------------------------------------
bool pgw_config::IsDottedApnHandled(
    const std::string& t_apn, const pdn_type_t& pdn_type) {
  for (auto p : spgw_app_.pdns) {
    Logger::pgwc_app().info(
        "pgw_config::IsDottedApnHandled: %s / %s", t_apn.c_str(),
        p.apn.c_str());
    if (0 == t_apn.compare(p.apn)) {
      // TODO refine
      if (pdn_type.pdn_type == p.pdn_type.pdn_type) {
        return true;
      }
    }
  }
  return false;
}

//------------------------------------------------------------------------------
int pgw_config::GetPfcpNodeId(pfcp::node_id_t& node_id) {
  node_id = {};
  if (sx_.iface.addr4.s_addr) {
    node_id.node_id_type    = pfcp::NODE_ID_TYPE_IPV4_ADDRESS;
    node_id.u1.ipv4_address = sx_.iface.addr4;
    return RETURNok;
  }
  if (sx_.iface.addr6.s6_addr32[0] | sx_.iface.addr6.s6_addr32[1] |
      sx_.iface.addr6.s6_addr32[2] | sx_.iface.addr6.s6_addr32[3]) {
    node_id.node_id_type    = pfcp::NODE_ID_TYPE_IPV6_ADDRESS;
    node_id.u1.ipv6_address = sx_.iface.addr6;
    return RETURNok;
  }
  return RETURNerror;
}
//------------------------------------------------------------------------------
int pgw_config::GetPfcpFseid(pfcp::fseid_t& fseid) {
  int rc = RETURNerror;
  fseid  = {};
  if (sx_.iface.addr4.s_addr) {
    fseid.v4           = 1;
    fseid.ipv4_address = sx_.iface.addr4;
    rc                 = RETURNok;
  }
  if (sx_.iface.addr6.s6_addr32[0] | sx_.iface.addr6.s6_addr32[1] |
      sx_.iface.addr6.s6_addr32[2] | sx_.iface.addr6.s6_addr32[3]) {
    fseid.v6           = 1;
    fseid.ipv6_address = sx_.iface.addr6;
    rc                 = RETURNok;
  }
  return rc;
}
//------------------------------------------------------------------------------
bool pgw_config::GetUpNodes(
    const apn_t& apn, const uli_t& uli, const paa_t& paa,
    std::vector<up_node_cfg_t>& up_nodes) {
  for (auto n = cups_.nodes.begin(); n != cups_.nodes.end(); ++n) {
    PdnCfg& pdncfg = pgw_config::spgw_app_.GetPdnCfg(n->pdn_index);
    if (paa.is_ip_assigned()) {
      if (not pdncfg.is_in_pool(paa)) {
        continue;
      }
    }
    if ((apn.access_point_name.compare(pdncfg.apn) == 0) &&
        (uli.is_tai(n->tai))) {
      up_nodes.push_back(*n);
      // std::cout << "up_nodes.push_back( " << n->id << " )" << std::endl;
    }
  }

  /*auto it = std::copy_if (cups_.nodes.begin(),
      cups_.nodes.end(),
      up_nodes.begin(),
      [apn, uli, paa](up_node_cfg_t& n) {
        PdnCfg& pdncfg = pgw_config::spgw_app_.GetPdnCfg(n.pdn_index);
        std::cout << "Eval PdnCfg id " << n.id << " pool " << n.pdn_index <<
  std::endl; if (paa.is_ip_assigned()) { if (not pdncfg.is_in_pool(paa)) {
            return false;
          }
        }
        std::cout << "apn.access_point_name " << apn.access_point_name << " /
  pdncfg.apn " << pdncfg.apn << std::endl; std::cout << "uli " << uli.toString()
  << " / pdncfg.tai " << n.tai.toString() << std::endl; return
  ((apn.access_point_name.compare(pdncfg.apn) == 0) && (uli.is_tai(n.tai))); }
  );
  // shrink container to new size
  up_nodes.resize(std::distance(up_nodes.begin(),it)); */
  return (up_nodes.size() > 0);
}
