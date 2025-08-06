<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_FILES['file'])) {
    $uploadDir = '/../download/';
    $uploadFile = $uploadDir . basename($_FILES['file']['name']);

    if (move_uploaded_file($_FILES['file']['tmp_name'], $uploadFile)) {
        echo "Content-Type: text/html\r\n\r\n";
        echo "<html><body>";
        echo "<h1>Archivo subido exitosamente.</h1>";
        echo "<a href='download.html'>Volver</a>";
        echo "</body></html>";
    } else {
        echo "Content-Type: text/html\r\n\r\n";
        echo "<html><body>";
        echo "<h1>Error al subir el archivo.</h1>";
        echo "<a href='download.html'>Volver</a>";
        echo "</body></html>";
    }
} else {
    echo "Content-Type: text/html\r\n\r\n";
    echo "<html><body>";
    echo "<h1>No se recibió ningún archivo.</h1>";
    echo "<a href='download.html'>Volver</a>";
    echo "</body></html>";
}
?>