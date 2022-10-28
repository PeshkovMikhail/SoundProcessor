#include "ArgParser.h"

ArgParser::ArgParser(std::list<std::string>* args)
{
	args_ = args;
}

ArgParser::~ArgParser()
{
}

void ArgParser::add_argument(Argument* arg)
{
	args_types_.push_back(arg);
}

void ArgParser::parse()
{
	if (args_types_.size() != args_->size()) {
		std::cout << "argument length conflict";
		throw "argument length conflict";
	}
	int arg_type_it = 0;
	for (auto iter = args_->begin(); iter != args_->end(); iter++) {
		Argument* arg = args_types_.at(arg_type_it);
		std::string str = *iter;
		arg->parse(str);
		++arg_type_it;
		delete arg;
	}
}
