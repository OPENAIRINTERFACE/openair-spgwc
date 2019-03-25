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

/*! \file msg_gtpv1u.hpp
  \brief
  \author Sebastien ROUX, Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/


#ifndef MSG_GTPV1U_HPP_INCLUDED_
#define MSG_GTPV1U_HPP_INCLUDED_

#include "3gpp_29.274.h"
#include "3gpp_29.281.h"
#include "common_defs.h"

#include <vector>

namespace oai::cn::proto::gtpv1u {

class gtpv1u_ies_container {
public:
  static const uint8_t msg_id = 0;

  virtual bool get(core::recovery_t& v) const {throw gtpu_msg_illegal_ie_exception(0, GTPU_IE_RECOVERY);}
  virtual bool get(core::tunnel_endpoint_identifier_data_i_t& v) const {throw gtpu_msg_illegal_ie_exception(0, GTPU_IE_TUNNEL_ENDPOINT_IDENTIFIER_DATA_I);}
  virtual bool get(core::gtp_u_peer_address_t& v) const {throw gtpu_msg_illegal_ie_exception(0, GTPU_IE_GTP_U_PEER_ADDRESS);}
  virtual bool get(core::extension_header_type_list_t& v) const {throw gtpu_msg_illegal_ie_exception(0, GTPU_IE_EXTENSION_HEADER_TYPE_LIST);}
  virtual bool get(core::private_extension_t& v) const {throw gtpu_msg_illegal_ie_exception(0, GTPU_IE_PRIVATE_EXTENSION);}

  virtual void set(const core::recovery_t& v) {throw gtpu_msg_illegal_ie_exception(0, GTPU_IE_RECOVERY);}
  virtual void set(const core::tunnel_endpoint_identifier_data_i_t& v) {throw gtpu_msg_illegal_ie_exception(0, GTPU_IE_TUNNEL_ENDPOINT_IDENTIFIER_DATA_I);}
  virtual void set(const core::gtp_u_peer_address_t& v) {throw gtpu_msg_illegal_ie_exception(0, GTPU_IE_GTP_U_PEER_ADDRESS);}
  virtual void set(const core::extension_header_type_list_t& v) {throw gtpu_msg_illegal_ie_exception(0, GTPU_IE_EXTENSION_HEADER_TYPE_LIST);}
  virtual void set(const core::private_extension_t& v) {throw gtpu_msg_illegal_ie_exception(0, GTPU_IE_PRIVATE_EXTENSION);}
  virtual ~gtpv1u_ies_container() {};
};

//------------------------------------------------------------------------------
class gtpv1u_echo_request : public gtpv1u_ies_container {
public:
  static const uint8_t msg_id = GTPU_ECHO_REQUEST;

  std::pair<bool, core::private_extension_t>     private_extension;

  gtpv1u_echo_request(): private_extension() {}
  gtpv1u_echo_request(const gtpv1u_echo_request& i) : private_extension(i.private_extension) {}
  gtpv1u_echo_request& operator=(gtpv1u_echo_request other)
  {
    std::swap(private_extension, other.private_extension);
    return *this;
  }
  static const char* get_msg_name() {return "GTPU_ECHO_REQUEST";};

  bool get(core::private_extension_t& v)  const {if (private_extension.first) {v = private_extension.second;return true;}return false;}

  void set(const core::private_extension_t& v) {private_extension.first = true; private_extension.second = v;}
};

//------------------------------------------------------------------------------
class gtpv1u_echo_response : public gtpv1u_ies_container {
public:
  static const uint8_t msg_id = GTPU_ECHO_RESPONSE;

  std::pair<bool, core::recovery_t>              recovery;
  std::pair<bool, core::private_extension_t>     private_extension;

  gtpv1u_echo_response(): recovery(), private_extension() {}

  gtpv1u_echo_response(const gtpv1u_echo_response& i): recovery(i.recovery), private_extension(i.private_extension) {}
  static const char* get_msg_name() {return "GTPU_ECHO_RESPONSE";};

  gtpv1u_echo_response& operator=(gtpv1u_echo_response other)
  {
    std::swap(recovery, other.recovery);
    std::swap(private_extension, other.private_extension);
    return *this;
  }

  bool get(core::recovery_t& v)  const {if (recovery.first) {v = recovery.second;return true;}return false;}
  bool get(core::private_extension_t& v)  const {if (private_extension.first) {v = private_extension.second;return true;}return false;}

  void set(const core::recovery_t& v) {recovery.first = true; recovery.second = v;}
  void set(const core::private_extension_t& v) {private_extension.first = true; private_extension.second = v;}
};

//------------------------------------------------------------------------------
class gtpv1u_error_indication : public gtpv1u_ies_container {
public:
  static const uint8_t msg_id = GTPU_ERROR_INDICATION;


  std::pair<bool, core::tunnel_endpoint_identifier_data_i_t>   tunnel_endpoint_identifier_data_i;
  std::pair<bool, core::gtp_u_peer_address_t>                  gtp_u_peer_address;
  std::pair<bool, core::private_extension_t>                   private_extension;

  gtpv1u_error_indication(): tunnel_endpoint_identifier_data_i(), gtp_u_peer_address(), private_extension() {}

  gtpv1u_error_indication(const gtpv1u_error_indication& i) :
    tunnel_endpoint_identifier_data_i(i.tunnel_endpoint_identifier_data_i),
    gtp_u_peer_address(i.gtp_u_peer_address),
    private_extension(i.private_extension) {}

  gtpv1u_error_indication& operator=(gtpv1u_error_indication other)
  {
    std::swap(tunnel_endpoint_identifier_data_i, other.tunnel_endpoint_identifier_data_i);
    std::swap(gtp_u_peer_address, other.gtp_u_peer_address);
    std::swap(private_extension, other.private_extension);
    return *this;
  }

  static const char* get_msg_name() {return "GTPU_ERROR_INDICATION";};

  bool get(core::tunnel_endpoint_identifier_data_i_t& v)  const {if (tunnel_endpoint_identifier_data_i.first) {v = tunnel_endpoint_identifier_data_i.second;return true;}return false;}
  bool get(core::gtp_u_peer_address_t& v)  const {if (gtp_u_peer_address.first) {v = gtp_u_peer_address.second;return true;}return false;}
  bool get(core::private_extension_t& v)  const {if (private_extension.first) {v = private_extension.second;return true;}return false;}

  void set(const core::tunnel_endpoint_identifier_data_i_t& v) {tunnel_endpoint_identifier_data_i.first = true; tunnel_endpoint_identifier_data_i.second = v;}
  void set(const core::gtp_u_peer_address_t& v) {gtp_u_peer_address.first = true; gtp_u_peer_address.second = v;}
  void set(const core::private_extension_t& v) {private_extension.first = true; private_extension.second = v;}
};
//------------------------------------------------------------------------------
class gtpv1u_supported_extension_headers_notification : public gtpv1u_ies_container {
public:
  static const uint8_t msg_id = GTPU_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION;

  std::pair<bool, core::extension_header_type_list_t>     extension_header_type_list;

  gtpv1u_supported_extension_headers_notification() :
    extension_header_type_list() {}

  gtpv1u_supported_extension_headers_notification(const gtpv1u_supported_extension_headers_notification& i) :
    extension_header_type_list(i.extension_header_type_list) {}

  gtpv1u_supported_extension_headers_notification& operator=(gtpv1u_supported_extension_headers_notification other)
  {
    std::swap(extension_header_type_list, other.extension_header_type_list);
    return *this;
  }

  static const char* get_msg_name() {return "GTPU_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION";};

  bool get(core::extension_header_type_list_t& v)  const {if (extension_header_type_list.first) {v = extension_header_type_list.second;return true;}return false;}

  void set(const core::extension_header_type_list_t& v) {extension_header_type_list.first = true; extension_header_type_list.second = v;}
};

//------------------------------------------------------------------------------
class gtpv1u_end_marker : public gtpv1u_ies_container {
public:
  static const uint8_t msg_id = GTPU_END_MARKER;

  std::pair<bool, core::private_extension_t>     private_extension;

  gtpv1u_end_marker() :
    private_extension() {}

  gtpv1u_end_marker(const gtpv1u_end_marker& i) :
    private_extension(i.private_extension) {}

  gtpv1u_end_marker& operator=(gtpv1u_end_marker other)
  {
    std::swap(private_extension, other.private_extension);
    return *this;
  }

  static const char* get_msg_name() {return "GTPU_END_MARKER";};

  bool get(core::private_extension_t& v)  const {if (private_extension.first) {v = private_extension.second;return true;}return false;}

  void set(const core::private_extension_t& v) {private_extension.first = true; private_extension.second = v;}
};

} // namespace gtpv1u

#endif /* MSG_GTPV1U_HPP_INCLUDED_ */
