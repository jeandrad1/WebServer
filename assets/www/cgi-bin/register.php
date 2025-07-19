<?php
$file = __DIR__ . '/../register/users.txt';

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

    if (user_exists($file, $user)) {
        $html = "<html><body>";
        $html .= "<h1>El usuario ya existe.</h1>";
        $html .= "<a href='/login/login.html'>login</a>";
        $html .= "</body></html>";
        send_response($html);
    } else {
        $entry = $user . ':' . $hashed_password . "\n";
        file_put_contents($file, $entry, FILE_APPEND);

        $html = "<html><body>";
        $html .= "<h1>Registro exitoso</h1>";
        $html .= "<a href='/portal/portal.php'>Continuar</a>";
        $html .= "</body></html>";
        send_response($html);
    }
} else {
    $html = "<html><body>";
    $html .= "<h1>Error en el registro</h1>";
    $html .= "<a href='/register/register.html'>Volver</a>";
    $html .= "</body></html>";
    send_response($html);
}
?>