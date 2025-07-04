const params = new URLSearchParams(window.location.search);
const code = params.get('code') || window.errorCode ||'Error';
const descripcionError = params.get("descriptionerror") || window.errorMessage ||"Ha ocurrido un error.";

document.getElementById('codeError').textContent = "Error: " + code;
document.getElementById('descripcionError').textContent = descripcionError;