#pragma once
#include "Wav.h"
#include <string>
#include <vector>
#include <list>
#include "ArgParser.h"
#include <fstream>

#define FREQ 44100

class Converter
{
protected:
	void show_percent(int per) {
		std::cout << std::flush << "[";
		char full = -37;
		for (int p = 0; p <= per; p++)
			std::cout << full;
		for (int p = per + 1; p < 101; p++)
			std::cout << ' ';
		std::cout << "] " << per << "%";
	}
	std::vector<Wav>* inputs_;
	void printHelp(std::string path) {
		std::ifstream in(path);
		std::string contents;
		in >> contents;
		std::cout << contents;
	}
	short get_vector(std::vector<short>* vec, size_t pos) {
		if (pos >= vec->size())
			return 0;
		return vec->at(pos);
	}
	void set_vector(std::vector<short>* vec, size_t pos, short val) {
		if (pos >= vec->size())
			vec->push_back(val);
		else
			vec->at(pos) = val;
	}
public:
	virtual void action(std::list<std::string>* args, std::vector<short>* out) = 0;
	virtual void help() = 0;
	virtual ~Converter() {};
	static Converter* newConverter(std::string name, std::vector<Wav>* input);
};

class Mix : public Converter
{
public:
	Mix(std::vector<Wav>* inputs) {
		inputs_ = inputs;
	}
	void action(std::list<std::string>* args, std::vector<short>* out) {
		int stream, start;
		ArgParser parser(args);
		parser.add_argument(new Pointer(&stream, inputs_->size()));
		parser.add_argument(new Integer(&start, 0, INT_MAX));
		parser.parse();
		std::vector<short>* input_stream = inputs_->at(stream-1).read();
		size_t size = std::max(input_stream->size(), out->size());
		if (start*FREQ >= size) {
			std::cerr << "start is greater than stream sizes" << std::endl;
			throw "start is greater than stream sizes";
		}
		out->reserve(size);
		std::cout << "Processing mix converter from " << start <<" minute" << std::endl;
		int showed = 0;
		int percent = size / 100;
		for (size_t it = start * FREQ; it < size; it++) {
			set_vector(out, it, (get_vector(out, it) + get_vector(input_stream, it)) / 2);
			if (it / percent >= showed + percent) {
				showed++;
				show_percent(showed);
			}
		}
		show_percent(100);
		std::cout << std::endl;
		input_stream->clear();
		delete input_stream;
	}
	void help() {
		printHelp("configs/mix.txt");
	}
};

class Mute : public Converter
{
public:
	void action(std::list<std::string>* args, std::vector<short>* out) {
		if (args->size() != 2) {
			std::cerr << "wrong args" << std::endl;
			throw "wrong args";
		}
		int start, end;
		ArgParser parser(args);
		parser.add_argument(new Integer(&start, 0, out->size() / FREQ));
		//parser.add_argument(new Integer(&end, start, out->size()/FREQ));
		parser.add_argument(new Integer(&end, 0, out->size() / FREQ));
		parser.parse();
		
		std::fill(out->begin() + start * FREQ, out->begin() + end * FREQ, 0);
	}
	void help() {
		printHelp("config/mute.txt");
	}
};

class Reverse : public Converter
{
public:
	void action(std::list<std::string>* args, std::vector<short>* out) {
		if (args->size() != 2) {
			std::cerr << "wrong args" << std::endl;
			throw "wrong args";
		}
		int start, end;
		ArgParser parser(args);
		parser.add_argument(new Integer(&start, 0, out->size() / FREQ));
		parser.add_argument(new Integer(&end, start, out->size() / FREQ));
		parser.parse();
		size_t len = (end - start) * FREQ / 2;
		int showed = 0;
		int percent = len / 100;
		for (size_t it = 0; it <= (end-start)*FREQ / 2; it++) {
			std::swap(out->at(start*FREQ + it), out->at(end*FREQ - it));
			if (it / percent >= showed + percent) {
				showed++;
				show_percent(showed);
			}
		}
		show_percent(100);
		std::cout << std::endl;
	}

	void help() {
		printHelp("configs/reverse.txt");
	}
};