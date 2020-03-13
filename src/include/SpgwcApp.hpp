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
/*! \file SpgwcApp.hpp
   \author  Lionel GAUTHIER
   \date 2018-2020
   \company Eurecom
   \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_SPGWC_APP_HPP_SEEN
#define FILE_SPGWC_APP_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
//--C++ includes ---------------------------------------------------------------
#include <atomic>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <thread>
//--Other includes -------------------------------------------------------------
#include <folly/AtomicHashMap.h>
#include <boost/atomic.hpp>
#include "Itti_Msg_s11.hpp"
#include "Itti_Msg_sxab.hpp"
#include "SpgwcContext.hpp"
#include "SpgwcPco.hpp"
#include "SpgwcS11.hpp"
#include "blockingconcurrentqueue.h"
#include "common_root_types.h"
#include "msg_gtpv2c.hpp"
//------------------------------------------------------------------------------
// Power of 2
#define SPGWC_APP_MAX_WORK_QUEUES 4

namespace spgwc {
enum WorkQueueItemType { kWkTypeNone = 0, kWkTypeS11, kWkTypeSxab, kWkTypeGx };
//------------------------------------------------------------------------------
class WorkQueueItem {
 public:
  WorkQueueItem() : type_(kWkTypeNone), trxn_id_(0){};
  WorkQueueItem(const int t_type, const uint64_t t_trxn_id = 0)
      : type_(t_type), trxn_id_(t_trxn_id){};
  virtual ~WorkQueueItem() = default;
  int type_;
  uint64_t trxn_id_;
#if PROFILING
#endif
};
//------------------------------------------------------------------------------
class S11WorkQueueMsgItem final : public WorkQueueItem {
 public:
  S11WorkQueueMsgItem(std::unique_ptr<gtpv2c::Gtpv2cIesContainer> t_msg,
                      const EndPoint& t_remote_endpoint,
                      const uint64_t t_trxn_id, uint32_t dest_teid)
      : WorkQueueItem(kWkTypeS11, t_trxn_id),
        msg_(std::move(t_msg)),
        remote_endpoint_(t_remote_endpoint),
        dest_teid_(dest_teid) {}
  virtual ~S11WorkQueueMsgItem() = default;
  std::unique_ptr<gtpv2c::Gtpv2cIesContainer> msg_;
  EndPoint remote_endpoint_;
  teid_t dest_teid_;
};
//------------------------------------------------------------------------------
class SxabWorkQueueMsgItem final : public WorkQueueItem {
 public:
  SxabWorkQueueMsgItem(std::unique_ptr<pfcp::PfcpIesContainer> t_msg,
                       const EndPoint& t_remote_endpoint,
                       const uint64_t t_trxn_id, seid_t dest_seid)
      : WorkQueueItem(kWkTypeSxab, t_trxn_id),
        msg_(std::move(t_msg)),
        remote_endpoint_(t_remote_endpoint),
        dest_seid_(dest_seid){};
  virtual ~SxabWorkQueueMsgItem() = default;
  std::unique_ptr<pfcp::PfcpIesContainer> msg_;
  EndPoint remote_endpoint_;
  seid_t dest_seid_;
};
//------------------------------------------------------------------------------
class SpgwcApp {
 private:
  std::array<std::atomic<uint64_t>, SPGWC_APP_MAX_WORK_QUEUES>
      failed_works_enqueued_;
#if PROFILING
  std::array<std::atomic<uint64_t>, SPGWC_APP_MAX_WORK_QUEUES> works_enqueued_;
  std::array<std::atomic<uint64_t>, SPGWC_APP_MAX_WORK_QUEUES> works_dequeued_;
  std::array<std::atomic<uint64_t>, SPGWC_APP_MAX_WORK_QUEUES>
      max_works_waiting_;
  std::array<std::atomic<uint>, SPGWC_APP_MAX_WORK_QUEUES>
      max_works_waiting_trigger_;
  std::array<std::atomic<uint64_t>, SPGWC_APP_MAX_WORK_QUEUES>
      max_time_ms_work_waiting_triggered_;
  std::array<std::atomic<uint32_t>, SPGWC_APP_MAX_WORK_QUEUES>
      max_time_ms_work_waiting_trigger_;
#endif
  std::array<moodycamel::BlockingConcurrentQueue<WorkQueueItem*>,
             SPGWC_APP_MAX_WORK_QUEUES>
      work_queues_;
  std::array<std::thread, SPGWC_APP_MAX_WORK_QUEUES> workers_;

  SpgwcS11& s11_stack_;
  std::thread::id thread_id;
  std::thread thread;

  static util::UintGenerator<uint32_t> s11_teid_generator;

  /* There shall be only one pair of TEID-C per UE over the S11 and the S4
     interfaces. The same tunnel shall be shared for the control messages
     related to the same UE operation. A TEID-C on the S11/S4 interface shall be
     released after all its associated EPS bearers are deleted.*/
  folly::AtomicHashMap<imsi64_t, std::shared_ptr<SpgwcContext>> imsi2context;
  folly::AtomicHashMap<teid_t, std::shared_ptr<SpgwcContext>> s11teid2context;
  folly::AtomicHashMap<seid_t, std::shared_ptr<SpgwcContext>> sxabseid2context;
  folly::AtomicHashMap<uint64_t, uint8_t> trxnid2queue;

  inline const uint8_t TrxnIdToWorkQueueIndex(const uint64_t t_trxn_id) const;
  void EnqueueWorkItem(WorkQueueItem* const item);

  teid_t GenerateS11CpTeid();
  bool IsS11cLocalTeidExists(const teid_t teid) const;

  // s11teid2context SpgwcContext collection
  bool IsS11LocalTeid2SpgwcContext(const teid_t tesi) const;

  // imsi2context SpgwcContext collection
  bool IsImsi642SpgwcContext(const imsi64_t imsi64) const;
  void SetImsi642SpgwcContext(const imsi64_t imsi64,
                              std::shared_ptr<SpgwcContext> sebc);

  // sxabseid2context SpgwcContext collection
  bool IsSeid2SpgwcContext(const seid_t seid) const;
  void SetSeid2SpgwcContext(const seid_t seid,
                            std::shared_ptr<SpgwcContext> pc);
  std::shared_ptr<SpgwcContext> Seid2SpgwcContext(const seid_t seid) const;

  int pco_push_protocol_or_container_id(
      protocol_configuration_options_t& pco,
      pco_protocol_or_container_id_t* const
          poc_id /* STOLEN_REF poc_id->contents*/);
  int process_pco_request_ipcp(
      protocol_configuration_options_t& pco_resp,
      const pco_protocol_or_container_id_t* const poc_id);
  int process_pco_dns_server_request(
      protocol_configuration_options_t& pco_resp,
      const pco_protocol_or_container_id_t* const poc_id);
  int process_pco_link_mtu_request(
      protocol_configuration_options_t& pco_resp,
      const pco_protocol_or_container_id_t* const poc_id);

  void ReceiveMessageFromWorkQueue(
      std::unique_ptr<gtpv2c::Gtpv2cCreateSessionRequest> m,
      const EndPoint& remote_endpoint, const teid_t local_teid,
      const uint64_t trxn_id);
  void ReceiveMessageFromWorkQueue(
      std::unique_ptr<gtpv2c::Gtpv2cDeleteSessionRequest> m,
      const EndPoint& remote_endpoint, const teid_t local_teid,
      const uint64_t trxn_id);
  void ReceiveMessageFromWorkQueue(
      std::unique_ptr<gtpv2c::Gtpv2cModifyBearerRequest> m,
      const EndPoint& remote_endpoint, const teid_t local_teid,
      const uint64_t trxn_id);
  void ReceiveMessageFromWorkQueue(
      std::unique_ptr<gtpv2c::Gtpv2cReleaseAccessBearersRequest> m,
      const EndPoint& remote_endpoint, const teid_t local_teid,
      const uint64_t trxn_id);
  void ReceiveMessageFromWorkQueue(
      std::unique_ptr<gtpv2c::Gtpv2cDownlinkDataNotificationAcknowledge> m,
      const EndPoint& remote_endpoint, const teid_t local_teid,
      const uint64_t trxn_id);

  void ReceiveMessageFromWorkQueue(
      std::unique_ptr<pfcp::PfcpSessionEstablishmentResponse> m,
      const EndPoint& remote_endpoint, const seid_t local_seid,
      const uint64_t trxn_id);
  void ReceiveMessageFromWorkQueue(
      std::unique_ptr<pfcp::PfcpSessionModificationResponse> m,
      const EndPoint& remote_endpoint, const seid_t local_seid,
      const uint64_t trxn_id);
  void ReceiveMessageFromWorkQueue(
      std::unique_ptr<pfcp::PfcpSessionDeletionResponse> m,
      const EndPoint& remote_endpoint, const seid_t local_seid,
      const uint64_t trxn_id);
  void ReceiveMessageFromWorkQueue(
      std::unique_ptr<pfcp::PfcpSessionReportRequest> m,
      const EndPoint& remote_endpoint, const seid_t local_seid,
      const uint64_t trxn_id);
  void ReceiveMessageFromWorkQueue(
      std::unique_ptr<pfcp::PfcpAssociationSetupRequest> m,
      const EndPoint& remote_endpoint, const seid_t local_seid,
      const uint64_t trxn_id);

 public:
  // key is S11 S-GW local teid, value is S11 tunnel id pair
  // map<teid_t, int> s11teid2mme_hashtable;

  // key is paa, value is S11 s-gw local teid
  // obj_hash_table_uint64_t *ip2s11teid;

  // key is S1-U S-GW local teid
  ////hash_table_t *s1uteid2enb_hashtable;

  // the key of this hashtable is the S11 s-gw local teid.
  // hash_table_ts_t *s11_bearer_context_information_hashtable;

  /* The TEID-C shall be unique per PDN-Connection on GTP based S2a, S2b, S5 and
     S8 interfaces. The same tunnel shall be shared for the control messages
     related to all bearers associated to the PDN-Connection. A TEID-C on the
     S2a/S2b/S5/S8 interface shall be released after all its associated EPS
     bearers are deleted. */

  // gtpv1u_data_t    gtpv1u_data;

  explicit SpgwcApp(const std::string& config_file, SpgwcS11& s11_stack);
  ~SpgwcApp();
  SpgwcApp(SpgwcApp const&) = delete;
  void operator=(SpgwcApp const&) = delete;

  fteid_t GenerateS11CpFteid(const struct in_addr ipv4_address);
  std::pair<std::shared_ptr<SpgwcContext>, std::shared_ptr<PdnConnection>>
  S11Teid2SpgwContexts(const teid_t s11_teid) const;
  std::shared_ptr<SpgwcContext> S11Teid2SpgwcContext(
      const teid_t sgw_teid) const;
  void SetS11LocalTeid2SpgwcContext(const teid_t sgw_teid,
                                    std::shared_ptr<SpgwcContext> sc);
  std::shared_ptr<SpgwcContext> Imsi642SpgwcContext(
      const imsi64_t imsi64) const;
  std::shared_ptr<SpgwcContext> LocalSeid2SpgwcContext(const seid_t seid) const;
  void SetLocalSeid2SpgwcContext(const seid_t seid,
                                 std::shared_ptr<SpgwcContext> sc);

  void ReleaseS11CpTeid(const teid_t teid);
  void DeleteS11LocalTeid2SpgwcContexts(const teid_t s11_teid);
  void DeleteSpgwcContext(std::shared_ptr<SpgwcContext> spc);

  int process_pco_request(const protocol_configuration_options_t& pco_req,
                          protocol_configuration_options_t& pco_resp,
                          protocol_configuration_options_ids_t& pco_ids);

  void SameQueueSelection(const uint64_t primary_trxn_id,
                          const uint64_t leg_trxn_id);

  //------------------------------------------------------------
  // Interface with S11
  //------------------------------------------------------------
  void SendCreateSessionResponseCause(const uint64_t t_gtpc_tx_id,
                                      const teid_t t_dest_teid,
                                      const EndPoint& t_remote_endpoint,
                                      const cause_t& t_cause) const;
  void SendDeleteSessionResponseCauseRequestAccepted(
      const uint64_t t_gtpc_tx_id, const teid_t t_dest_teid,
      const EndPoint& t_remote_endpoint) const;
  void SendModifyBearerResponseCauseContextNotFound(
      const uint64_t t_gtpc_tx_id, const teid_t t_dest_teid,
      const EndPoint& t_remote_endpoint) const;
  void SendDeleteSessionResponseCauseContextNotFound(
      const uint64_t t_gtpc_tx_id, const teid_t t_dest_teid,
      const EndPoint& t_remote_endpoint) const;
  void SendReleaseAccessBearersResponseCauseContextNotFound(
      const uint64_t t_gtpc_tx_id, const teid_t t_dest_teid,
      const EndPoint& t_remote_endpoint) const;
  void SendReleaseAccessBearersResponseCauseRequestAccepted(
      const uint64_t t_gtpc_tx_id, const teid_t t_dest_teid,
      const EndPoint& t_remote_endpoint) const;

  void Receive(std::unique_ptr<gtpv2c::Gtpv2cCreateSessionRequest> m,
               const EndPoint& remote_endpoint, const teid_t local_teid,
               const uint64_t trxn_id);
  void Receive(std::unique_ptr<gtpv2c::Gtpv2cDeleteSessionRequest> m,
               const EndPoint& remote_endpoint, const teid_t local_teid,
               const uint64_t trxn_id);
  void Receive(std::unique_ptr<gtpv2c::Gtpv2cModifyBearerRequest> m,
               const EndPoint& remote_endpoint, const teid_t local_teid,
               const uint64_t trxn_id);
  void Receive(std::unique_ptr<gtpv2c::Gtpv2cReleaseAccessBearersRequest> m,
               const EndPoint& remote_endpoint, const teid_t local_teid,
               const uint64_t trxn_id);
  void Receive(
      std::unique_ptr<gtpv2c::Gtpv2cDownlinkDataNotificationAcknowledge> m,
      const EndPoint& remote_endpoint, const teid_t local_teid,
      const uint64_t trxn_id);

  //------------------------------------------------------------
  // Interface with Sxab
  //------------------------------------------------------------
  void Receive(std::unique_ptr<pfcp::PfcpHeartbeatRequest> m,
               const EndPoint& remote_endpoint, const uint64_t trxn_id);
  void Receive(std::unique_ptr<pfcp::PfcpHeartbeatResponse> m,
               const EndPoint& remote_endpoint, const uint64_t trxn_id);
  void Receive(std::unique_ptr<pfcp::PfcpAssociationSetupRequest> m,
               const EndPoint& remote_endpoint, const uint64_t trxn_id);
  void Receive(std::unique_ptr<pfcp::PfcpAssociationSetupResponse> m,
               const EndPoint& remote_endpoint, const uint64_t trxn_id);
  void Receive(std::unique_ptr<pfcp::PfcpSessionEstablishmentResponse> m,
               const EndPoint& remote_endpoint, const seid_t local_seid,
               const uint64_t trxn_id);
  void Receive(std::unique_ptr<pfcp::PfcpSessionModificationResponse> m,
               const EndPoint& remote_endpoint, const seid_t local_seid,
               const uint64_t trxn_id);
  void Receive(std::unique_ptr<pfcp::PfcpSessionDeletionResponse> m,
               const EndPoint& remote_endpoint, const seid_t local_seid,
               const uint64_t trxn_id);
  void Receive(std::unique_ptr<pfcp::PfcpSessionReportRequest> m,
               const EndPoint& remote_endpoint, const seid_t local_seid,
               const uint64_t trxn_id);
  void Receive(std::unique_ptr<pfcp::PfcpNodeReportRequest> m,
               const EndPoint& remote_endpoint, const seid_t local_seid,
               const uint64_t trxn_id);
  void NotifyTriggeredResponseTimeOut(const EndPoint& t_remote_endpoint,
                                      const teid_t t_local_teid,
                                      const uint64_t t_trxn_id);
  void NotifySduServiceSubmitted2LowerLayer(
      const EndPoint& t_remote_endpoint, const teid_t t_local_teid,
      const uint64_t t_trxn_id, const std::pair<bool, teid_t> t_dest_teid);
};
}  // namespace spgwc
#endif /* FILE_SPGWC_APP_HPP_SEEN */
