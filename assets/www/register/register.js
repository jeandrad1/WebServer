
window.addEventListener('DOMContentLoaded', function() 
{
	const password = document.getElementById('password');
	const confirmPassword = document.getElementById('confirm_password');
	const registerBtn = document.querySelector('button[type="submit"]');

	let errorMsg = document.getElementById('passwordError');
	if (!errorMsg) {
		errorMsg = document.createElement('p');
		errorMsg.id = 'passwordError';
		errorMsg.style.color = 'red';
		errorMsg.style.display = 'none';
		registerBtn.parentNode.insertBefore(errorMsg, registerBtn.nextSibling);
	}

	function validatePasswords() {
		if (password.value && confirmPassword.value && password.value === confirmPassword.value) {
			registerBtn.disabled = false;
			errorMsg.style.display = 'none';
		}
		else {
			registerBtn.disabled = true;
			if (password.value && confirmPassword.value) {
				errorMsg.textContent = 'Las contraseñas no coinciden.';
				errorMsg.style.display = 'block';
			}
			else {
				errorMsg.style.display = 'none';
			}
		}
	}

	registerBtn.disabled = true;
	password.addEventListener('input', validatePasswords);
	confirmPassword.addEventListener('input', validatePasswords);
});