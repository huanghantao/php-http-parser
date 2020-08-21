/**
 * Copyright 2020 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include "include/context.h"

using http::Ctx;
using http::Request;

static int http_request_on_message_begin(http_parser *parser);
static int http_request_on_url(http_parser *parser, const char *at, size_t length);
static int http_request_on_status(http_parser *parser, const char *at, size_t length);
static int http_request_on_header_field(http_parser *parser, const char *at, size_t length);
static int http_request_on_header_value(http_parser *parser, const char *at, size_t length);
static int http_request_on_headers_complete(http_parser *parser);
static int http_request_on_body(http_parser *parser, const char *at, size_t length);
static int http_request_on_message_complete(http_parser *parser);

inline void set_http_version(Ctx *ctx, http_parser *parser) {
    ctx->request->version = parser->http_major * 100 + parser->http_minor;
}

inline void set_http_method(Ctx *ctx, http_parser *parser) {
    switch (parser->method) {
    case HTTP_GET:
        ctx->request->method = "GET";
        break;
    case HTTP_POST:
        ctx->request->method = "POST";
        break;
    default:
        break;
    }
}

static int http_request_on_message_begin(http_parser *parser) {
    return 0;
}

static int http_request_on_url(http_parser *parser, const char *at, size_t length) {
    Ctx *ctx = reinterpret_cast<Ctx *>(parser->data);
    /**
     * because const char *at may be destroyed, so must copy to ctx->request.path
     */
    ctx->request->path = new char[length + 1]();
    memcpy(ctx->request->path, at, length);
    ctx->request->path_len = length;
    return 0;
}

static int http_request_on_status(http_parser *parser, const char *at, size_t length) {
    return 0;
}

static int http_request_on_header_field(http_parser *parser, const char *at, size_t length) {
    Ctx *ctx = reinterpret_cast<Ctx *>(parser->data);
    ctx->current_header_name = const_cast<char *>(at);
    ctx->current_header_name_len = length;
    return 0;
}

static int http_request_on_header_value(http_parser *parser, const char *at, size_t length) {
    Ctx *ctx = reinterpret_cast<Ctx *>(parser->data);
    std::map<std::string, std::string> &headers = ctx->request->header;
    std::string header_name(ctx->current_header_name, ctx->current_header_name_len);
    std::string header_value(at, length);

    std::transform(header_name.begin(), header_name.end(), header_name.begin(), ::tolower);
    headers[header_name] = header_value;

    return 0;
}

static int http_request_on_headers_complete(http_parser *parser) {
    Ctx *ctx = reinterpret_cast<Ctx *>(parser->data);
    set_http_version(ctx, parser);
    set_http_method(ctx, parser);
    ctx->keep_alive = http_should_keep_alive(parser);
    return 0;
}

static int http_request_on_body(http_parser *parser, const char *at, size_t length) {
    Ctx *ctx = reinterpret_cast<Ctx *>(parser->data);
    ctx->request->body = new char[length + 1]();
    memcpy(ctx->request->body, at, length);
    ctx->request->body_length = length;
    return 0;
}

static int http_request_on_message_complete(http_parser *parser) {
    return 0;
}

static const http_parser_settings parser_settings = {
    .on_message_begin = http_request_on_message_begin,
    .on_url = http_request_on_url,
    .on_status = http_request_on_status,
    .on_header_field = http_request_on_header_field,
    .on_header_value = http_request_on_header_value,
    .on_headers_complete = http_request_on_headers_complete,
    .on_body = http_request_on_body,
    .on_message_complete = http_request_on_message_complete,
};

Request::Request() {}

Request::~Request() {
    clear();
}

Ctx::Ctx() {
    request = new Request();
    parser.data = this;
}

Ctx::~Ctx() {
    parser.data = nullptr;
    delete request;
}

size_t Ctx::parse(char *data, ssize_t recved) {
    size_t nparsed;

    nparsed = http_parser_execute(&parser, &parser_settings, data, recved);
    return nparsed;
}
