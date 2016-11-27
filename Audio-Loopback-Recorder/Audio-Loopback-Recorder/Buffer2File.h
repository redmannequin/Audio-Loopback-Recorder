#ifndef BUFFER_2_FILE_H
#define BUFFER_2_FILE_H

#include <mmdeviceapi.h>

class Buffer2File {

public:
	Buffer2File(char* path);
	~Buffer2File();
	void setFormat(WAVEFORMATEX format);
	void write(byte* pData, int numFramesAvailable, bool bDone);

private:
	WAVEFORMATEX foramt;
	byte* buffer;
	char* path;
};

#endif