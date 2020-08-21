--TEST--
parser_request: get all parsed content
--SKIPIF--
<?php
if (!extension_loaded('http_parser')) {
	echo 'skip';
}
?>
--FILE--
<?php
require __DIR__ . '/../include/bootstrap.php';

$data = "POST /api/test HTTP/1.1\r\nHost: www.foo.com\r\nUser-Agent: curl/7.64.1\r\nAccept: */*\r\nContent-Length: 7\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nfoo=bar";
$parser = new Http\Request\Parser;
$parser->parse($data);

Assert::eq("POST", $parser->getMethod());
Assert::eq("/api/test", $parser->getURL());
Assert::eq(101, $parser->getVersion());
Assert::eq([
    'accept' => '*/*',
    'content-length' => '7',
    'content-type' => 'application/x-www-form-urlencoded',
    'host' => 'www.foo.com',
    'user-agent' => 'curl/7.64.1',
], $parser->getHeaders());
Assert::eq('foo=bar', $parser->getBody());
?>
--EXPECT--
