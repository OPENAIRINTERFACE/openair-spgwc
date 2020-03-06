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
/*! \file Gtpv2Service.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_GTPV2CSERVICE_HPP_SEEN
#define FILE_GTPV2CSERVICE_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
#include <inttypes.h>
//--C++ includes ---------------------------------------------------------------
#include <cstddef>
//--Other includes -------------------------------------------------------------
#include "EndPoint.hpp"
#include "gtpv2c.hpp"
//------------------------------------------------------------------------------
namespace gtpv2c {
class GtpV2cService {
 public:
  virtual uint64_t SendInitialMessage(std::unique_ptr<gtpv2c::Gtpv2cMsg> t_msg,
                                      const EndPoint& t_remote_endpoint,
                                      const teid_t t_dest_teid,
                                      const uint64_t t_trxn_id = 0);
  virtual uint64_t SendTriggeredMessage(
      std::unique_ptr<gtpv2c::Gtpv2cMsg> t_msg,
      const EndPoint& t_remote_endpoint, const teid_t t_dest_teid,
      const uint64_t t_trxn_id, const int t_trxn_action);

  virtual void RegisterApplication(GtpV2cApplication* app);

  virtual ~GtpV2cService() = default;
};
}  // namespace gtpv2c
#endif /* FILE_GTPV2CSERVICE_HPP_SEEN */
