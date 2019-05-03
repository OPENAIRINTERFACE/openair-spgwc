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

/*! \file sSGW_CONFIG.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "async_shell_cmd.hpp"
#include "common_defs.h"
#include "if.hpp"
#include "logger.hpp"
#include "sgwc_config.hpp"
#include "string.hpp"


#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace libconfig;
using namespace sgwc;

// C includes
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//------------------------------------------------------------------------------
int sgwc_config::execute ()
{
  return RETURNok;
}
//------------------------------------------------------------------------------
int sgwc_config::load_thread_sched_params(const Setting& thread_sched_params_cfg, util::thread_sched_params& cfg)
{

  thread_sched_params_cfg.lookupValue(SGWC_CONFIG_STRING_THREAD_RD_CPU_ID, cfg.cpu_id);
  std::string thread_rd_sched_policy;
  thread_sched_params_cfg.lookupValue(SGWC_CONFIG_STRING_THREAD_RD_SCHED_POLICY, thread_rd_sched_policy);
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
    Logger::spgwu_app().error("thread_rd_sched_policy: %s, unknown in config file", thread_rd_sched_policy.c_str());
    return RETURNerror;
  }
  thread_sched_params_cfg.lookupValue(SGWC_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY, cfg.sched_priority);
  if ((cfg.sched_priority > 99) || (cfg.sched_priority < 1)) {
    Logger::spgwu_app().error("thread_rd_sched_priority: %d, must be in interval [1..99] in config file", cfg.sched_priority);
    return RETURNerror;
  }
  return RETURNok;
}
//------------------------------------------------------------------------------
int sgwc_config::load_itti(const Setting& itti_cfg, itti_cfg_t& cfg)
{
  const Setting& sched_params_cfg = itti_cfg[SGWC_CONFIG_STRING_ITTI_TIMER_SCHED_PARAMS];
  load_thread_sched_params(sched_params_cfg, cfg.itti_timer_sched_params);

  const Setting& s11_sched_params_cfg = itti_cfg[SGWC_CONFIG_STRING_S11_SCHED_PARAMS];
  load_thread_sched_params(s11_sched_params_cfg, cfg.s11_sched_params);

  const Setting& s5s8_sched_params_cfg = itti_cfg[SGWC_CONFIG_STRING_S5S8_SCHED_PARAMS];
  load_thread_sched_params(s5s8_sched_params_cfg, cfg.s5s8_sched_params);

  const Setting& sx_sched_params_cfg = itti_cfg[SGWC_CONFIG_STRING_SX_SCHED_PARAMS];
  load_thread_sched_params(sx_sched_params_cfg, cfg.sx_sched_params);

  const Setting& sgw_app_sched_params_cfg = itti_cfg[SGWC_CONFIG_STRING_SGW_APP_SCHED_PARAMS];
  load_thread_sched_params(sgw_app_sched_params_cfg, cfg.sgw_app_sched_params);

  const Setting& async_cmd_sched_params_cfg = itti_cfg[SGWC_CONFIG_STRING_ASYNC_CMD_SCHED_PARAMS];
  load_thread_sched_params(async_cmd_sched_params_cfg, cfg.async_cmd_sched_params);

  return RETURNok;
}
//------------------------------------------------------------------------------
int sgwc_config::load_interface(const Setting& if_cfg, interface_cfg_t& cfg)
{
  if_cfg.lookupValue(SGWC_CONFIG_STRING_INTERFACE_NAME, cfg.if_name);
  util::trim(cfg.if_name);
  if (not boost::iequals(cfg.if_name, "none")) {
    std::string address = {};
    if_cfg.lookupValue(SGWC_CONFIG_STRING_IPV4_ADDRESS, address);
    util::trim(address);
    if (boost::iequals(address, "read")) {
      if (get_inet_addr_infos_from_iface(cfg.if_name, cfg.addr4, cfg.network4, cfg.mtu)) {
        Logger::sgwc_app().error("Could not read %s network interface configuration", cfg.if_name);
        return RETURNerror;
      }
    } else {
      std::vector<std::string> words;
      boost::split(words, address, boost::is_any_of("/"), boost::token_compress_on);
      if (words.size() != 2) {
        Logger::sgwc_app().error("Bad value " SGWC_CONFIG_STRING_IPV4_ADDRESS " = %s in config file", address.c_str());
        return RETURNerror;
      }
      unsigned char buf_in_addr[sizeof(struct in6_addr)]; // you never know...
      if (inet_pton (AF_INET, util::trim(words.at(0)).c_str(), buf_in_addr) == 1) {
        memcpy (&cfg.addr4, buf_in_addr, sizeof (struct in_addr));
      } else {
        Logger::sgwc_app().error("In conversion: Bad value " SGWC_CONFIG_STRING_IPV4_ADDRESS " = %s in config file", util::trim(words.at(0)).c_str());
        return RETURNerror;
      }
      cfg.network4.s_addr = htons(ntohs(cfg.addr4.s_addr) & 0xFFFFFFFF << (32 - std::stoi (util::trim(words.at(1)))));
    }
    if_cfg.lookupValue(SGWC_CONFIG_STRING_PORT, cfg.port);
    const Setting& sched_params_cfg = if_cfg[SGWC_CONFIG_STRING_SCHED_PARAMS];
    load_thread_sched_params(sched_params_cfg, cfg.thread_rd_sched_params);
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int sgwc_config::load_mosaic_5g(const Setting& lib_cfg, mosaic_5g_cfg_t& cfg)
{
  cfg.enabled = false;
  std::string astring = {};
  if (lib_cfg.lookupValue(SGWC_CONFIG_STRING_REMOTE_CONTROLLER_ENABLED, astring)) {
    if (boost::iequals(astring, "yes")) {
      cfg.enabled = true;
    }
  }  

  std::string address = {};
  lib_cfg.lookupValue(SGWC_CONFIG_STRING_REMOTE_CONTROLLER_IPV4_ADDRESS, address);
  util::trim(address);
  unsigned char buf_in_addr[sizeof(struct in6_addr)]; 
  if (inet_pton (AF_INET, address.c_str(), buf_in_addr) == 1) {
    memcpy (&cfg.remote_controller, buf_in_addr, sizeof (struct in_addr));
  } else {
    Logger::sgwc_app().error("In conversion: Bad value " SGWC_CONFIG_STRING_REMOTE_CONTROLLER_IPV4_ADDRESS " = %s in config file", address.c_str());
    return RETURNerror;
  }

  lib_cfg.lookupValue(SGWC_CONFIG_STRING_REMOTE_CONTROLLER_PORT, cfg.remote_controller_port);
  return RETURNok;
}

//------------------------------------------------------------------------------
int sgwc_config::load(const string& config_file)
{
  Config                 cfg;

  // Read the file. If there is an error, report it and exit.
  try
  {
    cfg.readFile(config_file.c_str());
  }
  catch(const FileIOException &fioex)
  {
    Logger::sgwc_app().error("I/O error while reading file %s - %s", config_file.c_str(), fioex.what());
    throw;
  }
  catch(const ParseException &pex)
  {
    Logger::sgwc_app().error("Parse error at %s:%d - %s", pex.getFile(), pex.getLine(), pex.getError());
    throw;
  }

  const Setting& root = cfg.getRoot();

  try
  {
    const Setting& sgw_cfg = root[SGWC_CONFIG_STRING_SGW_CONFIG];

    sgw_cfg.lookupValue(SGWC_CONFIG_STRING_INSTANCE, instance);
    sgw_cfg.lookupValue(SGWC_CONFIG_STRING_PID_DIRECTORY, pid_dir);
    util::trim(pid_dir);

    const Setting& nw_if_cfg = sgw_cfg[SGWC_CONFIG_STRING_INTERFACES];

    const Setting& s11_cp_cfg = nw_if_cfg[SGWC_CONFIG_STRING_INTERFACE_S11_CP];
    load_interface(s11_cp_cfg, s11_cp);

    const Setting& s5s8_cp_cfg = nw_if_cfg[SGWC_CONFIG_STRING_INTERFACE_S5_S8_CP];
    load_interface(s5s8_cp_cfg, s5s8_cp);
    
  }
  catch(const SettingNotFoundException &nfex)
  {
    Logger::sgwc_app().error("%s : %s", nfex.what(), nfex.getPath());
    return RETURNerror;
  }
  
  try
  {
    const Setting& sgw_cfg = root[SGWC_CONFIG_STRING_SGW_CONFIG];
    const Setting& mosaic_cfg = sgw_cfg[SGWC_CONFIG_STRING_MOSAIC_5G];
    load_mosaic_5g(mosaic_cfg, mosaic_5g);
  }
  catch(const SettingNotFoundException &nfex)
  {
    Logger::sgwc_app().trace("%s : %s", nfex.what(), nfex.getPath());
  }    
  
  return RETURNok;
}

//------------------------------------------------------------------------------
void sgwc_config::display ()
{

  Logger::sgwc_app().info("==== EURECOM %s v%s ====", PACKAGE_NAME, PACKAGE_VERSION);
  Logger::sgwc_app().info( "Configuration SGW-C:");
  //Logger::sgwc_app().info( "- Instance ..............: %s", instance);
  //Logger::sgwc_app().info( "- PID dir ...............: %s", pid_dir.c_str());
  Logger::sgwc_app().info( "- S5_S8-C:");
  Logger::sgwc_app().info( "    iface ............: %s", s5s8_cp.if_name.c_str());
  Logger::sgwc_app().info( "    ipv4.addr ........: %s", inet_ntoa (s5s8_cp.addr4));
  Logger::sgwc_app().info( "    ipv4.mask ........: %s", inet_ntoa (s5s8_cp.network4));
  Logger::sgwc_app().info( "    port .............: %d", s5s8_cp.port);
  Logger::sgwc_app().info( "- S11-C:");
  Logger::sgwc_app().info( "    iface ............: %s", s11_cp.if_name.c_str());
  Logger::sgwc_app().info( "    ipv4.addr ........: %s", inet_ntoa (s11_cp.addr4));
  Logger::sgwc_app().info( "    ipv4.mask ........: %s", inet_ntoa (s11_cp.network4));
  Logger::sgwc_app().info( "    port .............: %u", s11_cp.port);
  if (mosaic_5g.enabled) {
    Logger::sgwc_app().info( "- MOSAIC-5G:");
    Logger::sgwc_app().info( "    Remote controller address : %s", inet_ntoa (mosaic_5g.remote_controller));
    Logger::sgwc_app().info( "    Remote controller port ...: %d", mosaic_5g.remote_controller_port);
  }
}

