const params = new URLSearchParams(window.location.search);
const code = params.get('code') || 'Error';
const descripcionError = params.get("descriptionerror") || "Ha ocurrido un error.";

document.getElementById('codeError').textContent = "Error: " + code;
document.getElementById('descripcionError').textContent = descripcionError;