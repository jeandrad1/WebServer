<?php
if (!isset($_COOKIE['user'])) {
    header("Location: /login/login.html");
    exit();
}
ob_start();
?> <!DOCTYPE html>
<html lang="es">
<head>
	<meta charset="UTF-8">
	<title>Portal</title>
	<link rel="stylesheet" href="portal.css">
	<link href="https://fonts.googleapis.com/css2?family=Anton&display=swap" rel="stylesheet">
</head>
<body>
	<h1>Bienvenido</h1>

	<div class="file-upload">
		<form action="upload.php" method="post" enctype="multipart/form-data">
			<label for="archivo">Selecciona un archivo para subir:</label>
			<input type="file" name="archivo" id="archivo" required>
			<button type="submit">Subir</button>
		</form>
	</div>
	<div class="file-list">
		<h2>Archivos subidos:</h2>
		<ul>
			<?php
			$directorio = 'file_library/';
            if (is_dir($directorio)) {
                $archivos = array_diff(scandir($directorio), ['.', '..']);
                foreach ($archivos as $archivo) {
                    $ruta = $directorio . urlencode($archivo);
                    echo "<li><a href='$ruta' download>" . htmlspecialchars($archivo) . "</a>" .
                         "<form action='delete.php' method='post' style='display: inline; margin-left: 10px;'>" .
                         "<input type='hidden' name='filename' value='" . htmlspecialchars($archivo) . "'>" .
                         "<button type='submit' class='delete-btn'>Borrar</button>" .
                         "</form></li>";
                }
            } else {
                echo "<p>No se encontró la carpeta de archivos.</p>";
			}
			?>
		</ul>
	</div>
</body>
</html>;
<?php

$content = ob_get_clean();
header("Content-Type: text/html; charset=UTF-8");
header("Content-Length: " . strlen($content));
echo $content;
?>