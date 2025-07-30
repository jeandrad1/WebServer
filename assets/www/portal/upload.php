<?php
header('Content-Type: text/html; charset=UTF-8');

$uploadDir = "file_library/";
if (!is_dir($uploadDir)) {
    mkdir($uploadDir, 0755, true);
}

$tmpFilePath = getenv('UPLOADED_FILE_PATH');

if ($tmpFilePath && file_exists($tmpFilePath)) {
    $raw_data = file_get_contents($tmpFilePath);
    
    $filename = "default_filename.dat";
    if (preg_match('/filename="([^"]+)"/', $raw_data, $matches)) {
        $filename = $matches[1];
    }

    $file_content_pos = strpos($raw_data, "\r\n\r\n");
    if ($file_content_pos !== false) {
        $file_content = substr($raw_data, $file_content_pos + 4);
        $end_boundary_pos = strrpos($file_content, "\r\n--");
        if ($end_boundary_pos !== false) {
            $file_content = substr($file_content, 0, $end_boundary_pos);
        }
    } else {
        $file_content = "";
    }

    $destino = $uploadDir . basename($filename);

    if (file_put_contents($destino, $file_content) !== false) {
        unlink($tmpFilePath);
        header("Location: portal.php");
        exit();
    } else {
        unlink($tmpFilePath);
        echo "Error al guardar el archivo final.";
    }
} else {
    echo "No se envió ningún archivo o hubo un error en el servidor.";
}
?>