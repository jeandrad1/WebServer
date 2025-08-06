<?php
echo "Content-Type: text/html\r\n\r\n";
if (isset($_COOKIE['user'])) {
    echo "<h1>Cookie 'user' está puesta: " . htmlspecialchars($_COOKIE['user']) . "</h1>";
} else {
    echo "<h1>Cookie 'user' NO está puesta</h1>";
}
?>