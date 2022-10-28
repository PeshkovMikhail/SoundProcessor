#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Argument.h"
#include <list>

class ArgParser
{
private:
	std::vector<Argument*> args_types_;
	std::list<std::string>* args_;
public:
	ArgParser(std::list<std::string>* args);
	~ArgParser();
	void add_argument(Argument* arg);
	void parse();
};

