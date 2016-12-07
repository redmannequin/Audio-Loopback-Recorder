#ifndef WAV_WRITER_H
#define WAV_WRITER_H

#include <fstream>
#include <mmdeviceapi.h>
#include <string>

class WavWriter {
	public:
		WavWriter(WAVEFORMATEX *fm);
		~WavWriter();

		void init(std::string path);
		void write(BYTE * data, int length);
		void close();

	private:
		// WAVE HEADER
		/*BYTE chunkID       [4];
		BYTE chunkSize     [4]; // 36 + SubChunk2Size
		BYTE format        [4];
		// FTM HEADER
		BYTE subchunk1ID   [4];
		BYTE subchunk1Size [4];
		BYTE audioFormat   [2];
		BYTE numChannels   [2];
		BYTE sampleRate    [4];
		BYTE byteRate      [4]; // SampleRate * NumChannels * BitsPerSample/8
		BYTE blockAlign    [2]; // NumChannels * BitsPerSample/8
		BYTE bitsPerSample [2];
		// DATA HEADER
		BYTE subchunk2ID   [4];
		BYTE subchunk2Size [4]; //NumSamples * NumChannels * BitsPerSample/8*/

		unsigned char chunkID[4];
		unsigned int chunkSize; // 36 + SubChunk2Size
		unsigned char format[4];
		// FTM HEADER
		unsigned char subchunk1ID[4];
		unsigned int subchunk1Size;
		unsigned char * fmt;

		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned int sampleRate;
		unsigned int byteRate; // SampleRate * NumChannels * BitsPerSample/8
		unsigned short blockAlign; // NumChannels * BitsPerSample/8
		unsigned short bitsPerSample;
		// DATA HEADER
		unsigned char subchunk2ID[4];
		unsigned int subchunk2Size; //NumSamples * NumChannels * BitsPerSample/8

		std::ofstream file;

		void intToByteAry(BYTE * ary, int i);
		void shortToByteAry(BYTE * ary, short i);

		WAVEFORMATEX * winFormat;
		int frames;
};

#endif