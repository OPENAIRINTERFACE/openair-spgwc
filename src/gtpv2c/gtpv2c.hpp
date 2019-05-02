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

/*! \file gtpv2c.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_GTPV2C_HPP_SEEN
#define FILE_GTPV2C_HPP_SEEN

#include "3gpp_29.274.hpp"
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
#include "msg_gtpv2c.hpp"

namespace gtpv2c {

class gtpv2c_procedure {
public:
  std::shared_ptr<gtpv2c_msg> retry_msg;
  boost::asio::ip::udp::endpoint remote_endpoint;
  timer_id_t   retry_timer_id;
  timer_id_t   proc_cleanup_timer_id;
  uint64_t                    gtpc_tx_id;
  uint8_t                     initial_msg_type; // sent or received
  uint8_t                     triggered_msg_type; // sent or received
  uint8_t                     retry_count;
  // Could add customized N3, and customized T3:
  // T3-RESPONSE timer and N3-REQUESTS counter setting is implementation
  // dependent. That is, the timers and counters may be configurable per procedure. Multileg communications (e.g. Create
  // Session Requests and Responses) however require longer timer values and possibly a higher number of retransmission
  // attempts compared to single leg communication.
  gtpv2c_procedure() :
      retry_msg(),
      remote_endpoint(),
      retry_timer_id(0),
      proc_cleanup_timer_id(0),
      gtpc_tx_id(0),
      initial_msg_type(0),
      triggered_msg_type(0),
      retry_count(0) {}

  gtpv2c_procedure(const gtpv2c_procedure& p) :
    retry_msg(p.retry_msg),
    remote_endpoint(p.remote_endpoint),
    retry_timer_id(p.retry_timer_id),
    proc_cleanup_timer_id(p.proc_cleanup_timer_id),
    gtpc_tx_id(p.gtpc_tx_id),
    initial_msg_type(p.initial_msg_type),
    triggered_msg_type(p.triggered_msg_type),
    retry_count(p.retry_count) {}
};

class gtpv2c_stack;

class udp_server
{
public:
  udp_server(boost::asio::io_service& io_service, boost::asio::ip::address address, const unsigned short port_num)
    : app(nullptr), local_address_(address), socket_(io_service, boost::asio::ip::udp::endpoint(address, port_num))
  {
    boost::asio::socket_base::reuse_address option(true);
    socket_.set_option(option);
    Logger::udp().debug( "udp_server::udp_server(%s:%d) for GTPv2-C", address.to_string().c_str(), port_num);
  }

  void async_send_to(boost::shared_ptr<std::string> message, boost::asio::ip::udp::endpoint remote_endpoint)
  {
    Logger::udp().trace( "udp_server::async_send_to(%s:%d) %d bytes", remote_endpoint.address().to_string().c_str(), remote_endpoint.port(), message.get()->size());
    socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint,
              boost::bind(&udp_server::handle_send, this, message,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
  }

  void start_receive(gtpv2c_stack * gtp_stack)
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


  gtpv2c_stack*     app;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint remote_endpoint_;
  boost::asio::ip::address local_address_;
  boost::array<char, 4096> recv_buffer_;
};

enum gtpv2c_transaction_action {
  DELETE_TX = 0,
  CONTINUE_TX
};

class gtpv2c_stack {
#define GTPV2C_T3_RESPONSE_MS            1000
#define GTPV2C_N3_REQUESTS               3
#define GTPV2C_PROC_TIME_OUT_MS          ((GTPV2C_T3_RESPONSE_MS) * (GTPV2C_N3_REQUESTS + 1) + 1000)

protected:
  uint32_t                          id;
  udp_server                        udp_s;
  udp_server                        udp_s_allocated;

  // seems no need for std::atomic_uint32_t
  uint32_t                        seq_num;
  uint32_t                        restart_counter;

  std::map<uint64_t, uint32_t>                gtpc_tx_id2seq_num;
  std::map<timer_id_t, uint32_t>  proc_cleanup_timers;
  std::map<timer_id_t, uint32_t>  msg_out_retry_timers;
  std::map<uint32_t , gtpv2c_procedure>       pending_procedures;

  boost::array<char, 4096> send_buffer;
  static const char* msg_type2cstr[256];

  uint32_t get_next_seq_num();

  static uint64_t generate_gtpc_tx_id() {
    return util::uint_uid_generator<uint64_t>::get_instance().get_uid();
  }

  static bool check_initial_message_type(const uint8_t initial);
  static bool check_triggered_message_type(const uint8_t initial, const uint8_t triggered);
  void start_proc_cleanup_timer(gtpv2c_procedure& p, uint32_t time_out_milli_seconds, const task_id_t& task_id, const uint32_t& seq_num);
  void start_msg_retry_timer(gtpv2c_procedure& p, uint32_t time_out_milli_seconds, const task_id_t& task_id, const uint32_t& seq_num);
  void stop_msg_retry_timer(gtpv2c_procedure& p);
  void stop_msg_retry_timer(timer_id_t& t);
  void stop_proc_cleanup_timer(gtpv2c_procedure& p);
  void notify_ul_error(const gtpv2c_procedure& p, const cause_value_e cause);

public:
  static const uint8_t version = 2;
  gtpv2c_stack(const std::string& ip_address, const unsigned short port_num);
  virtual void handle_receive(char* recv_buffer, const std::size_t bytes_transferred, boost::asio::ip::udp::endpoint& remote_endpoint);
  void handle_receive_message_cb(const gtpv2c_msg& msg, const boost::asio::ip::udp::endpoint& remote_endpoint, const task_id_t& task_id, bool& error, uint64_t& gtpc_tx_id);

  // Path mangement messages
  virtual uint32_t send_initial_message(const boost::asio::ip::udp::endpoint& dest, const gtpv2c_echo_request& gtp_ies, const task_id_t& task_id, const uint64_t gtp_tx_id);
  virtual void send_triggered_message(const boost::asio::ip::udp::endpoint& dest, const gtpv2c_echo_response& gtp_ies, const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a = DELETE_TX);

  // Tunnel management messages
  virtual uint32_t send_initial_message(const boost::asio::ip::udp::endpoint& dest, const teid_t teid, const gtpv2c_create_session_request& gtp_ies, const task_id_t& task_id, const uint64_t gtp_tx_id);
  virtual uint32_t send_initial_message(const boost::asio::ip::udp::endpoint& dest, const teid_t teid, const gtpv2c_delete_session_request& gtp_ies, const task_id_t& task_id, const uint64_t gtp_tx_id);
  virtual uint32_t send_initial_message(const boost::asio::ip::udp::endpoint& dest, const teid_t teid, const gtpv2c_modify_bearer_request& gtp_ies, const task_id_t& task_id, const uint64_t gtp_tx_id);
  virtual uint32_t send_initial_message(const boost::asio::ip::udp::endpoint& dest, const teid_t teid, const gtpv2c_release_access_bearers_request& gtp_ies, const task_id_t& task_id, const uint64_t gtp_tx_id);

  virtual void send_triggered_message(const boost::asio::ip::udp::endpoint& dest, const teid_t teid, const gtpv2c_create_session_response& gtp_ies, const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a = DELETE_TX);
  virtual void send_triggered_message(const boost::asio::ip::udp::endpoint& dest, const teid_t teid, const gtpv2c_delete_session_response& gtp_ies, const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a = DELETE_TX);
  virtual void send_triggered_message(const boost::asio::ip::udp::endpoint& dest, const teid_t teid, const gtpv2c_modify_bearer_response& gtp_ies, const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a = DELETE_TX);
  virtual void send_triggered_message(const boost::asio::ip::udp::endpoint& dest, const teid_t teid, const gtpv2c_release_access_bearers_response& gtp_ies, const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a = DELETE_TX);

  void time_out_event(const uint32_t timer_id, const task_id_t& task_id, bool &error);
};
} // namespace gtpv2c

#endif /* FILE_GTPV2C_HPP_SEEN */
