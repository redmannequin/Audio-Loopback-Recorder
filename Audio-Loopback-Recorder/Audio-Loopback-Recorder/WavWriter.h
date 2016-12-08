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

		unsigned char chunkID[4];
		unsigned int chunkSize; // 36 + SubChunk2Size
		unsigned char format[4];
		// FTM HEADER
		unsigned char subchunk1ID[4];
		unsigned int subchunk1Size;
		unsigned char * fmt;
		unsigned short blockAlign;

		// DATA HEADER
		unsigned char subchunk2ID[4];
		unsigned int subchunk2Size; //NumSamples * NumChannels * BitsPerSample/8

		std::ofstream file;

};

#endif