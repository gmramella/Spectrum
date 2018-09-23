#pragma once
#include <string>
#include <chrono>
#include <iostream>
#include "fftw3.h"

#define REAL 0
#define IMAG 1

typedef short int sample;

class Utils
{
private:
	static bool isBigEndian();
public:
	static int Utils::convertToInt(char* buffer, int len);
	static std::string convertToString(char* buffer, int len);
	static void Utils::fft(double* amplitudes, double* phases, sample* window, unsigned int numSamples);
	static double Utils::db(double amplitude);
	static void Utils::hamming(sample* window, sample* samples, unsigned int first, unsigned int size);
};

//https://stackoverflow.com/questions/31487876/getting-a-time-difference-in-milliseconds
class Timer
{
public:
	void start()
	{
		initial = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
	}

	void reset()
	{
		initial = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
	}

	long long update()
	{
		return (std::chrono::system_clock::now() - initial).count();
	}
private:
	std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> initial;
};