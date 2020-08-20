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

#ifdef HAVE_CONFIG_H
#include "./config.h"
#endif

#include "php/main/php.h"
#include "php/ext/standard/info.h"
#include "./php_http_parser.h"
#include "thirdparty/http/http_parser.h"

PHP_MINIT_FUNCTION(http_parser) {
    php_http_request_parser_minit(module_number);

    return SUCCESS;
}

PHP_RINIT_FUNCTION(http_parser) {
    return SUCCESS;
}

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

static const zend_function_entry http_parser_functions[] = {};

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

#ifdef COMPILE_DL_HTTP_PARSER
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(http_parser)
#endif
