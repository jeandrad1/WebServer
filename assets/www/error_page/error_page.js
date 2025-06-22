const params = new URLSearchParams(window.location.search);
const code = params.get('code') || 'Error';
const descripcionError = params.get("description_error") || "Ha ocurrido un error.";

document.getElementById('codigoError').textContent = "Error: " + code;
document.getElementById('descripcionError').textContent = descripcionError;