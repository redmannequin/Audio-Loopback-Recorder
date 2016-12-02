#ifndef WAV_WRITER_H
#define WAV_WRITER_H

#include <fstream>
#include <mmdeviceapi.h>
#include <string>

class WavWriter {
	public:
		WavWriter(WAVEFORMATEX *fm);

		void init(std::string path);
		void write(BYTE * data);
		void close();

		~WavWriter();
	private:
		unsigned char  chunkID[4] = {'R','I','F','F'};
		unsigned int   chunkSize; // 36 + SubChunk2Size
		unsigned char  format[4] = {'W','A','V','E'};
		unsigned char  subchunk1ID[4] = {'f','m','t',' '};
		unsigned int   subchunk1Size = 16;
		unsigned short audioFormat = 1;
		unsigned short numChannels;
		unsigned int   sampleRate;
		unsigned int   byteRate; // SampleRate * NumChannels * BitsPerSample/8
		unsigned int   blockAlign; // NumChannels * BitsPerSample/8
		unsigned short bitsPerSample;
		unsigned char  subchunk2ID[4] = {'d','a','t','a'};
		unsigned int   subchunk2Size; //NumSamples * NumChannels * BitsPerSample/8
		unsigned int   numSamples;
		std::ofstream file;

		void populateHeader();
};

#endif