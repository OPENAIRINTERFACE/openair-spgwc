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
/*! \file TimerApplication.hpp
  \brief
  \author Lionel Gauthier
  \date 2020
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_TIMERAPPLICATION_HPP_SEEN
#define FILE_TIMERAPPLICATION_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
#include <inttypes.h>
//--C++ includes ---------------------------------------------------------------
#include <cstddef>
//--Other includes -------------------------------------------------------------
#include "EndPoint.hpp"
//------------------------------------------------------------------------------
typedef uint32_t timer_id_t;
#define INVALID_TIMER_ID (timer_id_t)0
//------------------------------------------------------------------------------
class TimerApplication {
 public:
  virtual void TimeOutNotification(const timer_id_t tid, uint64_t arg1_user = 0,
                                   uint64_t arg2_user = 0) = 0;
  TimerApplication(){};
  virtual ~TimerApplication(){};
};
#endif /* FILE_TIMERAPPLICATION_HPP_SEEN */
