#include "AudioManager.h"

Wav* AudioManager::loadWAV(const char* title)
{
	const char* ChunkID;
	int ChunkSize;
	const char* Format;
	const char* Subchunk1ID;
	int Subchunk1Size;
	int AudioFormat;
	int NumChannels;
	int SampleRate;
	int ByteRate;
	int BlockAlign;
	int BitsPerSample;
	const char* Subchunk2ID;
	int Subchunk2Size;
	int NumSamples;
	sample* IntData;

	char buffer[4];
	std::ifstream in(title, std::ios::binary);
	if (!in.is_open())
	{
		std::cout << "Title not found: " << title << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	ChunkID = _strdup(Utils::convertToString(buffer, 4).c_str());
	if (strncmp(ChunkID, "RIFF", 4) != 0)
	{
		std::cout << "Invalid ChunkID: " << ChunkID << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	ChunkSize = Utils::convertToInt(buffer, 4);
	if (ChunkSize < 8)
	{
		std::cout << "Invalid ChunkSize: " << ChunkSize << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	Format = _strdup(Utils::convertToString(buffer, 4).c_str());
	if (strncmp(Format, "WAVE", 4) != 0)
	{
		std::cout << "Invalid Format: " << Format << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	Subchunk1ID = _strdup(Utils::convertToString(buffer, 4).c_str());
	if (strncmp(Subchunk1ID, "fmt ", 4) != 0)
	{
		std::cout << "Invalid Subchunk1ID: " << Subchunk1ID << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	Subchunk1Size = Utils::convertToInt(buffer, 4);
	if (Subchunk1Size != 16)
	{
		std::cout << "Invalid Subchunk1Size: " << Subchunk1Size << std::endl;
		return NULL;
	}
	in.read(buffer, 2);
	AudioFormat = Utils::convertToInt(buffer, 2);
	if (AudioFormat != 1)
	{
		std::cout << "Invalid AudioFormat: " << AudioFormat << std::endl;
		return NULL;
	}
	in.read(buffer, 2);
	NumChannels = Utils::convertToInt(buffer, 2);
	if (NumChannels != 1 && NumChannels != 2)
	{
		std::cout << "Invalid NumChannels: " << NumChannels << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	SampleRate = Utils::convertToInt(buffer, 4);
	if (SampleRate <= 0)
	{
		std::cout << "Invalid SampleRate: " << SampleRate << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	ByteRate = Utils::convertToInt(buffer, 4);
	if (ByteRate < SampleRate * NumChannels)
	{
		std::cout << "Invalid ByteRate: " << ByteRate << std::endl;
		return NULL;
	}
	in.read(buffer, 2);
	BlockAlign = Utils::convertToInt(buffer, 2);
	if (BlockAlign < NumChannels)
	{
		std::cout << "Invalid BlockAlign: " << BlockAlign << std::endl;
		return NULL;
	}
	in.read(buffer, 2);
	BitsPerSample = Utils::convertToInt(buffer, 2);
	if (BitsPerSample / 8 > 2)
	{
		std::cout << "Invalid BitsPerSample: " << BitsPerSample << std::endl;
		return NULL;
	}
	in.read(buffer, 4);
	Subchunk2ID = _strdup(Utils::convertToString(buffer, 4).c_str());
	if (strncmp(Subchunk2ID, "data", 4) != 0)
	{
		std::cout << "Invalid Subchunk2ID: " << Subchunk2ID << std::endl;
		return NULL;
	}
	in.read(buffer, 4);//NumSamples * NumChannels * BitsPerSample/8
	Subchunk2Size = Utils::convertToInt(buffer, 4);
	if (Subchunk2Size < NumChannels * BitsPerSample / 8)
	{
		std::cout << "Invalid Subchunk2Size: " << Subchunk2Size << std::endl;
		return NULL;
	}
	NumSamples = 8 * Subchunk2Size / NumChannels / BitsPerSample;

	if (BitsPerSample == 8)
	{
		IntData = new sample[Subchunk2Size];
		uint8_t data;
		for (size_t i = 0; i < Subchunk2Size; i++)
		{
			in.read(reinterpret_cast<char*>(&data), 1);
			IntData[i] = (sample)data;
		}
	}
	else
	{
		IntData = new sample[Subchunk2Size / 2];
		uint16_t data;
		for (size_t i = 0; i < Subchunk2Size / 2; i++)
		{
			in.read(reinterpret_cast<char*>(&data), 2);
			IntData[i] = (sample)0x8000 & data ? (int)(0x7FFF & data) - 0x8000 : data;
		}
	}
	in.close();

	return new Wav(_strdup(ChunkID), ChunkSize, _strdup(Format), _strdup(Subchunk1ID), Subchunk1Size, AudioFormat, NumChannels, SampleRate, ByteRate, BlockAlign, BitsPerSample, _strdup(Subchunk2ID), Subchunk2Size, NumSamples, IntData);
}