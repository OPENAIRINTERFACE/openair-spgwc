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

#ifndef __LOGGER_H
#define __LOGGER_H

#include <cstdarg>
#include <stdexcept>
#include <vector>

//#define SPDLOG_LEVEL_NAMES { "trace", "debug", "info",  "warning", "error",
//"critical", "off" };
#ifndef SPDLOG_LEVEL_NAMES
#define SPDLOG_LEVEL_NAMES \
  {"trace", "debug", "info ", "start", "warn ", "error", "off  "};
#endif

#define SPDLOG_ENABLE_SYSLOG
#include "spdlog/spdlog.h"

class LoggerException : public std::runtime_error {
 public:
  explicit LoggerException(const char *m) : std::runtime_error(m) {}
  explicit LoggerException(const std::string &m) : std::runtime_error(m) {}
};

class _Logger {
 public:
  _Logger(const char *category, std::vector<spdlog::sink_ptr> &sinks,
          const char *pattern);

  void Trace(const char *format, ...);
  void Trace(const std::string &format, ...);
  void Debug(const char *format, ...);
  void Debug(const std::string &format, ...);
  void Info(const char *format, ...);
  void Info(const std::string &format, ...);
  void Startup(const char *format, ...);
  void Startup(const std::string &format, ...);
  void Warn(const char *format, ...);
  void Warn(const std::string &format, ...);
  void Error(const char *format, ...);
  void Error(const std::string &format, ...);

 private:
  _Logger();

  enum _LogType { _ltTrace, _ltDebug, _ltInfo, _ltStartup, _ltWarn, _ltError };

  void Log(_LogType lt, const char *format, va_list &args);

  spdlog::logger log_;
};

class Logger {
 public:
  static void Init(const char *app, const bool log_stdout,
                   const bool log_rot_file) {
    Singleton()._init(app, log_stdout, log_rot_file);
  }
  static void Init(const std::string &app, const bool log_stdout,
                   const bool log_rot_file) {
    Init(app.c_str(), log_stdout, log_rot_file);
  }

  static _Logger &AsyncCmd() { return *Singleton().async_cmd_; }
  static _Logger &Gtpv1U() { return *Singleton().gtpv1_u_; }
  static _Logger &Gtpv2C() { return *Singleton().gtpv2_c_; }
  // static _Logger &Gx() { return *singleton().gx_; }
  static _Logger &Itti() { return *Singleton().itti_; }
  static _Logger &SpgwcSx() { return *Singleton().spgwc_sx_; }
  static _Logger &SpgwcApp() { return *Singleton().spgwc_app_; }
  static _Logger &SpgwcS11() { return *Singleton().spgwc_s11_; }
  static _Logger &SpgwuApp() { return *Singleton().spgwu_app_; }
  static _Logger &SpgwuS1u() { return *Singleton().spgwu_s1u_; }
  static _Logger &SpgwuSx() { return *Singleton().spgwu_sx_; }
  static _Logger &System() { return *Singleton().system_; }
  static _Logger &Udp() { return *Singleton().udp_; }
  static _Logger &Pfcp() { return *Singleton().pfcp_; }
  static _Logger &PfcpSwitch() { return *Singleton().pfcp_switch_; }

 private:
  static Logger *singleton_;
  static Logger &Singleton() {
    if (!singleton_) singleton_ = new Logger();
    return *singleton_;
  }

  Logger() {}
  ~Logger() {}

  void _init(const char *app, const bool log_stdout, const bool log_rot_file);

  std::vector<spdlog::sink_ptr> m_sinks;

  std::string m_pattern;

  _Logger *async_cmd_;
  _Logger *gtpv1_u_;
  _Logger *gtpv2_c_;
  //_Logger *gx_;
  _Logger *itti_;
  _Logger *spgwc_app_;
  _Logger *spgwc_sx_;
  _Logger *spgwc_s11_;
  _Logger *spgwu_app_;
  _Logger *spgwu_s1u_;
  _Logger *spgwu_sx_;
  _Logger *system_;
  _Logger *udp_;
  _Logger *pfcp_;
  _Logger *pfcp_switch_;
};

#endif  // __LOGGER_H
