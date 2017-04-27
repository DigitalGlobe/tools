// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef EXAMPLES_HELLONET_CURLFETCH_H__
#define EXAMPLES_HELLONET_CURLFETCH_H__

#include <string>
#include "kml/base/net_cache.h"
#include "kml/convenience/http_client.h"

bool CurlToString(const char* url, std::string* data);

// This class matches the NetFetcher concept used with kmlbase::NetCache.
class CurlNetFetcher : public kmlbase::NetFetcher {
 public:
  bool FetchUrl(const std::string& url, std::string* data) const {
    return CurlToString(url.c_str(), data);
  }
};

// This HttpClient uses libcurl to send the request.
class CurlHttpClient : public kmlconvenience::HttpClient {
 public:
  CurlHttpClient(const std::string& application_name);

  // HttpClient::SendRequest()
  virtual bool SendRequest(
    kmlconvenience::HttpMethodEnum http_method,
    const std::string& request_uri,
    const kmlconvenience::StringPairVector* request_headers,
    const std::string* post_data,
    std::string* response) const;
};

#endif  // EXAMPLES_HELLONET_CURLFETCH_H__
