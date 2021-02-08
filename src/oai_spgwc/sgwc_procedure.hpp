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
#ifndef FILE_SGWC_PROCEDURE_HPP_SEEN
#define FILE_SGWC_PROCEDURE_HPP_SEEN

/*! \file sgwc_procedure.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "itti_msg_s11.hpp"
#include "itti_msg_s5s8.hpp"
#include "msg_gtpv2c.hpp"
#include "uint_generator.hpp"

#include <list>
#include <memory>

namespace sgwc {

class sgw_eps_bearer_context;
class sgw_pdn_connection;

class sebc_procedure {
 private:
  static uint64_t generate_trxn_id() {
    return util::uint_uid_generator<uint64_t>::get_instance().get_uid();
  }
  uint64_t gtpc_tx_id;

 public:
  bool marked_for_removal;

  sebc_procedure() {
    gtpc_tx_id         = generate_trxn_id();
    marked_for_removal = false;
  }
  explicit sebc_procedure(uint64_t tx_id) {
    gtpc_tx_id         = tx_id;
    marked_for_removal = false;
  }
  virtual ~sebc_procedure() {}
  virtual itti_msg_type_t get_procedure_type() { return ITTI_MSG_TYPE_NONE; }
  virtual bool has_trxn_id(const uint64_t trxn_id) {
    return (trxn_id == gtpc_tx_id);
  }
  virtual uint64_t get_trxn_id() { return gtpc_tx_id; }
  virtual int run(std::shared_ptr<sgw_eps_bearer_context> ebc) {
    return RETURNerror;
  }
  virtual void handle_itti_msg(
      itti_s5s8_create_session_response& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc);
  virtual void handle_itti_msg(
      itti_s5s8_delete_session_response& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc);
  virtual void handle_itti_msg(
      itti_s5s8_modify_bearer_response& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc);
  virtual void handle_itti_msg(
      itti_s5s8_release_access_bearers_response& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc);
  virtual void handle_itti_msg(
      itti_s5s8_remote_peer_not_responding& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc);
  virtual void handle_itti_msg(
      itti_s5s8_downlink_data_notification& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc);
  virtual void handle_itti_msg(
      itti_s11_downlink_data_notification_acknowledge& resp);
};
//------------------------------------------------------------------------------
class sgw_eps_bearer_context;
class sgw_pdn_connection;

class create_session_request_procedure : public sebc_procedure {
 public:
  explicit create_session_request_procedure(
      itti_s11_create_session_request& msg)
      : sebc_procedure(msg.gtpc_tx_id), msg(msg), ebc(nullptr) {}
  int run(std::shared_ptr<sgw_eps_bearer_context> ebc);
  void handle_itti_msg(
      itti_s5s8_create_session_response& csresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc);

  // void handle_itti_msg (itti_s5s8_create_session_response& csresp,
  // std::shared_ptr<sgw_eps_bearer_context> ebc,
  // std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_delete_session_response& dsresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_modify_bearer_response& dsresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_release_access_bearers_response& dsresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_remote_peer_not_responding& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc);

  itti_s11_create_session_request msg;
  std::shared_ptr<sgw_eps_bearer_context> ebc;
};
//------------------------------------------------------------------------------
class pdn_bearers_to_be_xied {
 public:
  pdn_bearers_to_be_xied()
      : pdn(),
        bearer_contexts_to_be_modified(),
        bearer_contexts_to_be_removed(),
        msg(),
        gtpc_tx_id() {}
  pdn_bearers_to_be_xied(const pdn_bearers_to_be_xied& p)
      : pdn(p.pdn),
        bearer_contexts_to_be_modified(p.bearer_contexts_to_be_modified),
        bearer_contexts_to_be_removed(p.bearer_contexts_to_be_removed),
        msg(p.msg),
        gtpc_tx_id(p.gtpc_tx_id) {}
  std::shared_ptr<sgw_pdn_connection> pdn;
  std::vector<
      gtpv2c::bearer_context_to_be_modified_within_modify_bearer_request>
      bearer_contexts_to_be_modified;
  // Still there because of not complete S/P GW split
  std::vector<gtpv2c::bearer_context_to_be_removed_within_modify_bearer_request>
      bearer_contexts_to_be_removed;
  std::shared_ptr<itti_s5s8_modify_bearer_request> msg;
  uint64_t gtpc_tx_id;
};

class modify_bearer_request_procedure : public sebc_procedure {
 public:
  explicit modify_bearer_request_procedure(itti_s11_modify_bearer_request& msg)
      : sebc_procedure(msg.gtpc_tx_id),
        msg(msg),
        pdn_bearers(),
        null_pdn_bearers(),
        ebc(),
        bearer_contexts_modified(),
        bearer_contexts_marked_for_removal() {}

  bool has_trxn_id(const uint64_t trxn_id);
  int run(std::shared_ptr<sgw_eps_bearer_context> ebc);
  void handle_itti_msg(
      itti_s5s8_modify_bearer_response& s5resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> pdn);

  void handle_itti_msg(
      itti_s5s8_create_session_response& csresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_delete_session_response& dsresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  // void handle_itti_msg (itti_s5s8_modify_bearer_response& dsresp,
  // std::shared_ptr<sgw_eps_bearer_context> ebc,
  // std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_release_access_bearers_response& dsresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_remote_peer_not_responding& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}

  itti_s11_modify_bearer_request msg;
  std::vector<std::shared_ptr<pdn_bearers_to_be_xied>> pdn_bearers;
  pdn_bearers_to_be_xied null_pdn_bearers;
  std::vector<gtpv2c::bearer_context_modified_within_modify_bearer_response>
      bearer_contexts_modified;
  std::vector<
      gtpv2c::bearer_context_marked_for_removal_within_modify_bearer_response>
      bearer_contexts_marked_for_removal;
  std::shared_ptr<sgw_eps_bearer_context> ebc;
};
//------------------------------------------------------------------------------
class bearers_to_be_released {
 public:
  bearers_to_be_released() : pdn(), gtpc_tx_id() {}
  bearers_to_be_released(const bearers_to_be_released& p)
      : pdn(p.pdn), gtpc_tx_id(p.gtpc_tx_id) {}
  std::shared_ptr<sgw_pdn_connection> pdn;
  // Still there because of not complete S/P GW split
  // std::shared_ptr<itti_s5s8_release_access_bearers_request>          msg;
  uint64_t gtpc_tx_id;
};

class release_access_bearers_request_procedure : public sebc_procedure {
 public:
  explicit release_access_bearers_request_procedure(
      itti_s11_release_access_bearers_request& msg)
      : sebc_procedure(msg.gtpc_tx_id), msg(msg), bearers(), ebc(), cause() {}
  bool has_trxn_id(const uint64_t trxn_id);
  int run(std::shared_ptr<sgw_eps_bearer_context> ebc);
  void handle_itti_msg(
      itti_s5s8_release_access_bearers_response& s5resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> pdn);

  void handle_itti_msg(
      itti_s5s8_create_session_response& csresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_delete_session_response& dsresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_modify_bearer_response& dsresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  // void handle_itti_msg (itti_s5s8_release_access_bearers_response& dsresp,
  // std::shared_ptr<sgw_eps_bearer_context> ebc,
  // std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_remote_peer_not_responding& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}

  itti_s11_release_access_bearers_request msg;
  std::vector<std::shared_ptr<bearers_to_be_released>> bearers;
  std::shared_ptr<sgw_eps_bearer_context> ebc;
  cause_t cause;
};
//------------------------------------------------------------------------------
class delete_session_request_procedure : public sebc_procedure {
 public:
  delete_session_request_procedure(
      itti_s11_delete_session_request& msg,
      std::shared_ptr<sgw_pdn_connection>& pdn)
      : sebc_procedure(msg.gtpc_tx_id),
        msg(msg),
        pdn_connection(pdn),
        ebc(nullptr) {}
  int run(std::shared_ptr<sgw_eps_bearer_context> ebc);
  void handle_itti_msg(
      itti_s5s8_delete_session_response& dsresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection>& spc);

  void handle_itti_msg(
      itti_s5s8_create_session_response& csresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  // void handle_itti_msg (itti_s5s8_delete_session_response& dsresp,
  // std::shared_ptr<sgw_eps_bearer_context> ebc,
  // std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_modify_bearer_response& dsresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_release_access_bearers_response& dsresp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}
  void handle_itti_msg(
      itti_s5s8_remote_peer_not_responding& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}

  itti_s11_delete_session_request msg;
  std::shared_ptr<sgw_eps_bearer_context> ebc;
  std::shared_ptr<sgw_pdn_connection> pdn_connection;
};

//------------------------------------------------------------------------------
class downlink_data_notification_procedure : public sebc_procedure {
 public:
  explicit downlink_data_notification_procedure(
      itti_s5s8_downlink_data_notification& msg)
      : sebc_procedure(msg.gtpc_tx_id),
        msg(msg),
        s11_triggered(),
        ebc(),
        pdn_connection() {}
  int run(
      std::shared_ptr<sgw_eps_bearer_context> context,
      std::shared_ptr<sgw_pdn_connection> spc);
  void handle_itti_msg(itti_s11_downlink_data_notification_acknowledge& resp);
  void handle_itti_msg(
      itti_s5s8_remote_peer_not_responding& resp,
      std::shared_ptr<sgw_eps_bearer_context> ebc,
      std::shared_ptr<sgw_pdn_connection> spc) {}

  //~downlink_data_notification_procedure() {}

  itti_s5s8_downlink_data_notification msg;
  std::shared_ptr<itti_s11_downlink_data_notification> s11_triggered;
  std::shared_ptr<sgw_eps_bearer_context> ebc;
  std::shared_ptr<sgw_pdn_connection> pdn_connection;
};

}  // namespace sgwc
#include "sgwc_eps_bearer_context.hpp"

#endif
