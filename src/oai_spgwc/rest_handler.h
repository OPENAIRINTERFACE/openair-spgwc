/*
 * Copyright (c) 2017 Sprint
 *
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the terms found in the LICENSE file in the root of this source tree.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef REST_HANDLER_H_
#define REST_HANDLER_H_

#include <iostream>
#include <pistache/endpoint.h>
#include <pistache/router.h>

#include <memory>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

extern RAPIDJSON_NAMESPACE::Document docSchema;

class RestHandler : public Pistache::Http::Handler {
 public:
  HTTP_PROTOTYPE(RestHandler)
  void onRequest(
      const Pistache::Http::Request& request,
      Pistache::Http::ResponseWriter response);
};

#endif /* REST_HANDLER_H_ */
