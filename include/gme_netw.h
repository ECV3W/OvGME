/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/
 */

#ifndef GME_NETW_H_INCLUDED
#define GME_NETW_H_INCLUDED

#include <gme.h>

#define GME_HTTPGET_ERR_DNS 1
#define GME_HTTPGET_ERR_CNX 2
#define GME_HTTPGET_ERR_ENC 3
#define GME_HTTPGET_ERR_BAL 4
#define GME_HTTPGET_ERR_REC 5
#define GME_HTTPGET_ERR_FOP 6
#define GME_HTTPGET_ERR_FWR 7

#define HTTP_ERROR_BAD_REQUEST 400
#define HTTP_ERROR_UNAUTHORIZED 401
#define HTTP_ERROR_FORBIDDEN 403
#define HTTP_ERROR_PAGE_NOT_FOUND 404
#define HTTP_ERROR_METHOD_NOT_ALLOWED 405
#define HTTP_ERROR_PAYLOAD_TOO_LARGE 413
#define HTTP_ERROR_INTERNAL_SERVER_ERROR 500
#define HTTP_ERROR_BAD_GATEWAY 502
#define HTTP_ERROR_SERVICE_UNAVAILABLE 503
#define HTTP_ERROR_GATEWAY_TIMEOUT 504


bool GME_NetwIsUrl(const char* str);
std::string GME_NetwEncodeUrl(const char* url);
std::string GME_NetwEncodeUrl(const std::string& url);

typedef void(*GME_NetwGETOnErr)(const char* url);
typedef bool(*GME_NetwGETOnDnl)(unsigned percent, unsigned rate);
typedef int (*GME_NetwGETOnDnlCurl)(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
typedef void(*GME_NetwGETOnEnd)(const char* body, size_t body_size);
typedef void(*GME_NetwGETOnSav)(const wchar_t* path);

struct curlProgress {
  curl_off_t lastruntime;
  CURL *curl;
};

struct curlRes {
  CURLcode res;
  long http_code;
};

int GME_NetwHttpGET(const char* url, const GME_NetwGETOnErr, const GME_NetwGETOnDnl, const GME_NetwGETOnEnd);
int GME_NetwHttpGET(const char* url, const GME_NetwGETOnErr, const GME_NetwGETOnDnl, const GME_NetwGETOnSav, const std::wstring& path);
curlRes GME_NetwHttpGETCurl(const char* url, const GME_NetwGETOnErr, const GME_NetwGETOnDnlCurl, const GME_NetwGETOnSav, const std::wstring& path);
curlRes GME_NetwHttpGETCurl(const char* url_str, const GME_NetwGETOnErr on_err,  const GME_NetwGETOnDnlCurl on_dnl, const GME_NetwGETOnEnd on_end);

#endif // GME_NETW_H_INCLUDED
