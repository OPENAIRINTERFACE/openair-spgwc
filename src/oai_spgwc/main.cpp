/*
 * Copyright (c) 2017 Sprint
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "async_shell_cmd.hpp"
#include "common_defs.h"
#include "itti.hpp"
#include "logger.hpp"
#include "options.hpp"
#include "pgw_app.hpp"
#include "pgw_config.hpp"
#include "pid_file.hpp"
#include "rest_handler.h"
#include "sgwc_app.hpp"

#include <csignal>
#include <stdint.h>
#include <stdlib.h>  // srand
#include <unistd.h>  // get_pid(), pause()
#include <iostream>
#include <thread>

using namespace gtpv2c;
using namespace pgwc;
using namespace sgwc;
using namespace util;
using namespace std;

itti_mw* itti_inst                      = nullptr;
async_shell_cmd* async_shell_cmd_inst   = nullptr;
pgw_app* pgw_app_inst                   = nullptr;
sgwc_app* sgwc_app_inst                 = nullptr;
Pistache::Http::Endpoint* rest_endpoint = nullptr;

void send_heartbeat_to_tasks(const uint32_t sequence);

//------------------------------------------------------------------------------
void send_heartbeat_to_tasks(const uint32_t sequence) {
  itti_msg_ping* itti_msg =
      new itti_msg_ping(TASK_SGWC_APP, TASK_ALL, sequence);
  std::shared_ptr<itti_msg_ping> i = std::shared_ptr<itti_msg_ping>(itti_msg);
  int ret                          = itti_inst->send_broadcast_msg(i);
  if (RETURNok != ret) {
    Logger::sgwc_app().error(
        "Could not send ITTI message %s to task TASK_ALL", i->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void term_signal_handler(int signum) {
  std::cout << "Caught signal " << signum << std::endl;
  Logger::system().startup("exiting");
  if (itti_inst) {
    itti_inst->send_terminate_msg(TASK_SGWC_APP);
    itti_inst->wait_tasks_end();
  }
  std::cout << "Freeing Allocated memory..." << std::endl;
  if (async_shell_cmd_inst) {
    delete async_shell_cmd_inst;
    async_shell_cmd_inst = nullptr;
    std::cout << "Async Shell CMD memory done." << std::endl;
  }
  if (sgwc_app_inst) {
    delete sgwc_app_inst;
    sgwc_app_inst = nullptr;
    std::cout << "SGW APP memory done." << std::endl;
  }
  if (pgw_app_inst) {
    delete pgw_app_inst;
    pgw_app_inst = nullptr;
    std::cout << "PGW APP memory done." << std::endl;
  }
  // exit(signum);
  if (itti_inst) {
    delete itti_inst;
    itti_inst = nullptr;
    std::cout << "ITTI memory done." << std::endl;
  }
  std::cout << "Freeing Allocated memory done" << std::endl;
  exit(signum);
}
//------------------------------------------------------------------------------
int main(int argc, char** argv) {
  srand(time(NULL));

  pgw_config::Default();

  try {
    // Command line options
    if (!Options::parse(argc, argv)) {
      std::cout << "Options::parse() failed" << std::endl;
      return 1;
    }

    // Logger
    Logger::init("spgwc", Options::getlogStdout(), Options::getlogRotFilelog());

    Logger::sgwc_app().startup("Options parsed");

    // Config
    pgw_config::jsoncfg_ = Options::getConfig();
    if (!pgw_config::ParseJson()) {
      std::cout << "pgw_config::ParseJson() failed" << std::endl;
      return 1;
    }
    pgw_config::Display();

    // Inter task Interface
    itti_inst = new itti_mw();
    itti_inst->start(pgwc::pgw_config::timer_.sched_params);

    // system command
    async_shell_cmd_inst =
        new async_shell_cmd(pgwc::pgw_config::spgw_app_.sched_params);

    // PGW application layer
    pgw_app_inst = new pgw_app(Options::getConfig());

    // PID file
    // Currently hard-coded value. TODO: add as config option.
    string pid_file_name =
        get_exe_absolute_path("/var/run", pgwc::pgw_config::instance_);
    if (!is_pid_file_lock_success(pid_file_name.c_str())) {
      Logger::pgwc_app().error(
          "Lock PID file %s failed\n", pid_file_name.c_str());
      exit(-EDEADLK);
    }

    // SGW application layer
    sgwc_app_inst = new sgwc_app(Options::getConfig());

    try {
      Pistache::Address addr(
          Pistache::Ipv4::any(), Pistache::Port(pgwc::pgw_config::rest_port_));
      auto opts = Pistache::Http::Endpoint::options().threads(1).flags(
          Pistache::Tcp::Options::ReuseAddr);
      //      .flags( Pistache::Tcp::Options::InstallSignalHandler |
      //      Pistache::Tcp::Options::ReuseAddr );

      rest_endpoint = new Pistache::Http::Endpoint(addr);
      rest_endpoint->init(opts);
      rest_endpoint->setHandler(Pistache::Http::make_handler<RestHandler>());
      rest_endpoint->serveThreaded();

      Logger::system().startup(
          "Started REST server on port [%i]", pgwc::pgw_config::rest_port_);
    } catch (std::runtime_error& e) {
      std::cout << "Exception starting REST server - " << e.what() << std::endl;
      return 1;
    }

    {
      FILE* fp             = NULL;
      std::string filename = fmt::format("/tmp/spgwc_{}.status", getpid());
      fp                   = fopen(filename.c_str(), "w+");
      fprintf(fp, "STARTED\n");
      fflush(fp);
      fclose(fp);
    }

    signal(SIGINT, term_signal_handler);
    signal(SIGTERM, term_signal_handler);
    // Debugging with gdb: testing SIGINT, SIGTERM : gdb will catch those
    // signals
    // We can ask gdb not to stop on SIGUSR1:
    // handle SIGUSR1 pass
    // kill -USR1 pid
    signal(SIGUSR1, term_signal_handler);

    pause();
  } catch (std::exception& e) {
    std::cerr << "Caught " << e.what() << std::endl;
    std::cerr << "Emulating SIGTERM" << std::endl;
    term_signal_handler(SIGTERM);
  }
  return 0;
}
