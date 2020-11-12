/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
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
#ifndef FILE_PGWC_PROCEDURE_HPP_SEEN
#define FILE_PGWC_PROCEDURE_HPP_SEEN

/*! \file pgwc_procedure.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "3gpp_29.244.hpp"
#include "3gpp_29.274.hpp"
#include "itti_msg_s5s8.hpp"
#include "itti_msg_sx_restore.hpp"
#include "itti_msg_sxab.hpp"
#include "msg_gtpv2c.hpp"
#include "msg_pfcp.hpp"
#include "uint_generator.hpp"

#include <memory>
#include <mutex>
#include <set>

namespace pgwc {

class apn_context;
class pgw_context;
class pgw_pdn_connection;

//------------------------------------------------------------------------------
class pgw_procedure {
 private:
  static uint64_t trxn_id_generator;

  static uint64_t generate_trxn_id() {
    return util::uint_uid_generator<uint64_t>::get_instance().get_uid();
  }

 public:
  uint64_t trxn_id;
  pgw_procedure() { trxn_id = generate_trxn_id(); }
  explicit pgw_procedure(uint64_t tx) { trxn_id = tx; }
  virtual ~pgw_procedure() {}
  virtual itti_msg_type_t get_procedure_type() { return ITTI_MSG_TYPE_NONE; }
  virtual void handle_itti_msg(itti_sxab_session_establishment_response& resp) {
  }
  virtual void handle_itti_msg(itti_sxab_session_modification_response& resp) {}
  virtual void handle_itti_msg(itti_sxab_session_deletion_response& resp) {}
  virtual void handle_itti_msg(
      itti_s5s8_downlink_data_notification_acknowledge& resp) {}
};

class sgw_eps_bearer_context;
class sgw_pdn_connection;

//------------------------------------------------------------------------------
class sx_session_restore_procedure : public pgw_procedure {
 public:
  explicit sx_session_restore_procedure(
      std::set<pfcp::fseid_t>& sessions2restore)
      : pgw_procedure(),
        pending_sessions(sessions2restore),
        restored_sessions() {
    sessions2restore.clear();
  }

  int run();
  // void handle_itti_msg (itti_sxab_session_establishment_response& resp);

  //~sx_session_restore_procedure() {}

  std::set<pfcp::fseid_t> pending_sessions;
  std::set<pfcp::fseid_t> restored_sessions;
};

//------------------------------------------------------------------------------
class session_establishment_procedure : public pgw_procedure {
 public:
  explicit session_establishment_procedure(
      std::shared_ptr<pgw_pdn_connection>& sppc)
      : pgw_procedure(),
        ppc(sppc),
        sx_triggered(),
        s5_triggered_pending(),
        s5_trigger() {}

  int run(
      std::shared_ptr<itti_s5s8_create_session_request>& req,
      std::shared_ptr<itti_s5s8_create_session_response>& resp,
      std::shared_ptr<pgwc::pgw_context> pc);
  void handle_itti_msg(itti_sxab_session_establishment_response& resp);

  //~session_establishment_procedure() {}

  std::shared_ptr<itti_s5s8_create_session_request> s5_trigger;
  std::shared_ptr<itti_s5s8_create_session_response> s5_triggered_pending;
  std::shared_ptr<itti_sxab_session_establishment_request> sx_triggered;
  std::shared_ptr<pgw_pdn_connection> ppc;
  std::shared_ptr<pgwc::pgw_context> pc;
};

//------------------------------------------------------------------------------
class modify_bearer_procedure : public pgw_procedure {
 public:
  explicit modify_bearer_procedure(std::shared_ptr<pgw_pdn_connection>& sppc)
      : pgw_procedure(),
        ppc(sppc),
        sx_triggered(),
        s5_triggered_pending(),
        s5_trigger() {}
  int run(
      std::shared_ptr<itti_s5s8_modify_bearer_request>& req,
      std::shared_ptr<itti_s5s8_modify_bearer_response>& resp,
      std::shared_ptr<pgwc::pgw_context> pc);
  void handle_itti_msg(itti_sxab_session_modification_response& resp);

  //~modify_bearer_procedure() {}

  std::shared_ptr<itti_s5s8_modify_bearer_request> s5_trigger;
  std::shared_ptr<itti_s5s8_modify_bearer_response> s5_triggered_pending;
  std::shared_ptr<itti_sxab_session_modification_request> sx_triggered;
  std::shared_ptr<pgw_pdn_connection> ppc;
  std::shared_ptr<pgwc::pgw_context> pc;
};
//------------------------------------------------------------------------------
class release_access_bearers_procedure : public pgw_procedure {
 public:
  explicit release_access_bearers_procedure(
      std::shared_ptr<pgw_pdn_connection>& sppc)
      : pgw_procedure(),
        ppc(sppc),
        sx_triggered(),
        s5_triggered_pending(),
        s5_trigger() {}
  int run(
      std::shared_ptr<itti_s5s8_release_access_bearers_request>& req,
      std::shared_ptr<itti_s5s8_release_access_bearers_response>& resp,
      std::shared_ptr<pgwc::pgw_context> pc);
  void handle_itti_msg(itti_sxab_session_modification_response& resp);

  //~release_access_bearers_procedure() {}

  std::shared_ptr<itti_s5s8_release_access_bearers_request> s5_trigger;
  std::shared_ptr<itti_s5s8_release_access_bearers_response>
      s5_triggered_pending;
  std::shared_ptr<itti_sxab_session_modification_request> sx_triggered;
  std::shared_ptr<pgw_pdn_connection> ppc;
  std::shared_ptr<pgwc::pgw_context> pc;
};
//------------------------------------------------------------------------------
class delete_session_procedure : public pgw_procedure {
 public:
  explicit delete_session_procedure(
      std::shared_ptr<pgw_context> spc,
      std::shared_ptr<pgw_pdn_connection>& sppc)
      : pgw_procedure(),
        ppc(sppc),
        pc(spc),
        sx_triggered(),
        s5_triggered_pending(),
        s5_trigger() {}

  int run(
      std::shared_ptr<itti_s5s8_delete_session_request>& req,
      std::shared_ptr<itti_s5s8_delete_session_response>& resp,
      std::shared_ptr<pgwc::pgw_context> pc);
  void handle_itti_msg(itti_sxab_session_deletion_response& resp);

  //~delete_session_procedure() {}

  std::shared_ptr<itti_s5s8_delete_session_request> s5_trigger;
  std::shared_ptr<itti_s5s8_delete_session_response> s5_triggered_pending;
  std::shared_ptr<itti_sxab_session_deletion_request> sx_triggered;
  std::shared_ptr<pgw_pdn_connection> ppc;
  std::shared_ptr<pgwc::pgw_context> pc;
};
//------------------------------------------------------------------------------
class downlink_data_report_procedure : public pgw_procedure {
 public:
  explicit downlink_data_report_procedure(
      std::shared_ptr<itti_sxab_session_report_request>& req)
      : pgw_procedure(req->trxn_id),
        ppc(),
        pc(),
        sx_trigger(req),
        s5_triggered(),
        ebi() {}
  int run(
      std::shared_ptr<pgwc::pgw_context> context,
      std::shared_ptr<pgwc::pgw_pdn_connection> pdn, const ebi_t& e);
  void handle_itti_msg(itti_s5s8_downlink_data_notification_acknowledge& resp);
  void notify_failure_to_peer(const pfcp::cause_t& cause);

  //~downlink_data_report_procedure() {}

  std::shared_ptr<itti_s5s8_downlink_data_notification> s5_triggered;
  std::shared_ptr<itti_sxab_session_report_request> sx_trigger;
  std::shared_ptr<pgw_pdn_connection> ppc;
  std::shared_ptr<pgwc::pgw_context> pc;
  ebi_t ebi;
};

}  // namespace pgwc
#include "pgw_context.hpp"

#endif
