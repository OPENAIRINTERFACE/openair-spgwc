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
/*! \file UdpService.hpp
  \brief
  \author Lionel Gauthier
  \date 2020
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_UDPSERVICE_HPP_SEEN
#define FILE_UDPSERVICE_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
#include <inttypes.h>
//--C++ includes ---------------------------------------------------------------
#include <cstddef>
#include <utility>
//--Other includes -------------------------------------------------------------
#include "EndPoint.hpp"
#include "UdpApplication.hpp"
#include "thread_sched.hpp"
//------------------------------------------------------------------------------
class UdpService {
 public:
  virtual void AsyncSendTo(const uint key, const char* send_buffer,
                           const ssize_t num_bytes, const EndPoint& r_endpoint);

  virtual void AsyncSendTo(const uint key, const char* send_buffer,
                           const ssize_t num_bytes,
                           const struct sockaddr_in& r_endpoint);

  virtual void AsyncSendTo(const uint key, const char* send_buffer,
                           const ssize_t num_bytes,
                           const struct sockaddr_in6& r_endpoint);

  virtual uint16_t RegisterApplication(
      UdpApplication* app, const char* address, const uint16_t rx_port_num,
      const util::ThreadSchedParams& sched_params);
  virtual ~UdpService();
};
#endif /* FILE_UDPSERVICE_HPP_SEEN */
