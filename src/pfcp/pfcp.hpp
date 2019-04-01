/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
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

/*! \file pfcp.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_PFCP_HPP_SEEN
#define FILE_PFCP_HPP_SEEN

#include "3gpp_29.274.h"
#include "3gpp_29.244.hpp"
#include "itti.hpp"
#include "uint_generator.hpp"

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "msg_pfcp.hpp"

namespace pfcp {

class pfcp_procedure {
public:
  std::shared_ptr<pfcp_msg> retry_msg;
  boost::asio::ip::udp::endpoint remote_endpoint;
  timer_id_t   retry_timer_id;
  timer_id_t   proc_cleanup_timer_id;
  uint64_t                    trxn_id;
  uint8_t                     initial_msg_type; // sent or received
  uint8_t                     triggered_msg_type; // sent or received
  uint8_t                     retry_count;

  pfcp_procedure() : retry_msg(),
    remote_endpoint(),
    retry_timer_id(0),
    proc_cleanup_timer_id(0),
    trxn_id(0),
    initial_msg_type(0),
    triggered_msg_type(0),
    retry_count(0) {}

  pfcp_procedure(const pfcp_procedure& p) : retry_msg(p.retry_msg),
      remote_endpoint(p.remote_endpoint),
      retry_timer_id(p.retry_timer_id),
      proc_cleanup_timer_id(p.proc_cleanup_timer_id),
      trxn_id(p.trxn_id),
      initial_msg_type(p.initial_msg_type),
      triggered_msg_type(p.triggered_msg_type),
      retry_count(p.retry_count) {}
};

class pfcp_l4_stack;

class udp_server
{
public:
  udp_server(boost::asio::io_service& io_service, boost::asio::ip::address address, const unsigned short port_num)
    : app(nullptr), local_address_(address), socket_(io_service, boost::asio::ip::udp::endpoint(address, port_num))
  {
    boost::asio::socket_base::reuse_address option(true);
    socket_.set_option(option);

    Logger::udp().debug( "udp_server::udp_server(%s:%d) for PFCP", address.to_string().c_str(), port_num);
  }

  void async_send_to(boost::shared_ptr<std::string> message, boost::asio::ip::udp::endpoint remote_endpoint)
  {
    //Logger::udp().trace( "udp_server::async_send_to(%s:%d) %d bytes", remote_endpoint.address().to_string().c_str(), remote_endpoint.port(), message.get()->size());
    socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint,
              boost::bind(&udp_server::handle_send, this, message,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
  }

  void start_receive(pfcp_l4_stack * gtp_stack)
  {
    app = gtp_stack;
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&udp_server::handle_receive, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

protected:

  void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred);

  void handle_send(boost::shared_ptr<std::string> /*message*/,
      const boost::system::error_code& /*error*/,
      std::size_t /*bytes_transferred*/)
  {
  }


  pfcp_l4_stack*     app;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint remote_endpoint_;
  boost::asio::ip::address local_address_;
  boost::array<char, 4096> recv_buffer_;
};

enum pfcp_transaction_action {
  DELETE_TX = 0,
  CONTINUE_TX
};

class pfcp_l4_stack {
#define PFCP_T1_RESPONSE_MS            1000
#define PFCP_N1_REQUESTS               3
#define PFCP_PROC_TIME_OUT_MS          ((PFCP_T1_RESPONSE_MS) * (PFCP_N1_REQUESTS + 1 + 1))

protected:
  uint32_t                          id;
  udp_server                        udp_s_8805;
  udp_server                        udp_s_allocated;

  // seems no need for std::atomic_uint32_t
  uint32_t                        seq_num;
  uint32_t                        restart_counter;

  std::map<uint64_t, uint32_t>               trxn_id2seq_num;
  std::map<timer_id_t, uint32_t>       proc_cleanup_timers;
  std::map<timer_id_t, uint32_t>       msg_out_retry_timers;
  std::map<uint32_t , pfcp_procedure>        pending_procedures;

  boost::array<char, 4096> send_buffer;
  static const char* msg_type2cstr[256];

  uint32_t get_next_seq_num();

  static uint64_t generate_trxn_id() {
    return util::uint_uid_generator<uint64_t>::get_instance().get_uid();
  }

  static bool check_request_type(const uint8_t initial);
  static bool check_response_type(const uint8_t initial, const uint8_t triggered);
  void start_proc_cleanup_timer(pfcp_procedure& p, uint32_t time_out_milli_seconds, const task_id_t& task_id, const uint32_t& seq_num);
  void start_msg_retry_timer(pfcp_procedure& p, uint32_t time_out_milli_seconds, const task_id_t& task_id, const uint32_t& seq_num);
  void stop_msg_retry_timer(pfcp_procedure& p);
  void stop_msg_retry_timer(timer_id_t& t);
  void stop_proc_cleanup_timer(pfcp_procedure& p);
  void notify_ul_error(const pfcp_procedure& p, const ::cause_value_e cause);

public:
  static const uint8_t version = 2;
  pfcp_l4_stack(const std::string& ip_address, const unsigned short port_num);
  virtual void handle_receive(char* recv_buffer, const std::size_t bytes_transferred, boost::asio::ip::udp::endpoint& remote_endpoint);
  void handle_receive_message_cb(const pfcp_msg& msg, const boost::asio::ip::udp::endpoint& remote_endpoint, const task_id_t& task_id, bool& error, uint64_t& trxn_id);

  // Node related messages
//  virtual uint32_t send_request(const boost::asio::ip::udp::endpoint& dest, const uint64_t seid, const pfcp_pfd_management_request& pfcp_ies, const task_id_t& task_id, const uint64_t trxn_id);
  virtual uint32_t send_request(const boost::asio::ip::udp::endpoint& dest, const pfcp_heartbeat_request& pfcp_ies, const task_id_t& task_id, const uint64_t trxn_id);
  virtual uint32_t send_request(const boost::asio::ip::udp::endpoint& dest, const pfcp_association_setup_request& pfcp_ies, const task_id_t& task_id, const uint64_t trxn_id);
//  virtual uint32_t send_request(const boost::asio::ip::udp::endpoint& dest, const uint64_t seid, const pfcp_association_update_request& pfcp_ies, const task_id_t& task_id, const uint64_t trxn_id);
  virtual uint32_t send_request(const boost::asio::ip::udp::endpoint& dest, const pfcp_association_release_request& pfcp_ies, const task_id_t& task_id, const uint64_t trxn_id);
//  virtual uint32_t send_request(const boost::asio::ip::udp::endpoint& dest, const uint64_t seid, const pfcp_node_report_request& pfcp_ies, const task_id_t& task_id, const uint64_t trxn_id);

  // session related messages
  virtual uint32_t send_request(const boost::asio::ip::udp::endpoint& dest, const uint64_t seid, const pfcp_session_establishment_request& pfcp_ies, const task_id_t& task_id, const uint64_t trxn_id);
  virtual uint32_t send_request(const boost::asio::ip::udp::endpoint& dest, const uint64_t seid, const pfcp_session_modification_request& pfcp_ies, const task_id_t& task_id, const uint64_t trxn_id);
  virtual uint32_t send_request(const boost::asio::ip::udp::endpoint& dest, const uint64_t seid, const pfcp_session_deletion_request& pfcp_ies, const task_id_t& task_id, const uint64_t trxn_id);
//  virtual uint32_t send_request(const boost::asio::ip::udp::endpoint& dest, const uint64_t seid, const pfcp_session_report_request& pfcp_ies, const task_id_t& task_id, const uint64_t trxn_id);

  // Node related messages
  virtual void send_response(const boost::asio::ip::udp::endpoint& dest, const pfcp_heartbeat_response& pfcp_ies, const uint64_t trxn_id, const pfcp_transaction_action& a = DELETE_TX);
  virtual void send_response(const boost::asio::ip::udp::endpoint& dest, const pfcp_association_setup_response& pfcp_ies, const uint64_t trxn_id, const pfcp_transaction_action& a = DELETE_TX);
  virtual void send_response(const boost::asio::ip::udp::endpoint& dest, const pfcp_association_release_response& pfcp_ies, const uint64_t trxn_id, const pfcp_transaction_action& a = DELETE_TX);

  // session related messages
  virtual void send_response(const boost::asio::ip::udp::endpoint& dest, const uint64_t seid, const pfcp_session_establishment_response& pfcp_ies, const uint64_t trxn_id, const pfcp_transaction_action& a = DELETE_TX);
  virtual void send_response(const boost::asio::ip::udp::endpoint& dest, const uint64_t seid, const pfcp_session_modification_response& pfcp_ies, const uint64_t trxn_id, const pfcp_transaction_action& a = DELETE_TX);
  virtual void send_response(const boost::asio::ip::udp::endpoint& dest, const uint64_t seid, const pfcp_session_deletion_response& pfcp_ies, const uint64_t trxn_id, const pfcp_transaction_action& a = DELETE_TX);

  void time_out_event(const uint32_t timer_id, const task_id_t& task_id, bool &error);
};
} // namespace pfcp

#endif /* FILE_PFCP_HPP_SEEN */
