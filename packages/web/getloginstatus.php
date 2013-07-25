<?php

$req=pack("C8", 0, 0, 0, 8, 0, 10, 0, 10);

$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if( !@socket_connect($socket, "127.0.0.1", 2061) ) {
	header("HTTP/1.1 500 Internal Server Error");
	die();
}

if( !socket_write($socket, $req ) ) {
	header("HTTP/1.1 500 Internal Server Error");
	die();
}
$buffer=@socket_read($socket, 1024, PHP_BINARY_READ);
if( !$buffer ) {
	header("HTTP/1.1 500 Internal Server Error");
	die();
}

$array=unpack("C10header/a*json", $buffer);
echo $array["json"];

socket_close($socket);

?>