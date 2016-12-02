#include "stdafx.h"
#include "AudioCapture.h"

#include <iostream>

/*  name: AudioCapture constructor 
 *  purpose: set vars
 */
AudioCapture::AudioCapture() {
	hnsRequestedDuration = REFTIMES_PER_SEC;
	pEnumerator = NULL;
	pDevice = NULL;
	pAudioClient = NULL;
	pCaptureClient = NULL;
	pwfx = NULL;
	packetLength = 0;
	bDone = FALSE;
}

/*  name: AudioCapture destructor
 *  purpose: clear memory
 */
AudioCapture::~AudioCapture() {
	CoTaskMemFree(pwfx);
	if (pEnumerator != NULL) pEnumerator->Release();
	if (pDevice != NULL) pDevice->Release();
	if (pAudioClient != NULL) pAudioClient->Release();
	if (pCaptureClient != NULL) pCaptureClient->Release();
}

/*  name: init 
 *  purpose: initializes all the audio capture dependencies
 */
HRESULT AudioCapture::init() {
	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		std::cout << "Faild to CoInitialize\n";
		return hr;
	}

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	if (FAILED(hr)) {
		std::cout << "Faild to CoCreateInstance\n";
		return hr;
	}

	// get default output audio endpoint
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
	if (FAILED(hr)) {
		std::cout << "Faild to GetDefaultAudioEndpoint\n";
		return hr;
	}

	// activates device
	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
	if (FAILED(hr)) {
		std::cout << "Faild to Activate Decive\n";
		return hr;
	}

	// gets audio format
	hr = pAudioClient->GetMixFormat(&pwfx);
	if (FAILED(hr)) {
		std::cout << "Faild to GetMixFormat\n";
		return hr;
	}

	wav_file = new WavWriter(pwfx);

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, hnsRequestedDuration, 0, pwfx, NULL);
	if (FAILED(hr)) {
		std::cout << "Faild to Initialize Audio Client\n";
		return hr;
	}

	// Get the size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	if (FAILED(hr)) {
		std::cout << "Faild to GetBufferSize\n";
		return hr;
	}

	hr = pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient);
	if (FAILED(hr)) {
		std::cout << "Faild to GetService\n";
		return hr;
	}

	// Calculate the actual duration of the allocated buffer.
	hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;
	return S_OK;
}

/*  name: start
 *  purpose: create the thread to capture audio
 */
HRESULT AudioCapture::start() {
	wav_file->init(path);

	hr = pAudioClient->Start();  // Start recording.
	if (FAILED(hr)) {
		printf("8");
		return hr;
	}
	thread = CreateThread(NULL, 0, startThread, (void*) this, 0, &threadID);
	return S_OK;
}

/*  name: stop
 *  purpose: stop the capture of audio
 *  Extra Info: sets bDone to true
 */
HRESULT AudioCapture::stop() {
	// stop reading
	bDone = TRUE;
	CloseHandle(thread);
	
	wav_file->close();

	// Stop client.
	hr = pAudioClient->Stop();  
	if (FAILED(hr)) {
		printf("Faild to stop recorder\n");
		return hr;
	}

	return S_OK;
}

/*  name: recorderThread
 *  purpose: capture audio until stop is called 
 *  Extra Info: is run on a separate thread from the main program. 
 */
DWORD AudioCapture::recoderThread() {
	HRESULT hr;

	// loopes untill bDone if true
	while (bDone == FALSE) {
		// Sleep for half the buffer duration.
		Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2);

		// get next packet of data size
		hr = pCaptureClient->GetNextPacketSize(&packetLength);
		if (FAILED(hr)) {
			printf("Faild to get next data packet size\n");
			return hr;
		}

		while (packetLength != 0) {
			// Get the available data in the shared buffer.
			hr = pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
			if (FAILED(hr)) {
				printf("Faild to get buffer\n");
				return hr;
			}

			/*if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
				pData = NULL;  // Tell CopyData to write silence.
			}*/

			/*
			 * copy data here
			 */
			//printf(""+(int)numFramesAvailable);
			for (int i=0; i < numFramesAvailable; ++i) wav_file->write(pData+i);

			hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
			if (FAILED(hr)) {
				printf("Faild to release buffer\n");
				return hr;
			}

			hr = pCaptureClient->GetNextPacketSize(&packetLength);
			if (FAILED(hr)) {
				printf("Faild to get next data packet size\n");
				return hr;
			}
		}
	}
	return 0;
}

void AudioCapture::setPath(std::string path) {
	this->path = path;
}