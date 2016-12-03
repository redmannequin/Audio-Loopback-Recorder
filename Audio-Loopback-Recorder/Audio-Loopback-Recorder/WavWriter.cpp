#include "stdafx.h"
#include "WavWriter.h"


WavWriter::WavWriter(WAVEFORMATEX *fm) :
	chunkID    {'R','I','F','F'},
	format     {'W','A','V','A'},
	subchunk1ID{'f','m','t',' '},
	subchunk2ID{'d','a','t','a'} {

	winFormat = fm;
	frames = 0;

	intToByteAry(subchunk1Size, 16);
	shortToByteAry(audioFormat, 1);
	shortToByteAry(numChannels, winFormat->nChannels);
	intToByteAry(sampleRate, winFormat->nSamplesPerSec);
	intToByteAry(byteRate, winFormat->nSamplesPerSec * winFormat->nChannels * (winFormat->wBitsPerSample / 8));
	shortToByteAry(blockAlign, winFormat->nChannels * (winFormat->wBitsPerSample / 8));
	shortToByteAry(bitsPerSample, winFormat->wBitsPerSample);
}

void WavWriter::init(std::string path) {
	file.open(path + "temp.wav");
	BYTE header[44];
	for (int i = 0; i < 44; ++i) header[i] = 0;
	file.write((char*)header, 44);
}

void WavWriter::write(BYTE * data, int length) {
	frames += length;
	file.write((char*)data, length*(winFormat->nChannels * (winFormat->wBitsPerSample/8)));
}

void WavWriter::close() {
	long len = file.tellp();
	int subchunk2SizeInt = (len -44);
	int chunkSizeInt = 36 + subchunk2SizeInt;

	intToByteAry(chunkSize, chunkSizeInt);
	intToByteAry(subchunk2Size, subchunk2SizeInt);
	
	file.seekp(0, file.beg);
	file.write((char*)chunkID,       4);
	file.write((char*)chunkSize,     4); 
	file.write((char*)format,        4);
	file.write((char*)subchunk1ID,   4);
	file.write((char*)subchunk1Size, 4);
	file.write((char*)audioFormat,   2);
	file.write((char*)numChannels,   2);
	file.write((char*)sampleRate,    4);
	file.write((char*)byteRate,      4);
	file.write((char*)blockAlign,    2);
	file.write((char*)bitsPerSample, 2);
	file.write((char*)subchunk2ID,   4);
	file.write((char*)subchunk2Size, 4);
	file.flush(); 
	file.close();
}

WavWriter::~WavWriter() {

}

void WavWriter::intToByteAry(BYTE * ary, int i) {
	i = _byteswap_ulong(i);
	ary[0] = (i & 0x000000ff);
	ary[1] = (i & 0x0000ff00) >> 8;
	ary[2] = (i & 0x00ff0000) >> 16;
	ary[3] = (i & 0xff000000) >> 24;
}

void WavWriter::shortToByteAry(BYTE * ary, short i) {
	i = _byteswap_ushort(i);
	ary[0] = (i & 0x000000ff);
	ary[1] = (i & 0x0000ff00) >> 8;
}