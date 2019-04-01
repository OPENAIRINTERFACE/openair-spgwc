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

/*! \file gtpv1u.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_GTPV1U_HPP_SEEN
#define FILE_GTPV1U_HPP_SEEN

#include "3gpp_29.281.hpp"
#include "itti.hpp"
#include "msg_gtpv1u.hpp"
#include "thread_sched.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <stdint.h>
#include <string>
#include <system_error>
#include <thread>
#include <utility>
#include <vector>

namespace gtpv1u {

class gtpu_l4_stack;

class udp_server
{
public:
  udp_server(const struct in_addr& address, const uint16_t port_num)
    : app_(nullptr), port_(port_num)
  {
    socket_ = create_socket (address, port_);
    if (socket_ > 0) {
      Logger::udp().debug( "udp_server::udp_server(%s:%d)", conv::toString(address).c_str(), port_);
    } else {
      Logger::udp().error( "udp_server::udp_server(%s:%d)", conv::toString(address).c_str(), port_);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      throw std::system_error(socket_, std::generic_category(), "GTPV1-U socket creation failed!");
    }
  }

  udp_server(const struct in6_addr& address, const uint16_t port_num)
    : app_(nullptr), port_(port_num)
  {
    socket_ = create_socket (address, port_);
    if (socket_ > 0) {
      Logger::udp().debug( "udp_server::udp_server(%s:%d)", conv::toString(address).c_str(), port_);
    } else {
      Logger::udp().error( "udp_server::udp_server(%s:%d)", conv::toString(address).c_str(), port_);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      throw std::system_error(socket_, std::generic_category(), "GTPV1-U socket creation failed!");
    }
  }

  udp_server(char * address, const uint16_t port_num)
    : app_(nullptr), port_(port_num)
  {
    socket_ = create_socket (address, port_);
    if (socket_ > 0) {
      Logger::udp().debug( "udp_server::udp_server(%s:%d)", address, port_);
    } else {
      Logger::udp().error( "udp_server::udp_server(%s:%d)", address, port_);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      throw std::system_error(socket_, std::generic_category(), "GTPV1-U socket creation failed!");
    }
  }

  ~udp_server()
  {
    close(socket_);
  }

  void udp_read_loop(const util::thread_sched_params& thread_sched_params);

  void async_send_to(const char* send_buffer, const ssize_t num_bytes, const struct sockaddr_in& peer_addr)
  {
    //Logger::udp().trace( "udp_server::async_send_to(%s:%d) %d bytes", toString(peer_addr.sin_addr).c_str(), peer_addr.sin_port, num_bytes);
    ssize_t bytes_written = sendto (socket_, send_buffer, num_bytes, 0, (struct sockaddr *)&peer_addr, sizeof (struct sockaddr_in));
    if (bytes_written != num_bytes) {
      Logger::udp().error( "sendto failed(%d:%s)\n", errno, strerror (errno));
    }
  }

  void async_send_to(const char* send_buffer, const ssize_t num_bytes, const struct sockaddr_in6& peer_addr)
  {
    ssize_t bytes_written = sendto (socket_, send_buffer, num_bytes, 0, (struct sockaddr *)&peer_addr, sizeof (struct sockaddr_in6));
    if (bytes_written != num_bytes) {
      Logger::udp().error( "sendto failed(%d:%s)\n", errno, strerror (errno));
    }
  }


  void start_receive(gtpu_l4_stack * gtp_stack, const util::thread_sched_params& sched_params);

protected:
  int create_socket (const struct in_addr& address, const uint16_t port);
  int create_socket (const struct in6_addr& address, const uint16_t port);
  int create_socket (char * address, const uint16_t port_num);

 // void handle_receive(const int& error, std::size_t bytes_transferred);

  static void handle_send(const char *, /*buffer*/
      const int& /*error*/,
      std::size_t /*bytes_transferred*/)
  {
  }


  gtpu_l4_stack*    app_;
  std::thread       thread_;
  int               socket_;
  uint16_t          port_;
#define UDP_RECV_BUFFER_SIZE 8192
  char              recv_buffer_[UDP_RECV_BUFFER_SIZE];
};


class gtpu_l4_stack {
#define GTPV1U_T3_RESPONSE_MS            1000
#define GTPV1U_N3_REQUESTS               5
#define GTPV1U_PROC_TIME_OUT_MS          ((GTPV1U_T3_RESPONSE_MS) * (GTPV1U_N3_REQUESTS + 1))

protected:
  uint32_t                          id;
  udp_server                        udp_s;

  // seems no need for std::atomic_uint32_t
  uint32_t                        seq_num;
  uint32_t                        restart_counter;

  static bool check_initial_message_type(const uint8_t initial);
  static bool check_triggered_message_type(const uint8_t initial, const uint8_t triggered);

  uint32_t get_next_seq_num();

public:
  static const uint8_t version = 1;
  gtpu_l4_stack(const struct in_addr& address, const uint16_t port_num, const util::thread_sched_params& sched_params);
  gtpu_l4_stack(const struct in6_addr& address, const uint16_t port_num, const util::thread_sched_params& sched_params);
  gtpu_l4_stack(char * ip_address, const uint16_t port_num, const util::thread_sched_params& sched_params);
  virtual void handle_receive(char* recv_buffer, const std::size_t bytes_transferred, const struct sockaddr_storage& r_endpoint, const socklen_t& r_endpoint_addr_len);
  void handle_receive_message_cb(const gtpv1u_msg& msg, const struct sockaddr_storage& r_endpoint, const socklen_t& r_endpoint_addr_len, const task_id_t& task_id, bool &error, uint64_t& gtpc_tx_id);

  void send_g_pdu(const struct sockaddr_in& peer_addr, const teid_t teid, const char* payload, const ssize_t payload_len);
  void send_g_pdu(const struct sockaddr_in6& peer_addr, const teid_t teid, const char* payload, const ssize_t payload_len);

  void send_response(const gtpv1u_echo_response& gtp_ies);
  void send_indication(const gtpv1u_error_indication& gtp_ies);

};
} // namespace gtpv1u

#endif /* FILE_GTPV1U_HPP_SEEN */
