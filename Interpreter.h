#pragma once
#include "Wav.h"
class Interpreter
{
private:
	std::vector<Wav>* inputs_;
	std::string output_;
	std::vector<short>* current;
public:
	Interpreter(std::vector<Wav>* inputs, std::string output);
	~Interpreter();
	void process(std::string config_path);
};

