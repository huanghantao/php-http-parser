# Introduction

**A PHP extension to parse http protocol.**

## Example

You can find all the examples in the tests folder. For better programming, you can install the ide helper that we provide:

```bash
composer require hantaohuang/http-parser-ide-helper --dev
```

## Installation

### Compiling requirements

1. Linux, OS X or Cygwin, WSL
2. PHP 7.0.0 or later
3. GCC 4.8 or later

### Install from source

```bash
git clone https://github.com/huanghantao/php-http-parser.git && \
cd php-http-parser && \
phpize && \
./configure && \
make && make install
```

#### Enable extension in PHP

After compiling and installing to the system successfully, you have to add a new line extension=http_parser.so to php.ini to enable http-parser extension.

## Bug

You can feedback through issues, and we will reply within 24 hours.
