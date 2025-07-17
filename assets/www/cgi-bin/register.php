<?php
$file = '/home/jeandrad/Desktop/webserv/assets/www/register/users.txt';

function user_exists($file, $user) {
    if (!file_exists($file)) return false;
    $lines = file($file, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
    foreach ($lines as $line) {
        list($existing_user,) = explode(':', $line, 2);
        if ($existing_user === $user) return true;
    }
    return false;
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['user']) && isset($_POST['password'])) {
    $user = htmlspecialchars(trim($_POST['user']));
    $password = htmlspecialchars(trim($_POST['password']));
    $hashed_password = password_hash($password, PASSWORD_DEFAULT);

    if (user_exists($file, $user)) {
        echo "Content-Type: text/html\r\n\r\n";
        echo "<html><body>";
        echo "<h1>El usuario ya existe.</h1>";
        echo "<a href='register.html'>Volver</a>";
        echo "</body></html>";
    } else {
        $entry = $user . ':' . $hashed_password . "\n";
        file_put_contents($file, $entry, FILE_APPEND);

        echo "Content-Type: text/html\r\n\r\n";
        echo "<html><body>";
        echo "<h1>Registro exitoso</h1>";
        echo "<a href='register.html'>Volver</a>";
        echo "</body></html>";
    }
} else {
    echo "Content-Type: text/html\r\n\r\n";
    echo "<html><body>";
    echo "<h1>Error en el registro</h1>";
    echo "<a href='register.html'>Volver</a>";
    echo "</body></html>";
}
?>