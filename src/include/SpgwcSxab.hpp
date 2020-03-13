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

/*! \file SpgwcSxab.hpp
   \author  Lionel GAUTHIER
   \date 2018-2020
   \company Eurecom
   \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_SXAB_HPP_SEEN
#define FILE_SXAB_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
//--C++ includes ---------------------------------------------------------------
#include <array>
#include <thread>
//--Other includes -------------------------------------------------------------
#include "PfcpApplication.hpp"
#include "pfcp.hpp"
//------------------------------------------------------------------------------
namespace spgwc {
class SpgwcSxab;
typedef void (SpgwcSxab::*PfcpMsgHandler)(std::unique_ptr<pfcp::PfcpMsg>,
                                          const EndPoint&,
                                          const uint64_t t_trxn_id);
//------------------------------------------------------------------------------
class SpgwcSxab : public PfcpApplication {
 public:
  SpgwcSxab(pfcp::PfcpService& service);
  SpgwcSxab(SpgwcSxab const&) = delete;
  void operator=(SpgwcSxab const&) = delete;
  //--------------------------------------------
  // implement PfcpApplication interface
  void NotifyReceiveMsg(std::unique_ptr<pfcp::PfcpMsg> t_msg,
                        const EndPoint& t_remote_endpoint,
                        const uint16_t t_local_port, const uint64_t t_trxn_id);
  void NotifyTriggeredResponseTimeOut(std::unique_ptr<pfcp::PfcpMsg> t_msg,
                                      const EndPoint& t_remote_endpoint);
  void NotifyError(const EndPoint& t_remote_endpoint, const uint64_t t_trxn_id,
                   const seid_t t_seid, const pfcp::cause_value_e t_cause);
  void NotifySduServiceSubmitted2LowerLayer(const EndPoint& t_remote_endpoint,
                                            const uint64_t t_trxn_id,
                                            const seid_t t_seid);
  //--------------------------------------------
  // interface with SPGWC/SPGWU Application
  uint64_t SendMsg(std::shared_ptr<pfcp::PfcpAssociationSetupRequest> t_msg,
                   const EndPoint& t_remote_endpoint,
                   const std::pair<bool, seid_t> t_dest_seid,
                   const uint64_t t_trxn_id);
  void SendMsg(std::shared_ptr<pfcp::PfcpAssociationSetupResponse> t_msg,
               const EndPoint& t_remote_endpoint,
               const std::pair<bool, seid_t> t_dest_seid,
               const uint64_t t_trxn_id);
  uint64_t SendMsg(std::shared_ptr<pfcp::PfcpAssociationReleaseRequest> t_msg,
                   const EndPoint& t_remote_endpoint,
                   const std::pair<bool, seid_t> t_dest_seid,
                   const uint64_t t_trxn_id);
  void SendMsg(std::shared_ptr<pfcp::PfcpAssociationReleaseResponse> t_msg,
               const EndPoint& t_remote_endpoint,
               const std::pair<bool, seid_t> t_dest_seid,
               const uint64_t t_trxn_id);
  uint64_t SendMsg(std::shared_ptr<pfcp::PfcpAssociationUpdateRequest> t_msg,
                   const EndPoint& t_remote_endpoint,
                   const std::pair<bool, seid_t> t_dest_seid,
                   const uint64_t t_trxn_id);
  void SendMsg(std::shared_ptr<pfcp::PfcpAssociationUpdateResponse> t_msg,
               const EndPoint& t_remote_endpoint,
               const std::pair<bool, seid_t> t_dest_seid,
               const uint64_t t_trxn_id);
  void SendMsg(std::shared_ptr<pfcp::PfcpHeartbeatRequest> t_msg,
                   const EndPoint& t_remote_endpoint, const uint64_t t_trxn_id);
  void SendMsg(std::shared_ptr<pfcp::PfcpHeartbeatResponse> t_msg,
                   const EndPoint& t_remote_endpoint, const uint64_t t_trxn_id);
  uint64_t SendMsg(std::shared_ptr<pfcp::PfcpNodeReportRequest> t_msg,
                   const EndPoint& t_remote_endpoint,
                   const std::pair<bool, seid_t> t_dest_seid,
                   const uint64_t t_trxn_id);
  void SendMsg(std::shared_ptr<pfcp::PfcpNodeReportResponse> t_msg,
               const EndPoint& t_remote_endpoint,
               const std::pair<bool, seid_t> t_dest_seid,
               const uint64_t t_trxn_id);
  uint64_t SendMsg(std::shared_ptr<pfcp::PfcpSessionEstablishmentRequest> t_msg,
                   const EndPoint& t_remote_endpoint,
                   const std::pair<bool, seid_t> t_dest_seid,
                   const uint64_t t_trxn_id);
  uint64_t SendMsg(std::shared_ptr<pfcp::PfcpSessionModificationRequest> t_msg,
                   const EndPoint& t_remote_endpoint,
                   const std::pair<bool, seid_t> t_dest_seid,
                   const uint64_t t_trxn_id);
  uint64_t SendMsg(std::shared_ptr<pfcp::PfcpSessionDeletionRequest> t_msg,
                   const EndPoint& t_remote_endpoint,
                   const std::pair<bool, seid_t> t_dest_seid,
                   const uint64_t t_trxn_id);
  void SendMsg(std::shared_ptr<pfcp::PfcpSessionReportResponse> t_msg,
               const EndPoint& t_remote_endpoint,
               const std::pair<bool, seid_t> t_dest_seid,
               const uint64_t t_trxn_id);

  void SendHeartbeatResponse(const EndPoint& t_remote_endpoint,
                             uint32_t t_recovery_time_stamp,
                             const uint64_t t_trxn_id);
  void SendHeartbeatRequest(const EndPoint& t_remote_endpoint,
                            uint32_t t_recovery_time_stamp,
                            const uint64_t t_trxn_id);

 private:
  std::thread::id thread_id;
  std::thread thread;
  pfcp::PfcpService& pfcp_service_;
  static const std::array<PfcpMsgHandler, PFCP_LAST_HANDLED_MESSAGE> handlers_;
  void HandleReceiveUnhandledMessage(std::unique_ptr<pfcp::PfcpMsg> t_msg,
                                     const EndPoint& t_remote_endpoint,
                                     const uint64_t t_trxn_id);
  void HandleReceivePfcpMsg(std::unique_ptr<pfcp::PfcpMsg> t_msg,
                            const EndPoint& t_remote_endpoint,
                            const uint64_t t_trxn_id);

  void HandleReceiveHeartbeatRequest(std::unique_ptr<pfcp::PfcpMsg> t_msg,
                                     const EndPoint& t_remote_endpoint,
                                     const uint64_t t_trxn_id);
  void HandleReceiveHeartbeatResponse(std::unique_ptr<pfcp::PfcpMsg> t_msg,
                                      const EndPoint& t_remote_endpoint,
                                      const uint64_t t_trxn_id);
  void HandleReceiveAssociationSetupRequest(
      std::unique_ptr<pfcp::PfcpMsg> t_msg, const EndPoint& t_remote_endpoint,
      const uint64_t t_trxn_id);
  void HandleReceiveAssociationSetupResponse(
      std::unique_ptr<pfcp::PfcpMsg> t_msg, const EndPoint& t_remote_endpoint,
      const uint64_t t_trxn_id);
  void HandleReceiveSessionEstablishmentResponse(
      std::unique_ptr<pfcp::PfcpMsg> t_msg, const EndPoint& t_remote_endpoint,
      const uint64_t t_trxn_id);
  void HandleReceiveSessionModificationResponse(
      std::unique_ptr<pfcp::PfcpMsg> t_msg, const EndPoint& t_remote_endpoint,
      const uint64_t t_trxn_id);
  void HandleReceiveSessionDeletionResponse(
      std::unique_ptr<pfcp::PfcpMsg> t_msg, const EndPoint& t_remote_endpoint,
      const uint64_t t_trxn_id);
  void HandleReceiveSessionReportRequest(std::unique_ptr<pfcp::PfcpMsg> t_msg,
                                         const EndPoint& t_remote_endpoint,
                                         const uint64_t t_trxn_id);
  void HandleReceiveNodeReportRequest(std::unique_ptr<pfcp::PfcpMsg> t_msg,
                                      const EndPoint& t_remote_endpoint,
                                      const uint64_t t_trxn_id);
};
}  // namespace spgwc
#endif /* FILE_SXAB_HPP_SEEN */
