PHP_ARG_ENABLE(php-http-parser, whether to enable php-http-parser support,
[  --enable-php-http-parser          Enable php-http-parser support], no)

PHP_ARG_ENABLE(php-http-parser-dev, whether to enable php-http-parser developer build flags,
[  --enable-php-http-parser-dev       php-http-parser: Enable developer flags],, no)

AC_MSG_CHECKING([if compiling with clang])
AC_COMPILE_IFELSE([
    AC_LANG_PROGRAM([], [[
        #ifndef __clang__
            not clang
        #endif
    ]])],
    [CLANG=yes], [CLANG=no]
)
AC_MSG_RESULT([$CLANG])

if test "$CLANG" = "yes"; then
    CFLAGS="$CFLAGS -std=gnu89"
fi

AC_CANONICAL_HOST

if test "$PHP_PHP_HTTP_PARSER" != "no"; then
  if test "$PHP_PHP_HTTP_PARSER_DEV" = "yes"; then
    PHP_CHECK_GCC_ARG(-Wbool-conversion,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wbool-conversion")
    PHP_CHECK_GCC_ARG(-Wdiscarded-qualifiers,           _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wdiscarded-qualifiers")
    PHP_CHECK_GCC_ARG(-Wduplicate-enum,                 _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wduplicate-enum")
    PHP_CHECK_GCC_ARG(-Wempty-body,                     _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wempty-body")
    PHP_CHECK_GCC_ARG(-Wenum-compare,                   _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wenum-compare")
    PHP_CHECK_GCC_ARG(-Werror,                          _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Werror")
    PHP_CHECK_GCC_ARG(-Wextra,                          _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wextra")
    PHP_CHECK_GCC_ARG(-Wformat-security,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wformat-security")
    PHP_CHECK_GCC_ARG(-Wheader-guard,                   _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wheader-guard")
    PHP_CHECK_GCC_ARG(-Wincompatible-pointer-types-discards-qualifiers, _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wincompatible-pointer-types-discards-qualifiers")
    PHP_CHECK_GCC_ARG(-Winit-self,                      _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Winit-self")
    PHP_CHECK_GCC_ARG(-Wlogical-not-parentheses,        _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-not-parentheses")
    PHP_CHECK_GCC_ARG(-Wlogical-op,                     _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-op")
    PHP_CHECK_GCC_ARG(-Wlogical-op-parentheses,         _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wlogical-op-parentheses")
    PHP_CHECK_GCC_ARG(-Wloop-analysis,                  _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wloop-analysis")
    PHP_CHECK_GCC_ARG(-Wmaybe-uninitialized,            _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wmaybe-uninitialized")
    PHP_CHECK_GCC_ARG(-Wno-missing-field-initializers,  _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-missing-field-initializers")
    PHP_CHECK_GCC_ARG(-Wno-sign-compare,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-sign-compare")
    PHP_CHECK_GCC_ARG(-Wno-unused-but-set-variable,     _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-unused-but-set-variable")
    PHP_CHECK_GCC_ARG(-Wno-unused-parameter,            _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-unused-parameter")
    PHP_CHECK_GCC_ARG(-Wno-variadic-macros,             _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wno-variadic-macros")
    PHP_CHECK_GCC_ARG(-Wparentheses,                    _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wparentheses")
    PHP_CHECK_GCC_ARG(-Wpointer-bool-conversion,        _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wpointer-bool-conversion")
    PHP_CHECK_GCC_ARG(-Wsizeof-array-argument,          _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wsizeof-array-argument")
    PHP_CHECK_GCC_ARG(-Wwrite-strings,                  _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -Wwrite-strings")
    PHP_CHECK_GCC_ARG(-fdiagnostics-show-option,        _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fdiagnostics-show-option")
    PHP_CHECK_GCC_ARG(-fno-omit-frame-pointer,          _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-omit-frame-pointer")
    PHP_CHECK_GCC_ARG(-fno-optimize-sibling-calls,      _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fno-optimize-sibling-calls")
    PHP_CHECK_GCC_ARG(-fsanitize-address,               _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fsanitize-address")
    PHP_CHECK_GCC_ARG(-fstack-protector,                _MAINTAINER_CFLAGS="$_MAINTAINER_CFLAGS -fstack-protector")

    EXTRA_CFLAGS="$_MAINTAINER_CFLAGS"
    CFLAGS="-g -O0 -Wall $CFLAGS"
    CXXFLAGS="-g -O0 -Wall $CXXFLAGS"
  fi

  PHP_SUBST(HTTP_PARSER_SHARED_LIBADD)

  php_http_parser_file="\
    php_http_parser.cc \
    php_http_request_parser.cc \
  "

  php_http_parser_file="\
    $php_http_parser_file \
    thirdparty/http/http_parser.c \
  "

  PHP_NEW_EXTENSION(http_parser, $php_http_parser_file, $ext_shared,,, cxx)

  PHP_ADD_INCLUDE([$ext_srcdir])

  PHP_REQUIRE_CXX()

  CXXFLAGS="$CXXFLAGS -Wall -Wno-unused-function -Wno-deprecated -Wno-deprecated-declarations"

  if test "$SW_OS" = "CYGWIN" || test "$SW_OS" = "MINGW"; then
    CXXFLAGS="$CXXFLAGS -std=gnu++11"
  else
    CXXFLAGS="$CXXFLAGS -std=c++11"
  fi

  PHP_ADD_BUILD_DIR($ext_builddir/thirdparty/http)
fi
