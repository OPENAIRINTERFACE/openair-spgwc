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
/*! \file Endpoint.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_ENDPOINT_HPP_SEEN
#define FILE_ENDPOINT_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
#include <arpa/inet.h>
#include <inttypes.h>
#include <string.h>
#include <sys/socket.h>
//--C++ includes ---------------------------------------------------------------
//--Other includes -------------------------------------------------------------
#include <city.h>
#include "conversions.hpp"
//------------------------------------------------------------------------------
class EndPoint {
 public:
  struct sockaddr_storage addr_storage_;
  socklen_t addr_storage_len_;
  EndPoint()
      : addr_storage_(), addr_storage_len_(sizeof(struct sockaddr_storage)){};
  EndPoint(const EndPoint &e)
      : addr_storage_(e.addr_storage_),
        addr_storage_len_(e.addr_storage_len_){};
  EndPoint(const struct sockaddr_storage &addr, const socklen_t len)
      : addr_storage_(addr), addr_storage_len_(len){};
  EndPoint(const struct in_addr &addr, const uint16_t port) {
    struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr_storage_;
    addr_in->sin_family = AF_INET;
    addr_in->sin_port = htons(port);
    addr_in->sin_addr.s_addr = addr.s_addr;

    addr_storage_len_ = sizeof(struct sockaddr_in);
  };

  EndPoint(const struct in6_addr &addr6, const uint16_t port) {
    struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&addr_storage_;
    addr_in6->sin6_family = AF_INET6;
    addr_in6->sin6_port = htons(port);
    addr_in6->sin6_flowinfo = 0;
    memcpy(&addr_in6->sin6_addr, &addr6, sizeof(struct in6_addr));
    addr_in6->sin6_scope_id = 0;

    addr_storage_len_ = sizeof(struct sockaddr_in6);
  };

  uint16_t Port() const {
    return ntohs(((struct sockaddr_in *)&addr_storage_)->sin_port);
  }

  sa_family_t Family() const { return addr_storage_.ss_family; }

  std::string toString() const {
    std::string str;
    if (addr_storage_.ss_family == AF_INET) {
      struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr_storage_;
      str.append(conv::toString(addr_in->sin_addr));
      str.append(":").append(std::to_string(ntohs(addr_in->sin_port)));
    } else if (addr_storage_.ss_family == AF_INET6) {
      struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&addr_storage_;
      str.append(conv::toString(addr_in6->sin6_addr));
      str.append(":").append(std::to_string(ntohs(addr_in6->sin6_port)));
    }
    return str;
  }

  uint64_t GetAddrHash64() const {
    if (addr_storage_.ss_family == AF_INET) {
      const char *addr_in = (const char *)&addr_storage_;
      return CityHash64(addr_in, 4);
    } else if (addr_storage_.ss_family == AF_INET6) {
      const char *addr_in6 = (const char *)&addr_storage_;
      return CityHash64(addr_in6, 16);
    }
    return 0;
  }

  uint64_t GetAddrHash32() const {
    if (addr_storage_.ss_family == AF_INET) {
      const char *addr_in = (const char *)&addr_storage_;
      return CityHash32(addr_in, 4);
    } else if (addr_storage_.ss_family == AF_INET6) {
      const char *addr_in6 = (const char *)&addr_storage_;
      return CityHash32(addr_in6, 16);
    }
    return 0;
  }
};
#endif
