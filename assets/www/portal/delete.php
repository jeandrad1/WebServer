<?php
if (!isset($_COOKIE['user'])) {
    header("Location: /login/login.html");
    exit();
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['filename'])) {
    $filename = $_POST['filename'];
    $uploadDir = 'file_library/';
    $filePath = $uploadDir . $filename;

    // Security check to prevent directory traversal
    if (strpos(realpath($filePath), realpath($uploadDir)) === 0 && file_exists($filePath)) {
        unlink($filePath);
    }
}

header("Location: portal.php");
exit();
?>