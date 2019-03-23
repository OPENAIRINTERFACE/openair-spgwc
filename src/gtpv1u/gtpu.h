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
/*! \file gtpu.h
* \brief
* \author Lionel Gauthier
* \company Eurecom
* \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_GTPU_SEEN
#define FILE_GTPU_SEEN

#include <endian.h>
#include <stdint.h>

struct gtpuhdr
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
  unsigned int pn:1;
  unsigned int s:1;
  unsigned int e:1;
  unsigned int spare:1;
  unsigned int pt:1;
    unsigned int version:3;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:3;
    unsigned int pt:1;
    unsigned int spare:1;
    unsigned int e:1;
    unsigned int s:1;
    unsigned int pn:1;
#else
# error "Please fix <bits/endian.h>"
#endif
    // Message Type: This field indicates the type of GTP-U message.
    uint8_t                         message_type;
    // Length: This field indicates the length in octets of the payload, i.e. the rest of the packet following the mandatory
    // part of the GTP header (that is the first 8 octets). The Sequence Number, the N-PDU Number or any Extension
    // headers shall be considered to be part of the payload, i.e. included in the length count.
    uint16_t                        message_length;
    // Tunnel Endpoint Identifier (TEID): This field unambiguously identifies a tunnel endpoint in the receiving
    // GTP-U protocol entity. The receiving end side of a GTP tunnel locally assigns the TEID value the transmitting
    // side has to use. The TEID value shall be assigned in a non-predictable manner for PGW S5/S8/S2a/S2b
    // interfaces (see 3GPP TS 33.250 [32]). The TEID shall be used by the receiving entity to find the PDP context,
    // except for the following cases:
    // -) The Echo Request/Response and Supported Extension Headers notification messages, where the Tunnel
    //    Endpoint Identifier shall be set to all zeroes
    // -) The Error Indication message where the Tunnel Endpoint Identifier shall be set to all zeros.
    uint32_t                        teid;


    /*The options start here. */
  };
#endif
