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

/*! \file PfcpService.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_PFCPSERVICE_HPP_SEEN
#define FILE_PFCPSERVICE_HPP_SEEN
//--C includes -----------------------------------------------------------------
#include <inttypes.h>
//--C++ includes ---------------------------------------------------------------
#include <cstddef>
//--Other includes -------------------------------------------------------------
#include "EndPoint.hpp"
#include "PfcpApplication.hpp"
#include "pfcp.hpp"
//------------------------------------------------------------------------------
namespace pfcp {
class PfcpService {
 public:
  virtual uint64_t SendRequest(std::shared_ptr<pfcp::PfcpMsg> t_msg,
                               const EndPoint& t_remote_endpoint,
                               const uint64_t t_trxn_id);
  virtual void SendResponse(std::shared_ptr<pfcp::PfcpMsg> t_msg,
                            const EndPoint& t_remote_endpoint,
                            const uint64_t t_trxn_id, const int t_trxn_action);
  virtual void RegisterApplication(PfcpApplication* app);
  virtual ~PfcpService();
};
}  // namespace pfcp
#endif /* FILE_PFCPSERVICE_HPP_SEEN */
