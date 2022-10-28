#include "Interpreter.h"
#include <fstream>
#include <sstream>
#include <list>
#include "Converter.h"

Interpreter::Interpreter(std::vector<Wav>* inputs, std::string output)
{
	inputs_ = inputs;
	output_ = output;
	current = inputs->at(0).read();
}

Interpreter::~Interpreter()
{
}

void Interpreter::process(std::string config_path)
{
	std::ifstream in(config_path);
	if (!in) {
		std::cerr << "config file doesn't exist" << std::endl;
		throw "config file doesn't exist";
	}
	while (!in.eof()) {
		std::string line;
		std::getline(in, line);
		std::stringstream ss(line);
		std::list<std::string>* args = new std::list<std::string>;
		std::string a;
		bool not_sharp = true;
		while (ss >> a && not_sharp) {
			args->push_back(a);
			not_sharp = args->front()[0] != '#';
		}
		if (not_sharp && !args->empty()) {
			std::string cmd = args->front();
			args->pop_front();
			Converter* conv = Converter::newConverter(cmd, inputs_);
			conv->action(args, current);
		}
		args->clear();
		delete args;
	}
	Wav::save(output_, current);
}


