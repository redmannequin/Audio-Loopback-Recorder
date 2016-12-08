#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include <Audioclient.h>
#include <mmdeviceapi.h>
#include "WavWriter.h"

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000


class AudioCapture{
	public:
		AudioCapture();
		~AudioCapture();

		HRESULT init();
		HRESULT start();
		HRESULT stop();

	private:
		HRESULT hr;
		REFERENCE_TIME hnsRequestedDuration;
		REFERENCE_TIME hnsActualDuration;
		UINT32 bufferFrameCount;
		UINT32 numFramesAvailable;
		IMMDeviceEnumerator *pEnumerator;
		IMMDevice *pDevice;
		IAudioClient *pAudioClient;
		IAudioCaptureClient *pCaptureClient;
		WAVEFORMATEX *pwfx;
		UINT32 packetLength;
		BOOL bDone;
		BYTE *pData;
		DWORD flags;
		WavWriter *wav_file;

		const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);
		const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
		const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
		const IID IID_IAudioClient = __uuidof(IAudioClient);

		DWORD threadID;
		HANDLE thread;
		DWORD recoderThread();
		static DWORD WINAPI startThread(void* param) {
			AudioCapture *capture = (AudioCapture*)param;
			return capture->recoderThread();
		};

};

#endif