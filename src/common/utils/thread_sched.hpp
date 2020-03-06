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

/*! \file thread_sched.hpp
  \brief
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_THREAD_SCHED_HPP_SEEN
#define FILE_THREAD_SCHED_HPP_SEEN

#include <sched.h>
#include "logger.hpp"

namespace util {

class thread_sched_params {
 public:
  thread_sched_params()
      : cpu_id(0), sched_policy(SCHED_FIFO), sched_priority(84) {}
  int cpu_id;
  int sched_policy;
  int sched_priority;
  void apply(const int task_id, _Logger& logger) const;
};

}  // namespace util
#endif /* FILE_THREAD_SCHED_HPP_SEEN */
