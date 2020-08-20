#!/bin/sh -e
phpize --clean
phpize
./configure --enable-php-http-parser-dev
make clean
make
make install
