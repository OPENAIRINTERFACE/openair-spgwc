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

/*! \file conversions.h
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_EPC_CONVERSIONS_HPP_SEEN
#define FILE_EPC_CONVERSIONS_HPP_SEEN
#include "common_types.h"
#include "3gpp_23.003.h"
#include "3gpp_24.008.h"
#include "3gpp_29.274.h"
#include "EpsQualityOfService.h"

namespace oai {
namespace cn {
namespace util {

inline void build_fteid(core::fteid_t& fteid, const core::ip_address_t ip,const teid_t teid) {

  fteid.teid = teid;
  fteid.ipv4 = ip.ipv4;
  fteid.ipv4_address = ip.address;
}
#define FTEID_T_2_IP_ADDRESS_T(fte_p,ip_p) \
do { \
    (ip_p)->ipv4 = false; \
    (ip_p)->ipv6 = false; \
    if ((fte_p)->ipv4) { \
      (ip_p)->ipv4 = true; \
      (ip_p)->address.ipv4_address.s_addr = (fte_p)->ipv4_address.s_addr;         \
    } \
    else if ((fte_p)->ipv6) { \
      (ip_p)->ipv6 = true; \
      memcpy(&(ip_p)->address.ipv6_address, &(fte_p)->ipv6_address, sizeof((fte_p)->ipv6_address)); \
    } \
} while (0)

}
}
}

#endif /* FILE_CONVERSIONS_SEEN */
