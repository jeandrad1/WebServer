#!/usr/bin/php-cgi
<?php
header("Content-Type: text/plain");
header("Content-Length: 19");

echo "Hello from PHP CGI!\n";
echo "Request Method: " . $_SERVER['REQUEST_METHOD'] . "\n";
echo "Query String: " . $_SERVER['QUERY_STRING'] . "\n";
echo "Script Name: " . $_SERVER['SCRIPT_NAME'] . "\n";
?>