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
/*! \file SpgwcProcedure.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_SPGWC_PROCEDURE_HPP_SEEN
#define FILE_SPGWC_PROCEDURE_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
//--C++ includes ---------------------------------------------------------------
#include <list>
#include <memory>
//--Other includes -------------------------------------------------------------
#include "EndPoint.hpp"
#include "UintGenerator.hpp"
#include "msg_gtpv2c.hpp"
#include "msg_pfcp.hpp"
//------------------------------------------------------------------------------
namespace spgwc {
class SpgwcContext;
class PdnConnection;

class SpgwcProcedure {
 private:
  uint64_t trxn_id_;

  static uint64_t GenerateTrxnId() {
    return util::UintUidGenerator<uint64_t>::GetInstance().GetUid();
  }

 public:
  bool marked_for_removal_;

  SpgwcProcedure() {
    trxn_id_ = GenerateTrxnId();
    marked_for_removal_ = false;
  }
  explicit SpgwcProcedure(const uint64_t t_trxn_id) {
    trxn_id_ = t_trxn_id;
    marked_for_removal_ = false;
  }
  virtual ~SpgwcProcedure() {}
  virtual bool HasTrxnId(const uint64_t trxn_id) const {
    return (trxn_id == trxn_id);
  }
  virtual uint64_t TrxnId() const { return trxn_id_; }
  virtual int Run(std::shared_ptr<SpgwcContext> c) { return RETURNerror; }
  virtual void ProcessMsg(
      std::unique_ptr<pfcp::PfcpSessionEstablishmentResponse> resp);
  virtual void ProcessMsg(
      std::unique_ptr<pfcp::PfcpSessionModificationResponse> resp);
  virtual void ProcessMsg(
      std::unique_ptr<pfcp::PfcpSessionDeletionResponse> resp);
  virtual void ProcessMsg(
      std::unique_ptr<gtpv2c::Gtpv2cDownlinkDataNotificationAcknowledge> resp);
};
//------------------------------------------------------------------------------
class SpgwcContext;
class PdnConnection;
// WIP Create Session Request     MME  to SPGW
// TODO Create Bearer Request     SPGW to MME
// TODO Bearer Resource Command   MME  to SPGW
// WIP Modify Bearer Request      MME  to SPGW
// WIP Delete Session Request     MME  to SPGW
// Delete Bearer Request          SPGW to MME
// WIP Downlink Data Notification     SPGW to MME
// Delete Indirect Data Forwarding Tunnel Request MME to SPGW
// Modify Bearer Command          MME  to SPGW
// Update Bearer Request          SPGW to MME
// Delete Bearer Command          MME  to SPGW
// Create Indirect Data Forwarding Tunnel Request MME  to SGW
// WIP Release Access Bearers Request MME to SGW
// Stop Paging Indication         SGW to MME
// Modify Access Bearers Request  MME to SGW
// Remote UE Report Notification  MME to SGW
//
//------------------------------------------------------------------------------
// Handles S11 Create Session Request
class SessionEstablishmentProcedure : public SpgwcProcedure {
 public:
  explicit SessionEstablishmentProcedure(const EndPoint& t_remote_endpoint,
                                         const teid_t t_local_teid,
                                         const uint64_t t_trxn_id)
      : SpgwcProcedure(t_trxn_id),
        s11_remote_endpoint_(t_remote_endpoint),
        s11_local_teid_(t_local_teid),
        pdn_(),
        sx_triggered_(),
        s11_triggered_pending_(),
        s11_trigger_() {}

  int Run(std::shared_ptr<gtpv2c::Gtpv2cCreateSessionRequest> req,
          std::shared_ptr<gtpv2c::Gtpv2cCreateSessionResponse> resp,
          std::shared_ptr<spgwc::SpgwcContext> c,
          std::shared_ptr<spgwc::PdnConnection> p);
  void ProcessMsg(std::unique_ptr<pfcp::PfcpSessionEstablishmentResponse> resp);
  //~SessionEstablishmentProcedure() {}

  std::shared_ptr<gtpv2c::Gtpv2cCreateSessionRequest> s11_trigger_;
  std::shared_ptr<gtpv2c::Gtpv2cCreateSessionResponse> s11_triggered_pending_;
  std::shared_ptr<pfcp::PfcpSessionEstablishmentRequest> sx_triggered_;
  std::shared_ptr<spgwc::PdnConnection> pdn_;
  std::shared_ptr<spgwc::SpgwcContext> ctxt_;
  const EndPoint s11_remote_endpoint_;
  const teid_t s11_local_teid_;
};

//------------------------------------------------------------------------------
class pdn_bearers_to_be_xied {
 public:
  pdn_bearers_to_be_xied()
      : pdn(),
        bearer_contexts_to_be_modified(),
        bearer_contexts_to_be_removed(),
        // msg(),
        gtpc_tx_id() {}
  pdn_bearers_to_be_xied(const pdn_bearers_to_be_xied& p)
      : pdn(p.pdn),
        bearer_contexts_to_be_modified(p.bearer_contexts_to_be_modified),
        bearer_contexts_to_be_removed(p.bearer_contexts_to_be_removed),
        // msg(p.msg),
        gtpc_tx_id(p.gtpc_tx_id) {}
  std::shared_ptr<PdnConnection> pdn;
  std::vector<gtpv2c::BearerContextToBeModifiedWithinModifyBearerRequest>
      bearer_contexts_to_be_modified;
  // Still there because of not complete S/P GW split
  std::vector<gtpv2c::BearerContextToBeRemovedWithinModifyBearerRequest>
      bearer_contexts_to_be_removed;
  // std::shared_ptr<itti_s11_modify_bearer_request> msg;
  uint64_t gtpc_tx_id;
};

//------------------------------------------------------------------------------
// Handles S11 Modify Bearer Request
class ModifyBearerRequestProcedure : public SpgwcProcedure {
 public:
  explicit ModifyBearerRequestProcedure(const EndPoint& t_remote_endpoint,
                                        const teid_t t_local_teid,
                                        const uint64_t t_trxn_id)
      : SpgwcProcedure(t_trxn_id),
        s11_remote_endpoint_(t_remote_endpoint),
        s11_local_teid_(t_local_teid),
        pdn_bearers(),
        null_pdn_bearers(),
        pdn_(),
        ctxt_(),
        bearer_contexts_modified(),
        bearer_contexts_marked_for_removal() {}

  int Run(std::shared_ptr<gtpv2c::Gtpv2cModifyBearerRequest> req,
          std::shared_ptr<gtpv2c::Gtpv2cModifyBearerResponse> resp,
          std::shared_ptr<spgwc::SpgwcContext> c,
          std::shared_ptr<spgwc::PdnConnection> p);

  void ProcessMsg(std::unique_ptr<pfcp::PfcpSessionModificationResponse> resp);

  std::shared_ptr<gtpv2c::Gtpv2cModifyBearerRequest> s11_trigger;
  std::shared_ptr<gtpv2c::Gtpv2cModifyBearerResponse> s11_triggered_pending;
  std::shared_ptr<pfcp::PfcpSessionModificationRequest> sx_triggered;

  std::vector<std::shared_ptr<pdn_bearers_to_be_xied>> pdn_bearers;
  pdn_bearers_to_be_xied null_pdn_bearers;
  std::vector<gtpv2c::BearerContextModifiedWithinModifyBearerResponse>
      bearer_contexts_modified;
  std::vector<gtpv2c::BearerContextMarkedForRemovalWithinModifyBearerResponse>
      bearer_contexts_marked_for_removal;
  std::shared_ptr<spgwc::PdnConnection> pdn_;
  std::shared_ptr<spgwc::SpgwcContext> ctxt_;
  const EndPoint s11_remote_endpoint_;
  const teid_t s11_local_teid_;
};
//------------------------------------------------------------------------------
// Handles S11 Delete Session Request
class DeleteSessionRequestProcedure : public SpgwcProcedure {
 public:
  explicit DeleteSessionRequestProcedure(const EndPoint& t_remote_endpoint,
                                         const teid_t t_local_teid,
                                         const uint64_t t_trxn_id)
      : SpgwcProcedure(t_trxn_id),
        s11_remote_endpoint_(t_remote_endpoint),
        s11_local_teid_(t_local_teid),
        pdn_(nullptr),
        ctxt_(nullptr),
        sx_triggered_(),
        s11_triggered_pending_(),
        s11_trigger_(),
        sx_trxn_id_(0) {}

  int Run(std::unique_ptr<gtpv2c::Gtpv2cDeleteSessionRequest> req,
          std::shared_ptr<gtpv2c::Gtpv2cDeleteSessionResponse> resp,
          std::shared_ptr<spgwc::SpgwcContext> c,
          std::shared_ptr<spgwc::PdnConnection> p);
  void ProcessMsg(std::unique_ptr<pfcp::PfcpSessionDeletionResponse> resp);

  //~DeleteSessionRequestProcedure() {}

  std::unique_ptr<gtpv2c::Gtpv2cDeleteSessionRequest> s11_trigger_;
  std::shared_ptr<gtpv2c::Gtpv2cDeleteSessionResponse> s11_triggered_pending_;
  std::shared_ptr<pfcp::PfcpSessionDeletionRequest> sx_triggered_;
  std::shared_ptr<spgwc::PdnConnection> pdn_;
  std::shared_ptr<spgwc::SpgwcContext> ctxt_;
  const EndPoint s11_remote_endpoint_;
  const teid_t s11_local_teid_;
  uint64_t sx_trxn_id_;
};

//------------------------------------------------------------------------------
// Handles Downlink Data Report Procedure
class DownlinkDataReportProcedure : public SpgwcProcedure {
 public:
  explicit DownlinkDataReportProcedure(
      std::unique_ptr<pfcp::PfcpSessionReportRequest> t_msg,
      const EndPoint& t_remote_endpoint, const seid_t t_local_seid,
      const uint64_t t_trxn_id)
      : SpgwcProcedure(t_trxn_id),
        sx_remote_endpoint_(t_remote_endpoint),
        sx_local_seid_(),
        sx_trigger_(std::move(t_msg)),
        s11_triggered_(),
        pdn_(),
        ctxt_(),
        ebi(0) {}
  int Run(std::shared_ptr<spgwc::SpgwcContext> ctxt,
          std::shared_ptr<spgwc::PdnConnection> pdn, const ebi_t ebi);
  void ProcessMsg(
      std::unique_ptr<gtpv2c::Gtpv2cDownlinkDataNotificationAcknowledge> resp);
  void NotifyFailureToPeer(const pfcp::cause_t& cause);

  //~DownlinkDataReportProcedure() {}

  std::shared_ptr<gtpv2c::Gtpv2cDownlinkDataNotification> s11_triggered_;
  std::unique_ptr<pfcp::PfcpSessionReportRequest> sx_trigger_;
  std::shared_ptr<spgwc::PdnConnection> pdn_;
  std::shared_ptr<spgwc::SpgwcContext> ctxt_;
  const EndPoint sx_remote_endpoint_;
  const seid_t sx_local_seid_;
  ebi_t ebi;
};
//------------------------------------------------------------------------------
class BearersToBeReleased {
 public:
  BearersToBeReleased() : pdn_(), gtpc_tx_id_() {}
  BearersToBeReleased(const BearersToBeReleased& p)
      : pdn_(p.pdn_), gtpc_tx_id_(p.gtpc_tx_id_) {}
  std::shared_ptr<PdnConnection> pdn_;
  // Still there because of not complete S/P GW split
  // std::shared_ptr<itti_s11_release_access_bearers_request>          msg;
  uint64_t gtpc_tx_id_;
};
//------------------------------------------------------------------------------
// Handles Release Access Bearers Request
class ReleaseAccessBearersRequestProcedure : public SpgwcProcedure {
 public:
  explicit ReleaseAccessBearersRequestProcedure(
      const EndPoint& t_remote_endpoint, const teid_t t_local_teid,
      const uint64_t t_trxn_id)
      : SpgwcProcedure(t_trxn_id),
        s11_remote_endpoint_(t_remote_endpoint),
        s11_local_teid_(t_local_teid),
        s11_trigger_(),
        bearers_(),
        ctxt_(),
        cause_() {}
  int Run(std::unique_ptr<gtpv2c::Gtpv2cReleaseAccessBearersRequest> t_msg,
          std::shared_ptr<SpgwcContext> ctxt);
  void ProcessMsg(std::unique_ptr<pfcp::PfcpSessionModificationResponse> resp);

  std::unique_ptr<gtpv2c::Gtpv2cReleaseAccessBearersRequest> s11_trigger_;
  std::shared_ptr<gtpv2c::Gtpv2cReleaseAccessBearersResponse>
      s11_triggered_pending_;
  std::vector<std::shared_ptr<BearersToBeReleased>> bearers_;
  std::shared_ptr<SpgwcContext> ctxt_;
  cause_t cause_;
  const EndPoint s11_remote_endpoint_;
  const teid_t s11_local_teid_;
};

}  // namespace spgwc
#include "SpgwcContext.hpp"

#endif
