#pragma once
#include <vector>
#include <string>

typedef struct {
	char chunkId[4];
	unsigned int chunkSize;
	char format[4];
} WavHeader;

typedef struct {
	unsigned int subchankSize;
	short audioFormat;
	short numChannels;
	int sampleRate;
	int byteRate;
	short blockAlign;
	short bitsPerSample;
} WavFmt;


class Wav
{
private:
	std::string path_;
public:
	std::vector<short>* read();
	Wav(std::string path);
	static void save(std::string path, std::vector<short>* data);
};

