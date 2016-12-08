// Audio-Loopback-Recorder.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "AudioCapture.h"
#include <string>
#include <iostream>

bool checkPath(std::string path) {
	if (GetFileAttributesA(path.c_str()) == INVALID_FILE_ATTRIBUTES) return false;
	return true;
}

int _tmain(int argc, _TCHAR* argv[]) {

	AudioCapture * capture;
	std::string input = "";
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
	
	capture = new AudioCapture;
	capture->init();

	do {
		system("cls");
		std::cout << temp << std::endl;
		std::cout << "Choose an option: " << std::endl;
		std::cout << "(1) Start Recording" << std::endl;
		std::cout << "(2) Exit" << std::endl;
		std::cout << "Enter choice: ";
		std::cin >> input;
	} while (input != "1" && input != "2");

	if (input == "2") {
		return 0;
		delete capture;
	}

	capture->start();
	do {
		system("cls");
		std::cout << temp << std::endl;
		std::cout << "Do you want to stop recording <y/n>: ";
		std::cin >> input;
	} while (input != "y");

	capture->stop();
	delete capture;

	system("cls");
	std::cout << temp << std::endl;
	std::cout << "Enter output path: ";
	std::cin >> input;

	while (!checkPath(input)) {
		system("cls");
		std::cout << temp << std::endl;
		std::cout << "INVALID PATH ENTERED" << std::endl;
		std::cout << "Enter path: ";
		std::cin >> input;
	} 

	return 0;
}

