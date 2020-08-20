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
/* php-http-parser extension for PHP */

#ifndef PHP_HTTP_PARSER_H_
#define PHP_HTTP_PARSER_H_

extern zend_module_entry http_parser_module_entry;
#define phpext_php_http_parser_ptr &http_parser_module_entry

#define PHP_HTTP_PARSER_VERSION "0.1.0"

#if defined(ZTS) && defined(COMPILE_DL_PHP_HTTP_PARSER)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif  // PHP_HTTP_PARSER_H_
