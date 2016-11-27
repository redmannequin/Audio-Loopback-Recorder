// Audio-Loopback-Recorder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Audioclient.h>
#include <mmdeviceapi.h>

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);
const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);

int _tmain(int argc, _TCHAR* argv[]) {
	HRESULT hr;
	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	IMMDeviceEnumerator *pEnumerator = NULL;
	IMMDevice *pDevice = NULL;
	IAudioClient *pAudioClient = NULL;
	IAudioCaptureClient *pCaptureClient = NULL;
	WAVEFORMATEX *pwfx = NULL;
	UINT32 packetLength = 0;
	BOOL bDone = FALSE;
	BYTE *pData;
	DWORD flags;



	hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		printf("0");
		return hr;
	}

	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	if (FAILED(hr)) {
		printf("1");
		return hr;
	}

	// get default output audio endpoint
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
	if (FAILED(hr)) {
		printf("2");
		return hr;
	}

	// activates device
	hr = pDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
	if (FAILED(hr)) {
		printf("3");
		return hr;
	}

	// gets audio format
	hr = pAudioClient->GetMixFormat(&pwfx);
	if (FAILED(hr)) {
		printf("4");
		return hr;
	}

	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, hnsRequestedDuration, 0, pwfx, NULL);
	if (FAILED(hr)) {
		printf("5");
		return hr;
	}

	// Get the size of the allocated buffer.
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	if (FAILED(hr)) {
		printf("6");
		return hr;
	}

	hr = pAudioClient->GetService(IID_IAudioCaptureClient, (void**)&pCaptureClient);
	if (FAILED(hr)) {
		printf("7");
		return hr;
	}

	// Notify the audio sink which format to use.
	//hr = pMySink->SetFormat(pwfx);
	//if (FAILED(hr)) { return hr; }

	// Calculate the actual duration of the allocated buffer.
	hnsActualDuration = (double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec;

	hr = pAudioClient->Start();  // Start recording.
	if (FAILED(hr)) {
		printf("8");
		return hr;
	}

	// Each loop fills about half of the shared buffer.
	while (bDone == FALSE) {
		// Sleep for half the buffer duration.
		Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2);

		printf("waiting for event\n");
		hr = pCaptureClient->GetNextPacketSize(&packetLength);
		if (FAILED(hr)) {
			printf("9");
			return hr;
		}

		printf("reading data\n");
		while (packetLength != 0) {
			// Get the available data in the shared buffer.
			hr = pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
			if (FAILED(hr)) {
				printf("10");
				return hr;
			}

			if (flags & AUDCLNT_BUFFERFLAGS_SILENT) {
				pData = NULL;  // Tell CopyData to write silence.
			}

			for (int i = 0; i < 10; ++i) {
				int p = (int)pData[i];
				printf("%d", p);
			}
			printf("\n");

			// Copy the available capture data to the audio sink.
			//hr = pMySink->CopyData(pData, numFramesAvailable, &bDone);
			//if (FAILED(hr)) { return hr; }

			hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
			if (FAILED(hr)) {
				printf("11");
				return hr;
			}

			hr = pCaptureClient->GetNextPacketSize(&packetLength);
			if (FAILED(hr)) {
				printf("12");
				return hr;
			}
		}
	}

	hr = pAudioClient->Stop();  // Stop recording.
	if (FAILED(hr)) {
		printf("13");
		return hr;
	}

	CoTaskMemFree(pwfx);
	if (pEnumerator != NULL) pEnumerator->Release();
	if (pDevice != NULL) pDevice->Release();
	if (pAudioClient != NULL) pAudioClient->Release();
	if (pCaptureClient != NULL) pCaptureClient->Release();

	return 0;
}

