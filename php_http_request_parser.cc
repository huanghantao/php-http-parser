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
#include "include/context.h"

using http::Ctx;

typedef struct {
    Ctx *ctx;
    zend_object std;
} http_request_parser_t;

zend_class_entry *http_request_parser_ce;
static zend_object_handlers http_request_parser_handlers;

ZEND_BEGIN_ARG_INFO_EX(arginfo_http_request_parser_void, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_http_request_parser_parse, 0, 0, 1)
ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

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

static Ctx *php_http_request_parser_get_ptr(zval *zobject) {
    return php_http_request_parser_fetch_object(Z_OBJ_P(zobject))->ctx;
}

static PHP_METHOD(http_request_parser, __construct) {
    http_request_parser_t *request_parser = php_http_request_parser_fetch_object(Z_OBJ_P(ZEND_THIS));
    request_parser->ctx = new Ctx();

    http_parser_init(&request_parser->ctx->parser, HTTP_REQUEST);
}

static PHP_METHOD(http_request_parser, parse) {
    char *data;
    size_t data_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STRING(data, data_len)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    Ctx *ctx = php_http_request_parser_get_ptr(ZEND_THIS);
    /* Start up / continue the parser.
     * Note we pass recved==0 to signal that EOF has been received.
     */
    size_t nparsed = ctx->parse(data, data_len);
    RETURN_LONG(nparsed);
}

static PHP_METHOD(http_request_parser, getHeaders) {
    Ctx *ctx = php_http_request_parser_get_ptr(ZEND_THIS);

    array_init(return_value);

    for (auto iter = ctx->request->header.begin(); iter != ctx->request->header.end(); iter++) {
        add_assoc_stringl_ex(
            return_value, iter->first.c_str(), iter->first.length(), iter->second.c_str(), iter->second.length());
    }
    RETURN_ARR(Z_ARRVAL_P(return_value));
}

static PHP_METHOD(http_request_parser, getBody) {
    Ctx *ctx = php_http_request_parser_get_ptr(ZEND_THIS);
    RETURN_STRINGL(ctx->request->body, ctx->request->body_length);
}

static const zend_function_entry http_request_parser_methods[] = {
    PHP_ME(http_request_parser, __construct, arginfo_http_request_parser_void, ZEND_ACC_PUBLIC)
        PHP_ME(http_request_parser, parse, arginfo_http_request_parser_parse, ZEND_ACC_PUBLIC)
            PHP_ME(http_request_parser, getHeaders, arginfo_http_request_parser_void, ZEND_ACC_PUBLIC)
                PHP_ME(http_request_parser, getBody, arginfo_http_request_parser_void, ZEND_ACC_PUBLIC) PHP_FE_END};

void php_http_request_parser_minit(int module_number) {
    HTTP_PARSER_INIT_CLASS_ENTRY(http_request_parser, "Http\\Request\\Parser", NULL, NULL, http_request_parser_methods);
    HTTP_PARSER_SET_CLASS_CUSTOM_OBJECT(http_request_parser,
                                        php_http_request_parser_create_object,
                                        php_http_request_parser_free_object,
                                        http_request_parser_t,
                                        std);
}
