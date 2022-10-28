#include "Wav.h"
#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>

bool str_cmp(const char* s1, const char* s2) {
	for (int i = 0; i < 4; i++) {
		if (s1[i] != s2[i])
			return false;
	}
	return true;
}

void show_percent(int per) {
	std::cout << std::flush << "[";
	char full = -37;
	for (int p = 0; p <= per; p++)
		std::cout << full;
	for (int p = per + 1; p < 101; p++)
		std::cout << ' ';
	std::cout << "] " << per <<"%";
}

std::vector<short>* Wav::read()
{
	std::ifstream in(path_, std::ios::binary|std::ios::in);
	if (!in) {
		std::cerr << "no such file" << std::endl;
		throw "no such file";
	}
	WavHeader header;
	in.read((char*)&header.chunkId, 4);
	in.read((char*)&header.chunkSize, 4);
	in.read((char*)&header.format, 4);
	//in.read((char*)&header, sizeof(WavHeader));
	if (!str_cmp(header.chunkId, "RIFF") || !str_cmp(header.format, "WAVE")) {
		std::cerr << "Wrong file format" << std::endl;
		throw "Wrong file format";
	}
	char head[4];
	in.read(head, 4);
	while (!str_cmp(head, "data")) {
		if (str_cmp(head, "fmt ")) {
			WavFmt fmt;
			in.read((char*)&fmt, sizeof(WavFmt));
			if (fmt.audioFormat != 1) {
				std::cerr << "audio must be PCM" << std::endl;
				throw "audio must be PCM";
			}
			if (fmt.numChannels != 1) {
				std::cerr << "only 1 channel supported" << std::endl;
				throw "only 1 channel supported";
			}
			if (fmt.sampleRate != 44100 || fmt.byteRate != 88200) {
				std::cerr << "only 44100 sample rate supported" << std::endl;
				throw "only 44100 sample rate supported";
			}
			if (fmt.blockAlign != 2 || fmt.bitsPerSample != 16) {
				std::cerr << "sample must be signed short" << std::endl;
				throw "sample must be signed short";
			}
		}
		else if (str_cmp(head, "LIST")) {
			int block_size = 0;
			in.read((char*)&block_size, sizeof(int));
			in.ignore(block_size);
		}
		else {
			std::cerr << "unexpected subchank" << std::endl;
			throw "unexpected subchank";
		}
		in.read(head, 4);
	}
	int data_size = 0;
	in.read((char*)&data_size, sizeof(int));
	std::vector<short>* res = new std::vector<short>;
	std::cout << "Reading " << path_ << std::endl;
	int percent = data_size / 2 / 100;
	int showed = 0;
	for (int i = 0; i < data_size/2; i++) {
		if (in.eof()) {
			std::cerr << "file size less than expected" << std::endl;
			throw "file size less than expected";
		}
		short t;
		in.read((char*)&t, 2);
		res->push_back(t);
		if (i / percent >= showed + percent) {
			showed++;
			show_percent(showed);
		}
	}
	show_percent(100);
	std::cout << std::endl;
	return res;
}

Wav::Wav(std::string path)
{
	path_ = path;
	
}

void write_char(std::ofstream& out, char* s) {
	for (int i = 0; i < 4; i++)
		out << s[i];
}

void Wav::save(std::string path, std::vector<short>* data)
{
	int file_size = 4 + 4 + sizeof(WavFmt) + 4 + 4 + data->size();
	std::ofstream out(path, std::ios::out | std::ios::binary);
	char riff[4] = { 'R', 'I', 'F', 'F' };
	char wave[4] = { 'W', 'A', 'V', 'E' };
	char fmt[4] = { 'f', 'm', 't', ' ' };
	char data_c[4] = { 'd', 'a', 't', 'a' };
	out.write(riff, 4);
	out.write((char*) & file_size, sizeof(int));
	write_char(out, wave);
	write_char(out, fmt);
	out.write((char*) new WavFmt{ 16, 1, 1, 44100, 88200, 2, 16 }, sizeof(WavFmt));
	write_char(out, data_c);
	int s = data->size();
	out.write((char*)&s, 4);
	int percent = data->size() / 2 / 100;
	int showed = 0;
	for (int i = 0; i < data->size(); i++) {
		out.write((char*)&(data->at(i)), 2);
		if (i / percent >= showed + percent) {
			showed++;
			show_percent(showed);
		}
	}
}
