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

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['user']) && isset($_POST['password'])) {
    $user = htmlspecialchars(trim($_POST['user']));
    $password = htmlspecialchars(trim($_POST['password']));

	if (verify_user($file, $user, $password)) {
		setcookie("user", $user, time() + 3600, "/");
		header("Status: 302 Found");
		header("Location: /portal/portal.php");
		exit();    } 
	else {
        echo "Content-Type: text/html\r\n\r\n";
        echo "<html><body>";
        echo "<h1>Usuario o contraseña incorrectos</h1>";
        echo "<a href='login/login.html'>Volver</a>";
        echo "</body></html>";
    }
} else {
    echo "Content-Type: text/html\r\n\r\n";
    echo "<html><body>";
    echo "<h1>Error en el login</h1>";
    echo "<a href='login/login.html'>Volver</a>";
    echo "</body></html>";
}
?>