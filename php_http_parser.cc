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
/* php_http_parser extension for PHP */

#ifdef HAVE_CONFIG_H
#include "./config.h"
#endif

#include "php/main/php.h"
#include "php/ext/standard/info.h"
#include "./php_http_parser.h"
#include "thirdparty/http/http_parser.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE()                                                                                   \
    ZEND_PARSE_PARAMETERS_START(0, 0)                                                                                  \
    ZEND_PARSE_PARAMETERS_END()
#endif

typedef struct {
    http_parser *parser;
    zend_object std;
} http_parser_t;

zend_class_entry *http_parser_ce;
static zend_object_handlers http_parser_handlers;

static inline http_parser_t *php_http_parser_fetch_object(zend_object *obj) {
    return reinterpret_cast<http_parser_t *>((reinterpret_cast<char *>(obj) - http_parser_handlers.offset));
}

static zend_object *php_http_parser_create_object(zend_class_entry *ce) {
    http_parser_t *http_parser = reinterpret_cast<http_parser_t *>(zend_object_alloc(sizeof(http_parser_t), ce));
    zend_object_std_init(&http_parser->std, ce);
    object_properties_init(&http_parser->std, ce);
    http_parser->std.handlers = &http_parser_handlers;
    return &http_parser->std;
}

static void php_http_parser_free_object(zend_object *object) {
    http_parser_t *http_parser = reinterpret_cast<http_parser_t *>(php_http_parser_fetch_object(object));
    zend_object_std_dtor(&http_parser->std);
}

/* {{{ void php_http_parser_test1()
 */
PHP_FUNCTION(php_http_parser_test1) {
    ZEND_PARSE_PARAMETERS_NONE();

    php_printf("The extension %s is loaded and working!\r\n", "php_http_parser");
}
/* }}} */

/* {{{ string php_http_parser_test2( [ string $var ] )
 */
PHP_FUNCTION(php_http_parser_test2) {
    char *var = "World";
    size_t var_len = sizeof("World") - 1;
    zend_string *retval;

    ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_STRING(var, var_len)
    ZEND_PARSE_PARAMETERS_END();

    retval = strpprintf(0, "Hello %s", var);

    RETURN_STR(retval);
}
/* }}}*/

static const zend_function_entry http_parser_methods[] = {};

PHP_MINIT_FUNCTION(http_parser) {
    HTTP_PARSER_INIT_CLASS_ENTRY(http_parser, "Http\\Parser", NULL, NULL, http_parser_methods);
    HTTP_PARSER_SET_CLASS_CUSTOM_OBJECT(
        http_parser, php_http_parser_create_object, php_http_parser_free_object, http_parser_t, std);

    return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(http_parser) {
#if defined(ZTS) && defined(COMPILE_DL_php_http_parser)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(http_parser) {
    char buf[64];

    php_info_print_table_start();
    php_info_print_table_header(2, "Http Parser support", "enabled");
    php_info_print_table_row(2, "Author", "Codinghuang <https://github.com/huanghantao>");
    php_info_print_table_row(2, "Version", PHP_HTTP_PARSER_VERSION);
    snprintf(buf, sizeof(buf), "%s %s", __DATE__, __TIME__);
    php_info_print_table_row(2, "Built", buf);
    php_info_print_table_end();
    DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_php_http_parser_test1, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_php_http_parser_test2, 0)
ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ php_http_parser_functions[]
 */
static const zend_function_entry http_parser_functions[] = {
    PHP_FE(php_http_parser_test1, arginfo_php_http_parser_test1)
        PHP_FE(php_http_parser_test2, arginfo_php_http_parser_test2) PHP_FE_END};
/* }}} */

/* {{{ http_parser_module_entry
 */
zend_module_entry http_parser_module_entry = {STANDARD_MODULE_HEADER,
                                              "http_parser",           /* Extension name */
                                              http_parser_functions,   /* zend_function_entry */
                                              PHP_MINIT(http_parser),  /* PHP_MINIT - Module initialization */
                                              NULL,                    /* PHP_MSHUTDOWN - Module shutdown */
                                              PHP_RINIT(http_parser),  /* PHP_RINIT - Request initialization */
                                              NULL,                    /* PHP_RSHUTDOWN - Request shutdown */
                                              PHP_MINFO(http_parser),  /* PHP_MINFO - Module info */
                                              PHP_HTTP_PARSER_VERSION, /* Version */
                                              STANDARD_MODULE_PROPERTIES};
/* }}} */

#ifdef COMPILE_DL_HTTP_PARSER
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(http_parser)
#endif
