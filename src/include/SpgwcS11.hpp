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

/*! \file SpgwcS11.hpp
   \author  Lionel GAUTHIER
   \date 2018-2020
   \company Eurecom
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_SPGWC_S11_HPP_SEEN
#define FILE_SPGWC_S11_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
//--C++ includes ---------------------------------------------------------------
#include <array>
#include <thread>
//--Other includes -------------------------------------------------------------
#include "Gtpv2cApplication.hpp"
#include "gtpv2c.hpp"
//------------------------------------------------------------------------------
namespace spgwc {
class SpgwcS11;
typedef void (SpgwcS11::*GtpV2CMsgHandler)(std::unique_ptr<gtpv2c::Gtpv2cMsg>,
                                           const EndPoint&,
                                           const uint64_t gtpc_tx_id);

class SpgwcS11 : public GtpV2cApplication {
 public:
  SpgwcS11(gtpv2c::GtpV2cService& gtpv2c_service);
  SpgwcS11(SpgwcS11 const&) = delete;
  void operator=(SpgwcS11 const&) = delete;

  // implement GtpV2cApplication interface
  void NotifyReceiveMsg(std::unique_ptr<gtpv2c::Gtpv2cMsg> msg,
                        const EndPoint& remote_endpoint,
                        const uint64_t gtpc_tx_id);
  void NotifyTriggeredResponseTimeOut(gtpv2c::Gtpv2cMsg& msg,
                                      const EndPoint& remote_endpoint);
  void NotifyError(const EndPoint& remote_endpoint, const uint64_t gtpc_tx_id,
                   const uint32_t teid, const cause_value_e cause);

  // interface with SPGWC Application
  uint64_t SendMsg(std::shared_ptr<gtpv2c::Gtpv2cCreateSessionResponse> t_msg,
                   const EndPoint& t_remote_endpoint, const teid_t t_dest_teid,
                   const uint64_t t_gtpc_tx_id);
  uint64_t SendMsg(std::shared_ptr<gtpv2c::Gtpv2cDeleteSessionResponse> t_msg,
                   const EndPoint& t_remote_endpoint, const teid_t t_dest_teid,
                   const uint64_t t_gtpc_tx_id);
  uint64_t SendMsg(std::shared_ptr<gtpv2c::Gtpv2cModifyBearerResponse> t_msg,
                   const EndPoint& t_remote_endpoint, const teid_t t_dest_teid,
                   const uint64_t t_gtpc_tx_id);
  uint64_t SendMsg(
      std::shared_ptr<gtpv2c::Gtpv2cReleaseAccessBearersResponse> t_msg,
      const EndPoint& t_remote_endpoint, const teid_t t_dest_teid,
      const uint64_t t_gtpc_tx_id);
  uint64_t SendMsg(
      std::shared_ptr<gtpv2c::Gtpv2cDownlinkDataNotification> t_msg,
      const EndPoint& t_remote_endpoint, const teid_t t_dest_teid,
      const uint64_t t_gtpc_tx_id);
  uint64_t SendMsg(std::shared_ptr<gtpv2c::Gtpv2cEchoResponse> t_msg,
                   const EndPoint& t_remote_endpoint,
                   const uint64_t t_gtpc_tx_id);
  uint64_t SendEchoResponse(const EndPoint& t_remote_endpoint,
                            uint8_t restart_counter,
                            const uint64_t t_gtpc_tx_id);

 private:
  std::thread::id thread_id;
  std::thread thread;

  gtpv2c::GtpV2cService& gtpv2c_service_;

  static const std::array<GtpV2CMsgHandler, GTP_LAST_HANDLED_MESSAGE> handlers_;
  void HandleReceiveUnhandledMessage(std::unique_ptr<gtpv2c::Gtpv2cMsg> msg,
                                     const EndPoint& remote_endpoint,
                                     const uint64_t gtpc_tx_id);
  void HandleReceiveGtpv2cMsg(std::unique_ptr<gtpv2c::Gtpv2cMsg> msg,
                              const EndPoint& remote_endpoint,
                              const uint64_t gtpc_tx_id);
  void HandleReceiveEchoRequest(std::unique_ptr<gtpv2c::Gtpv2cMsg> msg,
                                const EndPoint& remote_endpoint,
                                const uint64_t gtpc_tx_id);
  void HandleReceiveEchoResponse(std::unique_ptr<gtpv2c::Gtpv2cMsg> msg,
                                 const EndPoint& remote_endpoint,
                                 const uint64_t gtpc_tx_id);
  void HandleReceiveCreateSessionRequest(std::unique_ptr<gtpv2c::Gtpv2cMsg> msg,
                                         const EndPoint& remote_endpoint,
                                         const uint64_t gtpc_tx_id);
  void HandleReceiveDeleteSessionRequest(std::unique_ptr<gtpv2c::Gtpv2cMsg> msg,
                                         const EndPoint& remote_endpoint,
                                         const uint64_t gtpc_tx_id);
  void HandleReceiveModifyBearerRequest(std::unique_ptr<gtpv2c::Gtpv2cMsg> msg,
                                        const EndPoint& remote_endpoint,
                                        const uint64_t gtpc_tx_id);
  void HandleReceiveReleaseAccessBearersRequest(
      std::unique_ptr<gtpv2c::Gtpv2cMsg> msg, const EndPoint& remote_endpoint,
      const uint64_t gtpc_tx_id);
  void HandleReceiveDownlinkDataNotificationAcknowledge(
      std::unique_ptr<gtpv2c::Gtpv2cMsg> msg, const EndPoint& remote_endpoint,
      const uint64_t gtpc_tx_id);
};
}  // namespace spgwc
#endif /* FILE_SPGWC_S11_HPP_SEEN */
