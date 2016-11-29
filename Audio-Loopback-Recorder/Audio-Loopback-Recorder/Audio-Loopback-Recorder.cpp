// Audio-Loopback-Recorder.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "AudioCapture.h"
#include <string>
#include <iostream>

int _tmain(int argc, _TCHAR* argv[]) {
	HRESULT hr;
	AudioCapture capture;
	hr = capture.init();
	if (hr != S_OK) return 0;

	std::string temp = "";
	temp += "                    _ _                 \n";
	temp += "     /\\            | (_)                \n";
	temp += "    /  \\  _   _  __| |_  ___            \n";
	temp += "   / /\\ \\| | | |/ _` | |/ _ \\           \n";
	temp += "  / ____ \\ |_| | (_| | | (_) |          \n";
	temp += " /_/____\\_\\__,_|\\__,_|_|\\___/           \n";
	temp += "  / ____|          | |                  \n";
	temp += " | |     __ _ _ __ | |_ _   _ _ __ ___  \n";
	temp += " | |    / _` | '_ \\| __| | | | '__/ _ \\ \n";
	temp += " | |___| (_| | |_) | |_| |_| | | |  __/ \n";
	temp += "  \\_____\\__,_| .__\\/ \__|\\__,_|_|  \\___| \n";
	temp += "             | |                        \n";
	temp += "             |_|     \n";
	temp += "----------------------------------------";
	
	
	std::cout << temp << std::endl;
	do {
		std::cout << "Do you want to start recording <y/n>: ";
		std::cin >> temp;
	} while (temp != "y");
	capture.start();
	do {
		std::cout << "Do you want to stop recording <y/n>: ";
		std::cin >> temp;
	} while (temp != "y");
	capture.stop();
	return 0;
}

