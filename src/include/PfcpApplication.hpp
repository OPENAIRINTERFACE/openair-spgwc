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
/*! \file PfcpApplication.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_PFCPAPPLICATION_HPP_SEEN
#define FILE_PFCPAPPLICATION_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
#include <inttypes.h>
//--C++ includes ---------------------------------------------------------------
#include <cstddef>
//--Other includes -------------------------------------------------------------
#include "3gpp_29.244.hpp"
#include "EndPoint.hpp"
//------------------------------------------------------------------------------
class PfcpApplication {
 public:
  virtual void NotifyReceiveMsg(std::unique_ptr<pfcp::PfcpMsg> t_msg,
                                const EndPoint& t_remote_endpoint,
                                const uint16_t t_local_port,
                                const uint64_t t_trxn_id) = 0;

  virtual void NotifyTriggeredResponseTimeOut(
      std::unique_ptr<pfcp::PfcpMsg> msg, const EndPoint& remote_endpoint) = 0;

  virtual void NotifyError(const EndPoint& t_remote_endpoint,
                           const uint64_t t_trxn_id, const seid_t t_seid,
                           const pfcp::cause_value_e t_cause) = 0;
  virtual void NotifySduServiceSubmitted2LowerLayer(
      const EndPoint& t_remote_endpoint, const uint64_t t_trxn_id,
      const seid_t t_seid) = 0;
  PfcpApplication(){};
  virtual ~PfcpApplication(){};
};

#endif /* FILE_PFCPAPPLICATION_HPP_SEEN */
