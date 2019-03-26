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
/*! \file gtpv1u.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "common_root_types.h"
#include "gtpu.h"
#include "gtpv1u.hpp"

#include <cstdlib>
#include <sched.h>

using namespace oai::cn::proto::gtpv1u;
using namespace oai::cn::core::itti;
using namespace std;

extern itti_mw *itti_inst;

//------------------------------------------------------------------------------
static std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}
//------------------------------------------------------------------------------
void udp_server::udp_read_loop(int cpu)
{
  socklen_t                               r_endpoint_addr_len = 0;
  struct sockaddr_storage                 r_endpoint = {};
  size_t                                  bytes_received = 0;

  if (cpu) {
    cpu_set_t cpuset;
    CPU_SET(cpu,&cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
  }
  int policy;
  struct sched_param sparam;
  memset(&sparam, 0, sizeof(sparam));
  sparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
  policy = SCHED_FIFO ;
  pthread_setschedparam(pthread_self(), policy, &sparam);

  while (1) {
    r_endpoint_addr_len = sizeof(struct sockaddr_storage);
    if ((bytes_received = recvfrom (socket_, recv_buffer_, UDP_RECV_BUFFER_SIZE, 0, (struct sockaddr *)&r_endpoint, &r_endpoint_addr_len)) > 0) {
      app_->handle_receive(recv_buffer_, bytes_received, r_endpoint, r_endpoint_addr_len);
    } else {
      Logger::udp().error( "Recvfrom failed %s\n", strerror (errno));
    }
  }
}
//------------------------------------------------------------------------------
int udp_server::create_socket (const struct in_addr &address, const uint16_t port)
{
  struct sockaddr_in                      addr = {};
  int                                     sd = 0;

  /*
   * Create UDP socket
   */
  if ((sd = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    /*
     * Socket creation has failed...
     */
    Logger::udp().error( "Socket creation failed (%s)\n", strerror (errno));
    return errno;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons (port);
  addr.sin_addr.s_addr = address.s_addr;

  std::string ipv4 = oai::cn::core::toString(address);
  Logger::udp().debug("Creating new listen socket on address %s and port %" PRIu16 "\n", ipv4.c_str(), port);

  if (bind (sd, (struct sockaddr *)&addr, sizeof (struct sockaddr_in)) < 0) {
    /*
     * Bind failed
     */
    Logger::udp().error("Socket bind failed (%s) for address %s and port %" PRIu16 "\n", strerror (errno), ipv4.c_str(), port);
    close (sd);
    return errno;
  }
  return sd;
}
//------------------------------------------------------------------------------
int udp_server::create_socket (const struct in6_addr &address, const uint16_t port)
{
  struct sockaddr_in6                     addr = {};
  int                                     sd = 0;

  /*
   * Create UDP socket
   */
  if ((sd = socket (AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    /*
     * Socket creation has failed...
     */
    Logger::udp().error( "Socket creation failed (%s)\n", strerror (errno));
    return errno;
  }

  addr.sin6_family = AF_INET6;
  addr.sin6_port = htons (port);
  addr.sin6_addr = address;

  std::string ipv6 = oai::cn::core::toString(address);
  Logger::udp().debug("Creating new listen socket on address %s and port %" PRIu16 "\n", ipv6.c_str(), port);

  if (bind (sd, (struct sockaddr *)&addr, sizeof (struct sockaddr_in6)) < 0) {
    /*
     * Bind failed
     */
    Logger::udp().error("Socket bind failed (%s) for address %s and port %" PRIu16 "\n", strerror (errno), ipv6.c_str(), port);
    close (sd);
    return errno;
  }
  return sd;
}
//------------------------------------------------------------------------------
int udp_server::create_socket (char * address, const uint16_t port_num)
{
  unsigned char buf_in_addr[sizeof(struct in6_addr)];
  if (inet_pton (AF_INET, address, buf_in_addr) == 1) {
    struct in_addr  addr4 = {};
    memcpy (&addr4, buf_in_addr, sizeof (struct in_addr));
    return create_socket(addr4, port_num);
  } else if (inet_pton (AF_INET6, address, buf_in_addr) == 1) {
    struct in6_addr addr6 = {};
    memcpy (&addr6, buf_in_addr, sizeof (struct in6_addr));
    return create_socket(addr6, port_num);
  } else {
    Logger::udp().error( "udp_server::create_socket(%s:%d)", address, port_num);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    throw std::system_error(socket_, std::generic_category(), "UDP socket creation failed!");
  }
}
//------------------------------------------------------------------------------
void udp_server::start_receive(gtpu_l4_stack * gtp_stack, const int cpu)
{
  app_ = gtp_stack;
  Logger::udp().trace( "udp_server::start_receive");
  thread_ = thread(&udp_server::udp_read_loop,this, cpu);
  thread_.detach();
}

////------------------------------------------------------------------------------
//void udp_server::handle_receive(const int& error, std::size_t bytes_transferred)
//{
//  if (!error) {
//    Logger::udp().trace( "udp_server::handle_receive on %s:%d from %s:%d",
//        socket_.local_endpoint().address().to_string().c_str(), socket_.local_endpoint().port(),
//        remote_endpoint_.address().to_string().c_str(), remote_endpoint_.port());
//    if (app_) {
//      app_->handle_receive(recv_buffer_.data(), bytes_transferred, remote_endpoint_);
//    } else {
//      Logger::udp().error( "No upper layer configured for handling UDP packet");
//    }
//    start_receive(app_);
//  } else {
//    Logger::udp().error( "udp_server::handle_receive err=%s/%d: %s", error.category().name(), error.value(), error.message());
//  }
//}

//------------------------------------------------------------------------------
gtpu_l4_stack::gtpu_l4_stack(const struct in_addr& address, const uint16_t port_num, const int cpu) :
    udp_s(udp_server(address, port_num))
{
  Logger::gtpv1_u().info( "gtpu_l4_stack created listening to %s:%d", oai::cn::core::toString(address).c_str(), port_num);

  id = 0;
  srand (time(NULL));
  seq_num = rand() & 0x7FFFFFFF;
  restart_counter = 0;
  udp_s.start_receive(this, cpu);
}
//------------------------------------------------------------------------------
gtpu_l4_stack::gtpu_l4_stack(const struct in6_addr& address, const uint16_t port_num, const int cpu) :
    udp_s(udp_server(address, port_num))
{
  Logger::gtpv1_u().info( "gtpu_l4_stack created listening to %s:%d", oai::cn::core::toString(address).c_str(), port_num);

  id = 0;
  srand (time(NULL));
  seq_num = rand() & 0x7FFFFFFF;
  restart_counter = 0;
  udp_s.start_receive(this, cpu);
}
//------------------------------------------------------------------------------
gtpu_l4_stack::gtpu_l4_stack(char * address, const uint16_t port_num, const int cpu) :
        udp_s(udp_server(address, port_num))
{
  Logger::gtpv1_u().info( "gtpu_l4_stack created listening to %s:%d", address, port_num);

  id = 0;
  srand (time(NULL));
  seq_num = rand() & 0x7FFFFFFF;
  restart_counter = 0;
  udp_s.start_receive(this, cpu);

}

//------------------------------------------------------------------------------
uint32_t gtpu_l4_stack::get_next_seq_num() {
  seq_num++;
  if (seq_num & 0x80000000) {
    seq_num = 0;
  }
  return seq_num;
}
//------------------------------------------------------------------------------
void gtpu_l4_stack::handle_receive(char* recv_buffer, const std::size_t bytes_transferred,  const struct sockaddr_storage& r_endpoint, const socklen_t& r_endpoint_addr_len)
{
  Logger::gtpv1_u().error( "TODO implement in derived class");
}
//------------------------------------------------------------------------------
bool gtpu_l4_stack::check_initial_message_type(const uint8_t initial)
{
  switch (initial) {
    case GTPU_ECHO_REQUEST:
    case GTPU_END_MARKER:
      return true;
      break;
    default:
      return false;
  }
}
//------------------------------------------------------------------------------
bool gtpu_l4_stack::check_triggered_message_type(const uint8_t initial, const uint8_t triggered)
{
  Logger::gtpv1_u().info( "check_triggered_message_type GTPV1-U msg type %d/%d", (int)initial, (int)triggered);
  switch (initial) {
    case GTPU_ECHO_REQUEST:
      if (triggered == GTPU_ECHO_RESPONSE) return true;
      return false;
      break;
    case GTPU_ERROR_INDICATION:
    case GTPU_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION:
       return true;
      break;
    default:
      return false;
  }
}
//------------------------------------------------------------------------------
void gtpu_l4_stack::handle_receive_message_cb(const gtpv1u_msg& msg, const struct sockaddr_storage& r_endpoint, const socklen_t& r_endpoint_addr_len, const task_id_t& task_id, bool &error, uint64_t& gtpc_tx_id)
{
}

//------------------------------------------------------------------------------
void gtpu_l4_stack::send_g_pdu(const struct sockaddr_in& peer_addr, const teid_t teid, const char* payload, const ssize_t payload_len)
{
  struct gtpuhdr *gtpuhdr = reinterpret_cast<struct gtpuhdr *>(reinterpret_cast<uintptr_t>(payload) - (uintptr_t)sizeof(struct gtpuhdr));
  gtpuhdr->spare = 0;
  gtpuhdr->e = 0;
  gtpuhdr->s = 0;
  gtpuhdr->pn = 0;
  gtpuhdr->pt = 1;
  gtpuhdr->version = 1;
  gtpuhdr->message_type = GTPU_G_PDU;
  gtpuhdr->message_length = htobe16(payload_len);
  gtpuhdr->teid = htobe32(teid);
  udp_s.async_send_to(reinterpret_cast<const char*>(gtpuhdr), payload_len + sizeof(struct gtpuhdr), peer_addr);
}
//------------------------------------------------------------------------------
void gtpu_l4_stack::send_g_pdu(const struct sockaddr_in6& peer_addr, const teid_t teid, const char* payload, const ssize_t payload_len)
{
  struct gtpuhdr *gtpuhdr = reinterpret_cast<struct gtpuhdr *>(reinterpret_cast<uintptr_t>(payload) - (uintptr_t)sizeof(struct gtpuhdr));
  gtpuhdr->spare = 0;
  gtpuhdr->e = 0;
  gtpuhdr->s = 0;
  gtpuhdr->pn = 0;
  gtpuhdr->pt = 1;
  gtpuhdr->version = 1;
  gtpuhdr->message_type = GTPU_G_PDU;
  gtpuhdr->message_length = htobe16(payload_len);
  gtpuhdr->teid = htobe32(teid);
  udp_s.async_send_to(reinterpret_cast<const char*>(gtpuhdr), payload_len + sizeof(struct gtpuhdr), peer_addr);
}
//------------------------------------------------------------------------------
void gtpu_l4_stack::send_response(const gtpv1u_echo_response& gtp_ies)
{
  std::ostringstream oss(std::ostringstream::binary);
  gtpv1u_msg msg(gtp_ies);
  uint32_t teid = UNASSIGNED_TEID;
  if (gtp_ies.get_teid(teid)) {
    msg.set_teid(teid);
  }
  uint16_t sn = 0;
  if (gtp_ies.get_sequence_number(sn)) {
    msg.set_sequence_number(sn);
  }
  msg.dump_to(oss);
  std::string bstream = oss.str();
  switch (gtp_ies.r_endpoint.ss_family) {
    case AF_INET: {
      const struct sockaddr_in * const sin = reinterpret_cast<const sockaddr_in* const>(&gtp_ies.r_endpoint);
      udp_s.async_send_to(reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), *sin);
    }
    break;
    case AF_INET6: {
      const struct sockaddr_in6 * const sin6 = reinterpret_cast<const sockaddr_in6* const>(&gtp_ies.r_endpoint);
      udp_s.async_send_to(reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), *sin6);
    }
    break;
    default:
      Logger::gtpv1_u().debug( "gtpu_l4_stack::send_response %s, no known peer addr", gtp_ies.get_msg_name());
  }
}
//------------------------------------------------------------------------------
void gtpu_l4_stack::send_indication(const gtpv1u_error_indication& gtp_ies)
{
  std::ostringstream oss(std::ostringstream::binary);
  gtpv1u_msg msg(gtp_ies);
  uint32_t teid = UNASSIGNED_TEID;
  if (gtp_ies.get_teid(teid)) {
    msg.set_teid(teid);
  }
  uint16_t sn = 0;
  if (gtp_ies.get_sequence_number(sn)) {
    msg.set_sequence_number(sn);
  }
  msg.dump_to(oss);
  std::string bstream = oss.str();
  switch (gtp_ies.r_endpoint.ss_family) {
    case AF_INET: {
      const struct sockaddr_in * const sin = reinterpret_cast<const sockaddr_in* const>(&gtp_ies.r_endpoint);
      udp_s.async_send_to(reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), *sin);
    }
    break;
    case AF_INET6: {
      const struct sockaddr_in6 * const sin6 = reinterpret_cast<const sockaddr_in6* const>(&gtp_ies.r_endpoint);
      udp_s.async_send_to(reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), *sin6);
    }
    break;
    default:
      Logger::gtpv1_u().debug( "gtpu_l4_stack::send_indication %s, no known peer addr", gtp_ies.get_msg_name());
  }
}
