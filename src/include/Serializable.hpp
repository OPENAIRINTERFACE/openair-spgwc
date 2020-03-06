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

/*! \file Serializable.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_SERIALIZABLE_HPP_SEEN
#define FILE_SERIALIZABLE_HPP_SEEN
//--related header -------------------------------------------------------------
//--C includes -----------------------------------------------------------------
//--C++ includes ---------------------------------------------------------------
#include <iostream>
#include <string>
//--Other includes -------------------------------------------------------------
//------------------------------------------------------------------------------
class StreamSerializable {
 public:
  virtual void DumpTo(std::ostream& os) = 0;
  virtual void LoadFrom(std::istream& is) = 0;
  virtual ~StreamSerializable() = 0;
};

#endif /* FILE_SERIALIZABLE_HPP_SEEN */
