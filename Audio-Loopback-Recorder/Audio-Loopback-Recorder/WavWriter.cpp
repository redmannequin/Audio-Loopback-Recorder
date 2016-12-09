#include "stdafx.h"
#include "WavWriter.h"
#include <iostream>

/*  name: WavWriter constructor
*  purpose: set vars
*/
WavWriter::WavWriter(WAVEFORMATEX *fm) :
	chunkID    {'R','I','F','F'},
	format     {'W','A','V','E'},
	subchunk1ID{'f','m','t',' '},
	subchunk2ID{'d','a','t','a'} {

	subchunk1Size = (unsigned int) sizeof(*fm) + fm->cbSize; 
	fmt = new unsigned char[subchunk1Size];
	blockAlign = fm->nBlockAlign;
	memcpy(fmt, fm, subchunk1Size);
}

/*  name: WavWriter destructor
*  purpose: clear memory
*/
WavWriter::~WavWriter() {
	delete fmt;
}

/*  name: init
*  purpose: create a .wav file and write temporary values to the header
*/
void WavWriter::init() {
	file.open("temp.wav", std::ofstream::binary);
	BYTE header[44];
	for (int i = 0; i < 44; ++i) header[i] = 0;
	file.write((char*)header, 44);
}

/*  name: write
*  purpose: write the audio data to the file
*/
void WavWriter::write(BYTE * data, int length) {
	file.write((char*)data, length * blockAlign);
	file.flush();
}

/*  name: close
*  purpose: fill in the header information and close the file
*/
void WavWriter::close() {
	int len = file.tellp();
	subchunk2Size = len-(subchunk1Size+28);
	chunkSize = 36 + subchunk2Size;
	
	file.seekp(0, file.beg);
	file.write((char*)chunkID,        4);
	file.write((char*)&chunkSize,     4);
	file.write((char*)format,         4);

	file.write((char*)subchunk1ID,    4);
	file.write((char*)&subchunk1Size, 4);
	file.write((char*)fmt, subchunk1Size);


	file.write((char*)subchunk2ID,    4);
	file.write((char*)&subchunk2Size, 4);

	file.flush(); 
	file.close();
}
