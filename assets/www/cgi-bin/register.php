<?php
$file = __DIR__ . '/../register/users.txt';

function verify_user($file, $user, $password) {
    if (!file_exists($file)) return false;
    $lines = file($file, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
    foreach ($lines as $line) {
        list($existing_user, $hashed_password) = explode(':', $line, 2);
        if ($existing_user === $user && password_verify($password, $hashed_password)) {
            return true;
        }
    }
    return false;
}

function user_exists($file, $user) {
    if (!file_exists($file)) return false;
    $lines = file($file, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
    foreach ($lines as $line) {
        list($existing_user,) = explode(':', $line, 2);
        if ($existing_user === $user) return true;
    }
    return false;
}

function send_response($html) {
    header("Content-Type: text/html");
    header("Content-Length: " . strlen($html));
    echo $html;
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['user']) && isset($_POST['password'])) {
    $user = htmlspecialchars(trim($_POST['user']));
    $password = htmlspecialchars(trim($_POST['password']));
    $hashed_password = password_hash($password, PASSWORD_DEFAULT);

    if (verify_user($file, $user, $password)) {
        setcookie("user", $user, time() + 3600, "/");
		header("Status: 302 Found");
        header("Location: /portal/portal.php");
        exit();
    } else {
        setcookie("user", $user, time() + 3600, "/");
        $entry = $user . ':' . $hashed_password . "\n";
        file_put_contents($file, $entry, FILE_APPEND);
		header("Status: 302 Found");
        header("Location: /portal/portal.php");
        exit();
    }
} else {
    $html = "<html><body>";
    $html .= "<h1>Error en el registro</h1>";
    $html .= "<a href='/register/register.html'>Volver</a>";
    $html .= "</body></html>";
    send_response($html);
}
?>