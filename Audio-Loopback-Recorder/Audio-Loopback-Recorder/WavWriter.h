#ifndef WAV_WRITER_H
#define WAV_WRITER_H

#include <fstream>
#include <mmdeviceapi.h>
#include <string>

class WavWriter {
	public:
		WavWriter(WAVEFORMATEX *fm);

		void init(std::string path);
		void write(BYTE * data, int length);
		void close();

		~WavWriter();
	private:
		// WAVE HEADER
		BYTE chunkID       [4];
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
		BYTE subchunk2Size [4]; //NumSamples * NumChannels * BitsPerSample/8
		std::ofstream file;

		void intToByteAry(BYTE * ary, int i);
		void shortToByteAry(BYTE * ary, short i);

		WAVEFORMATEX * winFormat;
		int frames;
};

#endif