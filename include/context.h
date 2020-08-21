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
#pragma once

#include <map>
#include <string>
#include <algorithm>

#include "include/http_parser.h"

namespace http {

class Request;

class Ctx {
 public:
    http_parser parser;
    Request *request;
    char *current_header_name;
    size_t current_header_name_len;
    bool keep_alive;

    Ctx();
    ~Ctx();
    size_t parse(char *data, ssize_t recved);
};

class Request {
 public:
    std::string method;
    int version;
    char *path = nullptr;
    uint32_t path_len;
    char *body = nullptr;
    size_t body_length;

    /**
     * header_name is converted to lowercase
     */
    std::map<std::string, std::string> header;

    Request();
    ~Request();

    inline bool has_header(std::string header_name) {
        return header.find(header_name) != header.end();
    }

    inline std::string get_header(std::string header_name) {
        auto it = header.find(header_name);
        if (it == header.end()) {
            return "";
        }
        return it->second;
    }

    inline bool header_contain_value(std::string header_name, std::string header_value) {
        auto it = header.find(header_name);
        if (it == header.end()) {
            return false;
        }

        std::string real_value_lowercase = it->second;
        std::transform(
            real_value_lowercase.begin(), real_value_lowercase.end(), real_value_lowercase.begin(), ::tolower);
        if (real_value_lowercase != header_value) {
            return false;
        }

        return true;
    }

    inline bool has_sec_websocket_key() {
        std::string key = "sec-websocket-key";
        return has_header(key);
    }

    inline void clear() {
        clear_path()->clear_header()->clear_body();
    }

    inline Request *clear_path() {
        delete[] path;
        path = nullptr;
        path_len = 0;
        return this;
    }

    inline Request *clear_header() {
        header.clear();
        return this;
    }

    inline Request *clear_body() {
        delete[] body;
        body = nullptr;
        body_length = 0;
        return this;
    }
};

}  // namespace http
