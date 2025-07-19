<?php
$uploadDir = "file_library/";
if (!is_dir($uploadDir)) {
    mkdir($uploadDir, 0755, true);
}

if (isset($_FILES['archivo']) && $_FILES['archivo']['error'] === UPLOAD_ERR_OK) {
    $nombreTmp = $_FILES['archivo']['tmp_name'];
    $nombreArchivo = basename($_FILES['archivo']['name']);
    $destino = $uploadDir . $nombreArchivo;

    // Opcional: Validación de tipo o tamaño

    if (move_uploaded_file($nombreTmp, $destino)) {
        header("Location: index.php");
        exit();
    } else {
        echo "Error al subir el archivo.";
    }
} else {
    echo "No se envió ningún archivo.";
}

?>