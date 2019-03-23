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

/*! \file msg_gtpv2c.hpp
  \brief
  \author Sebastien ROUX, Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/


#ifndef MSG_GTPV2C_HPP_INCLUDED_
#define MSG_GTPV2C_HPP_INCLUDED_

#include "3gpp_29.274.h"
#include "common_defs.h"

#include <vector>

namespace oai::cn::proto::gtpv2c {

class bearer_context;

class gtpv2c_ies_container {
public:
  static const uint8_t msg_id = 0;

  virtual bool get(core::imsi_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_IMSI, __FILE__, __LINE__);}
  virtual bool get(core::cause_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_CAUSE, __FILE__, __LINE__);}
  virtual bool get(core::recovery_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_RECOVERY_RESTART_COUNTER, __FILE__, __LINE__);}
  virtual bool get(core::apn_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_ACCESS_POINT_NAME, __FILE__, __LINE__);}
  virtual bool get(core::ambr_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_AGGREGATE_MAXIMUM_BIT_RATE, __FILE__, __LINE__);}
  virtual bool get(core::ebi_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_EPS_BEARER_ID, __FILE__, __LINE__);}
  virtual bool get(core::ip_address_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_IP_ADDRESS, __FILE__, __LINE__);}
  virtual bool get(core::mei_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_MOBILE_EQUIPMENT_IDENTITY, __FILE__, __LINE__);}
  virtual bool get(core::msisdn_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_MSISDN, __FILE__, __LINE__);}
  virtual bool get(core::indication_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_INDICATION, __FILE__, __LINE__);}
  virtual bool get(core::protocol_configuration_options_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_PROTOCOL_CONFIGURATION_OPTIONS, __FILE__, __LINE__);}
  virtual bool get(core::paa_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_PDN_ADDRESS_ALLOCATION, __FILE__, __LINE__);}
  virtual bool get(core::bearer_qos_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_BEARER_QUALITY_OF_SERVICE, __FILE__, __LINE__);}
  virtual bool get(core::flow_qos_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_FLOW_QUALITY_OF_SERVICE, __FILE__, __LINE__);}
  virtual bool get(core::rat_type_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_RAT_TYPE, __FILE__, __LINE__);}
  virtual bool get(core::serving_network_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_SERVING_NETWORK, __FILE__, __LINE__);}
  virtual bool get(core::traffic_flow_template_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_EPS_BEARER_LEVEL_TRAFFIC_FLOW_TEMPLATE, __FILE__, __LINE__);}
  // TODO GTP_IE_TRAFFIC_AGGREGATE_DESCRIPTION
  virtual bool get(core::uli_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_USER_LOCATION_INFORMATION, __FILE__, __LINE__);}
  virtual bool get(core::fteid_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, __FILE__, __LINE__);}
  // TODO GTP_IE_TMSI
  // TODO GTP_IE_GLOBAL_CN_ID
  // TODO GTP_IE_S103_PDN_DATA_FORWARDING_INFO
  // TODO GTP_IE_S1U_DATA_FORWARDING_INFO
  virtual bool get(core::delay_value_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_DELAY_VALUE, __FILE__, __LINE__);}
  virtual bool get(bearer_context& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_BEARER_CONTEXT, __FILE__, __LINE__);}
  virtual bool get(core::charging_id_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_CHARGING_ID, __FILE__, __LINE__);}
  // TODO GTP_IE_CHARGING_CHARACTERISTICS
  // TODO  GTP_IE_TRACE_INFORMATION
  virtual bool get(core::bearer_flags_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_BEARER_FLAGS, __FILE__, __LINE__);}
  virtual bool get(core::pdn_type_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_PDN_TYPE, __FILE__, __LINE__);}
  virtual bool get(core::procedure_transaction_id_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_PROCEDURE_TRANSACTION_ID, __FILE__, __LINE__);}
  // TODO GTP_IE_MM_CONTEXT_GSM_KEY_AND_TRIPLETS
  // TODO GTP_IE_MM_CONTEXT_UMTS_KEY_USED_CIPHER_AND_QUINTUPLETS
  // TODO GTP_IE_MM_CONTEXT_GSM_KEY_USED_CIPHER_AND_QUINTUPLETS
  // TODO GTP_IE_MM_CONTEXT_UMTS_KEY_AND_QUINTUPLETS
  // TODO GTP_IE_MM_CONTEXT_EPS_SECURITY_CONTEXT_AND_QUADRUPLETS
  // TODO GTP_IE_MM_CONTEXT_UMTS_KEY_QUADRUPLETS_AND_QUINTUPLETS
  // TODO GTP_IE_PDN_CONNECTION
  // TODO GTP_IE_PDU_NUMBERS
  // TODO GTP_IE_PACKET_TMSI
  // TODO GTP_IE_P_TMSI_SIGNATURE
  // TODO GTP_IE_HOP_COUNTER
  virtual bool get(core::ue_time_zone_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_UE_TIME_ZONE, __FILE__, __LINE__);}
  // TODO GTP_IE_TRACE_REFERENCE
  // TODO GTP_IE_COMPLETE_REQUEST_MESSAGE
  // TODO GTP_IE_GUTI
  // TODO GTP_IE_FULLY_QUALIFIED_CONTAINER
  // TODO  GTP_IE_FULLY_QUALIFIED_CAUSE
  // TODO GTP_IE_PLMN_ID
  // TODO GTP_IE_TARGET_IDENTIFICATION
  // TODO  GTP_IE_PACKET_FLOW_ID
  // TODO GTP_IE_RAB_CONTEXT
  // TODO GTP_IE_SOURCE_RNC_PDCP_CONTEXT_INFO
  // TODO  GTP_IE_PORT_NUMBER
  virtual bool get(core::access_point_name_restriction_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_APN_RESTRICTION, __FILE__, __LINE__);}
  virtual bool get(core::selection_mode_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_SELECTION_MODE, __FILE__, __LINE__);}
  // TODO
  // TODO GTP_IE_SOURCE_IDENTIFICATION
  // TODO GTP_IE_CHANGE_REPORTING_ACTION
  virtual bool get(core::fq_csid_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_FQ_CSID, __FILE__, __LINE__);}
  // TODO GTP_IE_CHANNEL_NEEDED
  // TODO GTP_IE_EMLPP_PRIORITY
  virtual bool get(core::node_type_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_NODE_TYPE, __FILE__, __LINE__);}
  // TODO GTP_IE_FULLY_QUALIFIED_DOMAIN_NAME
  // TODO GTP_IE_TRANSACTION_IDENTIFIER
  // TODO GTP_IE_MBMS_SESSION_DURATION
  // TODO GTP_IE_MBMS_SERVICE_AREA
  // TODO GTP_IE_MBMS_SESSION_IDENTIFIER
  // TODO GTP_IE_MBMS_FLOW_IDENTIFIER
  // TODO GTP_IE_MBMS_IP_MULTICAST_DISTRIBUTION
  // TODO GTP_IE_MBMS_DISTRIBUTION_ACKNOWLEDGE
  // TODO GTP_IE_RFSP_INDEX
  virtual bool get(core::uci_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_USER_CSG_INFORMATION, __FILE__, __LINE__);}
  // TODO GTP_IE_CSG_REPORTING_ACTION
  // TODO GTP_IE_CSG_ID
  // TODO GTP_IE_CSG_MEMBERSHIP_INDICATION
  // TODO GTP_IE_SERVICE_INDICATOR
  // TODO GTP_IE_DETACH_TYPE
  virtual bool get(core::local_distinguished_name_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_LOCAL_DISTINGUISHED_NAME, __FILE__, __LINE__);}
  virtual bool get(core::node_features_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_NODE_FEATURES, __FILE__, __LINE__);}
  // TODO GTP_IE_MBMS_TIME_TO_DATA_TRANSFER
  // TODO GTP_IE_THROTTLING
  // TODO GTP_IE_ALLOCATION_RETENTION_PRIORITY
  virtual bool get(core::epc_timer_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_EPC_TIMER, __FILE__, __LINE__);}
  // TODO GTP_IE_SIGNALLING_PRIORITY_INDICATION
  // TODO GTP_IE_TMGI
  // TODO GTP_IE_ADDITIONAL_MM_CONTEXT_FOR_SRVCC
  // TODO GTP_IE_ADDITIONAL_FLAGS_FOR_SRVCC
  // TODO GTP_IE_MDT_CONFIGURATION
  // TODO GTP_IE_ADDITIONAL_PROTOCOL_CONFIGURATION_OPTIONS
  // TODO GTP_IE_ABSOLUTE_TIME_OF_MBMS_DATA_TRANSFER
  // TODO GTP_IE_H_E_NB_INFORMATION_REPORTING
  // TODO GTP_IE_IPV4_CONFIGURATION_PARAMETERS
  // TODO GTP_IE_CHANGE_TO_REPORT_FLAGS
  // TODO GTP_IE_ACTION_INDICATION
  // TODO GTP_IE_TWAN_IDENTIFIER
  // TODO GTP_IE_ULI_TIMESTAMP
  // TODO GTP_IE_MBMS_FLAGS
  virtual bool get(core::ran_nas_cause_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_RAN_NAS_CAUSE, __FILE__, __LINE__);}
  // TODO GTP_IE_CN_OPERATOR_SELECTION_ENTITY
  // TODO GTP_IE_TRUSTED_WLAN_MODE_INDICATION
  // TODO GTP_IE_NODE_NUMBER
  // TODO GTP_IE_NODE_IDENTIFIER
  // TODO GTP_IE_PRESENCE_REPORTING_AREA_ACTION
  // TODO GTP_IE_PRESENCE_REPORTING_AREA_INFORMATION
  // TODO GTP_IE_TWAN_IDENTIFIER_TIMESTAMP
  // TODO GTP_IE_OVERLOAD_CONTROL_INFORMATION
  // TODO GTP_IE_LOAD_CONTROL_INFORMATION
  // TODO GTP_IE_METRIC
  // TODO GTP_IE_SEQUENCE_NUMBER
  // TODO GTP_IE_APN_AND_RELATIVE_CAPACITY
  // TODO GTP_IE_PAGING_AND_SERVICE_INFORMATION
  // TODO GTP_IE_INTEGER_NUMBER
  // TODO GTP_IE_MILLISECOND_TIME_STAMP
  // TODO GTP_IE_MONITORING_EVENT_INFORMATION
  // TODO GTP_IE_ECGI_LIST
  // TODO GTP_IE_REMOTE_UE_CONTEXT
  // TODO GTP_IE_REMOTE_USER_ID
  // TODO GTP_IE_REMOTE_UE_IP_INFORMATION
  virtual bool get(core::ciot_optimizations_support_indication_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION, __FILE__, 
__LINE__);}
  // TODO GTP_IE_SCEF_PDN_CONNECTION
  // TODO GTP_IE_HEADER_COMPRESSION_CONFIGURATION
  virtual bool get(core::extended_protocol_configuration_options_t& v, const uint8_t instance = 0) const {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS, 
__FILE__, __LINE__);}
  // TODO GTP_IE_SERVING_PLMN_RATE_CONTROL
  // TODO GTP_IE_COUNTER
  // TODO GTP_IE_MAPPED_UE_USAGE_TYPE
  // TODO GTP_IE_SECONDARY_RAT_USAGE_DATA_REPORT
  // TODO GTP_IE_UP_FUNCTION_SELECTION_INDICATION_FLAGS
  // TODO GTP_IE_PRIVATE_EXTENSION
  // TODO GTP_IE_WLAN_OFFLOADABILITY_INDICATION
  // TODO GTP_IE_MAXIMUM_PACKET_LOSS



  virtual void set(const core::imsi_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_IMSI, __FILE__, __LINE__);}
  virtual void set(const core::cause_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_CAUSE, __FILE__, __LINE__);}
  virtual void set(const core::recovery_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_RECOVERY_RESTART_COUNTER, __FILE__, __LINE__);}
  virtual void set(const core::apn_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_ACCESS_POINT_NAME, __FILE__, __LINE__);}
  virtual void set(const core::ambr_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_AGGREGATE_MAXIMUM_BIT_RATE, __FILE__, __LINE__);}
  virtual void set(const core::ebi_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_EPS_BEARER_ID, __FILE__, __LINE__);}
  virtual void set(const core::ip_address_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_IP_ADDRESS, __FILE__, __LINE__);}
  virtual void set(const core::mei_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_MOBILE_EQUIPMENT_IDENTITY, __FILE__, __LINE__);}
  virtual void set(const core::msisdn_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_MSISDN, __FILE__, __LINE__);}
  virtual void set(const core::indication_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_INDICATION, __FILE__, __LINE__);}
  virtual void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_PROTOCOL_CONFIGURATION_OPTIONS, __FILE__, __LINE__);}
  virtual void set(const core::paa_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_PDN_ADDRESS_ALLOCATION, __FILE__, __LINE__);}
  virtual void set(const core::bearer_qos_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_BEARER_QUALITY_OF_SERVICE, __FILE__, __LINE__);}
  virtual void set(const core::flow_qos_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_FLOW_QUALITY_OF_SERVICE, __FILE__, __LINE__);}
  virtual void set(const core::rat_type_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_RAT_TYPE, __FILE__, __LINE__);}
  virtual void set(const core::serving_network_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_SERVING_NETWORK, __FILE__, __LINE__);}
  virtual void set(const core::traffic_flow_template_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_EPS_BEARER_LEVEL_TRAFFIC_FLOW_TEMPLATE, __FILE__, __LINE__);}
  // TODO GTP_IE_TRAFFIC_AGGREGATE_DESCRIPTION
  virtual void set(const core::uli_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_USER_LOCATION_INFORMATION, __FILE__, __LINE__);}
  virtual void set(const core::fteid_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, __FILE__, __LINE__);}
  // TODO GTP_IE_TMSI
  // TODO GTP_IE_GLOBAL_CN_ID
  // TODO GTP_IE_S103_PDN_DATA_FORWARDING_INFO
  // TODO GTP_IE_S1U_DATA_FORWARDING_INFO
  virtual void set(const core::delay_value_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_DELAY_VALUE, __FILE__, __LINE__);}
  virtual void set(const bearer_context& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_BEARER_CONTEXT, __FILE__, __LINE__);}
  virtual void set(const core::charging_id_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_CHARGING_ID, __FILE__, __LINE__);}
  // TODO GTP_IE_CHARGING_CHARACTERISTICS
  // TODO  GTP_IE_TRACE_INFORMATION
  virtual void set(const core::bearer_flags_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_BEARER_FLAGS, __FILE__, __LINE__);}
  virtual void set(const core::pdn_type_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_PDN_TYPE, __FILE__, __LINE__);}
  virtual void set(const core::procedure_transaction_id_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_PROCEDURE_TRANSACTION_ID, __FILE__, __LINE__);}
  // TODO GTP_IE_MM_CONTEXT_GSM_KEY_AND_TRIPLETS
  // TODO GTP_IE_MM_CONTEXT_UMTS_KEY_USED_CIPHER_AND_QUINTUPLETS
  // TODO GTP_IE_MM_CONTEXT_GSM_KEY_USED_CIPHER_AND_QUINTUPLETS
  // TODO GTP_IE_MM_CONTEXT_UMTS_KEY_AND_QUINTUPLETS
  // TODO GTP_IE_MM_CONTEXT_EPS_SECURITY_CONTEXT_AND_QUADRUPLETS
  // TODO GTP_IE_MM_CONTEXT_UMTS_KEY_QUADRUPLETS_AND_QUINTUPLETS
  // TODO GTP_IE_PDN_CONNECTION
  // TODO GTP_IE_PDU_NUMBERS
  // TODO GTP_IE_PACKET_TMSI
  // TODO GTP_IE_P_TMSI_SIGNATURE
  // TODO GTP_IE_HOP_COUNTER
  virtual void set(const core::ue_time_zone_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_UE_TIME_ZONE, __FILE__, __LINE__);}
  // TODO GTP_IE_TRACE_REFERENCE
  // TODO GTP_IE_COMPLETE_REQUEST_MESSAGE
  // TODO GTP_IE_GUTI
  // TODO GTP_IE_FULLY_QUALIFIED_CONTAINER
  // TODO  GTP_IE_FULLY_QUALIFIED_CAUSE
  // TODO GTP_IE_PLMN_ID
  // TODO GTP_IE_TARGET_IDENTIFICATION
  // TODO  GTP_IE_PACKET_FLOW_ID
  // TODO GTP_IE_RAB_CONTEXT
  // TODO GTP_IE_SOURCE_RNC_PDCP_CONTEXT_INFO
  // TODO  GTP_IE_PORT_NUMBER
  virtual void set(const core::access_point_name_restriction_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_APN_RESTRICTION, __FILE__, __LINE__);}
  virtual void set(const core::selection_mode_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_SELECTION_MODE, __FILE__, __LINE__);}
  // TODO
  // TODO GTP_IE_SOURCE_IDENTIFICATION
  // TODO GTP_IE_CHANGE_REPORTING_ACTION
  virtual void set(const core::fq_csid_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_FQ_CSID, __FILE__, __LINE__);}
  // TODO GTP_IE_CHANNEL_NEEDED
  // TODO GTP_IE_EMLPP_PRIORITY
  virtual void set(const core::node_type_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_NODE_TYPE, __FILE__, __LINE__);}
  // TODO GTP_IE_FULLY_QUALIFIED_DOMAIN_NAME
  // TODO GTP_IE_TRANSACTION_IDENTIFIER
  // TODO GTP_IE_MBMS_SESSION_DURATION
  // TODO GTP_IE_MBMS_SERVICE_AREA
  // TODO GTP_IE_MBMS_SESSION_IDENTIFIER
  // TODO GTP_IE_MBMS_FLOW_IDENTIFIER
  // TODO GTP_IE_MBMS_IP_MULTICAST_DISTRIBUTION
  // TODO GTP_IE_MBMS_DISTRIBUTION_ACKNOWLEDGE
  // TODO GTP_IE_RFSP_INDEX
  virtual void set(const core::uci_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_USER_CSG_INFORMATION, __FILE__, __LINE__);}
  // TODO GTP_IE_CSG_REPORTING_ACTION
  // TODO GTP_IE_CSG_ID
  // TODO GTP_IE_CSG_MEMBERSHIP_INDICATION
  // TODO GTP_IE_SERVICE_INDICATOR
  // TODO GTP_IE_DETACH_TYPE
  virtual void set(const core::local_distinguished_name_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_LOCAL_DISTINGUISHED_NAME, __FILE__, __LINE__);}  
  virtual void set(const core::node_features_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_NODE_FEATURES, __FILE__, __LINE__);}
  // TODO GTP_IE_MBMS_TIME_TO_DATA_TRANSFER
  // TODO GTP_IE_THROTTLING
  // TODO GTP_IE_ALLOCATION_RETENTION_PRIORITY
  virtual void set(const core::epc_timer_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_EPC_TIMER, __FILE__, __LINE__);}
  // TODO GTP_IE_SIGNALLING_PRIORITY_INDICATION
  // TODO GTP_IE_TMGI
  // TODO GTP_IE_ADDITIONAL_MM_CONTEXT_FOR_SRVCC
  // TODO GTP_IE_ADDITIONAL_FLAGS_FOR_SRVCC
  // TODO GTP_IE_MDT_CONFIGURATION
  // TODO GTP_IE_ADDITIONAL_PROTOCOL_CONFIGURATION_OPTIONS
  // TODO GTP_IE_ABSOLUTE_TIME_OF_MBMS_DATA_TRANSFER
  // TODO GTP_IE_H_E_NB_INFORMATION_REPORTING
  // TODO GTP_IE_IPV4_CONFIGURATION_PARAMETERS
  // TODO GTP_IE_CHANGE_TO_REPORT_FLAGS
  // TODO GTP_IE_ACTION_INDICATION
  // TODO GTP_IE_TWAN_IDENTIFIER
  // TODO GTP_IE_ULI_TIMESTAMP
  // TODO GTP_IE_MBMS_FLAGS
  virtual void set(const core::ran_nas_cause_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_RAN_NAS_CAUSE, __FILE__, __LINE__);}
  // TODO GTP_IE_CN_OPERATOR_SELECTION_ENTITY
  // TODO GTP_IE_TRUSTED_WLAN_MODE_INDICATION
  // TODO GTP_IE_NODE_NUMBER
  // TODO GTP_IE_NODE_IDENTIFIER
  // TODO GTP_IE_PRESENCE_REPORTING_AREA_ACTION
  // TODO GTP_IE_PRESENCE_REPORTING_AREA_INFORMATION
  // TODO GTP_IE_TWAN_IDENTIFIER_TIMESTAMP
  // TODO GTP_IE_OVERLOAD_CONTROL_INFORMATION
  // TODO GTP_IE_LOAD_CONTROL_INFORMATION
  // TODO GTP_IE_METRIC
  // TODO GTP_IE_SEQUENCE_NUMBER
  // TODO GTP_IE_APN_AND_RELATIVE_CAPACITY
  // TODO GTP_IE_PAGING_AND_SERVICE_INFORMATION
  // TODO GTP_IE_INTEGER_NUMBER
  // TODO GTP_IE_MILLISECOND_TIME_STAMP
  // TODO GTP_IE_MONITORING_EVENT_INFORMATION
  // TODO GTP_IE_ECGI_LIST
  // TODO GTP_IE_REMOTE_UE_CONTEXT
  // TODO GTP_IE_REMOTE_USER_ID
  // TODO GTP_IE_REMOTE_UE_IP_INFORMATION
  virtual void set(const core::ciot_optimizations_support_indication_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION, __FILE__, __LINE__);}
  // TODO GTP_IE_SCEF_PDN_CONNECTION
  // TODO GTP_IE_HEADER_COMPRESSION_CONFIGURATION
  virtual void set(const core::extended_protocol_configuration_options_t& v, const uint8_t instance = 0) {throw gtpc_msg_illegal_ie_exception(0, GTP_IE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS, __FILE__, __LINE__);}
  // TODO GTP_IE_SERVING_PLMN_RATE_CONTROL
  // TODO GTP_IE_COUNTER
  // TODO GTP_IE_MAPPED_UE_USAGE_TYPE
  // TODO GTP_IE_SECONDARY_RAT_USAGE_DATA_REPORT
  // TODO GTP_IE_UP_FUNCTION_SELECTION_INDICATION_FLAGS
  // TODO GTP_IE_PRIVATE_EXTENSION
  // TODO GTP_IE_WLAN_OFFLOADABILITY_INDICATION
  // TODO GTP_IE_MAXIMUM_PACKET_LOSS
  virtual ~gtpv2c_ies_container() {};
};

class bearer_context : public gtpv2c_ies_container {
public:
  std::pair<bool, core::ebi_t>                    eps_bearer_id;
  std::pair<bool, core::traffic_flow_template_t>  tft;
#define BEARER_CONTEXT_MAX_FTEID_INSTANCES 12
  std::pair<bool, core::fteid_t>                  fteid[BEARER_CONTEXT_MAX_FTEID_INSTANCES];
  std::pair<bool, core::bearer_qos_t>             bearer_level_qos;
  std::pair<bool, core::cause_t>                  cause;
  std::pair<bool, core::charging_id_t>            charging_id;
  std::pair<bool, core::bearer_flags_t>           bearer_flags;
  std::pair<bool, core::protocol_configuration_options_t>  pco;
  std::pair<bool, core::extended_protocol_configuration_options_t>  epco;
  std::pair<bool, core::ran_nas_cause_t>          ran_nas_cause;

  bearer_context() :
    eps_bearer_id(),
    tft(),
    bearer_level_qos(),
    cause(),
    charging_id(),
    bearer_flags(),
    pco(),
    epco(),
    ran_nas_cause()
  {
    for (int i = 0; i < BEARER_CONTEXT_MAX_FTEID_INSTANCES; i++) fteid[i] = {};
  }
  
  bearer_context(const bearer_context& b) :
    eps_bearer_id(b.eps_bearer_id),
    tft(b.tft),
    bearer_level_qos(b.bearer_level_qos),
    cause(b.cause),
    charging_id(b.charging_id),
    bearer_flags(b.bearer_flags),
    pco(b.pco),
    epco(b.epco),
    ran_nas_cause(b.ran_nas_cause)
  {
    for (int i = 0; i < BEARER_CONTEXT_MAX_FTEID_INSTANCES; i++) fteid[i] = b.fteid[i];
  }
  //virtual ~bearer_context() {};
  virtual void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id.first = true; eps_bearer_id.second = v;}
  virtual void set(const core::traffic_flow_template_t& v, const uint8_t instance = 0) {tft.first = true; tft.second = v;}
  virtual void set(const core::fteid_t& v, const uint8_t instance = 0) {
    if (instance < BEARER_CONTEXT_MAX_FTEID_INSTANCES) {
      fteid[instance].second = v;
      fteid[instance].first = true;
    } else {
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }
  virtual void set(const core::bearer_qos_t& v, const uint8_t instance = 0) {bearer_level_qos.first = true; bearer_level_qos.second = v;}
  virtual void set(const core::cause_t& v, const uint8_t instance = 0) {cause.first = true; cause.second = v;}
  virtual void set(const core::charging_id_t& v, const uint8_t instance = 0) {charging_id.first = true; charging_id.second = v;}
  virtual void set(const core::bearer_flags_t& v, const uint8_t instance = 0) {bearer_flags.first = true; bearer_flags.second = v;}
  virtual void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {pco.first = true; pco.second = v;}
  virtual void set(const core::extended_protocol_configuration_options_t& v, const uint8_t instance = 0) {epco.first = true; epco.second = v;}
  virtual void set(const core::ran_nas_cause_t& v, const uint8_t instance = 0) {ran_nas_cause.first = true; ran_nas_cause.second = v;}

  virtual bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (eps_bearer_id.first) {v = eps_bearer_id.second;return true;}return false;}
  virtual bool get(core::traffic_flow_template_t& v, const uint8_t instance = 0) const {if (tft.first) {v = tft.second;return true;}return false;}
  virtual bool get(core::fteid_t& v, const uint8_t instance = 0) const {if ((instance < BEARER_CONTEXT_MAX_FTEID_INSTANCES) && (fteid[instance].first)) {v = fteid[instance].second;return true;}return false;}
  virtual bool get(core::bearer_qos_t& v, const uint8_t instance = 0) const {if (bearer_level_qos.first) {v = bearer_level_qos.second;return true;}return false;}
  virtual bool get(core::cause_t& v, const uint8_t instance = 0) const {if (cause.first) {v = cause.second;return true;}return false;}
  virtual bool get(core::charging_id_t& v, const uint8_t instance = 0) const {if (charging_id.first) {v = charging_id.second;return true;}return false;}
  virtual bool get(core::bearer_flags_t& v, const uint8_t instance = 0) const {if (bearer_flags.first) {v = bearer_flags.second;return true;}return false;}
  virtual bool get(core::protocol_configuration_options_t& v, const uint8_t instance = 0) const {if (pco.first) {v = pco.second;return true;}return false;}
  virtual bool get(core::extended_protocol_configuration_options_t& v, const uint8_t instance = 0) const {if (epco.first) {v = epco.second;return true;}return false;}
  virtual bool get(core::ran_nas_cause_t& v, const uint8_t instance = 0) const {if (ran_nas_cause.first) {v = ran_nas_cause.second;return true;}return false;}
};
// ridiculous long class name...
class bearer_context_to_be_created_within_create_session_request : public gtpv2c_ies_container {
public:
  uint32_t           ie_presence_mask;
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_EPS_BEARER_ID        ((uint64_t)1)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_TFT                  ((uint64_t)1 << 1)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S1_U_ENB_FTEID       ((uint64_t)1 << 2)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S4_U_SGSN_FTEID      ((uint64_t)1 << 3)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S5_S8_U_SGW_FTEID    ((uint64_t)1 << 4)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S5_S8_U_PGW_FTEID    ((uint64_t)1 << 5)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S12_RNC_FTEID        ((uint64_t)1 << 6)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S2B_U_EPDG_FTEID     ((uint64_t)1 << 7)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S2A_U_TWAN_FTEID     ((uint64_t)1 << 8)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_BEARER_LEVEL_QOS     ((uint64_t)1 << 9)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S11_U_MME_FTEID      ((uint64_t)1 << 10)
  core::ebi_t                    eps_bearer_id;       ///< EBI,  Mandatory CSR
  core::traffic_flow_template_t  tft;                 ///< Bearer TFT, Optional CSR, This IE may be included on the S4/S11 and S5/S8 interfaces.
  core::fteid_t                  s1_u_enb_fteid;      ///< S1-U eNodeB F-TEID, Conditional CSR, This IE shall be included on the S11 interface for X2-based handover with SGW relocation.
  core::fteid_t                  s4_u_sgsn_fteid;      ///< S4-U SGSN F-TEID, Conditional CSR, This IE shall be included on the S4 interface if the S4-U interface is used.
  core::fteid_t                  s5_s8_u_sgw_fteid;   ///< S5/S8-U SGW F-TEID, Conditional CSR, This IE shall be included on the S5/S8 interface for an "eUTRAN Initial Attach",
                                                ///  a "PDP Context Activation" or a "UE Requested PDN Connectivity".
  core::fteid_t                  s5_s8_u_pgw_fteid;   ///< S5/S8-U PGW F-TEID, Conditional CSR, This IE shall be included on the S4 and S11 interfaces for the TAU/RAU/Handover
                                                /// cases when the GTP-based S5/S8 is used.
  core::fteid_t                  s12_rnc_fteid;       ///< S12 RNC F-TEID, Conditional Optional CSR, This IE shall be included on the S4 interface if the S12
                                                /// interface is used in the Enhanced serving RNS relocation with SGW relocation procedure.
  core::fteid_t                  s2b_u_epdg_fteid;    ///< S2b-U ePDG F-TEID, Conditional CSR, This IE shall be included on the S2b interface for an Attach
                                                /// with GTP on S2b, a UE initiated Connectivity to Additional PDN with GTP on S2b and a Handover to Untrusted Non-
                                                /// 3GPP IP Access with GTP on S2b.
  core::fteid_t                  s2a_u_twan_fteid;
  /* This parameter is received only if the QoS parameters have been modified */
  core::bearer_qos_t             bearer_level_qos;    ///< Bearer QoS, Mandatory CSR
  core::fteid_t                  s11_u_mme_fteid;

  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_EPS_BEARER_ID;}
  void set(const core::traffic_flow_template_t& v, const uint8_t instance = 0) {tft = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_TFT;}
  void set_s1_u_enb_fteid(const core::fteid_t& v) {s1_u_enb_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S1_U_ENB_FTEID;}
  void set_s4_u_sgsn_fteid(const core::fteid_t& v) {s4_u_sgsn_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S4_U_SGSN_FTEID;}
  void set_s5_s8_u_sgw_fteid(const core::fteid_t& v) {s5_s8_u_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S5_S8_U_SGW_FTEID;}
  void set_s5_s8_u_pgw_fteid(const core::fteid_t& v) {s5_s8_u_pgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S5_S8_U_PGW_FTEID;}
  void set_s12_rnc_fteid(const core::fteid_t& v) {s12_rnc_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S12_RNC_FTEID;}
  void set_s2b_u_epdg_fteid(const core::fteid_t& v) {s2b_u_epdg_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S2B_U_EPDG_FTEID;}
  void set_s2a_u_twan_fteid(const core::fteid_t& v) {s2a_u_twan_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S2A_U_TWAN_FTEID;}
  void set(const core::bearer_qos_t& v, const uint8_t instance = 0) {bearer_level_qos = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_BEARER_LEVEL_QOS;}
  void set_s11_u_mme_fteid(const core::fteid_t& v) {s11_u_mme_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S11_U_MME_FTEID;}
  void set(const core::fteid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_s1_u_enb_fteid(v); break;
    case 1: set_s4_u_sgsn_fteid(v); break;
    case 2: set_s5_s8_u_sgw_fteid(v); break;
    case 3: set_s5_s8_u_pgw_fteid(v); break;
    case 4: set_s12_rnc_fteid(v); break;
    case 5: set_s2b_u_epdg_fteid(v); break;
    case 6: set_s2a_u_twan_fteid(v); break;
    case 7: set_s11_u_mme_fteid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }

  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_EPS_BEARER_ID) {v = eps_bearer_id;return true;}return false;}
  bool get(core::traffic_flow_template_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_TFT) {v = tft;return true;}return false;}
  bool get_s1_u_enb_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S1_U_ENB_FTEID) {v = s1_u_enb_fteid;return true;}return false;}
  bool get_s4_u_sgsn_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S4_U_SGSN_FTEID) {v = s4_u_sgsn_fteid;return true;}return false;}
  bool get_s5_s8_u_sgw_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S5_S8_U_SGW_FTEID) {v = s5_s8_u_sgw_fteid;return true;}return false;}
  bool get_s5_s8_u_pgw_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S5_S8_U_PGW_FTEID) {v = s5_s8_u_pgw_fteid;return true;}return false;}
  bool get_s12_rnc_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S12_RNC_FTEID) {v = s12_rnc_fteid;return true;}return false;}
  bool get_s2b_u_epdg_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S2B_U_EPDG_FTEID) {v = s2b_u_epdg_fteid;return true;}return false;}
  bool get_s2a_u_twan_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S2A_U_TWAN_FTEID) {v = s2a_u_twan_fteid;return true;}return false;}
  bool get(core::bearer_qos_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_BEARER_LEVEL_QOS) {v = bearer_level_qos;return true;}return false;}
  bool get_s11_u_mme_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S11_U_MME_FTEID) {v = s11_u_mme_fteid;return true;}return false;}
  bool get(core::fteid_t& v, const uint8_t instance = 0) const {
    switch (instance) {
    case 0: return get_s1_u_enb_fteid(v); break;
    case 1: return get_s4_u_sgsn_fteid(v); break;
    case 2: return get_s5_s8_u_sgw_fteid(v); break;
    case 3: return get_s5_s8_u_pgw_fteid(v); break;
    case 4: return get_s12_rnc_fteid(v); break;
    case 5: return get_s2b_u_epdg_fteid(v); break;
    case 6: return get_s2a_u_twan_fteid(v); break;
    case 7: return get_s11_u_mme_fteid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }

  bearer_context_to_be_created_within_create_session_request()
  {
    ie_presence_mask = 0;
    eps_bearer_id = {};
    tft = {};
    s1_u_enb_fteid = {};
    s4_u_sgsn_fteid = {};
    s5_s8_u_sgw_fteid = {};
    s5_s8_u_pgw_fteid = {};
    s12_rnc_fteid = {};
    s2b_u_epdg_fteid = {};
    s2a_u_twan_fteid = {};
    bearer_level_qos = {};
    s11_u_mme_fteid = {};
  }

  explicit bearer_context_to_be_created_within_create_session_request(const bearer_context& b) : bearer_context_to_be_created_within_create_session_request()
  {
    ie_presence_mask = 0;
    if (b.get(eps_bearer_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_EPS_BEARER_ID;
    if (b.get(tft, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_TFT;
    if (b.get(s1_u_enb_fteid, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S1_U_ENB_FTEID;
    if (b.get(s4_u_sgsn_fteid, 1)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S4_U_SGSN_FTEID;
    if (b.get(s5_s8_u_sgw_fteid, 2)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S5_S8_U_SGW_FTEID;
    if (b.get(s5_s8_u_pgw_fteid, 3)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S5_S8_U_PGW_FTEID;
    if (b.get(s12_rnc_fteid, 4)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S12_RNC_FTEID;
    if (b.get(s2b_u_epdg_fteid, 5)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S2B_U_EPDG_FTEID;
    if (b.get(s2a_u_twan_fteid, 6)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S2A_U_TWAN_FTEID;
    if (b.get(bearer_level_qos, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_BEARER_LEVEL_QOS;
    if (b.get(s11_u_mme_fteid, 7)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S11_U_MME_FTEID;
  }
};

class bearer_context_to_be_removed_within_create_session_request {
public:
  uint32_t           ie_presence_mask;
#define GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_EPS_BEARER_ID        ((uint64_t)1)
#define GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S4_U_SGSN_FTEID      ((uint64_t)1 << 1)
  core::ebi_t eps_bearer_id;      ///< EPS Bearer ID, Mandatory
  core::fteid_t s4_u_sgsn_fteid;     ///< S4-U SGSN F-TEID, Conditional , redundant

  bearer_context_to_be_removed_within_create_session_request()
  {
    ie_presence_mask = 0;
    eps_bearer_id = {};
    s4_u_sgsn_fteid = {};
  }
  explicit bearer_context_to_be_removed_within_create_session_request(const bearer_context& b) : bearer_context_to_be_removed_within_create_session_request()
  {
    ie_presence_mask = 0;
    if (b.get(eps_bearer_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_EPS_BEARER_ID;
    if (b.get(s4_u_sgsn_fteid, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S4_U_SGSN_FTEID;
  }
  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_EPS_BEARER_ID;}
  void set_s4_u_sgsn_fteid(const core::fteid_t& v) {s4_u_sgsn_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S4_U_SGSN_FTEID;}
  void set(const core::fteid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_s4_u_sgsn_fteid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }

  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_EPS_BEARER_ID) {v = eps_bearer_id;return true;}return false;}
  bool get_s4_u_sgsn_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S4_U_SGSN_FTEID) {v = s4_u_sgsn_fteid;return true;}return false;}
  bool get(core::fteid_t& v, const uint8_t instance = 0) const {
    switch (instance) {
    case 0: return get_s4_u_sgsn_fteid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }
};

class gtpv2c_create_session_request : public gtpv2c_ies_container {
public:
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_IMSI                                ((uint64_t)1)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MSISDN                              ((uint64_t)1 << 1)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MEI                                 ((uint64_t)1 << 2)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_ULI                                 ((uint64_t)1 << 3)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SERVING_NETWORK                     ((uint64_t)1 << 4)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_RAT_TYPE                            ((uint64_t)1 << 5)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_INDICATION_FLAGS                    ((uint64_t)1 << 6)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE      ((uint64_t)1 << 7)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PGW_S5S8_ADDRESS_FOR_CONTROL_PLANE  ((uint64_t)1 << 8)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN                                 ((uint64_t)1 << 9)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SELECTION_MODE                      ((uint64_t)1 << 10)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PDN_TYPE                            ((uint64_t)1 << 11)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PAA                                 ((uint64_t)1 << 12)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN_RESTRICTION                     ((uint64_t)1 << 13)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN_AMBR                            ((uint64_t)1 << 14)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_LINKED_EPS_BEARER_ID                ((uint64_t)1 << 15)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TRUSTED_WLAN_MODE_INDICATION        ((uint64_t)1 << 16)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PCO                                 ((uint64_t)1 << 17)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_CREATED       ((uint64_t)1 << 18)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_REMOVED       ((uint64_t)1 << 19)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TRACE_INFORMATION                   ((uint64_t)1 << 20)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_RECOVERY                            ((uint64_t)1 << 21)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MME_FQ_CSID                         ((uint64_t)1 << 22)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SGW_FQ_CSID                         ((uint64_t)1 << 23)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_EPDG_FQ_CSID                        ((uint64_t)1 << 24)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TWAN_FQ_CSID                        ((uint64_t)1 << 25)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UE_TIME_ZONE                        ((uint64_t)1 << 26)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UCI                                 ((uint64_t)1 << 27)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_CHARGING_CHARACTERISTICS            ((uint64_t)1 << 28)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MME_S4_SGSN_LDN                     ((uint64_t)1 << 29)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SGW_LDN                             ((uint64_t)1 << 30)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_EPDG_LDN                            ((uint64_t)1 << 31)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TWAN_LDN                            ((uint64_t)1 << 32)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SIGNALLING_PRIORITY_INDICATION      ((uint64_t)1 << 33)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UE_LOCAL_IP_ADDRESS                 ((uint64_t)1 << 34)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UE_UDP_PORT                         ((uint64_t)1 << 35)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APCO                                ((uint64_t)1 << 36)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_HENB_LOCAL_IP_ADDRESS               ((uint64_t)1 << 37)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_HENB_UDP_PORT                       ((uint64_t)1 << 38)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MME_S4_SGSN_IDENTIFIER              ((uint64_t)1 << 39)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TWAN_IDENTIFIER                     ((uint64_t)1 << 40)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_EPDG_IP_ADDRESS                     ((uint64_t)1 << 41)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_CN_OPERATOR_SELECTION_ENTITY        ((uint64_t)1 << 42)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PRESENCE_REPORTING_AREA_INFORMATION ((uint64_t)1 << 43)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MMES4_SGSN_OVERLOAD_CONTROL_INFORMATION ((uint64_t)1 << 44)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION    ((uint64_t)1 << 45)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TWAN_EPDG_OVERLOAD_CONTROL_INFORMATION ((uint64_t)1 << 46)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_ORIGINATION_TIME_STAMP              ((uint64_t)1 << 47)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MAXIMUM_WAIT_TIME                   ((uint64_t)1 << 48)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_WLAN_LOCATION_INFORMATION           ((uint64_t)1 << 49)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_WLAN_LOCATION_TIMESTAMP             ((uint64_t)1 << 50)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_NBIFOM_CONTAINER                    ((uint64_t)1 << 51)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_REMOTE_UE_CONTEXT_CONNECTED         ((uint64_t)1 << 52)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_3GPP_AAA_SERVER_IDENTIFIER          ((uint64_t)1 << 53)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_EPCO                                ((uint64_t)1 << 54)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SERVING_PLMN_RATE_CONTROL           ((uint64_t)1 << 55)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MO_EXCEPTION_DATA_COUNTER           ((uint64_t)1 << 56)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UE_TCP_PORT                         ((uint64_t)1 << 57)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MAPPED_UE_USAGE_TYPE                ((uint64_t)1 << 58)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_ULI_FOR_SGW                         ((uint64_t)1 << 59)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SGW_U_NODE_NAME                     ((uint64_t)1 << 60)
#define GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PRIVATE_EXTENSION                   ((uint64_t)1 << 61)
  static const uint8_t msg_id = GTP_CREATE_SESSION_REQUEST;
  uint64_t           ie_presence_mask;

  core::imsi_t             imsi;                ///< The IMSI shall be included in the message on the S4/S11
  ///< interface, and on S5/S8 interface if provided by the
  ///< MME/SGSN, except for the case:
  ///<     - If the UE is emergency attached and the UE is UICCless.
  ///< The IMSI shall be included in the message on the S4/S11
  ///< interface, and on S5/S8 interface if provided by the
  ///< MME/SGSN, but not used as an identifier
  ///<     - if UE is emergency attached but IMSI is not authenticated.
  ///< The IMSI shall be included in the message on the S2b interface.

  core::msisdn_t           msisdn;              ///< For an E-UTRAN Initial Attach the IE shall be included
  ///< when used on the S11 interface, if provided in the
  ///< subscription data from the HSS.
  ///< For a PDP Context Activation procedure the IE shall be
  ///< included when used on the S4 interface, if provided in the
  ///< subscription data from the HSS.
  ///< The IE shall be included for the case of a UE Requested
  ///< PDN Connectivity, if the MME has it stored for that UE.
  ///< It shall be included when used on the S5/S8 interfaces if
  ///< provided by the MME/SGSN.
  ///< The ePDG shall include this IE on the S2b interface during
  ///< an Attach with GTP on S2b and a UE initiated Connectivity
  ///< to Additional PDN with GTP on S2b, if provided by the
  ///< HSS/AAA.

  core::mei_t              mei;                 ///< The MME/SGSN shall include the ME Identity (MEI) IE on
  ///< the S11/S4 interface:
  ///<     - If the UE is emergency attached and the UE is UICCless
  ///<     - If the UE is emergency attached and the IMSI is not authenticated
  ///< For all other cases the MME/SGSN shall include the ME
  ///< Identity (MEI) IE on the S11/S4 interface if it is available.
  ///< If the SGW receives this IE, it shall forward it to the PGW
  ///< on the S5/S8 interface.

  core::uli_t              uli;                 ///< This IE shall be included on the S11 interface for E-
  ///< UTRAN Initial Attach and UE-requested PDN Connectivity
  ///< procedures. It shall include ECGI&TAI. The MME/SGSN
  ///< shall also include it on the S11/S4 interface for
  ///< TAU/RAU/X2-Handover/Enhanced SRNS Relocation
  ///< procedure if the PGW has requested location information
  ///< change reporting and MME/SGSN support location
  ///< information change reporting. The SGW shall include this
  ///< IE on S5/S8 if it receives the ULI from MME/SGSN.

  core::serving_network_t   serving_network;     ///< This IE shall be included on the S4/S11, S5/S8 and S2b
  ///< interfaces for an E-UTRAN initial attach, a PDP Context
  ///< Activation, a UE requested PDN connectivity, an Attach
  ///< with GTP on S2b, a UE initiated Connectivity to Additional
  ///< PDN with GTP on S2b and a Handover to Untrusted Non-
  ///< 3GPP IP Access with GTP on S2b.

  core::rat_type_t         rat_type;            ///< This IE shall be set to the 3GPP access type or to the
  ///< value matching the characteristics of the non-3GPP access
  ///< the UE is using to attach to the EPS.
  ///< The ePDG may use the access technology type of the
  ///< untrusted non-3GPP access network if it is able to acquire
  ///< it; otherwise it shall indicate Virtual as the RAT Type.
  ///< See NOTE 3, NOTE 4.

  core::indication_t indication_flags;    ///< This IE shall be included if any one of the applicable flags
  ///< is set to 1.
  ///< Applicable flags are:
  ///<     - S5/S8 Protocol Type: This flag shall be used on
  ///<       the S11/S4 interfaces and set according to the
  ///<       protocol chosen to be used on the S5/S8
  ///<       interfaces.
  ///<
  ///<     - Dual Address Bearer Flag: This flag shall be used
  ///<       on the S2b, S11/S4 and S5/S8 interfaces and shall
  ///<       be set to 1 when the PDN Type, determined based
  ///<       on UE request and subscription record, is set to
  ///<       IPv4v6 and all SGSNs which the UE may be
  ///<       handed over to support dual addressing. This shall
  ///<       be determined based on node pre-configuration by
  ///<       the operator.
  ///<
  ///<     - Handover Indication: This flag shall be set to 1 on
  ///<       the S11/S4 and S5/S8 interface during an E-
  ///<       UTRAN Initial Attach or a UE Requested PDN
  ///<       Connectivity or aPDP Context Activation procedure
  ///<       if the PDN connection/PDP Context is handed-over
  ///<       from non-3GPP access.
  ///<       This flag shall be set to 1 on the S2b interface
  ///<       during a Handover to Untrusted Non-3GPP IP
  ///<       Access with GTP on S2b and IP address
  ///<       preservation is requested by the UE.
  ///<
  ///<       ....
  ///<     - Unauthenticated IMSI: This flag shall be set to 1
  ///<       on the S4/S11 and S5/S8 interfaces if the IMSI
  ///<       present in the message is not authenticated and is
  ///<       for an emergency attached UE.

  core::fteid_t            sender_fteid_for_cp; ///< Sender F-TEID for control plane (MME)

  core::fteid_t            pgw_s5s8_address_for_cp;  ///< PGW S5/S8 address for control plane or PMIP
  ///< This IE shall be sent on the S11 / S4 interfaces. The TEID
  ///< or GRE Key is set to "0" in the E-UTRAN initial attach, the
  ///< PDP Context Activation and the UE requested PDN
  ///< connectivity procedures.

  // ACCESS_POINT_NAME_MAX_LENGTH
  core::apn_t        apn; ///< Access Point Name

  core::selection_mode_t    selection_mode;      ///< Selection Mode
  ///< This IE shall be included on the S4/S11 and S5/S8
  ///< interfaces for an E-UTRAN initial attach, a PDP Context
  ///< Activation and a UE requested PDN connectivity.
  ///< This IE shall be included on the S2b interface for an Initial
  ///< Attach with GTP on S2b and a UE initiated Connectivity to
  ///< Additional PDN with GTP on S2b.
  ///< It shall indicate whether a subscribed APN or a non
  ///< subscribed APN chosen by the MME/SGSN/ePDG was
  ///< selected.
  ///< CO: When available, this IE shall be sent by the MME/SGSN on
  ///< the S11/S4 interface during TAU/RAU/HO with SGW
  ///< relocation.

  core::pdn_type_t         pdn_type;            ///< PDN Type
  ///< This IE shall be included on the S4/S11 and S5/S8
  ///< interfaces for an E-UTRAN initial attach, a PDP Context
  ///< Activation and a UE requested PDN connectivity.
  ///< This IE shall be set to IPv4, IPv6 or IPv4v6. This is based
  ///< on the UE request and the subscription record retrieved
  ///< from the HSS (for MME see 3GPP TS 23.401 [3], clause
  ///< 5.3.1.1, and for SGSN see 3GPP TS 23.060 [35], clause
  ///< 9.2.1). See NOTE 1.

  core::paa_t              paa;                 ///< PDN Address Allocation
  ///< This IE shall be included the S4/S11, S5/S8 and S2b
  ///< interfaces for an E-UTRAN initial attach, a PDP Context
  ///< Activation, a UE requested PDN connectivity, an Attach
  ///< with GTP on S2b, a UE initiated Connectivity to Additional
  ///< PDN with GTP on S2b and a Handover to Untrusted Non-
  ///< 3GPP IP Access with GTP on S2b. For PMIP-based
  ///< S5/S8, this IE shall also be included on the S4/S11
  ///< interfaces for TAU/RAU/Handover cases involving SGW
  ///< relocation.
  ///< The PDN type field in the PAA shall be set to IPv4, or IPv6
  ///< or IPv4v6 by MME, based on the UE request and the
  ///< subscription record retrieved from the HSS.
  ///< For static IP address assignment (for MME see 3GPP TS
  ///< 23.401 [3], clause 5.3.1.1, for SGSN see 3GPP TS 23.060
  ///< [35], clause 9.2.1, and for ePDG see 3GPP TS 23.402 [45]
  ///< subclause 4.7.3), the MME/SGSN/ePDG shall set the IPv4
  ///< address and/or IPv6 prefix length and IPv6 prefix and
  ///< Interface Identifier based on the subscribed values
  ///< received from HSS, if available. The value of PDN Type
  ///< field shall be consistent with the value of the PDN Type IE,
  ///< if present in this message.
  ///< For a Handover to Untrusted Non-3GPP IP Access with
  ///< GTP on S2b, the ePDG shall set the IPv4 address and/or
  ///< IPv6 prefix length and IPv6 prefix and Interface Identifier
  ///< based on the IP address(es) received from the UE.
  ///< If static IP address assignment is not used, and for
  ///< scenarios other than a Handover to Untrusted Non-3GPP
  ///< IP Access with GTP on S2b, the IPv4 address shall be set
  ///< to 0.0.0.0, and/or the IPv6 Prefix Length and IPv6 prefix
  ///< and Interface Identifier shall all be set to zero.
  ///<
  ///< CO: This IE shall be sent by the MME/SGSN on S11/S4
  ///< interface during TAU/RAU/HO with SGW relocation.

  core::apn_restriction_t apn_restriction; ///< This IE shall be included on the S4/S11 and S5/S8
  ///< interfaces in the E-UTRAN initial attach, PDP Context
  ///< Activation and UE Requested PDN connectivity
  ///< procedures.
  ///< This IE denotes the most stringent restriction as required
  ///< by any already active bearer context. If there are no
  ///< already active bearer contexts, this value is set to the least
  ///< restrictive type.

  //core::ebi_t              default_ebi;

  core::ambr_t             ambr;                ///< Aggregate Maximum Bit Rate (APN-AMBR)
  ///< This IE represents the APN-AMBR. It shall be included on
  ///< the S4/S11, S5/S8 and S2b interfaces for an E-UTRAN
  ///< initial attach, UE requested PDN connectivity, the PDP
  ///< Context Activation procedure using S4, the PS mobility
  ///< from the Gn/Gp SGSN to the S4 SGSN/MME procedures,
  ///< Attach with GTP on S2b and a UE initiated Connectivity to
  ///< Additional PDN with GTP on S2b.

  core::ebi_t              linked_eps_bearer_id;///< This IE shall be included on S4/S11 in RAU/TAU/HO
  ///< except in the Gn/Gp SGSN to MME/S4-SGSN
  ///< RAU/TAU/HO procedures with SGW change to identify the
  ///< default bearer of the PDN Connection

  core::protocol_configuration_options_t         pco;                 /// PCO protocol_configuration_options
  ///< This IE is not applicable to TAU/RAU/Handover. If
  ///< MME/SGSN receives PCO from UE (during the attach
  ///< procedures), the MME/SGSN shall forward the PCO IE to
  ///< SGW. The SGW shall also forward it to PGW.

  std::vector<bearer_context_to_be_created_within_create_session_request> bearer_contexts_to_be_created;    ///< Bearer Contexts to be created
  ///< Several IEs with the same type and instance value shall be
  ///< included on the S4/S11 and S5/S8 interfaces as necessary
  ///< to represent a list of Bearers. One single IE shall be
  ///< included on the S2b interface.
  ///< One bearer shall be included for an E-UTRAN Initial
  ///< Attach, a PDP Context Activation, a UE requested PDN
  ///< Connectivity, an Attach with GTP on S2b, a UE initiated
  ///< Connectivity to Additional PDN with GTP on S2b and a
  ///< Handover to Untrusted Non-3GPP IP Access with GTP on
  ///< S2b.
  ///< One or more bearers shall be included for a
  ///< Handover/TAU/RAU with an SGW change.

  std::vector<bearer_context_to_be_removed_within_create_session_request> bearer_contexts_to_be_removed;///< This IE shall be included on the S4/S11 interfaces for the
  ///< TAU/RAU/Handover cases where any of the bearers
  ///< existing before the TAU/RAU/Handover procedure will be
  ///< deactivated as consequence of the TAU/RAU/Handover
  ///< procedure.
  ///< For each of those bearers, an IE with the same type and
  ///< instance value shall be included.

  // Trace Information trace_information  ///< This IE shall be included on the S4/S11 interface if an
  ///< SGW trace is activated, and/or on the S5/S8 and S2b
  ///< interfaces if a PGW trace is activated. See 3GPP TS
  ///< 32.422 [18].

  // Recovery Recovery                    ///< This IE shall be included on the S4/S11, S5/S8 and S2b
  ///< interfaces if contacting the peer for the first time
  core::recovery_t         recovery;


  core::fq_csid_t          mme_fq_csid;         ///< This IE shall be included by the MME on the S11 interface
  ///< and shall be forwarded by an SGW on the S5/S8 interfaces
  ///< according to the requirements in 3GPP TS 23.007 [17].

  core::fq_csid_t          sgw_fq_csid;         ///< This IE shall included by the SGW on the S5/S8 interfaces
  ///< according to the requirements in 3GPP TS 23.007 [17].

  core::fq_csid_t          epdg_fq_csid;      ///< This IE shall be included by the ePDG on the S2b interface
  ///< according to the requirements in 3GPP TS 23.007 [17].
  core::fq_csid_t          twan_fq_csid;      ///< This IE shall be included by the ePDG on the S2b interface

  core::ue_time_zone_t       ue_time_zone;        ///< This IE shall be included by the MME over S11 during
  ///< Initial Attach, UE Requested PDN Connectivity procedure.
  ///< This IE shall be included by the SGSN over S4 during PDP
  ///< Context Activation procedure.
  ///< This IE shall be included by the MME/SGSN over S11/S4
  ///< TAU/RAU/Handover with SGW relocation.
  ///< C: If SGW receives this IE, SGW shall forward it to PGW
  ///< across S5/S8 interface.

  core::uci_t              uci;                 ///< User CSG Information
  ///< CO This IE shall be included on the S4/S11 interface for E-
  ///< UTRAN Initial Attach, UE-requested PDN Connectivity and
  ///< PDP Context Activation using S4 procedures if the UE is
  ///< accessed via CSG cell or hybrid cell. The MME/SGSN
  ///< shall also include it for TAU/RAU/Handover procedures if
  ///< the PGW has requested CSG info reporting and
  ///< MME/SGSN support CSG info reporting. The SGW shall
  ///< include this IE on S5/S8 if it receives the User CSG
  ///< information from MME/SGSN.

  // Charging Characteristics
  // MME/S4-SGSN LDN
  // SGW LDN
  // ePDG LDN
  // Signalling Priority Indication
  // MMBR Max MBR/APN-AMBR
  // Private Extension

  gtpv2c_create_session_request() :
    ie_presence_mask(0),
    imsi(),
    msisdn(),
    mei(),
    uli(),
    serving_network(),
    indication_flags(),
    sender_fteid_for_cp(),
    pgw_s5s8_address_for_cp(),
    //apn();
    paa(),
    apn_restriction(),
    ambr(),
    pco(),
    bearer_contexts_to_be_created(),
    bearer_contexts_to_be_removed(),
    recovery(),
    mme_fq_csid(),
    sgw_fq_csid(),
    epdg_fq_csid(),
    twan_fq_csid(),
    ue_time_zone(),
    uci()
  {
    pdn_type.pdn_type = 0;
    rat_type.rat_type = core::RAT_TYPE_E_UTRAN;
    selection_mode.selec_mode = core::SELECTION_MODE_E_MS_OR_NETWORK_PROVIDED_APN_SUBSCRIPTION_VERIFIED;
    linked_eps_bearer_id.ebi = EPS_BEARER_IDENTITY_UNASSIGNED;
  }
  gtpv2c_create_session_request(const gtpv2c_create_session_request& i) :
    ie_presence_mask(i.ie_presence_mask),
    imsi(i.imsi),
    msisdn(i.msisdn),
    mei(i.mei),
    uli(i.uli),
    serving_network(i.serving_network),
    rat_type(i.rat_type),
    indication_flags(i.indication_flags),
    sender_fteid_for_cp(i.sender_fteid_for_cp),
    pgw_s5s8_address_for_cp(i.pgw_s5s8_address_for_cp),
    apn(i.apn),
    selection_mode(i.selection_mode),
    pdn_type(i.pdn_type),
    paa(i.paa),
    apn_restriction(i.apn_restriction),
    ambr(i.ambr),
    linked_eps_bearer_id(i.linked_eps_bearer_id),
    pco(i.pco),
    bearer_contexts_to_be_created(i.bearer_contexts_to_be_created),
    bearer_contexts_to_be_removed(i.bearer_contexts_to_be_removed),
    recovery(i.recovery),
    mme_fq_csid(i.mme_fq_csid),
    sgw_fq_csid(i.sgw_fq_csid),
    epdg_fq_csid(i.epdg_fq_csid),
    twan_fq_csid(i.twan_fq_csid),
    ue_time_zone(i.ue_time_zone),
    uci(i.uci) {}

    gtpv2c_create_session_request& operator=(gtpv2c_create_session_request i)
    {
      std::swap(ie_presence_mask, i.ie_presence_mask);
      std::swap(imsi, i.imsi);
      std::swap(msisdn, i.msisdn);
      std::swap(mei, i.mei);
      std::swap(uli, i.uli);
      std::swap(serving_network, i.serving_network);
      std::swap(rat_type, i.rat_type);
      std::swap(indication_flags, i.indication_flags);
      std::swap(sender_fteid_for_cp, i.sender_fteid_for_cp);
      std::swap(pgw_s5s8_address_for_cp, i.pgw_s5s8_address_for_cp);
      std::swap(apn, i.apn);
      std::swap(selection_mode, i.selection_mode);
      std::swap(pdn_type, i.pdn_type);
      std::swap(paa, i.paa);
      std::swap(apn_restriction, i.apn_restriction);
      std::swap(ambr, i.ambr);
      std::swap(linked_eps_bearer_id, i.linked_eps_bearer_id);
      std::swap(pco, i.pco);
      std::swap(bearer_contexts_to_be_created, i.bearer_contexts_to_be_created);
      std::swap(bearer_contexts_to_be_removed, i.bearer_contexts_to_be_removed);
      std::swap(recovery, i.recovery);
      std::swap(mme_fq_csid, i.mme_fq_csid);
      std::swap(sgw_fq_csid, i.sgw_fq_csid);
      std::swap(epdg_fq_csid, i.epdg_fq_csid);
      std::swap(twan_fq_csid, i.twan_fq_csid);
      std::swap(ue_time_zone, i.ue_time_zone);
      std::swap(uci, i.uci);
      return *this;
    }
    
  static const char* get_msg_name() {return "CREATE_SESSION_REQUEST";};

  bool get(core::imsi_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_IMSI) {v = imsi;return true;}return false;}
  bool get(core::msisdn_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MSISDN) {v = msisdn;return true;}return false;}
  bool get(core::mei_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MEI) {v = mei;return true;}return false;}
  bool get(core::uli_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_ULI) {v = uli;return true;}return false;}
  bool get(core::serving_network_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SERVING_NETWORK) {v = serving_network;return true;}return false;}
  bool get(core::rat_type_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_RAT_TYPE) {v = rat_type;return true;}return false;}
  bool get(core::indication_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_INDICATION_FLAGS) {v = indication_flags;return true;}return false;}
  bool get_sender_fteid_for_cp(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE) {v = sender_fteid_for_cp;return true;}return false;}
  bool get_pgw_s5s8_address_for_cp(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PGW_S5S8_ADDRESS_FOR_CONTROL_PLANE) {v = pgw_s5s8_address_for_cp;return true;}return false;}
  bool get(core::apn_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN) {v = apn;return true;}return false;}
  bool get(core::selection_mode_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SELECTION_MODE) {v = selection_mode;return true;}return false;}
  bool get(core::pdn_type_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PDN_TYPE) {v = pdn_type;return true;}return false;}
  bool get(core::paa_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PAA) {v = paa;return true;}return false;}
  bool get(core::apn_restriction_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN_RESTRICTION) {v = apn_restriction;return true;}return 
false;}
  bool get(core::ambr_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN_AMBR) {v = ambr;return true;}return false;}
  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_LINKED_EPS_BEARER_ID) {v = linked_eps_bearer_id;return true;}return false;}
  bool get(core::protocol_configuration_options_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PCO) {v = pco;return true;}return false;}
  bool get(core::recovery_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_RECOVERY) {v = recovery;return true;}return false;}
  bool get_mme_fq_csid(core::fq_csid_t& v) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MME_FQ_CSID) {v = mme_fq_csid;return true;}return false;}
  bool get_sgw_fq_csid(core::fq_csid_t& v) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SGW_FQ_CSID) {v = sgw_fq_csid;return true;}return false;}
  bool get_epdg_fq_csid(core::fq_csid_t& v) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_EPDG_FQ_CSID) {v = epdg_fq_csid;return true;}return false;}
  bool get_twan_fq_csid(core::fq_csid_t& v) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TWAN_FQ_CSID) {v = twan_fq_csid;return true;}return false;}
  bool get(core::ue_time_zone_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UE_TIME_ZONE) {v = ue_time_zone;return true;}return false;}
  bool get(core::uci_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UCI) {v = uci;return true;}return false;}
  bool get(core::fteid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: return get_sender_fteid_for_cp(v); break;
    case 1: return get_pgw_s5s8_address_for_cp(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, instance);
    }
  }
  bool get(core::fq_csid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: return get_mme_fq_csid(v); break;
    case 1: return get_sgw_fq_csid(v); break;
    case 2: return get_epdg_fq_csid(v); break;
    case 3: return get_twan_fq_csid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FQ_CSID, instance);
    }
  }
  bool has_bearer_context_to_be_created() const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_CREATED) return true; else return false;}
  bool has_bearer_context_to_be_removed() const {if (ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_REMOVED) return true; else return false;}

  void set(const core::imsi_t& v, const uint8_t instance = 0) {imsi = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_IMSI;}
  void set(const core::msisdn_t& v, const uint8_t instance = 0) {msisdn = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MSISDN;}
  void set(const core::mei_t& v, const uint8_t instance = 0) {mei = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MEI;}
  void set(const core::uli_t& v, const uint8_t instance = 0) {uli = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_ULI;}
  void set(const core::serving_network_t& v, const uint8_t instance = 0) {serving_network = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SERVING_NETWORK;}
  void set(const core::rat_type_t& v, const uint8_t instance = 0) {rat_type = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_RAT_TYPE;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_INDICATION_FLAGS;}
  void set_sender_fteid_for_cp(const core::fteid_t& v) {sender_fteid_for_cp = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE;}
  void set_pgw_s5s8_address_for_cp(const core::fteid_t& v, const uint8_t instance = 0) {pgw_s5s8_address_for_cp = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PGW_S5S8_ADDRESS_FOR_CONTROL_PLANE;}
  void set(const core::apn_t& v, const uint8_t instance = 0) {apn = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN;}
  void set(const core::selection_mode_t& v, const uint8_t instance = 0) {selection_mode = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SELECTION_MODE;}
  void set(const core::pdn_type_t& v, const uint8_t instance = 0) {pdn_type = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PDN_TYPE;}
  void set(const core::paa_t& v, const uint8_t instance = 0) {paa = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PAA;}
  void set(const core::apn_restriction_t& v, const uint8_t instance = 0) {apn_restriction = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN_RESTRICTION;}
  void set(const core::ambr_t& v, const uint8_t instance = 0) {ambr = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN_AMBR;}
  void set(const core::ebi_t& v, const uint8_t instance = 0) {linked_eps_bearer_id = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_LINKED_EPS_BEARER_ID;}
  void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {pco = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PCO;}
  void add_bearer_context_to_be_created(const bearer_context_to_be_created_within_create_session_request& v) {
      bearer_contexts_to_be_created.push_back(v);
      ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_CREATED;
  }
  void add_bearer_context_to_be_removed(const bearer_context_to_be_removed_within_create_session_request& v) {
      bearer_contexts_to_be_removed.push_back(v);
      ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_REMOVED;
  }
  void set(const core::recovery_t& v, const uint8_t instance = 0) {recovery = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_RECOVERY;}
  void set_mme_fq_csid(const core::fq_csid_t& v) {mme_fq_csid = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MME_FQ_CSID;}
  void set_sgw_fq_csid(const core::fq_csid_t& v) {sgw_fq_csid = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SGW_FQ_CSID;}
  void set_epdg_fq_csid(const core::fq_csid_t& v) {epdg_fq_csid = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_EPDG_FQ_CSID;}
  void set_twan_fq_csid(const core::fq_csid_t& v) {twan_fq_csid = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TWAN_FQ_CSID;}
  void set(const core::ue_time_zone_t& v, const uint8_t instance = 0) {ue_time_zone = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UE_TIME_ZONE;}
  void set(const core::uci_t& v, const uint8_t instance = 0) {uci = v; ie_presence_mask |= GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UCI;}
  void set(const core::fteid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_sender_fteid_for_cp(v); break;
    case 1: set_pgw_s5s8_address_for_cp(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, instance);
    }
  }
  void set(const core::fq_csid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_mme_fq_csid(v); break;
    case 1: set_sgw_fq_csid(v); break;
    case 2: set_epdg_fq_csid(v); break;
    case 3: set_twan_fq_csid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FQ_CSID, instance);
    }
  }
  void set(const bearer_context& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: {
        bearer_context_to_be_created_within_create_session_request b(v);
        add_bearer_context_to_be_created(b);
      }
      break;
    case 1: {
        bearer_context_to_be_removed_within_create_session_request b(v);
        add_bearer_context_to_be_removed(b);
      }
      break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }
};


class bearer_context_created_within_create_session_response {
public:
  uint32_t           ie_presence_mask;
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_EPS_BEARER_ID          ((uint64_t)1)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CAUSE                  ((uint64_t)1 << 1)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S1_U_SGW_FTEID         ((uint64_t)1 << 2)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S4_U_SGW_FTEID         ((uint64_t)1 << 3)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S5_S8_U_PGW_FTEID      ((uint64_t)1 << 4)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S12_SGW_FTEID          ((uint64_t)1 << 5)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S2B_U_PGW_FTEID        ((uint64_t)1 << 6)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S2A_U_PGW_FTEID        ((uint64_t)1 << 7)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S5_S8_BEARER_LEVEL_QOS ((uint64_t)1 << 8)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CHARGING_ID            ((uint64_t)1 << 9)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_BEARER_FLAGS           ((uint64_t)1 << 10)
#define GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S11_U_SGW_FTEID        ((uint64_t)1 << 11)
  core::ebi_t         eps_bearer_id;
  core::cause_t       cause;
  core::fteid_t       s1_u_sgw_fteid;
  core::fteid_t       s4_u_sgw_fteid;
  core::fteid_t       s5_s8_u_pgw_fteid;
  core::fteid_t       s12_sgw_fteid;
  core::fteid_t       s2b_u_pgw_fteid;
  core::fteid_t       s2a_u_pgw_fteid;
  core::bearer_qos_t  bearer_level_qos;
  core::charging_id_t charging_id;
  core::bearer_flags_t bearer_flags;
  core::fteid_t       s11_u_sgw_fteid;
  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_EPS_BEARER_ID;}
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CAUSE;}
  void set_s1_u_sgw_fteid(const core::fteid_t& v) {s1_u_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S1_U_SGW_FTEID;}
  void set_s4_u_sgw_fteid(const core::fteid_t& v) {s4_u_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S4_U_SGW_FTEID;}
  void set_s5_s8_u_pgw_fteid(const core::fteid_t& v) {s5_s8_u_pgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S5_S8_U_PGW_FTEID;}
  void set_s12_sgw_fteid(const core::fteid_t& v) {s12_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S12_SGW_FTEID;}
  void set_s2b_u_pgw_fteid(const core::fteid_t& v) {s2b_u_pgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S2B_U_PGW_FTEID;}
  void set_s2a_u_pgw_fteid(const core::fteid_t& v) {s2a_u_pgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S2A_U_PGW_FTEID;}
  void set(const core::bearer_qos_t& v, const uint8_t instance = 0) {bearer_level_qos = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S5_S8_BEARER_LEVEL_QOS;}
  void set(const core::charging_id_t& v, const uint8_t instance = 0) {charging_id = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CHARGING_ID;}
  void set(const core::bearer_flags_t& v, const uint8_t instance = 0) {bearer_flags = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_BEARER_FLAGS;}
  void set_s11_u_sgw_fteid(const core::fteid_t& v) {s11_u_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S11_U_SGW_FTEID;}
  void set(const core::fteid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_s1_u_sgw_fteid(v); break;
    case 1: set_s4_u_sgw_fteid(v); break;
    case 2: set_s5_s8_u_pgw_fteid(v); break;
    case 3: set_s12_sgw_fteid(v); break;
    case 4: set_s2b_u_pgw_fteid(v); break;
    case 5: set_s2a_u_pgw_fteid(v); break;
    case 6: set_s11_u_sgw_fteid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, instance);
    }
  }
  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_EPS_BEARER_ID) {v = eps_bearer_id;return true;}return false;}
  bool get(core::cause_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CAUSE) {v = cause;return true;}return false;}
  bool get_s1_u_sgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S1_U_SGW_FTEID) {v = s1_u_sgw_fteid;return true;}return false;}
  bool get_s4_u_sgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S4_U_SGW_FTEID) {v = s4_u_sgw_fteid;return true;}return false;}
  bool get_s5_s8_u_pgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S5_S8_U_PGW_FTEID) {v = s5_s8_u_pgw_fteid;return true;}return false;}
  bool get_s12_sgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S12_SGW_FTEID) {v = s12_sgw_fteid;return true;}return false;}
  bool get_s2b_u_pgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S2B_U_PGW_FTEID) {v = s2b_u_pgw_fteid;return true;}return false;}
  bool get_s2a_u_pgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S2A_U_PGW_FTEID) {v = s2a_u_pgw_fteid;return true;}return false;}
  bool get(core::bearer_qos_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S5_S8_BEARER_LEVEL_QOS) {v = bearer_level_qos;return true;}return false;}
  bool get(core::charging_id_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CHARGING_ID) {v = charging_id;return true;}return false;}
  bool get(core::bearer_flags_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_BEARER_FLAGS) {v = bearer_flags;return true;}return false;}
  bool get_s11_u_sgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S11_U_SGW_FTEID) {v = s11_u_sgw_fteid;return true;}return false;}
  bool get(core::fteid_t& v, const uint8_t instance = 0) const {
    switch (instance) {
    case 0: return get_s1_u_sgw_fteid(v); break;
    case 1: return get_s4_u_sgw_fteid(v); break;
    case 2: return get_s5_s8_u_pgw_fteid(v); break;
    case 3: return get_s12_sgw_fteid(v); break;
    case 4: return get_s2b_u_pgw_fteid(v); break;
    case 5: return get_s2a_u_pgw_fteid(v); break;
    case 6: return get_s11_u_sgw_fteid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, instance);
    }
  }

  bearer_context_created_within_create_session_response()
  {
    ie_presence_mask = 0;
    eps_bearer_id = {};
    cause = {};
    s1_u_sgw_fteid = {};
    s4_u_sgw_fteid = {};
    s5_s8_u_pgw_fteid = {};
    s12_sgw_fteid = {};
    s2b_u_pgw_fteid = {};
    s2a_u_pgw_fteid = {};
    bearer_level_qos = {};
    charging_id = {};
    bearer_flags = {};
    s11_u_sgw_fteid = {};
  }
  explicit bearer_context_created_within_create_session_response(const bearer_context& b) : bearer_context_created_within_create_session_response()
  {
    ie_presence_mask = 0;
    if (b.get(eps_bearer_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_EPS_BEARER_ID;
    if (b.get(cause, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CAUSE;
    if (b.get(s1_u_sgw_fteid, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S1_U_SGW_FTEID;
    if (b.get(s4_u_sgw_fteid, 1)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S4_U_SGW_FTEID;
    if (b.get(s5_s8_u_pgw_fteid, 2)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S5_S8_U_PGW_FTEID;
    if (b.get(s12_sgw_fteid, 3)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S12_SGW_FTEID;
    if (b.get(s2b_u_pgw_fteid, 4)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S2B_U_PGW_FTEID;
    if (b.get(s2a_u_pgw_fteid, 5)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S2A_U_PGW_FTEID;
    if (b.get(bearer_level_qos, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S5_S8_BEARER_LEVEL_QOS;
    if (b.get(charging_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CHARGING_ID;
    if (b.get(bearer_flags, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_BEARER_FLAGS;
    if (b.get(s11_u_sgw_fteid, 6)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S11_U_SGW_FTEID;
  }
};

class bearer_context_marked_for_removal_within_create_session_response {
public:
  uint32_t           ie_presence_mask;
#define GTPV2C_BEARER_CONTEXT_MARKED_FOR_REMOVAL_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_EPS_BEARER_ID ((uint64_t)1)
#define GTPV2C_BEARER_CONTEXT_MARKED_FOR_REMOVAL_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CAUSE         ((uint64_t)1 << 1)
  core::ebi_t   eps_bearer_id;      ///< EPS Bearer ID, Mandatory
  core::cause_t cause;
  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_MARKED_FOR_REMOVAL_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_EPS_BEARER_ID;}
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_MARKED_FOR_REMOVAL_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CAUSE;}
  bearer_context_marked_for_removal_within_create_session_response()
  {
    ie_presence_mask = 0;
    eps_bearer_id = {};
    cause = {};
  }
  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_MARKED_FOR_REMOVAL_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_EPS_BEARER_ID) {v = eps_bearer_id;return true;}return false;}
  bool get(core::cause_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_MARKED_FOR_REMOVAL_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CAUSE) {v = cause;return true;}return false;}
  explicit bearer_context_marked_for_removal_within_create_session_response(const bearer_context& b) : bearer_context_marked_for_removal_within_create_session_response()
  {
    ie_presence_mask = 0;
    if (b.get(eps_bearer_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_MARKED_FOR_REMOVAL_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_EPS_BEARER_ID;
    if (b.get(cause, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_MARKED_FOR_REMOVAL_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CAUSE;
  }
};

//-----------------------------------------------------------------------------
/** @class gtpv2c_create_session_response
 *  @brief Create Session Response
 *
 * The Create Session Response will be sent on S11 interface as
 * part of these procedures:
 * - E-UTRAN Initial Attach
 * - UE requested PDN connectivity
 * - Tracking Area Update procedure with SGW change
 * - S1/X2-based handover with SGW change
 */
class gtpv2c_create_session_response : public gtpv2c_ies_container {
public:
  static const uint8_t msg_id = GTP_CREATE_SESSION_RESPONSE;

  // here fields listed in 3GPP TS 29.274
  std::pair<bool, core::cause_t>            cause;               ///< If the SGW cannot accept any of the "Bearer Context Created" IEs within Create Session Request
  ///< message, the SGW shall send the Create Session Response with appropriate reject Cause value.

  // change_reporting_action                    ///< This IE shall be included on the S5/S8 and S4/S11
  ///< interfaces with the appropriate Action field if the location
  ///< Change Reporting mechanism is to be started or stopped
  ///< for this subscriber in the SGSN/MME.

  // csg_Information_reporting_action           ///< This IE shall be included on the S5/S8 and S4/S11
  ///< interfaces with the appropriate Action field if the CSG Info
  ///< reporting mechanism is to be started or stopped for this
  ///< subscriber in the SGSN/MME.

  std::pair<bool, core::fteid_t>                  sender_fteid_for_cp;        ///< Sender F-TEID for control plane
  ///< This IE shall be sent on the S11/S4 interfaces. For the
  ///< S5/S8/S2b interfaces it is not needed because its content
  ///< would be identical to the IE PGW S5/S8/S2b F-TEID for
  ///< PMIP based interface or for GTP based Control Plane
  ///< interface.

  std::pair<bool, core::fteid_t>                  s5_s8_pgw_fteid;      ///< PGW S5/S8/S2b F-TEID for PMIP based interface or for GTP based Control Plane interface
  ///< PGW shall include this IE on the S5/S8 interfaces during
  ///< the Initial Attach, UE requested PDN connectivity and PDP
  ///< Context Activation procedures.
  ///< If SGW receives this IE it shall forward the IE to MME/S4-
  ///< SGSN on S11/S4 interface.
  ///< This IE shall include the TEID in the GTP based S5/S8
  ///< case and the GRE key in the PMIP based S5/S8 case.
  ///< In PMIP based S5/S8 case, same IP address is used for
  ///< both control plane and the user plane communication.
  ///<
  ///< PGW shall include this IE on the S2b interface during the
  ///< Attach with GTP on S2b, UE initiated Connectivity to
  ///< Additional PDN with GTP on S2b and Handover to
  ///< Untrusted Non-3GPP IP Access with GTP on S2b
  ///< procedures.


  std::pair<bool, core::paa_t>                    paa;                 ///< PDN Address Allocation
  ///< This IE shall be included on the S5/S8, S4/S11 and S2b
  ///< interfaces for the E-UTRAN initial attach, PDP Context
  ///< Activation, UE requested PDN connectivity, Attach with
  ///< GTP on S2b, UE initiated Connectivity to Additional PDN
  ///< with GTP on S2b and Handover to Untrusted Non-3GPP IP
  ///< Access with GTP on S2b procedures.
  ///< The PDN type field in the PAA shall be set to IPv4, or IPv6
  ///< or IPv4v6 by the PGW.
  ///< For the interfaces other than S2b, if the DHCPv4 is used
  ///< for IPv4 address allocation, the IPv4 address field shall be
  ///< set to 0.0.0.0.

  std::pair<bool, core::access_point_name_restriction_t>         apn_restriction;     ///< This IE shall be included on the S5/S8 and S4/S11
  ///< interfaces in the E-UTRAN initial attach, PDP Context
  ///< Activation and UE Requested PDN connectivity
  ///< procedures.
  ///< This IE shall also be included on S4/S11 during the Gn/Gp
  ///< SGSN to S4 SGSN/MME RAU/TAU procedures.
  ///< This IE denotes the restriction on the combination of types
  ///< of APN for the APN associated with this EPS bearer
  ///< Context.

  std::pair<bool, core::ambr_t>             apn_ambr;                  ///< Aggregate Maximum Bit Rate (APN-AMBR)
  ///< This IE represents the APN-AMBR. It shall be included on
  ///< the S5/S8, S4/S11 and S2b interfaces if the received APN-
  ///< AMBR has been modified by the PCRF.

  std::pair<bool, core::ebi_t>              linked_eps_bearer_id;                ///< This IE shall be sent on the S4/S11 interfaces during
  ///< Gn/Gp SGSN to S4-SGSN/MME RAU/TAU procedure to
  ///< identify the default bearer the PGW selects for the PDN
  ///< Connection.

  std::pair<bool, core::protocol_configuration_options_t>         pco;// PCO protocol_configuration_options
  ///< This IE is not applicable for TAU/RAU/Handover. If PGW
  ///< decides to return PCO to the UE, PGW shall send PCO to
  ///< SGW. If SGW receives the PCO IE, SGW shall forward it
  ///< MME/SGSN.

  std::pair<bool, std::vector<bearer_context_created_within_create_session_response>> bearer_contexts_created;///< EPS bearers corresponding to Bearer Contexts sent in
  ///< request message. Several IEs with the same type and
  ///< instance value may be included on the S5/S8 and S4/S11
  ///< as necessary to represent a list of Bearers. One single IE
  ///< shall be included on the S2b interface.
  ///< One bearer shall be included for E-UTRAN Initial Attach,
  ///< PDP Context Activation or UE Requested PDN
  ///< Connectivity , Attach with GTP on S2b, UE initiated
  ///< Connectivity to Additional PDN with GTP on S2b, and
  ///< Handover to Untrusted Non-3GPP IP Access with GTP on
  ///< S2b.
  ///< One or more created bearers shall be included for a
  ///< Handover/TAU/RAU with an SGW change. See NOTE 2.

  std::pair<bool, std::vector<bearer_context_marked_for_removal_within_create_session_response>> bearer_contexts_marked_for_removal; ///< EPS bearers corresponding to Bearer Contexts to be
  ///< removed that were sent in the Create Session Request
  ///< message.
  ///< For each of those bearers an IE with the same type and
  ///< instance value shall be included on the S4/S11 interfaces.

  // Recovery Recovery                          ///< This IE shall be included on the S4/S11, S5/S8 and S2b
  ///< interfaces if contacting the peer for the first time

  // FQDN charging_Gateway_name                 ///< When Charging Gateway Function (CGF) Address is
  ///< configured, the PGW shall include this IE on the S5
  ///< interface.
  ///< NOTE 1: Both Charging Gateway Name and Charging Gateway Address shall not be included at the same
  ///< time. When both are available, the operator configures a preferred value.

  // IP Address charging_Gateway_address        ///< When Charging Gateway Function (CGF) Address is
  ///< configured, the PGW shall include this IE on the S5
  ///< interface. See NOTE 1.


  std::pair<bool, core::fq_csid_t>                pgw_fq_csid;         ///< This IE shall be included by the PGW on the S5/S8 and
  ///< S2b interfaces and, when received from S5/S8 be
  ///< forwarded by the SGW on the S11 interface according to
  ///< the requirements in 3GPP TS 23.007 [17].

  std::pair<bool, core::fq_csid_t>                sgw_fq_csid;         ///< This IE shall be included by the SGW on the S11 interface
  ///< according to the requirements in 3GPP TS 23.007 [17].

  std::pair<bool, core::local_distinguished_name_t> sgw_ldn;     ///< This IE is optionally sent by the SGW to the MME/SGSN
  ///< on the S11/S4 interfaces (see 3GPP TS 32.423 [44]),
  ///< when contacting the peer node for the first time.
  ///< Also:
  ///< This IE is optionally sent by the SGW to the MME/SGSN
  ///< on the S11/S4 interfaces (see 3GPP TS 32.423 [44]),
  ///< when communicating the LDN to the peer node for the first
  ///< time.

  std::pair<bool, core::local_distinguished_name_t> pgw_ldn;     ///< This IE is optionally included by the PGW on the S5/S8
  ///< and S2b interfaces (see 3GPP TS 32.423 [44]), when
  ///< contacting the peer node for the first time.
  ///< Also:
  ///< This IE is optionally included by the PGW on the S5/S8
  ///< interfaces (see 3GPP TS 32.423 [44]), when
  ///< communicating the LDN to the peer node for the first time.

  std::pair<bool, core::epc_timer_t> pgw_back_off_time;                ///< This IE may be included on the S5/S8 and S4/S11
  ///< interfaces when the PDN GW rejects the Create Session
  ///< Request with the cause "APN congestion". It indicates the
  ///< time during which the MME or S4-SGSN should refrain
  ///< from sending subsequent PDN connection establishment
  ///< requests to the PGW for the congested APN for services
  ///< other than Service Users/emergency services.
  ///< See NOTE 3:
  ///< The last received value of the PGW Back-Off Time IE shall supersede any previous values received
  ///< from that PGW and for this APN in the MME/SGSN.
  std::pair<bool, core::indication_t>  indication_flags;
  // Private Extension                          ///< This IE may be sent on the S5/S8, S4/S11 and S2b
  ///< interfaces.

  gtpv2c_create_session_response(): cause(), 
    pgw_back_off_time(), 
    indication_flags(),
    sender_fteid_for_cp(),
    s5_s8_pgw_fteid(),
    paa(),
    apn_restriction(),
    apn_ambr(),
    linked_eps_bearer_id(),
    pco(),
    bearer_contexts_created(),
    bearer_contexts_marked_for_removal(),
    pgw_fq_csid(),
    sgw_fq_csid(),
    sgw_ldn(),
    pgw_ldn() {}
    
  gtpv2c_create_session_response(const gtpv2c_create_session_response& i): cause(i.cause),
    pgw_back_off_time(i.pgw_back_off_time), 
    indication_flags(i.indication_flags),
    sender_fteid_for_cp(i.sender_fteid_for_cp),
    s5_s8_pgw_fteid(i.s5_s8_pgw_fteid),
    paa(i.paa),
    apn_restriction(i.apn_restriction),
    apn_ambr(i.apn_ambr),
    linked_eps_bearer_id(i.linked_eps_bearer_id),
    pco(i.pco),
    bearer_contexts_created(i.bearer_contexts_created),
    bearer_contexts_marked_for_removal(i.bearer_contexts_marked_for_removal),
    pgw_fq_csid(i.pgw_fq_csid),
    sgw_fq_csid(i.sgw_fq_csid),
    sgw_ldn(i.sgw_ldn),
    pgw_ldn(i.pgw_ldn) {}

  static const char* get_msg_name() {return "CREATE_SESSION_RESPONSE";};

  bool get(core::cause_t& v, const uint8_t instance = 0) const {if (cause.first) {v = cause.second;return true;}return false;}
  bool get_sender_fteid_for_cp(core::fteid_t& v, const uint8_t instance = 0) const {if (sender_fteid_for_cp.first) {v = sender_fteid_for_cp.second;return true;}return false;}
  bool get_s5_s8_pgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (s5_s8_pgw_fteid.first) {v = s5_s8_pgw_fteid.second;return true;}return false;}
  bool get(core::paa_t& v, const uint8_t instance = 0) const {if (paa.first) {v = paa.second;return true;}return false;}
  bool get(core::apn_restriction_t& v, const uint8_t instance = 0) const {if (apn_restriction.first) {v = apn_restriction.second;return true;}return false;}
  bool get(core::ambr_t& v, const uint8_t instance = 0) const {if (apn_ambr.first) {v = apn_ambr.second;return true;}return false;}
  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (linked_eps_bearer_id.first) {v = linked_eps_bearer_id.second;return true;}return false;}
  bool get(core::protocol_configuration_options_t& v, const uint8_t instance = 0) const {if (pco.first) {v = pco.second;return true;}return false;}
  bool get_pgw_fq_csid(core::fq_csid_t& v) const {if (pgw_fq_csid.first) {v = pgw_fq_csid.second;return true;}return false;}
  bool get_sgw_fq_csid(core::fq_csid_t& v) const {if (sgw_fq_csid.first) {v = sgw_fq_csid.second;return true;}return false;}
  bool get_sgw_ldn(core::local_distinguished_name_t& v) const {if (sgw_ldn.first) {v = sgw_ldn.second;return true;}return false;}
  bool get_pgw_ldn(core::local_distinguished_name_t& v) const {if (pgw_ldn.first) {v = pgw_ldn.second;return true;}return false;}
  bool get(core::epc_timer_t& v) const {if (pgw_back_off_time.first) {v = pgw_back_off_time.second;return true;}return false;}
  bool get(core::indication_t& v, const uint8_t instance = 0) const {if (indication_flags.first) {v = indication_flags.second;return true;}return false;}

  bool get(core::fteid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: return get_sender_fteid_for_cp(v); break;
    case 1: return get_s5_s8_pgw_fteid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, instance);
    }
  }
  bool get(core::fq_csid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: return get_pgw_fq_csid(v); break;
    case 1: return get_sgw_fq_csid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FQ_CSID, instance);
    }
  }
  bool get(core::local_distinguished_name_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: return get_sgw_ldn(v); break;
    case 1: return get_pgw_ldn(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_LOCAL_DISTINGUISHED_NAME, instance);
    }
  }
  bool has_bearer_context_created() const {if (bearer_contexts_created.first) {return true;}return false;}
  bool has_bearer_context_marked_for_removal() const {if (bearer_contexts_marked_for_removal.first) {return true;}return false;}

  void set(const core::cause_t& v, const uint8_t instance = 0) {cause.first = true; cause.second = v;}
  void set_sender_fteid_for_cp(const core::fteid_t& v) {sender_fteid_for_cp.first = true; sender_fteid_for_cp.second = v;}
  void set_s5_s8_pgw_fteid(const core::fteid_t& v) {s5_s8_pgw_fteid.first = true; s5_s8_pgw_fteid.second = v;}
  void set(const core::paa_t& v, const uint8_t instance = 0) {paa.first = true; paa.second = v;}
  void set(const core::apn_restriction_t& v, const uint8_t instance = 0) {apn_restriction.first = true; apn_restriction.second = v;}
  void set(const core::ambr_t& v, const uint8_t instance = 0) {apn_ambr.first = true; apn_ambr.second = v;}
  void set(const core::ebi_t& v, const uint8_t instance = 0) {linked_eps_bearer_id.first = true; linked_eps_bearer_id.second = v;}
  void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {pco.first = true; pco.second = v;}
  void add_bearer_context_created(const bearer_context_created_within_create_session_response& v) {
    bearer_contexts_created.second.push_back(v);
    bearer_contexts_created.first = true;
  }
  void add_bearer_context_marked_for_removal(const bearer_context_marked_for_removal_within_create_session_response& v) {
    bearer_contexts_marked_for_removal.second.push_back(v);
    bearer_contexts_marked_for_removal.first = true;
  }
  void set_pgw_fq_csid(const core::fq_csid_t& v) {pgw_fq_csid.first = true; pgw_fq_csid.second = v;}
  void set_sgw_fq_csid(const core::fq_csid_t& v) {sgw_fq_csid.first = true; sgw_fq_csid.second = v;}
  void set_sgw_ldn(const core::local_distinguished_name_t& v) {sgw_ldn.first = true; sgw_ldn.second = v;}
  void set_pgw_ldn(const core::local_distinguished_name_t& v) {pgw_ldn.first = true; pgw_ldn.second = v;}
  void set(const core::epc_timer_t& v, const uint8_t instance = 0) {pgw_back_off_time.first = true; pgw_back_off_time.second = v;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags.first = true; indication_flags.second = v;}
  void set(const core::fteid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_sender_fteid_for_cp(v); break;
    case 1: set_s5_s8_pgw_fteid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, instance);
    }
  }
  void set(const core::fq_csid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_pgw_fq_csid(v); break;
    case 1: set_sgw_fq_csid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FQ_CSID, instance);
    }
  }
  void set(const core::local_distinguished_name_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_sgw_ldn(v); break;
    case 1: set_pgw_ldn(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_LOCAL_DISTINGUISHED_NAME, instance);
    }
  }
  void set(const bearer_context& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: {
        bearer_context_created_within_create_session_response b(v);
        add_bearer_context_created(b);
      }
      break;
    case 1: {
        bearer_context_marked_for_removal_within_create_session_response b(v);
        add_bearer_context_marked_for_removal(b);
      }
      break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }

  void clear_cause(const uint8_t instance = 0) {cause.first = false; cause.second = {};}
  void clear_sender_fteid_for_cp() {sender_fteid_for_cp.first = false; sender_fteid_for_cp.second = {};}
  void clear_s5_s8_pgw_fteid() {s5_s8_pgw_fteid.first = false; s5_s8_pgw_fteid.second = {};}
  void clear_paa(const uint8_t instance = 0) {paa.first = false; paa.second = {};}
  void clear_apn_restriction(const uint8_t instance = 0) {apn_restriction.first = false; apn_restriction.second = {};}
  void clear_apn_ambr(const uint8_t instance = 0) {apn_ambr.first = false; apn_ambr.second = {};}
  void clear_linked_eps_bearer_id(const uint8_t instance = 0) {linked_eps_bearer_id.first = false; linked_eps_bearer_id.second = {};}
  void clear_pco(const uint8_t instance = 0) {pco.first = false; pco.second = {};}
  void clear_bearer_context_created() {
    bearer_contexts_created.second.clear();
    bearer_contexts_created.first = false;
  }
  void clear_bearer_context_marked_for_removal() {
    bearer_contexts_marked_for_removal.second.clear();
    bearer_contexts_marked_for_removal.first = false;
  }
  void clear_pgw_fq_csid() {pgw_fq_csid.first = false; pgw_fq_csid.second = {};}
  void clear_sgw_fq_csid() {sgw_fq_csid.first = false; sgw_fq_csid.second = {};}
  void clear_sgw_ldn() {sgw_ldn.first = false; sgw_ldn.second = {};}
  void clear_pgw_ldn() {pgw_ldn.first = false; pgw_ldn.second = {};}
  void clear_pgw_back_off_time(const uint8_t instance = 0) {pgw_back_off_time.first = false; pgw_back_off_time.second = {};}
  void clear_indication_flags(const uint8_t instance = 0) {indication_flags.first = false; indication_flags.second = {};}
};



//------------------------------------------------------------------------------
class gtpv2c_echo_request : public gtpv2c_ies_container {
public:
  static const uint8_t msg_id = GTP_ECHO_REQUEST;

  std::pair<bool, core::recovery_t>             recovery_restart_counter;
  std::pair<bool, core::node_features_t>        sending_node_features; 

  gtpv2c_echo_request() : recovery_restart_counter(), sending_node_features() {}
  gtpv2c_echo_request(const gtpv2c_echo_request& i): recovery_restart_counter(i.recovery_restart_counter), sending_node_features(i.sending_node_features) {}
  static const char* get_msg_name() {return "ECHO_REQUEST";};

  bool get(core::recovery_t& v) const {if (recovery_restart_counter.first) {v = recovery_restart_counter.second;return true;}return false;}
  bool get(core::node_features_t& v) const {if (sending_node_features.first) {v = sending_node_features.second;return true;}return false;}
  void set(const core::recovery_t& v) {recovery_restart_counter.first = true; recovery_restart_counter.second = v;}
  void set(const core::node_features_t& v) {sending_node_features.first = true; sending_node_features.second = v;}
};

//------------------------------------------------------------------------------
class gtpv2c_echo_response : public gtpv2c_ies_container {
public:
  static const uint8_t msg_id = GTP_ECHO_RESPONSE;

  std::pair<bool, core::recovery_t>             recovery_restart_counter; 
  std::pair<bool, core::node_features_t>        sending_node_features;  

  gtpv2c_echo_response() :
    recovery_restart_counter(),
    sending_node_features() {}
  
  gtpv2c_echo_response(const gtpv2c_echo_response& i) :
    recovery_restart_counter(i.recovery_restart_counter),
    sending_node_features(i.sending_node_features) {}
  
  static const char* get_msg_name() {return "ECHO_RESPONSE";};

  bool get(core::recovery_t& v) const {if (recovery_restart_counter.first) {v = recovery_restart_counter.second;return true;}return false;}
  bool get(core::node_features_t& v) const {if (sending_node_features.first) {v = sending_node_features.second;return true;}return false;}
  void set(const core::recovery_t& v) {recovery_restart_counter.first = true; recovery_restart_counter.second = v;}
  void set(const core::node_features_t& v) {sending_node_features.first = true; sending_node_features.second = v;}
};


//-----------------------------------------------------------------------------
/** @class gtpv2c_create_bearer_request
 *  @brief Create Bearer Request
 *
 * The direction of this message shall be from PGW to SGW and from SGW to MME/S4-SGSN, and from PGW to ePDG
 * The Create Bearer Request message shall be sent on the S5/S8 interface by the PGW to the SGW and on the S11
 * interface by the SGW to the MME as part of the Dedicated Bearer Activation procedure.
 * The message shall also be sent on the S5/S8 interface by the PGW to the SGW and on the S4 interface by the SGW to
 * the SGSN as part of the Secondary PDP Context Activation procedure or the Network Requested Secondary PDP
 * Context Activation procedure.
 * The message shall also be sent on the S2b interface by the PGW to the ePDG as part of the Dedicated S2b bearer
 * activation with GTP on S2b.
 */

class bearer_context_to_be_created_within_create_bearer_request {
public:
  uint32_t           ie_presence_mask;
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_EPS_BEARER_ID        ((uint64_t)1)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_TFT                  ((uint64_t)1 << 1)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S1_U_SGW_FTEID       ((uint64_t)1 << 2)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S5_S8_U_PGW_FTEID    ((uint64_t)1 << 3)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S12_SGW_FTEID        ((uint64_t)1 << 4)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S4_U_SGW_FTEID       ((uint64_t)1 << 5)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S2B_U_PGW_FTEID      ((uint64_t)1 << 6)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S2A_U_PGW_FTEID      ((uint64_t)1 << 7)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_BEARER_LEVEL_QOS     ((uint64_t)1 << 9)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_CHARGING_ID          ((uint64_t)1 << 10)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_BEARER_FLAGS         ((uint64_t)1 << 11)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_PCO                  ((uint64_t)1 << 12)
#define GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_EPCO                 ((uint64_t)1 << 13)
  core::ebi_t                    eps_bearer_id;       ///< EBI,  Mandatory
  core::traffic_flow_template_t  tft;                 ///< Bearer TFT, Mandatory.
  core::fteid_t                  s1_u_sgw_fteid;
  core::fteid_t                  s5_s8_u_pgw_fteid;
  core::fteid_t                  s12_sgw_fteid;
  core::fteid_t                  s4_u_sgw_fteid;
  core::fteid_t                  s2b_u_pgw_fteid;
  core::fteid_t                  s2a_u_pgw_fteid;
  core::bearer_qos_t             bearer_level_qos;    ///< Bearer QoS, Mandatory CSR
  core::charging_id_t            charging_id;
  core::bearer_flags_t           bearer_flags;
  core::protocol_configuration_options_t  pco;
  core::extended_protocol_configuration_options_t  epco;
  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_EPS_BEARER_ID;}
  void set(const core::traffic_flow_template_t& v, const uint8_t instance = 0) {tft = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_TFT;}
  void set_s1_u_sgw_fteid(const core::fteid_t& v) {s1_u_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S1_U_SGW_FTEID;}
  void set_s5_s8_u_pgw_fteid(const core::fteid_t& v) {s5_s8_u_pgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S5_S8_U_PGW_FTEID;}
  void set_s12_sgw_fteid(const core::fteid_t& v) {s12_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S12_SGW_FTEID;}
  void set_s4_u_sgw_fteid(const core::fteid_t& v) {s4_u_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S4_U_SGW_FTEID;}
  void set_s2b_u_pgw_fteid(const core::fteid_t& v) {s2b_u_pgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S2B_U_PGW_FTEID;}
  void set_s2a_u_pgw_fteid(const core::fteid_t& v) {s2a_u_pgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S2A_U_PGW_FTEID;}
  void set(const core::bearer_qos_t& v, const uint8_t instance = 0) {bearer_level_qos = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_BEARER_LEVEL_QOS;}
  void set(const core::charging_id_t& v, const uint8_t instance = 0) {charging_id = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_CHARGING_ID;}
  void set(const core::bearer_flags_t& v, const uint8_t instance = 0) {bearer_flags = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_BEARER_FLAGS;}
  void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {pco = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_PCO;}
  void set(const core::extended_protocol_configuration_options_t& v, const uint8_t instance = 0) {epco = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_EPCO;}
  void set(const core::fteid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_s1_u_sgw_fteid(v); break;
    case 1: set_s5_s8_u_pgw_fteid(v); break;
    case 2: set_s12_sgw_fteid(v); break;
    case 3: set_s4_u_sgw_fteid(v); break;
    case 4: set_s2b_u_pgw_fteid(v); break;
    case 5: set_s2a_u_pgw_fteid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, instance);
    }
  }
  bearer_context_to_be_created_within_create_bearer_request()
  {
    ie_presence_mask = 0;
    eps_bearer_id = {};
    tft = {};
    s1_u_sgw_fteid = {};
    s5_s8_u_pgw_fteid = {};
    s12_sgw_fteid = {};
    s4_u_sgw_fteid = {};
    s2b_u_pgw_fteid = {};
    s2a_u_pgw_fteid = {};
    bearer_level_qos = {};
    charging_id = {};
    bearer_flags = {};
    pco = {};
    epco = {};
  }

  explicit bearer_context_to_be_created_within_create_bearer_request(const bearer_context& b) : bearer_context_to_be_created_within_create_bearer_request()
  {
    ie_presence_mask = 0;
    if (b.get(eps_bearer_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_EPS_BEARER_ID;
    if (b.get(tft, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_TFT;
    if (b.get(s1_u_sgw_fteid, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S1_U_SGW_FTEID;
    if (b.get(s5_s8_u_pgw_fteid, 1)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S5_S8_U_PGW_FTEID;
    if (b.get(s12_sgw_fteid, 2)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S12_SGW_FTEID;
    if (b.get(s4_u_sgw_fteid, 3)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S4_U_SGW_FTEID;
    if (b.get(s2b_u_pgw_fteid, 4)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S2B_U_PGW_FTEID;
    if (b.get(s2a_u_pgw_fteid, 5)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_S2A_U_PGW_FTEID;
    if (b.get(bearer_level_qos, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_BEARER_LEVEL_QOS;
    if (b.get(charging_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_CHARGING_ID;
    if (b.get(bearer_flags, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_BEARER_FLAGS;
    if (b.get(pco, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_PCO;
    if (b.get(epco, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_BEARER_REQUEST_PR_IE_EPCO;
  }
};

class gtpv2c_create_bearer_request : public gtpv2c_ies_container {
public:
  gtpv2c_create_bearer_request() {
    ie_presence_mask = 0;
    pti.procedure_transaction_id = 0;
    linked_eps_bearer_id.ebi = EPS_BEARER_IDENTITY_UNASSIGNED;
    pco = {};
    //bearer_contexts = {};
    pgw_fq_csid = {};
    sgw_fq_csid = {};
    indication_flags = {};
  }
  gtpv2c_create_bearer_request(const gtpv2c_create_bearer_request& i) :
    ie_presence_mask(i.ie_presence_mask),
    pti(i.pti),
    linked_eps_bearer_id(i.linked_eps_bearer_id),
    pco(i.pco),
    bearer_contexts(i.bearer_contexts),
    pgw_fq_csid(i.pgw_fq_csid),
    sgw_fq_csid(i.sgw_fq_csid),
    indication_flags(i.indication_flags) {}

  static const char* get_msg_name() {return "CREATE_BEARER_REQUEST";};

#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PROCEDURE_TRANSACTION_IDENTIFIER   ((uint64_t)1)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_LINKED_EPS_BEARER_ID               ((uint64_t)1 << 1)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PCO                                ((uint64_t)1 << 2)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_BEARER_CONTEXTS                    ((uint64_t)1 << 3)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PGW_FQ_CSID                        ((uint64_t)1 << 4)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_SGW_FQ_CSID                        ((uint64_t)1 << 5)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_CHANGE_REPORTING_ACTION            ((uint64_t)1 << 6)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_CSG_INFORMATION_REPORTING_ACTION   ((uint64_t)1 << 7)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_HENB_INFORMATION_REPORTING         ((uint64_t)1 << 8)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PRESENCE_REPORTING_AREA_ACTION     ((uint64_t)1 << 9)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_INDICATION_FLAGS                   ((uint64_t)1 << 10)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PGW_NODE_LEVEL_LOAD_CONTROL_INFORMATION ((uint64_t)1 << 11)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PGW_APN_LEVEL_LOAD_CONTROL_INFORMATION  ((uint64_t)1 << 12)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_SGW_NODE_LEVEL_LOAD_CONTROL_INFORMATION ((uint64_t)1 << 13)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PGW_OVERLOAD_CONTROL_INFORMATION   ((uint64_t)1 << 14)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION   ((uint64_t)1 << 15)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_NBIFOM_CONTAINER                   ((uint64_t)1 << 16)
#define GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PRIVATE_EXTENSION                  ((uint64_t)1 << 17)

  static const uint8_t msg_id = GTP_CREATE_BEARER_REQUEST;
  uint64_t           ie_presence_mask;

  core::procedure_transaction_id_t pti; ///< C: This IE shall be sent on the S5/S8 and S4/S11 interfaces
  ///< when the procedure was initiated by a UE Requested
  ///< Bearer Resource Modification Procedure or UE Requested
  ///< Bearer Resource Allocation Procedure (see NOTE 1) or
  ///< Secondary PDP Context Activation Procedure.
  ///< The PTI shall be the same as the one used in the
  ///< corresponding Bearer Resource Command.

  core::ebi_t                      linked_eps_bearer_id; ///< M: This IE shall be included to indicate the default bearer
  ///< associated with the PDN connection.

  core::protocol_configuration_options_t pco;///< O: This IE may be sent on the S5/S8 and S4/S11 interfaces

  std::vector<bearer_context_to_be_created_within_create_bearer_request> bearer_contexts;    ///< M: Several IEs with this type and instance values shall be
  ///< included as necessary to represent a list of Bearers.

  core::fq_csid_t                  pgw_fq_csid;       ///< C: This IE shall be included by MME on S11 and shall be
  ///< forwarded by SGW on S5/S8 according to the
  ///< requirements in 3GPP TS 23.007 [17].

  core::fq_csid_t                  sgw_fq_csid;       ///< C:This IE shall be included by the SGW on the S11 interface
  ///< according to the requirements in 3GPP TS 23.007 [17].

  //Change Reporting Action ///< This IE shall be included on the S5/S8 and S4/S11
  ///< interfaces with the appropriate Action field If the location
  ///< Change Reporting mechanism is to be started or stopped
  ///< for this subscriber in the SGSN/MME.

  //CSG Information ///< This IE shall be included on the S5/S8 and S4/S11
  ///< interfaces with the appropriate Action field if the CSG Info Reporting Action
  ///< reporting mechanism is to be started or stopped for this
  ///< subscriber in the SGSN/MME.

  core::indication_t indication_flags;

  // Private Extension   Private Extension

  void set(const core::procedure_transaction_id_t& v, const uint8_t instance = 0) {pti = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PROCEDURE_TRANSACTION_IDENTIFIER;}
  void set(const core::ebi_t& v, const uint8_t instance = 0) {linked_eps_bearer_id = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_REQUEST_PR_IE_LINKED_EPS_BEARER_ID;}
  void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {pco = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PCO;}
  void set(const bearer_context_to_be_created_within_create_bearer_request& v, const uint8_t instance = 0) {bearer_contexts.push_back(v);ie_presence_mask |= GTPV2C_CREATE_BEARER_REQUEST_PR_IE_BEARER_CONTEXTS;}
  void set_pgw_fq_csid(const core::fq_csid_t& v) {pgw_fq_csid = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_REQUEST_PR_IE_PGW_FQ_CSID;}
  void set_sgw_fq_csid(const core::fq_csid_t& v) {sgw_fq_csid = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_REQUEST_PR_IE_SGW_FQ_CSID;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_REQUEST_PR_IE_INDICATION_FLAGS;}
  void set(const core::fq_csid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_pgw_fq_csid(v); break;
    case 1: set_sgw_fq_csid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FQ_CSID, instance);
    }
  }
  void set(const bearer_context& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: {
        bearer_context_to_be_created_within_create_bearer_request b(v);
        set(b);
      }
      break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }
} ;

//-----------------------------------------------------------------------------
/** @class gtpv2c_create_bearer_response
 *  @brief Create Bearer Response
 *
 * The Create Bearer Response message shall be sent on the S5/S8 interface by the SGW to the PGW, and on the S11
 * interface by the MME to the SGW as part of the Dedicated Bearer Activation procedure.
 * The message shall also be sent on the S5/S8 interface by the SGW to the PGW and on the S4 interface by the SGSN to
 * the SGW as part of Secondary PDP Context Activation procedure or the Network Requested Secondary PDP Context
 * Activation procedure.
 * The message shall also be sent on the S2b interface by the ePDG to the PGW as part of the Dedicated S2b bearer
 * activation with GTP on S2b.
 * Possible Cause values are specified in Table 8.4-1. Message specific cause values are:
 * - "Request accepted".
 * - "Request accepted partially".
 * - "Context not found".
 * - "Semantic error in the TFT operation".
 * - "Syntactic error in the TFT operation".
 * - "Semantic errors in packet filter(s)".
 * - "Syntactic errors in packet filter(s)".
 * - "Service not supported".
 * - "Unable to page UE".
 * - "UE not responding".
 * - "Unable to page UE due to Suspension".
 * - "UE refuses".
 * - "Denied in RAT".
 * - "UE context without TFT already activated".
 */

class bearer_context_within_create_bearer_response {
public:
  uint32_t           ie_presence_mask;
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_EPS_BEARER_ID          ((uint64_t)1)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_CAUSE                  ((uint64_t)1 << 1)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S1_U_ENB_FTEID         ((uint64_t)1 << 2)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S1_U_SGW_FTEID         ((uint64_t)1 << 3)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S5_S8_U_SGW_FTEID      ((uint64_t)1 << 4)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S5_S8_U_PGW_FTEID      ((uint64_t)1 << 5)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S12_RNC_FTEID          ((uint64_t)1 << 6)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S12_SGW_FTEID          ((uint64_t)1 << 7)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S4_U_SGSN_FTEID        ((uint64_t)1 << 8)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S4_U_SGW_FTEID         ((uint64_t)1 << 9)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2B_U_EPDG_FTEID       ((uint64_t)1 << 10)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2B_U_PGW_FTEID        ((uint64_t)1 << 11)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2A_U_TWAN_FTEID       ((uint64_t)1 << 12)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2A_U_PGW_FTEID        ((uint64_t)1 << 13)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_PCO                    ((uint64_t)1 << 14)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_RAN_NAS_CAUSE          ((uint64_t)1 << 15)
#define GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_EPCO                   ((uint64_t)1 << 16)
  //----------------------------------------------------------------------------
  core::ebi_t         eps_bearer_id;
  core::cause_t       cause;
  core::fteid_t       s1_u_enb_fteid;
  core::fteid_t       s1_u_sgw_fteid;
  core::fteid_t       s5_s8_u_sgw_fteid;
  core::fteid_t       s5_s8_u_pgw_fteid;
  core::fteid_t       s12_rnc_fteid;
  core::fteid_t       s12_sgw_fteid;
  core::fteid_t       s4_u_sgsn_fteid;
  core::fteid_t       s4_u_sgw_fteid;
  core::fteid_t       s2b_u_epdg_fteid;
  core::fteid_t       s2b_u_pgw_fteid;
  core::fteid_t       s2a_u_twan_fteid;
  core::fteid_t       s2a_u_pgw_fteid;
  core::protocol_configuration_options_t pco;
  core::extended_protocol_configuration_options_t  epco;
  //----------------------------------------------------------------------------
  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_EPS_BEARER_ID;}
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_CAUSE;}
  void set_s1_u_enb_fteid(const core::fteid_t& v) {s1_u_enb_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S1_U_ENB_FTEID;}
  void set_s1_u_sgw_fteid(const core::fteid_t& v) {s1_u_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S1_U_SGW_FTEID;}
  void set_s5_s8_u_sgw_fteid(const core::fteid_t& v) {s5_s8_u_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S5_S8_U_SGW_FTEID;}
  void set_s5_s8_u_pgw_fteid(const core::fteid_t& v) {s5_s8_u_pgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S5_S8_U_PGW_FTEID;}
  void set_s12_rnc_fteid(const core::fteid_t& v) {s12_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S12_RNC_FTEID;}
  void set_s12_sgw_fteid(const core::fteid_t& v) {s12_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S12_SGW_FTEID;}
  void set_s4_u_sgsn_fteid(const core::fteid_t& v) {s4_u_sgsn_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S4_U_SGSN_FTEID;}
  void set_s4_u_sgw_fteid(const core::fteid_t& v) {s4_u_sgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S4_U_SGW_FTEID;}
  void set_s2b_u_epdg_fteid(const core::fteid_t& v) {s2b_u_epdg_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2B_U_EPDG_FTEID;}
  void set_s2b_u_pgw_fteid(const core::fteid_t& v) {s2b_u_pgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2B_U_PGW_FTEID;}
  void set_s2a_u_twan_fteid(const core::fteid_t& v) {s2a_u_twan_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2A_U_TWAN_FTEID;}
  void set_s2a_u_pgw_fteid(const core::fteid_t& v) {s2a_u_pgw_fteid = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2A_U_PGW_FTEID;}
  void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {pco = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_PCO;}
  void set(const core::extended_protocol_configuration_options_t& v, const uint8_t instance = 0) {epco = v; ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_EPCO;}
  void set(const core::fteid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_s1_u_enb_fteid(v); break;
    case 1: set_s1_u_sgw_fteid(v); break;
    case 2: set_s5_s8_u_sgw_fteid(v); break;
    case 3: set_s5_s8_u_pgw_fteid(v); break;
    case 4: set_s12_rnc_fteid(v); break;
    case 5: set_s12_sgw_fteid(v); break;
    case 6: set_s4_u_sgsn_fteid(v); break;
    case 7: set_s4_u_sgw_fteid(v); break;
    case 8: set_s2b_u_epdg_fteid(v); break;
    case 9: set_s2b_u_pgw_fteid(v); break;
    case 10: set_s2a_u_twan_fteid(v); break;
    case 11: set_s2a_u_pgw_fteid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, instance);
    }
  }
  bearer_context_within_create_bearer_response()
  {
    ie_presence_mask = 0;
    eps_bearer_id = {};
    cause = {};
    s1_u_enb_fteid = {};
    s1_u_sgw_fteid = {};
    s5_s8_u_sgw_fteid = {};
    s5_s8_u_pgw_fteid = {};
    s12_rnc_fteid = {};
    s12_sgw_fteid = {};
    s4_u_sgsn_fteid = {};
    s4_u_sgw_fteid = {};
    s2b_u_epdg_fteid = {};
    s2b_u_pgw_fteid = {};
    s2a_u_twan_fteid = {};
    s2a_u_pgw_fteid = {};
    pco = {};
    epco = {};
  }

  explicit bearer_context_within_create_bearer_response(const bearer_context& b) : bearer_context_within_create_bearer_response()
  {
    ie_presence_mask = 0;
    if (b.get(eps_bearer_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_EPS_BEARER_ID;
    if (b.get(cause, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_CAUSE;
    if (b.get(s1_u_enb_fteid, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S1_U_ENB_FTEID;
    if (b.get(s1_u_sgw_fteid, 1)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S1_U_SGW_FTEID;
    if (b.get(s5_s8_u_sgw_fteid, 2)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S5_S8_U_SGW_FTEID;
    if (b.get(s5_s8_u_pgw_fteid, 3)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S5_S8_U_PGW_FTEID;
    if (b.get(s12_rnc_fteid, 4)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S12_RNC_FTEID;
    if (b.get(s12_sgw_fteid, 5)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S12_SGW_FTEID;
    if (b.get(s4_u_sgsn_fteid, 6)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S4_U_SGSN_FTEID;
    if (b.get(s4_u_sgw_fteid, 7)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S4_U_SGW_FTEID;
    if (b.get(s2b_u_epdg_fteid, 8)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2B_U_EPDG_FTEID;
    if (b.get(s2b_u_pgw_fteid, 9)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2B_U_PGW_FTEID;
    if (b.get(s2a_u_twan_fteid, 10)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2A_U_TWAN_FTEID;
    if (b.get(s2a_u_pgw_fteid, 11)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_S2A_U_PGW_FTEID;
    if (b.get(pco, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_PCO;
    if (b.get(epco, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_CREATE_BEARER_RESPONSE_PR_IE_EPCO;
  }
};

class gtpv2c_create_bearer_response : public gtpv2c_ies_container {
public:
  gtpv2c_create_bearer_response() :
    ie_presence_mask(0),
    cause(),
    bearer_contexts(),
    mme_fq_csid(),
    sgw_fq_csid(),
    epdg_fq_csid(),
    twan_fq_csid(),
    pco(),
    ue_time_zone(),
    uli() {}

  gtpv2c_create_bearer_response(const gtpv2c_create_bearer_response& i) :
    ie_presence_mask(i.ie_presence_mask),
    cause(i.cause),
    bearer_contexts(i.bearer_contexts),
    mme_fq_csid(i.mme_fq_csid),
    sgw_fq_csid(i.sgw_fq_csid),
    epdg_fq_csid(i.epdg_fq_csid),
    twan_fq_csid(i.twan_fq_csid),
    pco(i.pco),
    ue_time_zone(i.ue_time_zone),
    uli(i.uli) {}

  static const char* get_msg_name() {return "CREATE_BEARER_RESPONSE";};

#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_CAUSE                              ((uint64_t)1)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_BEARER_CONTEXTS                    ((uint64_t)1 << 1)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_RECOVERY                           ((uint64_t)1 << 2)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_MME_FQ_CSID                        ((uint64_t)1 << 3)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_SGW_FQ_CSID                        ((uint64_t)1 << 4)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_EPDG_FQ_CSID                       ((uint64_t)1 << 5)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_TWAN_FQ_CSID                       ((uint64_t)1 << 6)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_PCO                                ((uint64_t)1 << 7)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_UE_TIME_ZONE                       ((uint64_t)1 << 8)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_ULI                                ((uint64_t)1 << 9)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_TWAN_IDENTIFIER                    ((uint64_t)1 << 10)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_MMES4_SGSN_OVERLOAD_CONTROL_INFORMATION ((uint64_t)1 << 11)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION   ((uint64_t)1 << 12)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_PRESENCE_REPORTING_AREA_INFORMATION ((uint64_t)1 << 13)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_MME_S4_SGSN_IDENTIFIER             ((uint64_t)1 << 14)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_TWAN_EPDG_OVERLOAD_CONTROL_INFORMATION ((uint64_t)1 << 15)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_WLAN_LOCATION_INFORMATION          ((uint64_t)1 << 16)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_WLAN_LOCATION_TIMESTAMP            ((uint64_t)1 << 17)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_UE_LOCAL_IP_ADDRESS                ((uint64_t)1 << 18)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_UE_UDP_PORT                        ((uint64_t)1 << 19)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_NBIFOM_CONTAINER                   ((uint64_t)1 << 20)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_UE_TCP_PORT                        ((uint64_t)1 << 21)
#define GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_PRIVATE_EXTENSION                  ((uint64_t)1 << 22)

  static const uint8_t msg_id = GTP_CREATE_BEARER_RESPONSE;
  uint64_t           ie_presence_mask;

  // here fields listed in 3GPP TS 29.274
  core::cause_t            cause;               ///< M

  std::vector<bearer_context_within_create_bearer_response> bearer_contexts;///< Several IEs with this type and instance value shall be
  ///< included on the S4/S11, S5/S8 and S2b interfaces as
  ///< necessary to represent a list of Bearers.

  // Recovery   C This IE shall be included on the S4/S11, S5/S8 and S2b interfaces if contacting the peer for the first time

  core::fq_csid_t                mme_fq_csid;         ///< C This IE shall be included by the MME on the S11
  ///< interface and shall be forwarded by the SGW on the S5/S8
  ///< interfaces according to the requirements in 3GPP TS
  ///< 23.007 [17].

  core::fq_csid_t                sgw_fq_csid;         ///< C This IE shall be included by the MME on the S11
  ///< interface and shall be forwarded by the SGW on the S5/S8
  ///< interfaces according to the requirements in 3GPP TS
  ///< 23.007 [17].

  core::fq_csid_t                epdg_fq_csid;         ///< C This IE shall be included by the ePDG on the S2b interface
  ///< according to the requirements in 3GPP TS 23.007 [17].
  core::fq_csid_t                twan_fq_csid;

  core::protocol_configuration_options_t pco;///< C: If the UE includes the PCO IE, then the MME/SGSN shall
  ///< copy the content of this IE transparently from the PCO IE
  ///< included by the UE. If the SGW receives PCO from
  ///< MME/SGSN, SGW shall forward it to the PGW.

  core::ue_time_zone_t             ue_time_zone;      ///< O: This IE is optionally included by the MME on the S11
  ///< interface or by the SGSN on the S4 interface.
  ///< CO: The SGW shall forward this IE on the S5/S8 interface if the
  ///< SGW supports this IE and it receives it from the
  ///< MME/SGSN.

  core::uli_t                    uli;              ///< O: This IE is optionally included by the MME on the S11
  ///< interface or by the SGSN on the S4 interface.
  ///< CO: The SGW shall forward this IE on the S5/S8 interface if the
  ///< SGW supports this IE and it receives it from the
  ///< MME/SGSN.

  // Private Extension Private Extension        ///< optional
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_CAUSE;}
  void set(const bearer_context_within_create_bearer_response& v) {bearer_contexts.push_back(v);ie_presence_mask |= GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_BEARER_CONTEXTS;}
  void set_mme_fq_csid(const core::fq_csid_t& v) {mme_fq_csid = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_MME_FQ_CSID;}
  void set_sgw_fq_csid(const core::fq_csid_t& v) {sgw_fq_csid = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_SGW_FQ_CSID;}
  void set_epdg_fq_csid(const core::fq_csid_t& v) {epdg_fq_csid = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_EPDG_FQ_CSID;}
  void set_twan_fq_csid(const core::fq_csid_t& v) {twan_fq_csid = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_TWAN_FQ_CSID;}
  void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {pco = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_PCO;}
  void set(const core::ue_time_zone_t& v, const uint8_t instance = 0) {ue_time_zone = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_UE_TIME_ZONE;}
  void set(const core::uli_t& v, const uint8_t instance = 0) {uli = v;ie_presence_mask |= GTPV2C_CREATE_BEARER_RESPONSE_PR_IE_ULI;}
  void set(const core::fq_csid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_mme_fq_csid(v); break;
    case 1: set_sgw_fq_csid(v); break;
    case 2: set_epdg_fq_csid(v); break;
    case 3: set_twan_fq_csid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FQ_CSID, instance);
    }
  }
  void set(const bearer_context& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: {
        bearer_context_within_create_bearer_response b(v);
        set(b);
      }
      break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }
};


//-----------------------------------------------------------------------------
/** @class gtpv2c_modify_bearer_request
 *  @brief Modify Bearer Request
 *
 * The Modify Bearer Request will be sent on S11 interface as
 * part of these procedures:
 * - E-UTRAN Tracking Area Update without SGW Change
 * - UE triggered Service Request
 * - S1-based Handover
 * - E-UTRAN Initial Attach
 * - UE requested PDN connectivity
 * - X2-based handover without SGWrelocation
 */
class bearer_context_to_be_modified_within_modify_bearer_request {
public:
  uint32_t           ie_presence_mask;
#define GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_EPS_BEARER_ID        ((uint64_t)1)
#define GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S1_U_ENB_FTEID       ((uint64_t)1 << 1)
#define GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S5_S8_U_SGW_FTEID    ((uint64_t)1 << 2)
#define GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S12_RNC_FTEID        ((uint64_t)1 << 3)
#define GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S4_U_SGSN_FTEID      ((uint64_t)1 << 4)
#define GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S11_U_MME_FTEID      ((uint64_t)1 << 5)
  core::ebi_t                    eps_bearer_id;
  core::fteid_t                  s1_u_enb_fteid;
  core::fteid_t                  s5_s8_u_sgw_fteid;
  core::fteid_t                  s12_rnc_fteid;
  core::fteid_t                  s4_u_sgsn_fteid;
  core::fteid_t                  s11_u_mme_fteid;

  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_EPS_BEARER_ID;}
  void set_s1_u_enb_fteid(const core::fteid_t& v) {s1_u_enb_fteid = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S1_U_ENB_FTEID;}
  void set_s5_s8_u_sgw_fteid(const core::fteid_t& v) {s5_s8_u_sgw_fteid = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S5_S8_U_SGW_FTEID;}
  void set_s12_rnc_fteid(const core::fteid_t& v) {s12_rnc_fteid = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S12_RNC_FTEID;}
  void set_s4_u_sgsn_fteid(const core::fteid_t& v) {s4_u_sgsn_fteid = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S4_U_SGSN_FTEID;}
  void set_s11_u_mme_fteid(const core::fteid_t& v) {s11_u_mme_fteid = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S11_U_MME_FTEID;}

  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_EPS_BEARER_ID) {v = eps_bearer_id;return true;}return false;}
  bool get_s1_u_enb_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S1_U_ENB_FTEID) {v = s1_u_enb_fteid;return true;}return false;}
  bool get_s5_s8_u_sgw_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S5_S8_U_SGW_FTEID) {v = s5_s8_u_sgw_fteid;return true;}return false;}
  bool get_s12_rnc_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S12_RNC_FTEID) {v = s12_rnc_fteid;return true;}return false;}
  bool get_s4_u_sgsn_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S4_U_SGSN_FTEID) {v = s4_u_sgsn_fteid;return true;}return false;}
  bool get_s11_u_mme_fteid(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S11_U_MME_FTEID) {v = s11_u_mme_fteid;return true;}return false;}

  bearer_context_to_be_modified_within_modify_bearer_request()
  {
    ie_presence_mask = 0;
    eps_bearer_id = {};
    s1_u_enb_fteid = {};
    s5_s8_u_sgw_fteid = {};
    s12_rnc_fteid = {};
    s4_u_sgsn_fteid = {};
    s11_u_mme_fteid = {};
  }

  explicit bearer_context_to_be_modified_within_modify_bearer_request(const bearer_context& b) : bearer_context_to_be_modified_within_modify_bearer_request()
  {
    ie_presence_mask = 0;
    if (b.get(eps_bearer_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_EPS_BEARER_ID;
    if (b.get(s1_u_enb_fteid, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S1_U_ENB_FTEID;
    if (b.get(s5_s8_u_sgw_fteid, 1)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S5_S8_U_SGW_FTEID;
    if (b.get(s12_rnc_fteid, 2)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S12_RNC_FTEID;
    if (b.get(s4_u_sgsn_fteid, 3)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S4_U_SGSN_FTEID;
    if (b.get(s11_u_mme_fteid, 4)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S11_U_MME_FTEID;
  }
  static bool predic_same_eps_bearer_id (const bearer_context_to_be_modified_within_modify_bearer_request& b1, const bearer_context_to_be_modified_within_modify_bearer_request& b2) {
    return (b1.eps_bearer_id.ebi == b2.eps_bearer_id.ebi);
  }
};

class bearer_context_to_be_removed_within_modify_bearer_request {
public:
  uint32_t           ie_presence_mask;
#define GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_EPS_BEARER_ID        ((uint64_t)1)
  core::ebi_t                    eps_bearer_id;

  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_EPS_BEARER_ID;}
  bearer_context_to_be_removed_within_modify_bearer_request()
  {
    ie_presence_mask = 0;
    eps_bearer_id = {};
  }

  explicit bearer_context_to_be_removed_within_modify_bearer_request(const bearer_context& b) : bearer_context_to_be_removed_within_modify_bearer_request()
  {
    ie_presence_mask = 0;
    if (b.get(eps_bearer_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_EPS_BEARER_ID;
  }
};

class gtpv2c_modify_bearer_request : public gtpv2c_ies_container {
public:
  gtpv2c_modify_bearer_request() :
    ie_presence_mask(0),
    mei(),
    uli(),
    serving_network(),
    indication_flags(),
    sender_fteid_for_cp(),
    apn_ambr(),
    bearer_contexts_to_be_modified(),
    bearer_contexts_to_be_removed(),
    ue_time_zone(),
    mme_fq_csid(),
    sgw_fq_csid(),
    uci() ,
    imsi()
  {
    rat_type.rat_type = core::RAT_TYPE_E_UTRAN;
    delay_dl_packet_notif_req.delay_value = 0;
  }
  gtpv2c_modify_bearer_request(const gtpv2c_modify_bearer_request& i) :
    ie_presence_mask(i.ie_presence_mask),
    mei(i.mei),
    uli(i.uli),
    serving_network(i.serving_network),
    rat_type(i.rat_type),
    indication_flags(i.indication_flags),
    sender_fteid_for_cp(i.sender_fteid_for_cp),
    apn_ambr(i.apn_ambr),
    delay_dl_packet_notif_req(i.delay_dl_packet_notif_req),
    bearer_contexts_to_be_modified(i.bearer_contexts_to_be_modified),
    bearer_contexts_to_be_removed(i.bearer_contexts_to_be_removed),
    ue_time_zone(i.ue_time_zone),
    mme_fq_csid(i.mme_fq_csid),
    sgw_fq_csid(i.sgw_fq_csid),
    uci(i.uci),
    imsi(i.imsi) {}

  static const char* get_msg_name() {return "MODIFY_BEARER_REQUEST";};

#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MEI                                 ((uint64_t)1)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_ULI                                 ((uint64_t)1 << 1)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SERVING_NETWORK                     ((uint64_t)1 << 2)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_RAT_TYPE                            ((uint64_t)1 << 3)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_INDICATION_FLAGS                    ((uint64_t)1 << 4)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE      ((uint64_t)1 << 5)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_APN_AMBR                            ((uint64_t)1 << 6)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_DELAY_DOWNLINK_PACKET_NOTIFICATION_REQUEST ((uint64_t)1 << 7)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_MODIFIED      ((uint64_t)1 << 8)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_REMOVED       ((uint64_t)1 << 9)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_RECOVERY                            ((uint64_t)1 << 10)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UE_TIME_ZONE                        ((uint64_t)1 << 11)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MME_FQ_CSID                         ((uint64_t)1 << 12)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SGW_FQ_CSID                         ((uint64_t)1 << 13)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UCI                                 ((uint64_t)1 << 14)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UE_LOCAL_IP_ADDRESS                 ((uint64_t)1 << 15)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UE_UDP_PORT                         ((uint64_t)1 << 16)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MME_S4_SGSN_LDN                     ((uint64_t)1 << 17)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SGW_LDN                             ((uint64_t)1 << 18)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_HENB_LOCAL_IP_ADDRESS               ((uint64_t)1 << 19)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_HENB_UDP_PORT                       ((uint64_t)1 << 20)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MME_S4_SGSN_IDENTIFIER              ((uint64_t)1 << 21)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_CN_OPERATOR_SELECTION_ENTITY        ((uint64_t)1 << 22)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_PRESENCE_REPORTING_AREA_INFORMATION ((uint64_t)1 << 23)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MMES4_SGSN_OVERLOAD_CONTROL_INFORMATION ((uint64_t)1 << 24)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION    ((uint64_t)1 << 25)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_EPDG_OVERLOAD_CONTROL_INFORMATION   ((uint64_t)1 << 26)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SERVING_PLMN_RATE_CONTROL           ((uint64_t)1 << 27)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MO_EXCEPTION_DATA_COUNTER           ((uint64_t)1 << 28)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_IMSI                                ((uint64_t)1 << 29)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_ULI_FOR_SGW                         ((uint64_t)1 << 30)
#define GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_PRIVATE_EXTENSION                   ((uint64_t)1 << 34)

  static const uint8_t msg_id = GTP_MODIFY_BEARER_REQUEST;
  uint64_t           ie_presence_mask;

  core::mei_t                      mei; ///< ME Identity (MEI)  ///< C:This IE shall be sent on the S5/S8 interfaces for the Gn/Gp
  ///< SGSN to MME TAU.

  core::uli_t                      uli;              ///< C: The MME/SGSN shall include this IE for
  ///< TAU/RAU/Handover procedures if the PGW has requested
  ///< location information change reporting and MME/SGSN
  ///< support location information change reporting.
  ///< An MME/SGSN which supports location information
  ///< change shall include this IE for UE-initiated Service
  ///< Request procedure if the PGW has requested location
  ///< information change reporting and the UE’s location info
  ///< has changed.
  ///< The SGW shall include this IE on S5/S8 if it receives the
  ///< ULI from MME/SGSN.
  ///< CO:This IE shall also be included on the S4/S11 interface for a
  ///< TAU/RAU/Handover with MME/SGSN change without
  ///< SGW change procedure, if the level of support (User
  ///< Location Change Reporting and/or CSG Information
  ///< Change Reporting) changes the MME shall include the
  ///< ECGI/TAI in the ULI, the SGSN shall include the CGI/SAI
  ///< in the ULI.
  ///< The SGW shall include this IE on S5/S8 if it receives the
  ///< ULI from MME/SGSN.

  core::serving_network_t           serving_network;  ///< CO:This IE shall be included on S11/S4 interface during the
  ///< following procedures:
  ///< - TAU/RAU/handover if Serving Network is changed.
  ///< - TAU/RAU when the UE was ISR activated which is
  ///<   indicated by ISRAU flag.
  ///< - UE triggered Service Request when UE is ISR
  ///<   activated.
  ///< - UE initiated Service Request if ISR is not active, but
  ///<   the Serving Network has changed during previous
  ///<   mobility procedures, i.e. intra MME/S4-SGSN
  ///<   TAU/RAU and the change has not been reported to
  ///<   the PGW yet.
  ///< - TAU/RAU procedure as part of the optional network
  ///<   triggered service restoration procedure with ISR, as
  ///<   specified by 3GPP TS 23.007 [17].
  ///<
  ///< CO:This IE shall be included on S5/S8 if the SGW receives this
  ///< IE from MME/SGSN and if ISR is not active.
  ///< This IE shall be included on S5/S8 if the SGW receives this
  ///< IE from MME/SGSN and ISR is active and the Modify
  ///< Bearer Request message needs to be sent to the PGW as
  ///< specified in the 3GPP TS 23.401 [3].

  core::rat_type_t                 rat_type;         ///< C: This IE shall be sent on the S11 interface for a TAU with
  ///< an SGSN interaction, UE triggered Service Request or an I-
  ///< RAT Handover.
  ///< This IE shall be sent on the S4 interface for a RAU with
  ///< MME interaction, a RAU with an SGSN change, a UE
  ///< Initiated Service Request or an I-RAT Handover.
  ///< This IE shall be sent on the S5/S8 interface if the RAT type
  ///< changes.
  ///< CO: CO If SGW receives this IE from MME/SGSN during a
  ///< TAU/RAU/Handover with SGW change procedure, the
  ///< SGW shall forward it across S5/S8 interface to PGW.
  ///< CO: The IE shall be sent on the S11/S4 interface during the
  ///< following procedures:
  ///< - an inter MM TAU or inter SGSN RAU when UE was
  ///<   ISR activated which is indicated by ISRAU flag.
  ///< - TAU/RAU procedure as part of optional network
  ///<   triggered service restoration procedure with ISR, as
  ///<   specified by 3GPP TS 23.007 [17].
  ///< If ISR is active, this IE shall also be included on the S11
  ///< interface in the S1-U GTP-U tunnel setup procedure during
  ///< an intra-MME intra-SGW TAU procedure.

  core::indication_t         indication_flags; ///< C:This IE shall be included if any one of the applicable flags
  ///< is set to 1.
  ///< Applicable flags are:
  ///< -ISRAI: This flag shall be used on S4/S11 interface
  ///<   and set to 1 if the ISR is established between the
  ///<   MME and the S4 SGSN.
  ///< - Handover Indication: This flag shall be set to 1 on
  ///<   the S4/S11 and S5/S8 interfaces during an E-
  ///<   UTRAN Initial Attach or for a UE Requested PDN
  ///<   Connectivity or a PDP Context Activation
  ///<   procedure, if the PDN connection/PDP context is
  ///<   handed-over from non-3GPP access.
  ///< - Direct Tunnel Flag: This flag shall be used on the
  ///<   S4 interface and set to 1 if Direct Tunnel is used.
  ///< - Change Reporting support Indication: shall be
  ///<   used on S4/S11, S5/S8 and set if the SGSN/MME
  ///<   supports location Info Change Reporting. This flag
  ///<   should be ignored by SGW if no message is sent
  ///<   on S5/S8. See NOTE 4.
  ///< - CSG Change Reporting Support Indication: shall
  ///<   be used on S4/S11, S5/S8 and set if the
  ///<   SGSN/MME supports CSG Information Change
  ///<   Reporting. This flag shall be ignored by SGW if no
  ///<   message is sent on S5/S8. See NOTE 4.
  ///< - Change F-TEID support Indication: This flag shall
  ///<   be used on S4/S11 for an IDLE state UE initiated
  ///<   TAU/RAU procedure and set to 1 to allow the
  ///<   SGW changing the GTP-U F-TEID.

  core::fteid_t                  sender_fteid_for_cp; ///< C: Sender F-TEID for control plane
  ///< This IE shall be sent on the S11 and S4 interfaces for a
  ///< TAU/RAU/ Handover with MME/SGSN change and without
  ///< any SGW change.
  ///< This IE shall be sent on the S5 and S8 interfaces for a
  ///< TAU/RAU/Handover with a SGW change.

  core::ambr_t                   apn_ambr;            ///< C: Aggregate Maximum Bit Rate (APN-AMBR)
  ///< The APN-AMBR shall be sent for the PS mobility from the
  ///< Gn/Gp SGSN to the S4 SGSN/MME procedures..

  /* Delay Value in integer multiples of 50 millisecs, or zero */
  core::delay_value_t               delay_dl_packet_notif_req; ///<C:This IE shall be sent on the S11 interface for a UE
  ///< triggered Service Request.
  ///< CO: This IE shall be sent on the S4 interface for a UE triggered
  ///< Service Request.

  std::vector<bearer_context_to_be_modified_within_modify_bearer_request> bearer_contexts_to_be_modified;///< C: This IE shall be sent on the S4/S11 interface and S5/S8
  ///< interface except on the S5/S8 interface for a UE triggered
  ///< Service Request.
  ///< When Handover Indication flag is set to 1 (i.e., for
  ///< EUTRAN Initial Attach or UE Requested PDN Connectivity
  ///< when the UE comes from non-3GPP access), the PGW
  ///< shall ignore this IE. See NOTE 1.
  ///< Several IEs with the same type and instance value may be
  ///< included as necessary to represent a list of Bearers to be
  ///< modified.
  ///< During a TAU/RAU/Handover procedure with an SGW
  ///< change, the SGW includes all bearers it received from the
  ///< MME/SGSN (Bearer Contexts to be created, or Bearer
  ///< Contexts to be modified and also Bearer Contexts to be
  ///< removed) into the list of 'Bearer Contexts to be modified'
  ///< IEs, which are then sent on the S5/S8 interface to the
  ///< PGW (see NOTE 2).

  std::vector<bearer_context_to_be_removed_within_modify_bearer_request> bearer_contexts_to_be_removed;    ///< C: This IE shall be included on the S4 and S11 interfaces for
  ///< the TAU/RAU/Handover and Service Request procedures
  ///< where any of the bearers existing before the
  ///< TAU/RAU/Handover procedure and Service Request
  ///< procedures will be deactivated as consequence of the
  ///< TAU/RAU/Handover procedure and Service Request
  ///< procedures. (NOTE 3)
  ///< For each of those bearers, an IE with the same type and
  ///< instance value, shall be included.

  // recovery_t(restart counter) recovery;      ///< C: This IE shall be included if contacting the peer for the first
  ///< time.

  core::ue_time_zone_t               ue_time_zone;      ///< CO: This IE shall be included by the MME/SGSN on the S11/S4
  ///< interfaces if the UE Time Zone has changed in the case of
  ///< TAU/RAU/Handover.
  ///< C: If SGW receives this IE, SGW shall forward it to PGW
  ///< across S5/S8 interface.

  core::fq_csid_t                  mme_fq_csid;       ///< C: This IE shall be included by MME on S11 and shall be
  ///< forwarded by SGW on S5/S8 according to the
  ///< requirements in 3GPP TS 23.007 [17].

  core::fq_csid_t                  sgw_fq_csid;       ///< C: This IE shall be included by SGW on S5/S8 according to
  ///< the requirements in 3GPP TS 23.007 [17].

  core::uci_t                      uci;               ///< CO: The MME/SGSN shall include this IE for
  ///< TAU/RAU/Handover procedures and UE-initiated Service
  ///< Request procedure if the PGW has requested CSG Info
  ///< reporting and the MME/SGSN support the CSG
  ///< information reporting. The SGW shall include this IE on
  ///< S5/S8 if it receives the User CSG Information from
  ///< MME/SGSN.

  // UE Local IP Address: CO If the UE local IP Address has changed, the ePDG shall
  // include this IE on S2b interface based on local policy for Fixed Broadband access network interworking (see 3GPP
  // TS 23.139 [51]).

  // UE UDP Port: CO: The ePDG shall include this IE on S2b interface if NAT is detected and UE Local IP Address is present for Fixed
  // Broadband access network interworking (see 3GPP TS 23.139 [51]).

  // Local Distinguished Name (LDN) MME/S4-SGSN LDN ///< O: This IE is optionally sent by the MME to the SGW on the
  ///< S11 interface and by the SGSN to the SGW on the S4
  ///< interface (see 3GPP TS 32.423 [44]), when communicating
  ///< the LDN to the peer node for the first time.

  // Local Distinguished Name (LDN) SGW LDN     ///< O: This IE is optionally sent by the SGW to the PGW on the
  ///< S5/S8 interfaces (see 3GPP TS 32.423 [44]), for inter-
  ///< SGW mobity, when communicating the LDN to the peer
  ///< node for the first time.

  // H(e)NB Local IP Address CO
  // H(e)NB UDP Port
  // MME/S4-SGSN Identifier
  // CN Operator Selection Entity
  // Presence Reporting Area Information
  // MME/S4-SGSN's Overload Control Information
  // SGW's Overload Control Information
  // ePDG's Overload Control Information
  // Serving PLMN Rate Control
  // MO Exception Data Counter

  core::imsi_t            imsi; ///< O: The MME/SGSN may include the IMSI if available.
  // core::uli_t             user_location_information_for_sgw; ///< CO: The MME/SGSN shall include this IE on the S11/S4
                                                       ///< interface, based on operator policy for the User Location
  ///< Information to be sent to the SGW, if the user location
  ///< information to be passed to the SGW is not already
  ///< reported in the ULI IE in this message.
  // WLAN Location Information
  // WLAN Location Timestamp
  // Secondary RAT Usage Data Report
  // Private Extension   Private Extension
  void set(const core::mei_t& v, const uint8_t instance = 0) {mei = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MEI;}
  void set(const core::uli_t& v, const uint8_t instance = 0) {uli = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_ULI;}
  void set(const core::serving_network_t& v, const uint8_t instance = 0) {serving_network = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SERVING_NETWORK;}
  void set(const core::rat_type_t& v, const uint8_t instance = 0) {rat_type = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_RAT_TYPE;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_INDICATION_FLAGS;}
  void set_sender_fteid_for_cp(const core::fteid_t& v) {sender_fteid_for_cp = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE;}
  void set(const core::ambr_t& v, const uint8_t instance = 0) {apn_ambr = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_APN_AMBR;}
  void set(const core::delay_value_t& v, const uint8_t instance = 0) {delay_dl_packet_notif_req = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_DELAY_DOWNLINK_PACKET_NOTIFICATION_REQUEST;}
  void add_bearer_context_to_be_modified(const bearer_context_to_be_modified_within_modify_bearer_request& v) {bearer_contexts_to_be_modified.push_back(v);ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_MODIFIED;}
  void add_bearer_context_to_be_removed(const bearer_context_to_be_removed_within_modify_bearer_request& v) {bearer_contexts_to_be_removed.push_back(v);ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_REMOVED;}
  void set(const core::ue_time_zone_t& v, const uint8_t instance = 0) {ue_time_zone = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UE_TIME_ZONE;}
  void set_mme_fq_csid(const core::fq_csid_t& v) {mme_fq_csid = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MME_FQ_CSID;}
  void set_sgw_fq_csid(const core::fq_csid_t& v) {sgw_fq_csid = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SGW_FQ_CSID;}
  void set(const core::uci_t& v, const uint8_t instance = 0) {uci = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UCI;}
  void set(const core::imsi_t& v, const uint8_t instance = 0) {imsi = v;ie_presence_mask |= GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_IMSI;}
  void set(const bearer_context& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: {
        bearer_context_to_be_modified_within_modify_bearer_request b(v);
        add_bearer_context_to_be_modified(b);
      }
      break;
    case 1: {
        bearer_context_to_be_removed_within_modify_bearer_request b(v);
        add_bearer_context_to_be_removed(b);
      }
      break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }

  bool get(core::mei_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MEI) {v = mei;return true;}return false;}
  bool get(core::uli_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_ULI) {v = uli;return true;}return false;}
  bool get(core::serving_network_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SERVING_NETWORK) {v = serving_network;return true;}return false;}
  bool get(core::rat_type_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_RAT_TYPE) {v = rat_type;return true;}return false;}
  bool get(core::indication_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_INDICATION_FLAGS) {v = indication_flags;return true;}return false;}
  bool get_sender_fteid_for_cp(core::fteid_t& v) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE) {v = sender_fteid_for_cp;return true;}return false;}
  bool get(core::ambr_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_APN_AMBR) {v = apn_ambr;return true;}return false;}
  bool get(core::delay_value_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_DELAY_DOWNLINK_PACKET_NOTIFICATION_REQUEST) {v = delay_dl_packet_notif_req;return true;}return false;}
  bool get(core::ue_time_zone_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UE_TIME_ZONE) {v = ue_time_zone;return true;}return false;}
  bool get_mme_fq_csid(core::fq_csid_t& v) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MME_FQ_CSID) {v = mme_fq_csid;return true;}return false;}
  bool get_sgw_fq_csid(core::fq_csid_t& v) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SGW_FQ_CSID) {v = sgw_fq_csid;return true;}return false;}
  bool get(core::uci_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UCI) {v = uci;return true;}return false;}
  bool get(core::imsi_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_IMSI) {v = imsi;return true;}return false;}

  bool get(core::fteid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: return get_sender_fteid_for_cp(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER, instance);
    }
  }
  bool get(core::fq_csid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: return get_mme_fq_csid(v); break;
    case 1: return get_sgw_fq_csid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FQ_CSID, instance);
    }
  }
  bool has_bearer_context_to_be_modified() const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_MODIFIED) return true; else return false;}
  bool has_bearer_context_to_be_removed() const {if (ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_REMOVED) return true; else return false;}

} ;

//-----------------------------------------------------------------------------
/** @class gtpv2c_modify_bearer_response
 *  @brief Modify Bearer Response
 *
 * The Modify Bearer Response will be sent on S11 interface as
 * part of these procedures:
 * - E-UTRAN Tracking Area Update without SGW Change
 * - UE triggered Service Request
 * - S1-based Handover
 * - E-UTRAN Initial Attach
 * - UE requested PDN connectivity
 * - X2-based handover without SGWrelocation
 */
class bearer_context_modified_within_modify_bearer_response {
public:
  std::pair<bool,core::ebi_t>                    eps_bearer_id;
  std::pair<bool,core::cause_t>                  cause;
  std::pair<bool,core::fteid_t>                  s1_u_sgw_fteid;
  std::pair<bool,core::fteid_t>                  s12_sgw_fteid;
  std::pair<bool,core::fteid_t>                  s4_u_sgw_fteid;
  std::pair<bool,core::charging_id_t>            charging_id;
  std::pair<bool,core::bearer_flags_t>           bearer_flags;
  std::pair<bool,core::fteid_t>                  s11_u_sgw_fteid;

  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id.first = true; eps_bearer_id.second = v;}
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause.first = true; cause.second = v;}
  void set_s1_u_sgw_fteid(const core::fteid_t& v, const uint8_t instance = 0) {s1_u_sgw_fteid.first = true; s1_u_sgw_fteid.second = v;}
  void set_s12_sgw_fteid(const core::fteid_t& v, const uint8_t instance = 0) {s12_sgw_fteid.first = true; s12_sgw_fteid.second = v;}
  void set_s4_u_sgw_fteid(const core::fteid_t& v, const uint8_t instance = 0) {s4_u_sgw_fteid.first = true; s4_u_sgw_fteid.second = v;}

  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (eps_bearer_id.first) {v = eps_bearer_id.second;return true;}return false;}
  bool get(core::cause_t& v, const uint8_t instance = 0) const {if (cause.first) {v = cause.second;return true;}return false;}
  bool get_s1_u_sgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (s1_u_sgw_fteid.first) {v = s1_u_sgw_fteid.second;return true;}return false;}
  bool get_s12_sgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (s12_sgw_fteid.first) {v = s12_sgw_fteid.second;return true;}return false;}
  bool get_s4_u_sgw_fteid(core::fteid_t& v, const uint8_t instance = 0) const {if (s4_u_sgw_fteid.first) {v = s4_u_sgw_fteid.second;return true;}return false;}

  bearer_context_modified_within_modify_bearer_response() :
    eps_bearer_id(),
    cause(),
    s1_u_sgw_fteid(),
    s12_sgw_fteid(),
    s4_u_sgw_fteid(),
    charging_id(),
    bearer_flags(),
    s11_u_sgw_fteid() {}

  explicit bearer_context_modified_within_modify_bearer_response(const bearer_context& b) :
    eps_bearer_id(b.eps_bearer_id),
    cause(b.cause),
    charging_id(b.charging_id),
    bearer_flags(b.bearer_flags)
  {
    if (b.get(s1_u_sgw_fteid.second, 0)) s1_u_sgw_fteid.first = true;
    if (b.get(s12_sgw_fteid.second, 1)) s12_sgw_fteid.first = true;
    if (b.get(s4_u_sgw_fteid.second, 2)) s4_u_sgw_fteid.first = true;
    if (b.get(s11_u_sgw_fteid.second, 3)) s11_u_sgw_fteid.first = true;
  }
};

class bearer_context_marked_for_removal_within_modify_bearer_response {
public:
  std::pair<bool,core::ebi_t>                    eps_bearer_id;
  std::pair<bool, core::cause_t>                 cause;

  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id.first = true; eps_bearer_id.second = v;}
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause.first = true; cause.second = v;}

  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (eps_bearer_id.first) {v = eps_bearer_id.second;return true;}return false;}
  bool get(core::cause_t& v, const uint8_t instance = 0) const {if (cause.first) {v = cause.second;return true;}return false;}

  bearer_context_marked_for_removal_within_modify_bearer_response() :
    eps_bearer_id(),
    cause() {}

  explicit bearer_context_marked_for_removal_within_modify_bearer_response(const bearer_context& b) :
    eps_bearer_id(b.eps_bearer_id),
    cause(b.cause) {}
};

class gtpv2c_modify_bearer_response : public gtpv2c_ies_container {
public:
  gtpv2c_modify_bearer_response() :
    cause(),
    msisdn(),
    linked_eps_bearer_id(),
    apn_restriction(),
    pco(),
    bearer_contexts_modified(),
    bearer_contexts_marked_for_removal(),
    pgw_fq_csid(),
    sgw_fq_csid(),
    indication_flags(),
    pdn_connection_charging_id() {}

  gtpv2c_modify_bearer_response(const gtpv2c_modify_bearer_response& i) :
    cause(i.cause),
    msisdn(i.msisdn),
    linked_eps_bearer_id(i.linked_eps_bearer_id),
    apn_restriction(i.apn_restriction),
    pco(i.pco),
    bearer_contexts_modified(i.bearer_contexts_modified),
    bearer_contexts_marked_for_removal(i.bearer_contexts_marked_for_removal),
    pgw_fq_csid(i.pgw_fq_csid),
    sgw_fq_csid(i.sgw_fq_csid),
    indication_flags(i.indication_flags),
    pdn_connection_charging_id(i.pdn_connection_charging_id) {}

  static const char* get_msg_name() {return "MODIFY_BEARER_RESPONSE";};

  static const uint8_t msg_id = GTP_MODIFY_BEARER_RESPONSE;

  // here fields listed in 3GPP TS 29.274
  std::pair<bool, core::cause_t>            cause;               ///<
  std::pair<bool, core::msisdn_t>           msisdn;
  std::pair<bool, core::ebi_t>                    linked_eps_bearer_id;///< This IE shall be sent on S5/S8 when the UE moves from a
  ///< Gn/Gp SGSN to the S4 SGSN or MME to identify the
  ///< default bearer the PGW selects for the PDN Connection.
  ///< This IE shall also be sent by SGW on S11, S4 during
  ///< Gn/Gp SGSN to S4-SGSN/MME HO procedures to identify
  ///< the default bearer the PGW selects for the PDN
  ///< Connection.


  std::pair<bool, core::access_point_name_restriction_t>         apn_restriction;     ///< This IE denotes the restriction on the combination of types
  ///< of APN for the APN associated with this EPS bearer
  ///< Context. This IE shall be included over S5/S8 interfaces,
  ///< and shall be forwarded over S11/S4 interfaces during
  ///< Gn/Gp SGSN to MME/S4-SGSN handover procedures.
  ///< This IE shall also be included on S5/S8 interfaces during
  ///< the Gn/Gp SGSN to S4 SGSN/MME RAU/TAU
  ///< procedures.
  ///< The target MME or SGSN determines the Maximum APN
  ///< Restriction using the APN Restriction.

  std::pair<bool, core::protocol_configuration_options_t>         pco; ///< If SGW receives this IE from PGW on GTP or PMIP based
  ///< S5/S8, the SGW shall forward PCO to MME/S4-SGSN
  ///< during Inter RAT handover from the UTRAN or from the
  ///< GERAN to the E-UTRAN. See NOTE 2:
  ///< If MME receives the IE, but no NAS message is sent, MME discards the IE.

  std::pair<bool, std::vector<bearer_context_modified_within_modify_bearer_response>> bearer_contexts_modified;///< EPS bearers corresponding to Bearer Contexts to be
  ///< modified that were sent in Modify Bearer Request
  ///< message. Several IEs with the same type and instance
  ///< value may be included as necessary to represent a list of
  ///< the Bearers which are modified.

  std::pair<bool, std::vector<bearer_context_marked_for_removal_within_modify_bearer_response>> bearer_contexts_marked_for_removal; ///< EPS bearers corresponding to Bearer Contexts to be
  ///< removed sent in the Modify Bearer Request message.
  ///< Shall be included if request message contained Bearer
  ///< Contexts to be removed.
  ///< For each of those bearers an IE with the same type and
  ///< instance value shall be included.

  // change_reporting_action                    ///< This IE shall be included with the appropriate Action field If
  ///< the location Change Reporting mechanism is to be started
  ///< or stopped for this subscriber in the SGSN/MME.

  // csg_Information_reporting_action           ///< This IE shall be included with the appropriate Action field if
  ///< the location CSG Info change reporting mechanism is to be
  ///< started or stopped for this subscriber in the SGSN/MME.

  // H(e)NB Information reporting

  // Charging Gateway Name                 ///< When Charging Gateway Function (CGF) Address is
  ///< configured, the PGW shall include this IE on the S5
  ///< interface during SGW relocation and when the UE moves
  ///< from Gn/Gp SGSN to S4-SGSN/MME. See NOTE 1:

  // IP Address Charging Gateway Address        ///< When Charging Gateway Function (CGF) Address is
  ///< configured, the PGW shall include this IE on the S5
  ///< interface during SGW relocation and when the UE moves
  ///< from Gn/Gp SGSN to S4-SGSN/MME. See NOTE 1:
  ///< Both Charging Gateway Name and Charging Gateway Address shall not be included at the same
  ///< time. When both are available, the operator configures a preferred value.

  std::pair<bool, core::fq_csid_t>                pgw_fq_csid;         ///< This IE shall be included by PGW on S5/S8and shall be
  ///< forwarded by SGW on S11 according to the requirements
  ///< in 3GPP TS 23.007 [17].

  std::pair<bool, core::fq_csid_t>                sgw_fq_csid;         ///< This IE shall be included by SGW on S11 according to the
  ///< requirements in 3GPP TS 23.007 [17].

  // recovery_t(restart counter) recovery;      ///< This IE shall be included if contacting the peer for the first
  ///< time.

  // Local Distinguished Name (LDN) SGW LDN     ///< This IE is optionally sent by the SGW to the MME/SGSN
  ///< on the S11/S4 interfaces (see 3GPP TS 32.423 [44]),
  ///< when contacting the peer node for the first time.

  // Local Distinguished Name (LDN) PGW LDN     ///< This IE is optionally included by the PGW on the S5/S8
  ///< and S2b interfaces (see 3GPP TS 32.423 [44]), when
  ///< contacting the peer node for the first time.

  std::pair<bool, core::indication_t>              indication_flags;
  // Presence Reporting Area Action
  // PGW's node level Load Control Information
  // PGW's APN level Load Control Information
  // SGW's node level Load Control Information
  // PGW's Overload Control Information
  // SGW's Overload Control Information
  std::pair<bool, core::charging_id_t>              pdn_connection_charging_id;
  // Private Extension Private Extension        ///< optional

  void set(const core::cause_t& v, const uint8_t instance = 0) {cause.first = true; cause.second = v;}
  void set(const core::msisdn_t& v, const uint8_t instance = 0) {msisdn.first = true; msisdn.second = v;}
  void set(const core::ebi_t& v, const uint8_t instance = 0) {linked_eps_bearer_id.first = true; linked_eps_bearer_id.second = v;}
  void set(const core::access_point_name_restriction_t& v, const uint8_t instance = 0) {apn_restriction.first = true; apn_restriction.second = v;}
  void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {pco.first = true; pco.second = v;}
  void add_bearer_context_modified(const bearer_context_modified_within_modify_bearer_response& v) {
    bearer_contexts_modified.second.push_back(v);
    bearer_contexts_modified.first = true;
  }
  void add_bearer_context_marked_for_removal(const bearer_context_marked_for_removal_within_modify_bearer_response& v) {
    bearer_contexts_marked_for_removal.second.push_back(v);
    bearer_contexts_marked_for_removal.first = true;
  }
  void add(const bearer_context& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: {
        bearer_context_modified_within_modify_bearer_response b(v);
        add_bearer_context_modified(b);
      }
      break;
    case 1: {
      bearer_context_marked_for_removal_within_modify_bearer_response b(v);
        add_bearer_context_marked_for_removal(b);
      }
      break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }
  void set(const bearer_context& v, const uint8_t instance = 0) {
    add(v,instance);
  }

  void set_pgw_fq_csid(const core::fq_csid_t& v) {pgw_fq_csid.first = true; pgw_fq_csid.second = v;}
  void set_sgw_fq_csid(const core::fq_csid_t& v) {sgw_fq_csid.first = true; sgw_fq_csid.second = v;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags.first = true; indication_flags.second = v;}
  void set(const core::charging_id_t& v, const uint8_t instance = 0) {pdn_connection_charging_id.first = true; pdn_connection_charging_id.second = v;}

  void set(const core::fq_csid_t& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: set_pgw_fq_csid(v); break;
    case 1: set_sgw_fq_csid(v); break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_FQ_CSID, instance);
    }
  }

  bool get(core::cause_t& v, const uint8_t instance = 0) const {if (cause.first) {v = cause.second;return true;}return false;}
  bool get(core::msisdn_t& v, const uint8_t instance = 0) const {if (msisdn.first) {v = msisdn.second;return true;}return false;}
  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (linked_eps_bearer_id.first) {v = linked_eps_bearer_id.second;return true;}return false;}
  bool get(core::access_point_name_restriction_t& v, const uint8_t instance = 0) const {if (apn_restriction.first) {v = apn_restriction.second;return true;}return false;}
  bool get(core::protocol_configuration_options_t& v, const uint8_t instance = 0) const {if (pco.first) {v = pco.second;return true;}return false;}
  bool get_pgw_fq_csid(core::fq_csid_t& v) const {if (pgw_fq_csid.first) {v = pgw_fq_csid.second;return true;}return false;}
  bool get_sgw_fq_csid(core::fq_csid_t& v) const {if (sgw_fq_csid.first) {v = sgw_fq_csid.second;return true;}return false;}
  bool get(core::indication_t& v, const uint8_t instance = 0) const {if (indication_flags.first) {v = indication_flags.second;return true;}return false;}
  bool get(core::charging_id_t& v) const {if (pdn_connection_charging_id.first) {v = pdn_connection_charging_id.second;return true;}return false;}

  bool has_bearer_context_modified() const {if (bearer_contexts_modified.first) {return true;}return false;}
  bool has_bearer_context_marked_for_removal() const {if (bearer_contexts_marked_for_removal.first) {return true;}return false;}

} ;

//-----------------------------------------------------------------------------
class gtpv2c_delete_session_request : public gtpv2c_ies_container {
public:
  gtpv2c_delete_session_request() :
    ie_presence_mask(0),
    cause(),
    uli(),
    indication_flags(),
    pco(),
    originating_node(),
    sender_fteid_for_cp(),
    ue_time_zone(),
    ran_nas_release_cause(),
    epco()
  {
    linked_eps_bearer_id.ebi = EPS_BEARER_IDENTITY_UNASSIGNED;
  }
  gtpv2c_delete_session_request(const gtpv2c_delete_session_request& i) :
    ie_presence_mask(i.ie_presence_mask),
    cause(i.cause),
    linked_eps_bearer_id(i.linked_eps_bearer_id),
    uli(i.uli),
    indication_flags(i.indication_flags),
    pco(i.pco),
    originating_node(i.originating_node),
    sender_fteid_for_cp(i.sender_fteid_for_cp),
    ue_time_zone(i.ue_time_zone),
    ran_nas_release_cause(i.ran_nas_release_cause),
    epco(i.epco) {}
    
  static const char* get_msg_name() {return "DELETE_SESSION_REQUEST";};

#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_CAUSE                              ((uint64_t)1)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_LINKED_EPS_BEARER_ID               ((uint64_t)1 << 1)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_ULI                                ((uint64_t)1 << 2)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_INDICATION_FLAGS                   ((uint64_t)1 << 3)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_PCO                                ((uint64_t)1 << 4)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_ORIGINATING_NODE                   ((uint64_t)1 << 5)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE     ((uint64_t)1 << 6)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_UE_TIME_ZONE                       ((uint64_t)1 << 7)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_ULI_TIMESTAMP                      ((uint64_t)1 << 8)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_RAN_NAS_RELEASE_CAUSE              ((uint64_t)1 << 9)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_TWAN_IDENTIFIER                    ((uint64_t)1 << 10)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_TWAN_IDENTIFIER_TIMESTAMP          ((uint64_t)1 << 11)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_MMES4_SGSN_OVERLOAD_CONTROL_INFORMATION ((uint64_t)1 << 12)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION   ((uint64_t)1 << 13)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_TWAN_EPDG_OVERLOAD_CONTROL_INFORMATION ((uint64_t)1 << 14)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_WLAN_LOCATION_INFORMATION          ((uint64_t)1 << 15)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_WLAN_LOCATION_TIMESTAMP            ((uint64_t)1 << 16)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_UE_LOCAL_IP_ADDRESS                ((uint64_t)1 << 17)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_UE_UDP_PORT                        ((uint64_t)1 << 18)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_EPCO                               ((uint64_t)1 << 19)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_UE_TCP_PORT                        ((uint64_t)1 << 20)
#define GTPV2C_DELETE_SESSION_REQUEST_PR_IE_PRIVATE_EXTENSION                  ((uint64_t)1 << 21)

  static const uint8_t msg_id = GTP_DELETE_SESSION_REQUEST;
  uint64_t            ie_presence_mask;
  core::cause_t             cause;
  core::ebi_t               linked_eps_bearer_id;
  core::uli_t               uli;
  core::indication_t        indication_flags;
  core::protocol_configuration_options_t  pco;
  core::node_type_t         originating_node;
  core::fteid_t             sender_fteid_for_cp;
  core::ue_time_zone_t      ue_time_zone;
  core::ran_nas_cause_t     ran_nas_release_cause;
  core::extended_protocol_configuration_options_t  epco;

  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_CAUSE;}
  void set(const core::ebi_t& v, const uint8_t instance = 0) {linked_eps_bearer_id = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_LINKED_EPS_BEARER_ID;}
  void set(const core::uli_t& v, const uint8_t instance = 0) {uli = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_ULI;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_INDICATION_FLAGS;}
  void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {pco = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_PCO;}
  void set(const core::node_type_t& v, const uint8_t instance = 0) {originating_node = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_ORIGINATING_NODE;}
  void set_sender_fteid_for_cp(const core::fteid_t& v) {sender_fteid_for_cp = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE;}
  void set(const core::fteid_t& v, const uint8_t instance = 0) {sender_fteid_for_cp = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE;}
  void set(const core::ue_time_zone_t& v, const uint8_t instance = 0) {ue_time_zone = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_UE_TIME_ZONE;}
  void set(const core::ran_nas_cause_t& v, const uint8_t instance = 0) {ran_nas_release_cause = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_RAN_NAS_RELEASE_CAUSE;}
  void set(const core::extended_protocol_configuration_options_t& v, const uint8_t instance = 0) {epco = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_REQUEST_PR_IE_EPCO;}

  bool get(core::cause_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_CAUSE) {v = cause;return true;}return false;}
  bool get(core::ebi_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_LINKED_EPS_BEARER_ID) {v = linked_eps_bearer_id;return true;}return false;}
  bool get(core::uli_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_ULI) {v = uli;return true;}return false;}
  bool get(core::indication_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_INDICATION_FLAGS) {v = indication_flags;return true;}return false;}
  bool get(core::protocol_configuration_options_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_PCO) {v = pco;return true;}return false;}
  bool get(core::node_type_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_ORIGINATING_NODE) {v = originating_node;return true;}return false;}
  bool get(core::fteid_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE) {v = sender_fteid_for_cp;return true;}return false;}
  bool get_sender_fteid_for_cp(core::fteid_t& v) const {return get(v,0);}
  bool get(core::ue_time_zone_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_UE_TIME_ZONE) {v = ue_time_zone;return true;}return false;}
  bool get(core::ran_nas_cause_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_RAN_NAS_RELEASE_CAUSE) {v = ran_nas_release_cause;return true;}return false;}
  bool get(core::extended_protocol_configuration_options_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_EPCO) {v = epco;return true;}return false;}
};


//-----------------------------------------------------------------------------
/** @class gtpv2c_delete_session_response
 *  @brief Delete Session Response
 *
 * The Delete Session Response will be sent on S11 interface as
 * part of these procedures:
 * - EUTRAN Initial Attach
 * - UE, HSS or MME Initiated Detach
 * - UE or MME Requested PDN Disconnection
 * - Tracking Area Update with SGW Change
 * - S1 Based Handover with SGW Change
 * - X2 Based Handover with SGW Relocation
 * - S1 Based handover cancel with SGW change
 */
class gtpv2c_delete_session_response : public gtpv2c_ies_container {
public:
  gtpv2c_delete_session_response() :
    ie_presence_mask(0),
    cause(),
    pco(),
    indication_flags(),
    epco() {}

  gtpv2c_delete_session_response(const gtpv2c_delete_session_response& i) :
    ie_presence_mask(i.ie_presence_mask),
    cause(i.cause),
    pco(i.pco),
    indication_flags(i.indication_flags),
    epco(i.epco) {}

  static const char* get_msg_name() {return "DELETE_SESSION_RESPONSE";};

#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_CAUSE                             ((uint64_t)1)
#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_RECOVERY                          ((uint64_t)1 << 1)
#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PCO                               ((uint64_t)1 << 2)
#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_INDICATION_FLAGS                  ((uint64_t)1 << 3)
#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PGW_NODE_LEVEL_LOAD_CONTROL_INFORMATION ((uint64_t)1 << 4)
#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PGW_APN_LEVEL_LOAD_CONTROL_INFORMATION  ((uint64_t)1 << 5)
#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_SGW_NODE_LEVEL_LOAD_CONTROL_INFORMATION ((uint64_t)1 << 6)
#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PGW_OVERLOAD_CONTROL_INFORMATION  ((uint64_t)1 << 7)
#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION  ((uint64_t)1 << 8)
#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_EPCO                              ((uint64_t)1 << 9)
#define GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PRIVATE_EXTENSION                 ((uint64_t)1 << 10)

  static const uint8_t msg_id = GTP_DELETE_SESSION_RESPONSE;
  uint64_t                  ie_presence_mask;

  core::cause_t  cause;
  //recovery_t recovery;              ///< This IE shall be included on the S5/S8, S4/S11 and S2b
                                      ///< interfaces if contacting the peer for the first time
  core::protocol_configuration_options_t pco;///< PGW shall include Protocol Configuration Options (PCO)
                                      ///< IE on the S5/S8 interface, if available and if the UE
                                      ///< or the network does not support ePCO.
                                      ///< If SGW receives this IE, SGW shall forward it to
                                      ///< SGSN/MME on the S4/S11 interface.
  core::indication_t               indication_flags;
  // PGW's node level Load Control Information
  // PGW's APN level Load Control Information
  // SGW's node level Load Control Information
  // PGW's Overload Control Information
  // SGW's Overload Control Information
  core::extended_protocol_configuration_options_t  epco;
  // APN RATE Control Status
  // Private Extension
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_CAUSE;}
  void set(const core::protocol_configuration_options_t& v, const uint8_t instance = 0) {pco = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PCO;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_INDICATION_FLAGS;}
  void set(const core::extended_protocol_configuration_options_t& v, const uint8_t instance = 0) {epco = v;ie_presence_mask |= GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_EPCO;}

  bool get(core::cause_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_CAUSE) {v = cause;return true;}return false;}
  bool get(core::protocol_configuration_options_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PCO) {v = pco;return true;}return false;}
  bool get(core::indication_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_INDICATION_FLAGS) {v = indication_flags;return true;}return false;}
  bool get(core::extended_protocol_configuration_options_t& v, const uint8_t instance = 0) const {if (ie_presence_mask & GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_EPCO) {v = epco;return true;}return false;}
};


//-----------------------------------------------------------------------------
/** @class gtpv2c_release_access_bearers_request
 *  @brief Release AccessBearers Request
 *
 * The Release Access Bearers Request message shall sent on the S11 interface by
 * the MME to the SGW as part of the S1 release procedure.
 * The message shall also be sent on the S4 interface by the SGSN to the SGW as
 * part of the procedures:
 * -    RAB release using S4
 * -    Iu Release using S4
 * -    READY to STANDBY transition within the network
 */
class gtpv2c_release_access_bearers_request : public gtpv2c_ies_container {
public:
  gtpv2c_release_access_bearers_request() :
    originating_node(),
    indication_flags() {}

  gtpv2c_release_access_bearers_request(const gtpv2c_release_access_bearers_request& i) :
    originating_node(i.originating_node),
    indication_flags(i.indication_flags) {}
  
  static const char* get_msg_name() {return "RELEASE_ACCESS_BEARERS_REQUEST";};
  static const uint8_t msg_id = GTP_RELEASE_ACCESS_BEARERS_REQUEST;

  // List of RABs: Shall be present on S4 interface when this message is used to release a subset of all active RABs according to
  //               the RAB release procedure. Several IEs with this type and instance values shall be included as necessary to
  //               represent a list of RABs to be released.
  std::pair<bool, core::node_type_t>         originating_node;        ///< This IE shall be sent on S11 interface, if ISR is active in the MME.
                                                     ///< This IE shall be sent on S4 interface, if ISR is active in the SGSN
  std::pair<bool, core::indication_t>        indication_flags;
  // Secondary RAT Usage Data Report
  // Private Extension Private Extension ///< optional
  void set(const core::node_type_t& v, const uint8_t instance = 0) {originating_node.first = true; originating_node.second = v;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags.first = true; indication_flags.second = v;}
  bool get(core::node_type_t& v, const uint8_t instance = 0) const {if (originating_node.first) {v = originating_node.second;return true;}return false;}
  bool get(core::indication_t& v, const uint8_t instance = 0) const {if (indication_flags.first) {v = indication_flags.second;return true;}return false;}
};


//-----------------------------------------------------------------------------
/** @class gtpv2c_release_access_bearers_response
 *  @brief Release AccessBearers Response
 *
 * The Release Access Bearers Response message is sent on the S11 interface by the SGW to the MME as part of the S1
 * release procedure.
 * The message shall also be sent on the S4 interface by the SGW to the SGSN as part of the procedures:
 * -  RAB release using S4
 * -  Iu Release using S4
 * -  READY to STANDBY transition within the network
 * Possible Cause values are specified in Table 8.4-1. Message specific cause values are:
 * - "Request accepted".
 * - "Request accepted partially".
 * - "Context not found
 */
class gtpv2c_release_access_bearers_response : public gtpv2c_ies_container {
public:
  gtpv2c_release_access_bearers_response() :
    cause(),
    indication_flags() {}
  
  gtpv2c_release_access_bearers_response(const gtpv2c_release_access_bearers_response& i) :
    cause(i.cause),
    indication_flags(i.indication_flags) {}

  static const char* get_msg_name() {return "RELEASE_ACCESS_BEARERS_RESPONSE";};

  std::pair<bool, core::cause_t>            cause;
  // SGW's node level Load Control Information
  // SGW's Overload Control Information
  // Recovery           ///< optional This IE shall be included if contacting the peer for the first time
  std::pair<bool, core::indication_t>       indication_flags;

  // Private Extension  ///< optional
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause.first = true; cause.second = v;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags.first = true; indication_flags.second = v;}
  bool get(core::cause_t& v, const uint8_t instance = 0) const {if (cause.first) {v = cause.second;return true;}return false;}
  bool get(core::indication_t& v, const uint8_t instance = 0) const {if (indication_flags.first) {v = indication_flags.second;return true;}return false;}
};

//-----------------------------------------------------------------------------
/** @class gtpv2c_delete_bearer_command
 *  @brief Initiate Delete Bearer procedure
 *
 * A Delete Bearer Command message shall be sent on the S11 interface by the MME to the SGW and on the S5/S8
 * interface by the SGW to the PGW as a part of the eNodeB requested bearer release or MME-Initiated Dedicated Bearer
 * Deactivation procedure.
 * The message shall also be sent on the S4 interface by the SGSN to the SGW and on the S5/S8 interface by the SGW to
 * the PGW as part of the MS and SGSN Initiated Bearer Deactivation procedure using S4.
 */

class bearer_context_within_delete_bearer_command {
public:
  uint32_t           ie_presence_mask;
#define GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_COMMAND_PR_IE_EPS_BEARER_ID         ((uint64_t)1)
#define GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_COMMAND_PR_IE_BEARER_FLAGS          ((uint64_t)1 << 1)
#define GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_COMMAND_PR_IE_RAN_NAS_RELEASE_CAUSE ((uint64_t)1 << 2)
  core::ebi_t                    eps_bearer_id;
  core::bearer_flags_t           bearer_flags;
  core::ran_nas_cause_t          ran_nas_release_cause;

  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_COMMAND_PR_IE_EPS_BEARER_ID;}
  void set(const core::bearer_flags_t& v, const uint8_t instance = 0) {bearer_flags = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_COMMAND_PR_IE_BEARER_FLAGS;}
  void set(const core::ran_nas_cause_t& v, const uint8_t instance = 0) {ran_nas_release_cause = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_COMMAND_PR_IE_RAN_NAS_RELEASE_CAUSE;}
  bearer_context_within_delete_bearer_command() :
    ie_presence_mask(0),
    bearer_flags(),
    ran_nas_release_cause() {}

  explicit bearer_context_within_delete_bearer_command(const bearer_context& b)
  {
    ie_presence_mask = 0;
    if (b.get(eps_bearer_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_COMMAND_PR_IE_EPS_BEARER_ID;
    if (b.get(bearer_flags, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_COMMAND_PR_IE_BEARER_FLAGS;
    if (b.get(ran_nas_release_cause, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_COMMAND_PR_IE_RAN_NAS_RELEASE_CAUSE;
  }
};

class gtpv2c_delete_bearer_command : public gtpv2c_ies_container {
public:
  gtpv2c_delete_bearer_command() : bearer_contexts() {
    ie_presence_mask = 0;
    uli = {};
    uli_timestamp = {};
    ue_time_zone = {};
    sender_fteid_for_cp = {};
  }
  gtpv2c_delete_bearer_command(const gtpv2c_delete_bearer_command& i) : bearer_contexts(i.bearer_contexts) {
    ie_presence_mask = i.ie_presence_mask;
    uli = i.uli;
    uli_timestamp = i.uli_timestamp;
    ue_time_zone = i.ue_time_zone;
    sender_fteid_for_cp = i.sender_fteid_for_cp;
  }
  static const char* get_msg_name() {return "DELETE_BEARER_COMMAND";};

  static const uint8_t msg_id = GTP_DELETE_BEARER_COMMAND;
  uint64_t           ie_presence_mask;

#define GTPV2C_DELETE_BEARER_COMMAND_PR_IE_BEARER_CONTEXTS                     ((uint64_t)1)
#define GTPV2C_DELETE_BEARER_COMMAND_PR_IE_ULI                                 ((uint64_t)1 << 1)
#define GTPV2C_DELETE_BEARER_COMMAND_PR_IE_ULI_TIMESTAMP                       ((uint64_t)1 << 2)
#define GTPV2C_DELETE_BEARER_COMMAND_PR_IE_UE_TIME_ZONE                        ((uint64_t)1 << 3)
#define GTPV2C_DELETE_BEARER_COMMAND_PR_IE_MMES4_SGSN_OVERLOAD_CONTROL_INFORMATION ((uint64_t)1 << 4)
#define GTPV2C_DELETE_BEARER_COMMAND_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION    ((uint64_t)1 << 5)
#define GTPV2C_DELETE_BEARER_COMMAND_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE      ((uint64_t)1 << 6)
#define GTPV2C_DELETE_BEARER_COMMAND_PR_IE_PRIVATE_EXTENSION                   ((uint64_t)1 << 7)
  std::vector<bearer_context_within_delete_bearer_command> bearer_contexts;
  core::uli_t                     uli;
  core::uli_timestamp_t           uli_timestamp;
  core::ue_time_zone_t            ue_time_zone;
  // MME/S4-SGSN's Overload Control Information
  // SGW's Overload Control Information
  core::fteid_t                   sender_fteid_for_cp;
  // Secondary RAT Usage Data Report
  // Private Extension
  void set(const bearer_context_within_delete_bearer_command& v, const uint8_t instance = 0) {bearer_contexts.push_back(v);ie_presence_mask |= GTPV2C_DELETE_BEARER_COMMAND_PR_IE_BEARER_CONTEXTS;}
  void set(const core::uli_t& v, const uint8_t instance = 0) {uli = v;ie_presence_mask |= GTPV2C_DELETE_BEARER_COMMAND_PR_IE_ULI;}
  void set(const core::uli_timestamp_t& v, const uint8_t instance = 0) {uli_timestamp = v;ie_presence_mask |= GTPV2C_DELETE_BEARER_COMMAND_PR_IE_ULI_TIMESTAMP;}
  void set(const core::ue_time_zone_t& v, const uint8_t instance = 0) {ue_time_zone = v;ie_presence_mask |= GTPV2C_DELETE_BEARER_COMMAND_PR_IE_UE_TIME_ZONE;}
  void set(const core::fteid_t& v, const uint8_t instance = 0) {sender_fteid_for_cp = v;ie_presence_mask |= GTPV2C_DELETE_BEARER_COMMAND_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE;}
  void set(const bearer_context& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: {
        bearer_context_within_delete_bearer_command b(v);
        set(b);
      }
      break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }
};

//-----------------------------------------------------------------------------
/** @class gtpv2c_delete_bearer_failure_indication
 */
class bearer_context_within_delete_bearer_failure_indication {
public:
  uint32_t           ie_presence_mask;
#define GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_FAILURE_INDICATION_PR_IE_EPS_BEARER_ID         ((uint64_t)1)
#define GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_FAILURE_INDICATION_PR_IE_CAUSE                 ((uint64_t)1 << 1)
  core::ebi_t            eps_bearer_id;
  core::cause_t          cause;

  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_FAILURE_INDICATION_PR_IE_EPS_BEARER_ID;}
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v;ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_FAILURE_INDICATION_PR_IE_CAUSE;}
  bearer_context_within_delete_bearer_failure_indication()
  {
    ie_presence_mask = 0;
    eps_bearer_id = {};
    cause = {};
  }

  explicit bearer_context_within_delete_bearer_failure_indication(const bearer_context& b)
  {
    ie_presence_mask = 0;
    if (b.get(eps_bearer_id, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_FAILURE_INDICATION_PR_IE_EPS_BEARER_ID;
    if (b.get(cause, 0)) ie_presence_mask |= GTPV2C_BEARER_CONTEXT_WITHIN_DELETE_BEARER_FAILURE_INDICATION_PR_IE_CAUSE;
  }
};
class gtpv2c_delete_bearer_failure_indication : public gtpv2c_ies_container {
public:
  gtpv2c_delete_bearer_failure_indication() : bearer_contexts() {
    ie_presence_mask = 0;
    cause = {};
    indication_flags = {};
  }
  gtpv2c_delete_bearer_failure_indication(const gtpv2c_delete_bearer_failure_indication& i) : bearer_contexts(i.bearer_contexts) {
    ie_presence_mask = i.ie_presence_mask;
    cause = i.cause;
    indication_flags = i.indication_flags;
  }
  static const char* get_msg_name() {return "DELETE_BEARER_FAILURE_INDICATION";};

  static const uint8_t msg_id = GTP_DELETE_BEARER_FAILURE_INDICATION;
  uint64_t           ie_presence_mask;

#define GTPV2C_DELETE_BEARER_FAILURE_INDICATION_PR_IE_CAUSE                             ((uint64_t)1)
#define GTPV2C_DELETE_BEARER_FAILURE_INDICATION_PR_IE_BEARER_CONTEXTS                   ((uint64_t)1 << 1)
#define GTPV2C_DELETE_BEARER_FAILURE_INDICATION_PR_IE_RECOVERY                          ((uint64_t)1 << 2)
#define GTPV2C_DELETE_BEARER_FAILURE_INDICATION_PR_IE_INDICATION_FLAGS                  ((uint64_t)1 << 3)
#define GTPV2C_DELETE_BEARER_FAILURE_INDICATION_PR_IE_PGW_OVERLOAD_CONTROL_INFORMATION  ((uint64_t)1 << 4)
#define GTPV2C_DELETE_BEARER_FAILURE_INDICATION_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION  ((uint64_t)1 << 5)
#define GTPV2C_DELETE_BEARER_FAILURE_INDICATION_PR_IE_PRIVATE_EXTENSION                 ((uint64_t)1 << 6)

  core::cause_t   cause;
  std::vector<bearer_context_within_delete_bearer_failure_indication> bearer_contexts;
  // recovery
  core::indication_t        indication_flags;
  // PGW's Overload Control Information
  // SGW's Overload Control Information
  // Private Extension
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v;ie_presence_mask |= GTPV2C_DELETE_BEARER_FAILURE_INDICATION_PR_IE_CAUSE;}
  void set(const bearer_context_within_delete_bearer_failure_indication& v, const uint8_t instance = 0) {bearer_contexts.push_back(v);ie_presence_mask |= GTPV2C_DELETE_BEARER_FAILURE_INDICATION_PR_IE_BEARER_CONTEXTS;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags = v;ie_presence_mask |= GTPV2C_DELETE_BEARER_FAILURE_INDICATION_PR_IE_INDICATION_FLAGS;}
  void set(const bearer_context& v, const uint8_t instance = 0) {
    switch (instance) {
    case 0: {
        bearer_context_within_delete_bearer_failure_indication b(v);
        set(b);
      }
      break;
    default:
      throw gtpc_tlv_bad_instance_exception(GTP_IE_BEARER_CONTEXT, instance);
    }
  }
};

//-----------------------------------------------------------------------------
/** @class gtpv2c_downlink_data_notification
 *  @brief Downlink Data Notification
 *
 * The Downlink Data Notification message is sent on the S11 interface by the SGW to the MME as part of the S1 paging procedure.
 */
class gtpv2c_downlink_data_notification : public gtpv2c_ies_container {
public:
  gtpv2c_downlink_data_notification() {
    ie_presence_mask = 0;
    cause = {};
    eps_bearer_id.ebi = EPS_BEARER_IDENTITY_UNASSIGNED;
    arp = {};
    imsi = {};
    sender_fteid_for_cp = {};
    indication_flags = {};
  }
  gtpv2c_downlink_data_notification(const gtpv2c_downlink_data_notification& i) {
    ie_presence_mask = i.ie_presence_mask;
    cause = i.cause;
    eps_bearer_id = i.eps_bearer_id;
    arp = i.arp;
    imsi = i.imsi;
    sender_fteid_for_cp = i.sender_fteid_for_cp;
    indication_flags = i.indication_flags;
  }
  static const char* get_msg_name() {return "DOWNLINK_DATA_NOTIFICATION";};

#define DOWNLINK_DATA_NOTIFICATION_PR_IE_CAUSE                  ((uint64_t)1)
#define DOWNLINK_DATA_NOTIFICATION_PR_IE_EPS_BEARER_ID          ((uint64_t)1 << 1)
#define DOWNLINK_DATA_NOTIFICATION_PR_IE_ARP                    ((uint64_t)1 << 2)
#define DOWNLINK_DATA_NOTIFICATION_PR_IE_IMSI                   ((uint64_t)1 << 3)
#define DOWNLINK_DATA_NOTIFICATION_PR_IE_SENDER_FTEID_FOR_CP    ((uint64_t)1 << 4)
#define DOWNLINK_DATA_NOTIFICATION_PR_IE_INDICATION_FLAGS       ((uint64_t)1 << 5)
#define DOWNLINK_DATA_NOTIFICATION_PR_IE_SGW_NODE_LEVEL_LCI     ((uint64_t)1 << 6)
#define DOWNLINK_DATA_NOTIFICATION_PR_IE_SGW_OVERLOAD_CI        ((uint64_t)1 << 7)
  static const uint8_t msg_id = GTP_DOWNLINK_DATA_NOTIFICATION;
  uint64_t            ie_presence_mask;

  core::cause_t         cause;
  core::ebi_t           eps_bearer_id;
  core::arp_t           arp;
  core::imsi_t          imsi;
  core::fteid_t         sender_fteid_for_cp;
  core::indication_t    indication_flags;
  // SGW's node level Load Control Information
  // SGW's Overload Control Information
  // Paging and Service Information
  // Private extensions
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_PR_IE_CAUSE;}
  void set(const core::ebi_t& v, const uint8_t instance = 0) {eps_bearer_id = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_PR_IE_EPS_BEARER_ID;}
  void set(const core::arp_t& v, const uint8_t instance = 0) {arp = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_PR_IE_ARP;}
  void set(const core::imsi_t& v, const uint8_t instance = 0) {imsi = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_PR_IE_IMSI;}
  void set(const core::fteid_t& v, const uint8_t instance = 0) {sender_fteid_for_cp = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_PR_IE_SENDER_FTEID_FOR_CP;}
  void set(const core::indication_t& v, const uint8_t instance = 0) {indication_flags = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_PR_IE_INDICATION_FLAGS;}
};

//-----------------------------------------------------------------------------
/** @class gtpv2c_downlink_data_notification_acknowledge
 *  @brief Downlink Data Notification Acknowledge
 *
 * The Downlink Data Notification Acknowledge message is sent on the S11 interface by the MME to the SGW as part of the S1 paging procedure.
 */
class gtpv2c_downlink_data_notification_acknowledge : public gtpv2c_ies_container {
public:
  gtpv2c_downlink_data_notification_acknowledge() {
    ie_presence_mask = 0;
    cause = {};
    imsi = {};
  }
  gtpv2c_downlink_data_notification_acknowledge(const gtpv2c_downlink_data_notification_acknowledge& i) {
    ie_presence_mask = i.ie_presence_mask;
    cause = i.cause;
    imsi = i.imsi;
  }
  static const char* get_msg_name() {return "DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE";};

#define DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_CAUSE                                 ((uint64_t)1)
#define DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_DATA_NOTIFICATION_DELAY               ((uint64_t)1 << 1)
#define DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_RECOVERY                              ((uint64_t)1 << 2)
#define DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_DL_LOW_PRIORITY_TRAFFIC_THROTTLING    ((uint64_t)1 << 3)
#define DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_IMSI                                  ((uint64_t)1 << 4)
#define DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_DL_BUFFERING_DURATION                 ((uint64_t)1 << 5)
#define DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_DL_BUFFERING_SUGGESTED_PACKET_COUNT   ((uint64_t)1 << 6)
  static const uint8_t msg_id = GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE;
  uint64_t            ie_presence_mask;

  core::cause_t         cause;
  // Data Notification Delay
  // recovery
  // DL low priority traffic Throttling
  core::imsi_t          imsi;
  // DL Buffering Duration
  // DL Buffering Suggested Packet Count
  // Private Extension  ///< optional
  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_CAUSE;}
  void set(const core::imsi_t& v, const uint8_t instance = 0) {imsi = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_IMSI;}
} ;


//-----------------------------------------------------------------------------
/** @class gtpv2c_downlink_data_notification_failure_indication
 *  @brief Downlink Data Notification Acknowledge
 *
 * The Downlink Data Notification Acknowledge message is sent on the S11 interface by the MME to the SGW as part of the S1 paging procedure.
 */
class gtpv2c_downlink_data_notification_failure_indication : public gtpv2c_ies_container {
public:
  gtpv2c_downlink_data_notification_failure_indication() :
    ie_presence_mask(0),
    cause(),
    originating_node(),
    imsi() {}
  
  gtpv2c_downlink_data_notification_failure_indication(const gtpv2c_downlink_data_notification_failure_indication& i) :
    ie_presence_mask(i.ie_presence_mask),
    cause(i.cause),
    originating_node(i.originating_node),
    imsi(i.imsi) {}

  static const char* get_msg_name() {return "DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION";};

  core::cause_t         cause;
  core::node_type_t     originating_node;
  core::imsi_t          imsi;
  // Private Extension  ///< optional
#define DOWNLINK_DATA_NOTIFICATION_FAILURE_IND_PR_IE_CAUSE                ((uint64_t)1)
#define DOWNLINK_DATA_NOTIFICATION_FAILURE_IND_PR_IE_ORIGINATING_NODE     ((uint64_t)1 << 1)
#define DOWNLINK_DATA_NOTIFICATION_FAILURE_IND_PR_IE_IMSI                 ((uint64_t)1 << 2)
#define DOWNLINK_DATA_NOTIFICATION_FAILURE_IND_PR_IE_PRIVATE_EXTENSION    ((uint64_t)1 << 3)
  static const uint8_t msg_id = GTP_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION;
  uint64_t            ie_presence_mask;

  void set(const core::cause_t& v, const uint8_t instance = 0) {cause = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_FAILURE_IND_PR_IE_CAUSE;}
  void set(const core::node_type_t& v, const uint8_t instance = 0) {originating_node = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_FAILURE_IND_PR_IE_ORIGINATING_NODE;}
  void set(const core::imsi_t& v, const uint8_t instance = 0) {imsi = v;ie_presence_mask |= DOWNLINK_DATA_NOTIFICATION_FAILURE_IND_PR_IE_IMSI;}
};

} // namespace gtpv2c

#endif /* MSG_GTPV2C_HPP_INCLUDED_ */
