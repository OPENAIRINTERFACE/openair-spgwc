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
/*! \file TimerService.hpp
  \brief
  \author Lionel Gauthier
  \date 2020
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_TIMERSERVICE_HPP_SEEN
#define FILE_TIMERSERVICE_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
#include <inttypes.h>
//--C++ includes ---------------------------------------------------------------
#include <cstddef>
//--Other includes -------------------------------------------------------------
#include "TimerApplication.hpp"
//------------------------------------------------------------------------------
class TimerService {
 public:
  /** \brief Request a new timer
   *  \param interval_sec timer interval in seconds
   *  \param interval_us  timer interval in micro seconds
   *  \param task_id      task id of the task requesting the timer
   *  @returns 0 on failure, timer id otherwise
   **/
  virtual timer_id_t TimerSetup(const uint32_t interval_sec,
                                const uint32_t interval_us,
                        TimerApplication *ta, uint64_t arg1_user = 0,
                                uint64_t arg2_user = 0) = 0;

  /** \brief Remove the timer from list
   *  \param timer_id unique timer id
   *  \param task_id task id of the task that requested the timer
   *  @returns -1 on failure, 0 otherwise
   **/
  virtual int TimerRemove(const timer_id_t timer_id) = 0;
  TimerService(){};
  virtual ~TimerService(){};
};
#endif /* FILE_TIMERSERVICE_HPP_SEEN */
