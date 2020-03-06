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
/*! \file GtpV2cApplication.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_GTPV2CAPPLICATION_HPP_SEEN
#define FILE_GTPV2CAPPLICATION_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
#include <inttypes.h>
//--C++ includes ---------------------------------------------------------------
#include <cstddef>
//--Other includes -------------------------------------------------------------
#include "3gpp_29.274.hpp"
#include "EndPoint.hpp"
//------------------------------------------------------------------------------
class GtpV2cApplication {
 public:
  virtual void NotifyReceiveMsg(const std::unique_ptr<gtpv2c::Gtpv2cMsg> msg,
                                const EndPoint& remote_endpoint,
                                const uint64_t gtpc_tx_id);

  virtual void NotifyTriggeredResponseTimeOut(gtpv2c::Gtpv2cMsg& msg,
                                              const EndPoint& remote_endpoint);

  virtual void NotifyError(const EndPoint& remote_endpoint,
                           const uint64_t gtpc_tx_id, const uint32_t teid,
                           const cause_value_e cause);
  virtual void NotifySduServiceSubmitted2LowerLayer(
      const EndPoint& remote_endpoint, const uint64_t gtpc_tx_id,
      const uint32_t teid);

  virtual ~GtpV2cApplication();
};

#endif /* FILE_GTPV2CAPPLICATION_HPP_SEEN */
