#include "stdafx.h"
#include "WavWriter.h"
#include <iostream>


WavWriter::WavWriter(WAVEFORMATEX *fm) :
	chunkID    {'R','I','F','F'},
	format     {'W','A','V','E'},
	subchunk1ID{'f','m','t',' '},
	subchunk2ID{'d','a','t','a'} {

	winFormat = fm;
	frames = 0;

	subchunk1Size = (unsigned int) sizeof(*fm) + fm->cbSize; 
	fmt = new unsigned char[subchunk1Size];
	memcpy(fmt, fm, subchunk1Size);
	audioFormat = (unsigned short)1;
	numChannels = (unsigned short)winFormat->nChannels; 
	sampleRate = (unsigned long)winFormat->nSamplesPerSec; 
	bitsPerSample = (unsigned short)winFormat->wBitsPerSample;
	byteRate = sampleRate * numChannels * bitsPerSample / 8; 
	blockAlign = (numChannels * bitsPerSample / 8);

	std::cout << "wBitsPerSample: " << fm->wBitsPerSample << std::endl;
	std::cout << "nSamplesPerSec: " << fm->nSamplesPerSec << std::endl;
	std::cout << "nChannels: " << fm->nChannels << std::endl;
	std::cout << "nBlockAlign: " << fm->nBlockAlign << std::endl;
	std::cout << "nAvgBytesPerSec: " << fm->nAvgBytesPerSec << std::endl;
	std::cout << "format: " << fm->wFormatTag << std::endl;

	/*intToByteAry((BYTE*)subchunk1Size, 16);
	shortToByteAry((BYTE*)audioFormat, 1);
	shortToByteAry((BYTE*)numChannels, winFormat->nChannels);
	intToByteAry((BYTE*)sampleRate, winFormat->nSamplesPerSec);
	intToByteAry((BYTE*)byteRate, winFormat->nSamplesPerSec * winFormat->nChannels * (winFormat->wBitsPerSample / 8));
	shortToByteAry((BYTE*)blockAlign, winFormat->nChannels * (winFormat->wBitsPerSample / 8));
	shortToByteAry((BYTE*)bitsPerSample, winFormat->wBitsPerSample);*/
}

WavWriter::~WavWriter() {
}


void WavWriter::init(std::string path) {
	//file.open(path + "temp.wav");
	file.open("temp.wav", std::ofstream::binary); //put the file in the same directory as the source code for now
	BYTE header[44];
	for (int i = 0; i < 44; ++i) header[i] = 0;
	file.write((char*)header, 44);
}

void WavWriter::write(BYTE * data, int length) {
	frames += length;
	file.write((char*)data, length * blockAlign);
	file.flush();
}

void WavWriter::close() {
	int len = file.tellp();
	subchunk2Size = len-44;
	chunkSize = 36 + subchunk2Size;
	
	//chunkSize = _byteswap_ulong(chunkSize);
	//subchunk1Size = _byteswap_ulong(subchunk1Size);

	//audioFormat = _byteswap_ushort(audioFormat);
	//numChannels = _byteswap_ushort(numChannels);
	//sampleRate = _byteswap_ulong(sampleRate);
	//byteRate = _byteswap_ulong(byteRate);
	//blockAlign = _byteswap_ushort(blockAlign);
	//bitsPerSample = _byteswap_ushort(bitsPerSample);
	//subchunk2Size = _byteswap_ulong(subchunk2Size);
	        
	
	file.seekp(0, file.beg);
	file.write((char*)chunkID,        4);
	file.write((char*)&chunkSize,     4);
	file.write((char*)format,         4);

	file.write((char*)subchunk1ID,    4);
	file.write((char*)&subchunk1Size, 4);
	file.write((char*)fmt, subchunk1Size);
	

	//file.write((char*)&audioFormat,   2);
	//file.write((char*)&numChannels,   2);
	//file.write((char*)&sampleRate,    4);
	//file.write((char*)&byteRate,      4);
	//file.write((char*)&blockAlign,    2);
	//file.write((char*)&bitsPerSample, 2);

	file.write((char*)subchunk2ID,    4);
	file.write((char*)&subchunk2Size, 4);

	file.flush(); 
	file.close();
}

void WavWriter::intToByteAry(BYTE * ary, int i) {
	i = _byteswap_ulong(i);
	//ary[0] = (i & 0x000000ff);
	//ary[1] = (i & 0x0000ff00) >> 8;
	//ary[2] = (i & 0x00ff0000) >> 16;
	//ary[3] = (i & 0xff000000) >> 24;
}

void WavWriter::shortToByteAry(BYTE * ary, short i) {
	i = _byteswap_ushort(i);
	//ary[0] = (i & 0x000000ff);
	//ary[1] = (i & 0x0000ff00) >> 8;
}