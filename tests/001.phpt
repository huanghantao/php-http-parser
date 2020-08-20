--TEST--
Check if php-http-parser is loaded
--SKIPIF--
<?php
if (!extension_loaded('php-http-parser')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "php-http-parser" is available';
?>
--EXPECT--
The extension "php-http-parser" is available
