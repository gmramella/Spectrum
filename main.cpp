#pragma once
#include "libUnicornio.h"
#include "fftw3.h"
#include "AudioManager.h"
#include "Utils.h"
#include "Wav.h"

#define LOOP false				//audio plays nonstop
#define DEBUG_ON true			//shows debug window
#define RENDER_ON true			//draws samples and amplitudes
#define POINT_OR_LINE true		//draws samples and amplitudes as points or lines (false = point, true = line)

typedef short int sample;

Cor white(255, 255, 255, 255);
Cor gray(127, 127, 127, 255);
Cor black(0, 0, 0, 255);
Cor red(255, 0, 0, 255);
Cor green(0, 255, 0, 255);
Cor blue(0, 0, 255, 255);
Cor yellow(255, 255, 0, 255);
Cor magenta(255, 0, 255, 255);
Cor cyan(0, 255, 255, 255);

int main(int argc, char* argv[])
{
	uniInicializar(800, 600, false);
	
	gRecursos.carregarFonte("font1", "./fonte_padrao.ttf", 32);
	gRecursos.carregarFonte("font2", "./fonte_padrao.ttf", 8);
	Texto time;
	time.setFonte("font1");
	time.setCor(blue);
	time.setAlinhamento(TEXTO_CENTRALIZADO);
	
	unsigned short scaleStep = 32;
	Texto* scale;
	scale = new Texto[gJanela.getLargura() / scaleStep];
	for (unsigned int i = 0; i < gJanela.getLargura() / scaleStep; i++)
	{
		scale[i].setFonte("font2");
		scale[i].setCor(blue);
		scale[i].setAlinhamento(TEXTO_CENTRALIZADO);
	}

	const char* path = "./Musics/Who Are You.wav";
	std::string title(path);
	title = title.substr(title.find_last_of("/\\") + 1);
	gRecursos.carregarMusica("audio1", path);
	Wav* wav = AudioManager::loadWAV(path);
	
	if (wav == NULL)
	{
		gRecursos.descarregarTudo();
		uniFinalizar();
		return EXIT_FAILURE;
	}
	wav->printHeader();
	/*wav->writeHeader();
	wav->writeSamples();*/

	sample* samplesMono = NULL;
	sample* samplesLeft = NULL;
	sample* samplesRight = NULL;
	double* amplitudesMono = NULL;
	double* amplitudesLeft = NULL;
	double* amplitudesRight = NULL;
	double* phasesMono = NULL;
	double* phasesLeft = NULL;
	double* phasesRight = NULL;
	double highestAmplitudeMono = 0;
	double highestAmplitudeLeft = 0;
	double highestAmplitudeRight = 0;
	unsigned int chunkMono = (int)pow(2, 10 + 6 * !RENDER_ON);//fps increases if it does not need to render
	unsigned int chunkStereo = chunkMono / 2;
	sample* windowMono = new sample[chunkMono];
	sample* windowLeft = new sample[chunkStereo];
	sample* windowRight = new sample[chunkStereo];
	unsigned int numBinsMono = chunkMono / 2;
	float binWidthMono = 1.0 * wav->SampleRate / 2 / numBinsMono;
	unsigned int numBinsStereo = chunkStereo / 2;
	float binWidthStereo = 1.0 * wav->SampleRate / 2 / numBinsStereo;
	unsigned int frameSize;

	if (wav->NumChannels == 1)
	{
		cout << "NumBins " << numBinsMono << endl;
		cout << "BinWidth " << binWidthMono << endl;
		samplesMono = new sample[wav->NumSamples];
		amplitudesMono = new double[chunkMono / 2];
		phasesMono = new double[chunkMono / 2];

		for (int i = 0; i < wav->NumSamples; i++)
		{
			samplesMono[i] = wav->IntData[i];
		}
	}
	else
	{
		cout << "NumBins " << numBinsStereo << endl;
		cout << "BinWidth " << binWidthStereo << endl;
		samplesLeft = new sample[wav->NumSamples];
		samplesRight = new sample[wav->NumSamples];
		amplitudesLeft = new double[chunkStereo / 2];
		amplitudesRight = new double[chunkStereo / 2];
		phasesLeft = new double[chunkStereo / 2];
		phasesRight = new double[chunkStereo / 2];

		for (int i = 0; i < 2 * wav->NumSamples; i++)
		{
			if (i % 2 == 0)
			{
				samplesLeft[i / 2] = wav->IntData[i];
			}
			else
			{
				samplesRight[i / 2] = wav->IntData[i];
			}
		}
	}

	float max = (float)pow(2, wav->BitsPerSample - 1);
	Timer timer;
	timer.start();
	gMusica.tocar("audio1", false);
	
	Cor samplesColor = gray;
	Cor amplitudesColor = cyan;
	Cor centerColor = red;
	while (!gTeclado.soltou[TECLA_ESC] && !gEventos.sair)
	{
		uniIniciarFrame();
		double seconds = timer.update() / 1e9;
		
		int frameFirst = (int)round(wav->SampleRate * seconds);
		if (frameFirst <= wav->NumSamples)
		{
			if (wav->NumChannels == 1)
			{
				if (frameFirst <= wav->NumSamples - chunkMono)
				{
					frameSize = chunkMono;
				}
				else
				{
					frameSize = wav->NumSamples - frameFirst;
				}
				Utils::hamming(windowMono, samplesMono, frameFirst, frameSize);
				Utils::fft(amplitudesMono, phasesMono, windowMono, frameSize);

				if (RENDER_ON)
				{
					//samples
					for (unsigned int i = 0; i < frameSize; i++)
					{
						int x1 = i;
						int x2 = x1;
						int y1 = 0.83 * gJanela.getAltura();
						int y2 = y1 - (wav->NumSamples >= (frameFirst + wav->NumChannels * i) ? (gJanela.getAltura() / 8) * samplesMono[frameFirst + wav->NumChannels * i] / max : 0);
						if (POINT_OR_LINE)
						{
							gGraficos.desenharLinha(x1, y1, x2, y2, samplesColor.r, samplesColor.g, samplesColor.b, samplesColor.a);
						}
						else
						{
							gGraficos.desenharPixel(x1, y1, samplesColor.r, samplesColor.g, samplesColor.b, samplesColor.a);
							gGraficos.desenharPixel(x2, y2, samplesColor.r, samplesColor.g, samplesColor.b, samplesColor.a);
						}
					}

					//amplitudes
					highestAmplitudeMono = amplitudesMono[2];
					for (unsigned int i = 3; i < frameSize / 2; i++) {
						if (highestAmplitudeMono < amplitudesMono[i]) {
							highestAmplitudeMono = amplitudesMono[i];
						}
					}
					if (highestAmplitudeMono == 0) highestAmplitudeMono = 1;
					for (unsigned int i = 2; i < frameSize / 2; i++)
					{
						int x3 = i;
						int x4 = x3;
						int y3 = 0.98 * gJanela.getAltura();
						int y4 = y3 - (gJanela.getAltura() / 8) * amplitudesMono[i] / highestAmplitudeMono;
						if (POINT_OR_LINE)
						{
							gGraficos.desenharLinha(x3, y3, x4, y4, amplitudesColor.r, amplitudesColor.g, amplitudesColor.b, amplitudesColor.a);
						}
						else
						{
							gGraficos.desenharPixel(x3, y3, amplitudesColor.r, amplitudesColor.g, amplitudesColor.b, amplitudesColor.a);
							gGraficos.desenharPixel(x4, y4, amplitudesColor.r, amplitudesColor.g, amplitudesColor.b, amplitudesColor.a);
						}
					}

					//middle red lines
					gGraficos.desenharLinha(0, 0.83 * gJanela.getAltura(), gJanela.getLargura(), 0.83 * gJanela.getAltura(), centerColor.r, centerColor.g, centerColor.b, centerColor.a);
					gGraficos.desenharLinha(0, 0.98 * gJanela.getAltura(), gJanela.getLargura(), 0.98 * gJanela.getAltura(), centerColor.r, centerColor.g, centerColor.b, centerColor.a);

					for (unsigned int i = 0; i < gJanela.getLargura() / scaleStep; i++)
					{
						scale[i].setString(to_string(binWidthMono * scaleStep * i));
						scale[i].desenhar(scaleStep * i, 0.99 * gJanela.getAltura());
					}
				}
			}
			else
			{
				if (frameFirst <= wav->NumSamples - chunkStereo)
				{
					frameSize = chunkStereo;
				}
				else
				{
					frameSize = wav->NumSamples - frameFirst;
				}
				Utils::hamming(windowLeft, samplesLeft, frameFirst, frameSize);
				Utils::hamming(windowRight, samplesRight, frameFirst, frameSize);
				Utils::fft(amplitudesLeft, phasesLeft, windowLeft, frameSize);
				Utils::fft(amplitudesRight, phasesRight, windowRight, frameSize);

				if (RENDER_ON)
				{
					//samples
					for (unsigned int i = 0; i < frameSize; i++)
					{
						int x1 = i;
						int x2 = x1;
						int y1 = 0.30 * gJanela.getAltura();
						int y2 = y1 - (wav->NumSamples >= (frameFirst + wav->NumChannels * i) ? (gJanela.getAltura() / 8) * samplesLeft[frameFirst + wav->NumChannels * i] / max : 0);
						if (POINT_OR_LINE)
						{
							gGraficos.desenharLinha(x1, y1, x2, y2, samplesColor.r, samplesColor.g, samplesColor.b, samplesColor.a);
						}
						else
						{
							gGraficos.desenharPixel(x1, y1, samplesColor.r, samplesColor.g, samplesColor.b, samplesColor.a);
							gGraficos.desenharPixel(x2, y2, samplesColor.r, samplesColor.g, samplesColor.b, samplesColor.a);
						}

						int x3 = i;
						int x4 = x3;
						int y3 = 0.60 * gJanela.getAltura();
						int y4 = y3 - (wav->NumSamples >= (frameFirst + wav->NumChannels * i) ? (gJanela.getAltura() / 8) * samplesRight[frameFirst + wav->NumChannels * i] / max : 0);
						if (POINT_OR_LINE)
						{
							gGraficos.desenharLinha(x3, y3, x4, y4, samplesColor.r, samplesColor.g, samplesColor.b, samplesColor.a);
						}
						else
						{
							gGraficos.desenharPixel(x3, y3, samplesColor.r, samplesColor.g, samplesColor.b, samplesColor.a);
							gGraficos.desenharPixel(x4, y4, samplesColor.r, samplesColor.g, samplesColor.b, samplesColor.a);
						}
					}

					//amplitudes
					highestAmplitudeLeft = amplitudesLeft[2];
					highestAmplitudeRight = amplitudesRight[2];
					for (unsigned int i = 3; i < frameSize / 2; i++) {
						if (highestAmplitudeLeft < amplitudesLeft[i]) {
							highestAmplitudeLeft = amplitudesLeft[i];
						}
						if (highestAmplitudeRight < amplitudesRight[i]) {
							highestAmplitudeRight = amplitudesRight[i];
						}
					}
					if (highestAmplitudeLeft == 0) highestAmplitudeLeft = 1;
					if (highestAmplitudeRight == 0) highestAmplitudeRight = 1;
					for (unsigned int i = 2; i < frameSize / 2; i++)
					{
						int x1 = i;
						int x2 = x1;
						int y1 = 0.83 * gJanela.getAltura();
						int y2 = y1 - (gJanela.getAltura() / 8) * amplitudesLeft[i] / highestAmplitudeLeft;
						if (POINT_OR_LINE)
						{
							gGraficos.desenharLinha(x1, y1, x2, y2, amplitudesColor.r, amplitudesColor.g, amplitudesColor.b, amplitudesColor.a);
						}
						else
						{
							gGraficos.desenharPixel(x1, y1, amplitudesColor.r, amplitudesColor.g, amplitudesColor.b, amplitudesColor.a);
							gGraficos.desenharPixel(x2, y2, amplitudesColor.r, amplitudesColor.g, amplitudesColor.b, amplitudesColor.a);
						}

						int x3 = i;
						int x4 = x3;
						int y3 = 0.98 * gJanela.getAltura();
						int y4 = y3 - (gJanela.getAltura() / 8) * amplitudesRight[i] / highestAmplitudeRight;
						if (POINT_OR_LINE)
						{
							gGraficos.desenharLinha(x3, y3, x4, y4, amplitudesColor.r, amplitudesColor.g, amplitudesColor.b, amplitudesColor.a);
						}
						else
						{
							gGraficos.desenharPixel(x3, y3, amplitudesColor.r, amplitudesColor.g, amplitudesColor.b, amplitudesColor.a);
							gGraficos.desenharPixel(x4, y4, amplitudesColor.r, amplitudesColor.g, amplitudesColor.b, amplitudesColor.a);
						}
					}

					//middle red lines
					gGraficos.desenharLinha(0, 0.30 * gJanela.getAltura(), gJanela.getLargura(), 0.30 * gJanela.getAltura(), centerColor.r, centerColor.g, centerColor.b, centerColor.a);
					gGraficos.desenharLinha(0, 0.60 * gJanela.getAltura(), gJanela.getLargura(), 0.60 * gJanela.getAltura(), centerColor.r, centerColor.g, centerColor.b, centerColor.a);
					gGraficos.desenharLinha(0, 0.83 * gJanela.getAltura(), gJanela.getLargura(), 0.83 * gJanela.getAltura(), centerColor.r, centerColor.g, centerColor.b, centerColor.a);
					gGraficos.desenharLinha(0, 0.98 * gJanela.getAltura(), gJanela.getLargura(), 0.98 * gJanela.getAltura(), centerColor.r, centerColor.g, centerColor.b, centerColor.a);

					for (unsigned int i = 0; i < gJanela.getLargura() / scaleStep; i++)
					{
						scale[i].setString(to_string((int)binWidthStereo * scaleStep * i));
						scale[i].desenhar(scaleStep * i, 0.99 * gJanela.getAltura());
					}
				}
			}
		}
		
		time.setString(to_string(seconds));
		time.desenhar(gJanela.getLargura() / 2, gJanela.getAltura() / 2);

		if (!gMusica.estaTocando())
		{
			highestAmplitudeMono = 0;
			highestAmplitudeLeft = 0;
			highestAmplitudeRight = 0;
			//restart audio and timer
			if (LOOP && !gMusica.estaTocando())
			{
				gMusica.tocar("audio1", false);
				timer.reset();
			}
		}

		//WAV file parsing information
		if (DEBUG_ON)
		{
			gDebug.depurar("Title", title);
			gDebug.depurar("Chunk ID", wav->ChunkID);
			gDebug.depurar("Chunk Size", wav->ChunkSize);
			gDebug.depurar("Format", wav->Format);
			gDebug.depurar("Subchunk1 ID", wav->Subchunk1ID);
			gDebug.depurar("Subchunk1 Size", wav->Subchunk1Size);
			gDebug.depurar("Audio Format", wav->AudioFormat);
			gDebug.depurar("Num Channels", wav->NumChannels);
			gDebug.depurar("Sample Rate", wav->SampleRate);
			gDebug.depurar("Byte Rate", wav->ByteRate);
			gDebug.depurar("Block Align", wav->BlockAlign);
			gDebug.depurar("Bits Per Sample", wav->BitsPerSample);
			gDebug.depurar("Subchunk2 ID", wav->Subchunk2ID);
			gDebug.depurar("Subchunk2 Size", wav->Subchunk2Size);
			gDebug.depurar("Num Samples Per Channel", wav->NumSamples);
		}

		//FFT information
		if (DEBUG_ON)
		{
			gDebug.depurar("Highest Amplitude Left", highestAmplitudeLeft);
			gDebug.depurar("Highest Amplitude Right", highestAmplitudeRight);
			gDebug.depurar("Highest Amplitude Mono", highestAmplitudeMono);
			gDebug.depurar("Chunk Mono", chunkMono);
			gDebug.depurar("Chunk Stereo", chunkStereo);
			gDebug.depurar("Num Bins Mono", numBinsMono);
			gDebug.depurar("Bin Width Mono", binWidthMono);
			gDebug.depurar("Num Bins Stereo", numBinsStereo);
			gDebug.depurar("Bin Width Stereo", binWidthStereo);
			gDebug.depurar("Frame Size", frameSize);
		}

		//Rendering information
		if (DEBUG_ON)
		{
			gDebug.depurar("dt", gTempo.getDeltaTempo());
			gDebug.depurar("fps", gTempo.getFPS());
		}

		uniTerminarFrame();
	}

	if (samplesMono != NULL) delete[] samplesMono;
	if (samplesLeft != NULL) delete[] samplesLeft;
	if (samplesRight != NULL) delete[] samplesRight;
	if (amplitudesMono != NULL) delete[] amplitudesMono;
	if (amplitudesLeft != NULL) delete[] amplitudesLeft;
	if (amplitudesRight != NULL) delete[] amplitudesRight;
	if (phasesMono != NULL) delete[] phasesMono;
	if (phasesLeft != NULL) delete[] phasesLeft;
	if (phasesRight != NULL) delete[] phasesRight;
	highestAmplitudeMono = 0;
	highestAmplitudeLeft = 0;
	highestAmplitudeRight = 0;
	chunkMono = 0;
	chunkStereo = 0;
	if (windowMono != NULL) delete[] windowMono;
	if (windowLeft != NULL) delete[] windowLeft;
	if (windowRight != NULL) delete[] windowRight;
	numBinsMono = 0;
	binWidthMono = 0;
	numBinsStereo = 0;
	binWidthStereo = 0;
	frameSize = 0;

	delete[] scale;
	gRecursos.descarregarTudo();
	uniFinalizar();

	return 0;
}