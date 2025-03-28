#include "../../../../include/strategy/blocks/LocationBlockStrategy.hpp"
//#include "../../../../include/parser/config/Directive.hpp"

LocationBlockStrategy::LocationBlockStrategy()
{
	this->validateBlock = {	"root", "index", \
		"client_max_body_size", "autoindex", \
		"error_page", "return"};
}

bool LocationBlockStrategy::validate(std::vector<AConfigBlock*> const &block) const
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
    Directive valid1("root", "p");
    Directive valid2("index", "p");
    Directive invalid("alohomora", "p");

    // Crear vectores de prueba
    std::vector<AConfigBlock*> validBlocks = { &valid1, &valid2 };   // Todos válidos
    std::vector<AConfigBlock*> invalidBlocks = { &valid1, &invalid }; // Uno inválido

    // Crear la estrategia de validación
    LocationBlockStrategy validator;

    // Probar validación
    std::cout << "Valid blocks: " << (validator.validate(validBlocks) ? "PASS" : "FAIL") << std::endl;
    std::cout << "Invalid blocks: " << (validator.validate(invalidBlocks) ? "PASS" : "FAIL") << std::endl;

    return 0;
} */