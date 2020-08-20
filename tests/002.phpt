--TEST--
php-http-parser_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('php-http-parser')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = php-http-parser_test1();

var_dump($ret);
?>
--EXPECT--
The extension php-http-parser is loaded and working!
NULL
