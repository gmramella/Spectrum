#pragma once
#include <fstream>
#include <iostream>
#include "Utils.h"

class Wav
{
public:
	Wav();
	Wav(char* ChunkID, unsigned int ChunkSize, char* Format, char* Subchunk1ID, unsigned int Subchunk1Size, unsigned int AudioFormat, unsigned int NumChannels, unsigned int SampleRate, unsigned int ByteRate, unsigned int BlockAlign, unsigned int BitsPerSample, char* Subchunk2ID, unsigned int Subchunk2Size, unsigned int NumSamples, sample* IntData);
	~Wav();

	char* ChunkID;
	unsigned int ChunkSize;
	char* Format;
	char* Subchunk1ID;
	unsigned int Subchunk1Size;
	unsigned int AudioFormat;
	unsigned int NumChannels;
	unsigned int SampleRate;
	unsigned int ByteRate;
	unsigned int BlockAlign;
	unsigned int BitsPerSample;
	char* Subchunk2ID;
	unsigned int Subchunk2Size;
	unsigned int NumSamples;
	short int* IntData;

	void printHeader();
	void printData();
	void writeHeader();
	void writeData();
};

