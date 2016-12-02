#include "stdafx.h"
#include "WavWriter.h"


WavWriter::WavWriter(WAVEFORMATEX *fm) {
	numChannels = fm->nChannels;
	sampleRate = fm->nSamplesPerSec;
	byteRate = sampleRate * numChannels * (fm->wBitsPerSample/8);
	blockAlign = numChannels * (fm->wBitsPerSample / 8);
	bitsPerSample = fm->wBitsPerSample;
	//subchunk2Size = numSamples * numChannels * (bitsPerSample / 8);
	numSamples = 0;
}

void WavWriter::init(std::string path) {
	file.open(path + "temp.wav");
	BYTE header[44];
	for (int i = 0; i < 44; ++i) header[i] = 0;
	file << header;
}

void WavWriter::write(BYTE * data) {
	numSamples++;
	file << *data;
	file.flush();
}

void WavWriter::close() {
	long len = file.tellp();
	subchunk2Size = (len - 44);
	printf(""+len);

	file.seekp(0, file.beg);
	file << chunkID;
	file << chunkSize; 
	file << format;
	file << subchunk1ID;
	file << subchunk1Size;
	file << audioFormat;
	file << numChannels;
	file << sampleRate;
	file << byteRate;
	file << blockAlign;
	file << bitsPerSample;
	file << subchunk2ID;
	file << subchunk2Size;
	file.flush(); 
	file.close();
}

WavWriter::~WavWriter() {

}
