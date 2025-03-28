#include "../../../../include/strategy/blocks/HttpBlockStrategy.hpp"
//#include "../../../../include/parser/config/Directive.hpp"

HttpBlockStrategy::HttpBlockStrategy()
{
	this->validateBlock = {	"client_max_body_size", "error_page"};
}

bool HttpBlockStrategy::validate(std::vector<AConfigBlock*> const &block) const
{
	std::vector<AConfigBlock*>::const_iterator itb = block.begin();
	for(std::vector<AConfigBlock*>::const_iterator ite = block.end(); itb != ite; itb++)
	{
		if (std::find(this->validateBlock.begin(), this->validateBlock.end(), (*itb)->getName()) == this->validateBlock.end())
			return false;
	}
	return true;
}

/* int main() {
    // Crear algunos bloques de configuración válidos e inválidos
    Directive valid1("client_max_body_size", "p");
    Directive valid2("error_page", "p");
    Directive invalid("server_name", "p");

    // Crear vectores de prueba
    std::vector<AConfigBlock*> validBlocks = { &valid1, &valid2 };   // Todos válidos
    std::vector<AConfigBlock*> invalidBlocks = { &valid1, &invalid }; // Uno inválido

    // Crear la estrategia de validación
    HttpBlockStrategy validator;

    // Probar validación
    std::cout << "Valid blocks: " << (validator.validate(validBlocks) ? "PASS" : "FAIL") << std::endl;
    std::cout << "Invalid blocks: " << (validator.validate(invalidBlocks) ? "PASS" : "FAIL") << std::endl;

    return 0;
} */