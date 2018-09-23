#include "Utils.h"

bool Utils::isBigEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}

int Utils::Utils::convertToInt(char* buffer, int len)
{
	int a = 0;
	if (!isBigEndian())
		for (int i = 0; i<len; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i<len; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}

std::string Utils::Utils::convertToString(char* buffer, int len)
{
	return std::string(buffer, len);
}

void Utils::fft(double* amplitudes, double* phases, sample* samples, unsigned int numSamples)
{
	//https://www.youtube.com/watch?v=CMyG4hsKCJo
	//https://www.youtube.com/watch?v=mkGsMWi_j4Q&index=11&list=PLT5_DQAJJLh-ogHjHcLtFYMQy7SkZ7-3i
	fftw_complex* x = NULL;
	fftw_complex* y = NULL;
	fftw_complex* z = NULL;
	x = new fftw_complex[numSamples];
	y = new fftw_complex[numSamples];
	z = new fftw_complex[numSamples / 2];

	for (unsigned int i = 0; i < numSamples; i++)
	{
		x[i][REAL] = samples[i];//sin(400 * i * 2 * 3.1415926535 / sampleRate);
		x[i][IMAG] = 0;
	}

	fftw_plan plan = fftw_plan_dft_1d(numSamples, x, y, FFTW_FORWARD, FFTW_ESTIMATE);//FFTW_FORWARD = -1, FFTW_ESTIMATE = (1U << 6)
	fftw_execute(plan);
	fftw_destroy_plan(plan);
	fftw_cleanup();

	for (unsigned int i = 0; i < numSamples / 2; i++)
	{
		//k(a + bi) = (ka + kbi)
		z[i][REAL] = 2 * y[i][REAL];
		z[i][IMAG] = 2 * y[i][IMAG];
	}

	for (unsigned int i = 0; i < numSamples / 2; i++)
	{
		//amplitude = r
		//phase = theta
		amplitudes[i] = sqrt(z[i][REAL] * z[i][REAL] + z[i][IMAG] * z[i][IMAG]) / numSamples;
		phases[i] = atan2(z[i][IMAG], z[i][REAL]);
	}

	delete[] x;
	x = NULL;
	delete[] y;
	y = NULL;
	delete[] z;
	z = NULL;
}

double Utils::db(double amplitude)
{
	return 20 * log10(amplitude);
}

void Utils::hamming(sample* window, sample* samples, unsigned int first, unsigned int size)
{
	for (int i = 0; i < size; i++) {
		double multiplier = 0.5 * (1 - cos(2 * 3.1415926535 * i / (size - 1)));
		window[i] = multiplier * samples[first + i];
	}
}
