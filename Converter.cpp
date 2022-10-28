#include "Converter.h"
#include <iostream>

Converter* Converter::newConverter(std::string name, std::vector<Wav>* input) {
	if (name == "mix")
		return new Mix(input);
	else if (name == "mute")
		return new Mute();
	else if (name == "reverse")
		return new Reverse();
	else {
		std::cout << "unexpected function" << std::endl;
		throw std::exception("unexpected function");
	}
	return nullptr;
}