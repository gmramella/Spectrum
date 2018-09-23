#include "Wav.h"

Wav::Wav()
{
}

Wav::~Wav()
{
	if (ChunkID) { delete[] ChunkID; ChunkID = NULL; }
	if (Format) { delete[] Format; Format = NULL; }
	if (Subchunk1ID) { delete[] Subchunk1ID; Subchunk1ID = NULL; }
	if (Subchunk2ID) { delete[] Subchunk2ID; Subchunk2ID = NULL; }
	if (IntData) { delete[] IntData; IntData = NULL; }
}

Wav::Wav(char* ChunkID, unsigned int ChunkSize, char* Format, char* Subchunk1ID, unsigned int Subchunk1Size, unsigned int AudioFormat, unsigned int NumChannels, unsigned int SampleRate, unsigned int ByteRate, unsigned int BlockAlign, unsigned int BitsPerSample, char* Subchunk2ID, unsigned int Subchunk2Size, unsigned int NumSamples, sample* IntData)
{
	this->ChunkID = ChunkID;
	this->ChunkSize = ChunkSize;
	this->Format = Format;
	this->Subchunk1ID = Subchunk1ID;
	this->Subchunk1Size = Subchunk1Size;
	this->AudioFormat = AudioFormat;
	this->NumChannels = NumChannels;
	this->SampleRate = SampleRate;
	this->ByteRate = ByteRate;
	this->BlockAlign = BlockAlign;
	this->BitsPerSample = BitsPerSample;
	this->Subchunk2ID = Subchunk2ID;
	this->Subchunk2Size = Subchunk2Size;
	this->NumSamples = NumSamples;
	this->IntData = IntData;
}

void Wav::printHeader()
{
	printf("ChunkID: %s\r\n", ChunkID);
	printf("ChunkSize: %d\r\n", ChunkSize);
	printf("Format: %s\r\n", Format);
	printf("Subchunk1ID: %s\r\n", Subchunk1ID);
	printf("Subchunk1Size: %d\r\n", Subchunk1Size);
	printf("AudioFormat: %d\r\n", AudioFormat);
	printf("NumChannels: %d\r\n", NumChannels);
	printf("SampleRate: %d\r\n", SampleRate);
	printf("ByteRate: %d\r\n", ByteRate);
	printf("BlockAlign: %d\r\n", BlockAlign);
	printf("BitsPerSample: %d\r\n", BitsPerSample);
	printf("Subchunk2ID: %s\r\n", Subchunk2ID);
	printf("Subchunk2Size: %d\r\n", Subchunk2Size);
	printf("NumSamples: %d\r\n", NumSamples);
}
void Wav::printData()
{
	for (int i = 0; i < NumSamples * NumChannels; i++)
	{
		std::cout << IntData[i] << std::endl;
	}
}
void Wav::writeHeader()
{
	std::ofstream headerFile("./Files/header.txt");
	headerFile << "ChunkID: " << ChunkID << std::endl;
	headerFile << "ChunkSize: " << ChunkSize << std::endl;
	headerFile << "Format: " << Format << std::endl;
	headerFile << "Subchunk1ID: " << Subchunk1ID << std::endl;
	headerFile << "Subchunk1Size: " << Subchunk1Size << std::endl;
	headerFile << "AudioFormat: " << AudioFormat << std::endl;
	headerFile << "NumChannels: " << NumChannels << std::endl;
	headerFile << "SampleRate: " << SampleRate << std::endl;
	headerFile << "ByteRate: " << ByteRate << std::endl;
	headerFile << "BlockAlign: " << BlockAlign << std::endl;
	headerFile << "BitsPerSample: " << BitsPerSample << std::endl;
	headerFile << "Subchunk2ID: " << Subchunk2ID << std::endl;
	headerFile << "Subchunk2Size: " << Subchunk2Size << std::endl;
	headerFile << "NumSamples: " << NumSamples << std::endl;
	headerFile.close();
}
void Wav::writeData()
{
	std::ofstream samplesFile("./Files/data.txt");
	for (int i = 0; i < NumSamples * NumChannels; i++)
	{
		samplesFile << IntData[i] << std::endl;
	}
	samplesFile.close();
}