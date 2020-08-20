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

#include "php/main/php.h"
#include "php/ext/standard/info.h"
#include "./php_http_parser.h"
#include "thirdparty/http/http_parser.h"

typedef struct {
    http_parser *parser;
    zend_object std;
} http_request_parser_t;

zend_class_entry *http_request_parser_ce;
static zend_object_handlers http_request_parser_handlers;

static inline http_request_parser_t *php_http_request_parser_fetch_object(zend_object *obj) {
    return reinterpret_cast<http_request_parser_t *>(
        (reinterpret_cast<char *>(obj) - http_request_parser_handlers.offset));
}

static zend_object *php_http_request_parser_create_object(zend_class_entry *ce) {
    http_request_parser_t *http_request_parser =
        reinterpret_cast<http_request_parser_t *>(zend_object_alloc(sizeof(http_request_parser_t), ce));
    zend_object_std_init(&http_request_parser->std, ce);
    object_properties_init(&http_request_parser->std, ce);
    http_request_parser->std.handlers = &http_request_parser_handlers;
    return &http_request_parser->std;
}

static void php_http_request_parser_free_object(zend_object *object) {
    http_request_parser_t *http_request_parser =
        reinterpret_cast<http_request_parser_t *>(php_http_request_parser_fetch_object(object));
    zend_object_std_dtor(&http_request_parser->std);
}

static const zend_function_entry http_request_parser_methods[] = {};

void php_http_request_parser_minit(int module_number) {
    HTTP_PARSER_INIT_CLASS_ENTRY(http_request_parser, "Http\\Request\\Parser", NULL, NULL, http_request_parser_methods);
    HTTP_PARSER_SET_CLASS_CUSTOM_OBJECT(http_request_parser,
                                        php_http_request_parser_create_object,
                                        php_http_request_parser_free_object,
                                        http_request_parser_t,
                                        std);
}
