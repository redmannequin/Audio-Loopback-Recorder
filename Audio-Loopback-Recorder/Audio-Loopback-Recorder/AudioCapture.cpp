#include "stdafx.h"
#include "AudioCapture.h"


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

AudioCapture::~AudioCapture() {
	CoTaskMemFree(pwfx);
	if (pEnumerator != NULL) pEnumerator->Release();
	if (pDevice != NULL) pDevice->Release();
	if (pAudioClient != NULL) pAudioClient->Release();
	if (pCaptureClient != NULL) pCaptureClient->Release();
}


HRESULT AudioCapture::init() {
	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		printf("Faild to CoInitialize\n");
		return hr;
	}

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	if (FAILED(hr)) {
		printf("Faild to CoCreateInstance\n");
		return hr;
	}

	// get default output audio endpoint
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
	if (FAILED(hr)) {
		printf("Faild to GetDefaultAudioEndpoint\n");
		return hr;
	}

	// activates device
	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
	if (FAILED(hr)) {
		printf("Faild to Activate Decive\n");
		return hr;
	}

	// gets audio format
	hr = pAudioClient->GetMixFormat(&pwfx);
	if (FAILED(hr)) {
		printf("Faild to GetMixFormat\n");
		return hr;
	}

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, hnsRequestedDuration, 0, pwfx, NULL);
	if (FAILED(hr)) {
		printf("Faild to Initialize Audio Client\n");
		return hr;
	}

	// Get the size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	if (FAILED(hr)) {
		printf("Faild to GetBufferSize\n");
		return hr;
	}

	hr = pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient);
	if (FAILED(hr)) {
		printf("Faild to GetService\n");
		return hr;
	}

	// Calculate the actual duration of the allocated buffer.
	hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;
	return S_OK;
}

HRESULT AudioCapture::start() {
	hr = pAudioClient->Start();  // Start recording.
	if (FAILED(hr)) {
		printf("8");
		return hr;
	}

	thread = CreateThread(NULL, 0, startThread, (void*) this, 0, &threadID);
	return S_OK;
}

HRESULT AudioCapture::stop() {
	
	// stop reading
	bDone = TRUE;
	CloseHandle(thread);
	
	// Stop client.
	hr = pAudioClient->Stop();  
	if (FAILED(hr)) {
		printf("Faild to stop recorder\n");
		return hr;
	}

	return S_OK;
}


DWORD AudioCapture::recoderThread() {
	HRESULT hr;

	// Each loop fills about half of the shared buffer.
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

			if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
				pData = NULL;  // Tell CopyData to write silence.
			}


			/*
			 * copy data here
			 */

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