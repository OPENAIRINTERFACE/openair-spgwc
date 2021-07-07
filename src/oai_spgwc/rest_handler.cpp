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

#include "logger.hpp"
#include "rest_handler.h"

#include "rapidjson/error/en.h"

#include <pistache/endpoint.h>

void RestHandler::onRequest(
    const Pistache::Http::Request& request,
    Pistache::Http::ResponseWriter response) {
  std::cout << request.resource() << std::endl;
  std::cout << request.method() << std::endl;
  std::cout << request.body() << std::endl;

  if (request.resource() == "/status") {
    RAPIDJSON_NAMESPACE::Document doc;
    std::string keyschema("userschema");

    doc.Parse(request.body().c_str());
    if (doc.HasParseError()) {
      std::stringstream ss;
      ss << "Body parsing error offset=" << doc.GetErrorOffset() << " error="
         << RAPIDJSON_NAMESPACE::GetParseError_En(doc.GetParseError());
      response.send(Pistache::Http::Code::Bad_Request, ss.str());
      return;
    }
    response.send(Pistache::Http::Code::Ok, "Started");
  } else {
    std::stringstream ss;
    ss << "Unrecognized resource [" << request.resource() << "]";
    response.send(Pistache::Http::Code::Bad_Request, ss.str());
  }
}
