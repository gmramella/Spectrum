#pragma once
#include "Utils.h"
#include "Wav.h"

class AudioManager
{
public:
	static Wav * AudioManager::loadWAV(const char* title);
};