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

/*! \file sgwc_eps_bearer_context.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_SGWC_EPS_BEARER_CONTEXT_HPP_SEEN
#define FILE_SGWC_EPS_BEARER_CONTEXT_HPP_SEEN

#include "3gpp_29.274.h"
#include "itti_msg_s11.hpp"
#include "itti_msg_s5s8.hpp"
#include "sgwc_procedure.hpp"

#include <map>
#include <memory>
#include <vector>

namespace sgwc {

class sgw_eps_bearer {
 public:
  sgw_eps_bearer() : ebi() {
    tft                        = {};
    pgw_fteid_s5_s8_up         = {};
    sgw_fteid_s5_s8_up         = {};
    sgw_fteid_s1u_s12_s4u_s11u = {};
    sgw_fteid_s11u             = {};
    mme_fteid_s11u             = {};
    enb_fteid_s1u              = {};
    eps_bearer_qos             = {};
  }

  sgw_eps_bearer(const sgw_eps_bearer& b) : ebi(b.ebi) {
    tft                        = b.tft;
    pgw_fteid_s5_s8_up         = b.pgw_fteid_s5_s8_up;
    sgw_fteid_s5_s8_up         = b.sgw_fteid_s5_s8_up;
    sgw_fteid_s1u_s12_s4u_s11u = b.sgw_fteid_s1u_s12_s4u_s11u;
    sgw_fteid_s11u             = b.sgw_fteid_s11u;
    mme_fteid_s11u             = b.mme_fteid_s11u;
    enb_fteid_s1u              = b.enb_fteid_s1u;
    eps_bearer_qos             = b.eps_bearer_qos;
  }

  void clear() {
    ebi.ebi                    = EPS_BEARER_IDENTITY_UNASSIGNED;
    tft                        = {};
    pgw_fteid_s5_s8_up         = {};
    sgw_fteid_s5_s8_up         = {};
    sgw_fteid_s1u_s12_s4u_s11u = {};
    sgw_fteid_s11u             = {};
    mme_fteid_s11u             = {};
    enb_fteid_s1u              = {};
    eps_bearer_qos             = {};
  }

  bool update(
      const gtpv2c::bearer_context_modified_within_modify_bearer_response& b) {
    ebi_t check_ebi = {};
    if ((b.get(check_ebi)) && (check_ebi == ebi)) {
      b.get_s1_u_sgw_fteid(sgw_fteid_s1u_s12_s4u_s11u);
      return true;
    }
    return false;
  }

  sgw_eps_bearer(sgw_eps_bearer&& b) = delete;

  void deallocate_ressources();
  std::string toString() const;
  ebi_t ebi;  // EPS Bearer Id   //An EPS bearer identity uniquely identifies an
              // EPS bearer for one UE accessing via E-UTRAN.
  traffic_flow_template_t tft;  // Traffic Flow Template
  // ip_address_t            pgw_address_in_use_up;       // The IP address of
  // the P-GW currently used for sending user plane traffic. (For GTP-based
  // S5/S8 only). teid_t                        pgw_teid_s5_s8_up;           //
  // P-GW Tunnel Endpoint Identifier for the S5/S8 interface for the user plane.
  // (For GTP-based S5/S8 only). replaced by
  fteid_t pgw_fteid_s5_s8_up;  // P-GW FTEID for the S5/S8 interface for the
                               // user plane. (For GTP-based S5/S8 only).
  // ip_address_t            s_gw_ip_address_s5_s8_up;        // S-GW IP address
  // for user plane data received from PDN GW. (For GTP-based S5/S8 only).
  // teid_t s_gw_teid_s5_s8_up;              // S-GW Tunnel Endpoint Identifier
  // for the S5/S8 interface for the user plane. (For GTP-based S5/S8 only).
  // replaced by
  fteid_t sgw_fteid_s5_s8_up;  // S-GW FTEID for the S5/S8 interface for the
                               // user plane. (For GTP-based S5/S8 only).

  // ip_address_t            s_gw_ip_address_s1u_s12_s4u_s11u;// S-GW IP address
  // for the S1-u interface (used by the eNodeB), for the S12 interface (used by
  // the RNC) and for the S4
  // interface (used by the SGSN). Also S-GW IP address for the S11-u interface
  // (used by the MME) if no separation of S1-U and S11-U is required. The S11-u
  // interface is used for Control Plane CIoT EPS Optimisation.
  // teid_t                  sgw_teid_s1u_s12_s4u_s11u;       // S-GW Tunnel
  // Endpoint Identifier for the S1-u interface, for the S12 interface (used by
  // the RNC) and for the S4 interface (used  by the SGSN). Also S-GW Tunnel
  // Endpoint Identifier for the
  // S11-u interface (used by the MME) if no separation of S1-U and S11-U is
  // required. The S11-u interface is used for Control Plane CIoT EPS
  // Optimisation.
  // replaced by
  fteid_t sgw_fteid_s1u_s12_s4u_s11u;

  // ip_address_t            sgw_ip_address_s11u;             // S-GW IP address
  // for the S11-u interface (used by the MME). if S11-u is separated from S1-u.
  // The S11-u interface is used for Control Plane CIoT EPS Optimisation. teid_t
  // sgw_teid_s11u;                   // S-GW Tunnel Endpoint Identifier for the
  // S11-u interface (used by the MME) if S11-u is separated from S1-u. The
  // S11-u interface is used for Control Plane CIoT EPS Optimisation. replaced
  // by
  fteid_t sgw_fteid_s11u;

  // ip_address_t            mme_ip_address_s11u;             // MME IP address
  // for the S11-u interface (Used by the S-GW). The S11-u interface is used for
  // Control Plane CIoT EPS Optimisation. teid_t                  mme_teid_s11u;
  // // MME Tunnel Endpoint Identifier for the S11-u interface (Used by the
  // S-GW). The S11-u interface is used for Control Plane CIoT EPS Optimisation.
  // replaced by
  fteid_t mme_fteid_s11u;  // MME FTEID for the S11-u interface (Used by the
                           // S-GW). The S11-u interface is used for Control
                           // Plane CIoT EPS Optimisation.
  // ip_address_t            enb_ip_address_s1u;              // eNodeB IP
  // address for the S1-u interface (Used by the S-GW). teid_t enb_teid_s1u; //
  // eNodeB Tunnel Endpoint Identifier for the S1-u interface.
  // replaced by
  fteid_t enb_fteid_s1u;  // eNodeB FTEID for the S1-u interface.
  // RNC IP address for S12                             // RNC IP address for
  // the S12 interface (Used by the S-GW). RNC TEID for S12 // RNC Tunnel
  // Endpoint Identifier for the S12 interface. SGSN IP address for S4 (user
  // plane)                // SGSN IP address for the S4 interface (Used by the
  // S-GW). SGSN TEID for S4 (user plane)                      // SGSN Tunnel
  // Endpoint Identifier for the S4 interface.
  bearer_qos_t eps_bearer_qos;  ///< ARP, GBR, MBR, QCI.
  // NOT NEEDED        charging_id                       ///< Charging
  // identifier, identifies charging records generated by S-GW and PDN GW.
};

class sgw_pdn_connection {
 public:
  sgw_pdn_connection()
      : apn_in_use(),
        pdn_type(),
        pgw_fteid_s5_s8_cp(),
        pgw_address_in_use_up(),
        sgw_fteid_s5_s8_cp(),
        is_dl_up_tunnels_released(false),
        sgw_eps_bearers() {
    default_bearer.ebi = EPS_BEARER_IDENTITY_UNASSIGNED;
  }

  sgw_pdn_connection(sgw_pdn_connection& b) = delete;

  bool get_eps_bearer(const ebi_t& ebi, std::shared_ptr<sgw_eps_bearer>& b);
  void add_eps_bearer(std::shared_ptr<sgw_eps_bearer> eps_bearer);
  bool update_eps_bearer(
      const gtpv2c::bearer_context_modified_within_modify_bearer_response& b);
  void remove_eps_bearer(const ebi_t& ebi);
  void remove_eps_bearer(std::shared_ptr<sgw_eps_bearer> bearer);
  void delete_bearers();
  int get_num_bearers() { return sgw_eps_bearers.size(); };
  bool is_released() const { return is_dl_up_tunnels_released; }

  // fteid_t generate_s5s8_up_fteid(const struct in_addr ipv4_address, const
  // bearer_qos_t& bearer_qos);
  void deallocate_ressources();
  std::string toString() const;

  std::string apn_in_use;  ///< The APN currently used, as received from the MME
                           ///< or S4 SGSN.
  // PDN Type                // IPv4, IPv6, IPv4v6 or Non-IP. (This information
  // is needed by the SGW in order to determine whether to perform Paging Policy
  // Differentiation).
  pdn_type_t pdn_type;

  // EPS PDN Charging Characteristics                    ///< The charging
  // characteristics of this PDN connection, e.g.normal, prepaid, flat-rate
  // and/or hot billing.

  // ip_address_t         pgw_address_in_use_cp;         ///< The IP address of
  // the P-GW currently used for sending control plane signalling.

  // The TEID-C shall be unique per PDN-Connection on GTP based S2a, S2b, S5 and
  // S8 interfaces. The same tunnel shall be shared for the control messages
  // related to all bearers associated to the PDN-Connection. A TEID-C on the
  // S2a/S2b/S5/S8 interface shall be released after all its associated EPS
  // bearers are deleted.
  // teid_t               pgw_teid_s5_s8_cp;             ///< P-GW Tunnel
  // Endpoint Identifier for the S5/S8 interface for the control plane. (For
  // GTP-based S5/S8 only).
  // replaced by
  fteid_t pgw_fteid_s5_s8_cp;

  // NOT IMPLEMENTED NOW
  ip_address_t pgw_address_in_use_up;  ///< The IP address of the P-GW currently
                                       ///< used for sending user plane traffic.
                                       ///< (For PMIP-based S5/S8 only)
  // NOT NEEDED p_gw_gre_key_for_uplink_traffic_up     ///< PDN GW assigned GRE
  // Key for the S5/S8 interface for the user plane for uplink traffic. (For
  // PMIP-based S5/S8 only)
  // ip_address_t         sgw_ip_address_s5_s8_cp;       ///< S-GW IP address
  // for the S5/S8 for the control plane signalling. teid_t sgw_teid_s5_s8_cp;
  // ///< S-GW Tunnel Endpoint Identifier for the S5/S8 control plane interface.
  // (For GTP-based S5/S8 only).
  // replaced by (seems PMIP wont be used)
  fteid_t sgw_fteid_s5_s8_cp;
  // ip_address_t         sgw_address_in_use_up;         ///< The IP address of
  // the S-GW currently used for sending user plane traffic. (For PMIP-based
  // S5/S8 only)
  // NOT NEEDED s_gw_gre_key_for_dl_traffic_up         ///< user plane for
  // downlink traffic. (For PMIP-based S5/S8 only)
  ebi_t default_bearer;  ///< Identifies the default bearer within the PDN
                         ///< connection by its EPS Bearer Id. (For PMIP based
                         ///< S5/S8.)

  // eps bearers
  bool is_dl_up_tunnels_released;
  std::map<uint8_t, std::shared_ptr<sgw_eps_bearer>> sgw_eps_bearers;
};

// pair apn,pdn_type
typedef std::pair<std::string, uint8_t> kpdn_t;
// struct keypdncmp {
//    bool operator()(const kpdn_t & a, const kpdn_t & b) const {
//        return a.second < b.first;
//    }
//};
//
// template<typename S, typename T>
// bool operator<(const std::pair<S, T> & a, const std::pair<S, T> & b)
//{
//   return (a.first < b.first) || (a.first == b.first && a.second < b.second);
//}

class sgw_eps_bearer_context
    : public std::enable_shared_from_this<sgw_eps_bearer_context> {
 private:
  void erase_pdn_connection(std::shared_ptr<sgw_pdn_connection> spc);

 public:
  sgw_eps_bearer_context()
      : imsi(),
        imsi_unauthenticated_indicator(false),
        msisdn(),
        mme_fteid_s11(),
        sgw_fteid_s11_s4_cp(),
        sgsn_fteid_s4_cp(),
        last_known_cell_Id(),
        pending_procedures(),
        pdn_connections() {}

  void release();
  void create_procedure(itti_s11_create_session_request&);
  void create_procedure(itti_s11_modify_bearer_request&);
  void create_procedure(itti_s11_delete_session_request&);
  void create_procedure(itti_s11_release_access_bearers_request&);
  void create_procedure(
      itti_s5s8_downlink_data_notification&,
      std::shared_ptr<sgw_pdn_connection> spc);

  void insert_procedure(sebc_procedure* proc);
  std::shared_ptr<sebc_procedure> find_procedure(const uint64_t& gtpc_tx_id);
  void remove_procedure(sebc_procedure* proc);

  std::shared_ptr<sgw_pdn_connection> insert_pdn_connection(
      sgw_pdn_connection* p);
  bool find_pdn_connection(
      const std::string& apn, const pdn_type_t pdn_type,
      std::shared_ptr<sgw_pdn_connection>& sp);
  bool find_pdn_connection(
      const ebi_t& ebi, std::shared_ptr<sgw_pdn_connection>& sp);
  void delete_pdn_connection(std::shared_ptr<sgw_pdn_connection> spc);
  int get_num_pdn_connections() { return pdn_connections.size(); };

  sgw_eps_bearer_context(sgw_eps_bearer_context& b) = delete;
  void handle_itti_msg(itti_s11_create_session_request& m);
  void handle_itti_msg(itti_s11_modify_bearer_request& m);
  void handle_itti_msg(itti_s11_release_access_bearers_request& m);
  void handle_itti_msg(itti_s11_delete_session_request& m);
  void handle_itti_msg(itti_s11_downlink_data_notification_acknowledge& m);
  void handle_itti_msg(
      itti_s5s8_create_session_response& m,
      std::shared_ptr<sgw_pdn_connection> spc);
  void handle_itti_msg(
      itti_s5s8_delete_session_response& m,
      std::shared_ptr<sgw_pdn_connection> spc);
  void handle_itti_msg(
      itti_s5s8_modify_bearer_response& m,
      std::shared_ptr<sgw_pdn_connection> spc);
  void handle_itti_msg(
      itti_s5s8_release_access_bearers_response& m,
      std::shared_ptr<sgw_pdn_connection> spc);
  void handle_itti_msg(
      itti_s5s8_downlink_data_notification& m,
      std::shared_ptr<sgw_pdn_connection> spc);
  void handle_itti_msg(
      itti_s5s8_remote_peer_not_responding& resp,
      std::shared_ptr<sgw_pdn_connection> spc);

  std::string toString() const;

  imsi_t imsi;  // IMSI (International Mobile Subscriber Identity) is the
                // subscriber permanent identity.
  bool imsi_unauthenticated_indicator;  // This is an IMSI indicator to show the
                                        // IMSI is unauthenticated.
  // TO BE CHECKED me_identity_t    me_identity;       // Mobile Equipment
  // Identity (e.g. IMEI/IMEISV).
  msisdn_t msisdn;  // The basic MSISDN of the UE. The presence is dictated by
                    // its storage in the HSS.
  //  selected_cn_operator_id                          // Selected core network
  //  operator identity (to support networksharing as defined in TS 23.251
  // LTE-M Indication                                  // Information used by
  // the core network to differentiate traffic from category M UEs for charging
  // purposes, where different values of M (e.g. M1, M2) are defined in
  // TS 36.306 [82].

  // teid_t               mme_teid_s11;                 // MME Tunnel Endpoint
  // Identifier for the S11 interface ip_address_t         mme_ip_address_s11;
  // // MME IP address the S11 interface. replaced by
  fteid_t mme_fteid_s11;

  // teid_t               sgw_teid_s11_s4_cp;           // S-GW Tunnel Endpoint
  // Identifier for the S11 Interface and the S4 Interface (control plane)
  // ip_address_t         sgw_ip_address_s11_s4_cp;     // S-GW IP address for
  // the S11 interface and the S4 Interface (control plane).
  // replaced by
  fteid_t sgw_fteid_s11_s4_cp;

  // ip_address_t         sgsn_ip_address_for_S4_cp;    // S-GW IP address for
  // the S11 interface and the S4 Interface (control plane). teid_t
  // sgsn_teid_s4_cp;              // S-GW Tunnel Endpoint Identifier for the
  // S11 Interface and the S4 Interface (control plane) replaced by
  fteid_t sgsn_fteid_s4_cp;

  // NOT NEEDED Trace reference                        // Identifies a record or
  // a collection of records for a particular trace. NOT NEEDED Trace type //
  // Indicates the type of trace NOT NEEDED Trigger id // Identifies the entity
  // that initiated the trace NOT NEEDED OMC identity // Identifies the OMC that
  // shall receive the trace record(s).
  ecgi_field_t last_known_cell_Id;  // This is the last location of the UE known
                                    // by the network
  // NOT NEEDED NOW Last known Cell Id age             // This is the age of the
  // above UE location information DL Data Buffer Expiration Time // The time
  // until the Serving GW buffers DL data, when the MME has requested extended
  // buffering in an DDN Ack (e.g. when a UE is in PSM). Serving
  // PLMN-Rate-Control                         // For inclusion on Serving GW
  // CDR to allow post processing of CDRs and permit detection of abusive UEs.

  std::map<kpdn_t, std::shared_ptr<sgw_pdn_connection>> pdn_connections;

  //--------------------------------------------
  // internals
  std::vector<std::shared_ptr<sebc_procedure>> pending_procedures;
};
}  // namespace sgwc

#endif
