#pragma once
#include <regex>
#include <string>
#include <iostream>

class Argument {
protected:
	static int parse_int(std::regex check, std::string str) {
		std::cmatch m;
		if (!std::regex_match(str.c_str(), m, check)) {
			std::cerr << "wrong argument type" << std::endl;
			throw "wrong argument type";
		}
		std::string st = m[m.size() - 1];
		return std::stoi(st);
	}
public:
	~Argument() = default;
	virtual void parse(std::string) =0 ;
};

class Pointer : public Argument {
private:
	int limit_;
	int* value_;
public:
	Pointer(int* ptr, int limit) {
		value_ = ptr;
		limit_ = limit;
	}
	void parse(std::string str) {
		std::regex check("^\\$([\\d]+)$");
		*value_ = parse_int(check, str);

		if (*value_ > limit_ && *value_ < 1) {
			std::cerr << "limit exceeded" << std::endl;
			throw "limit exceeded";
		}
	}
};

class Integer : public Argument {
private:
	int limit_start_, limit_end_;
	int* value_;
public:
	Integer(int* ptr, int limit_start, int limit_end) {
		value_ = ptr;
		limit_start_ = limit_start;
		limit_end_ = limit_end;
	}
	void parse(std::string str) {
		std::regex check("^([\\d]+)$");

		*value_ = parse_int(check, str);
		if (*value_ > limit_end_ || *value_ < limit_start_) {
			std::cerr << "limit exceeded" << std::endl;
			throw "limit exceeded";
		}
	}
};