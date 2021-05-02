#pragma once
#pragma comment(lib, "winmm.lib")

#include <thread>
#include <Windows.h>
#include <mmsystem.h>

class DeeJay
{
public:
	DeeJay()
	{

	}

	void playSoundtrack();
	void stopSoundtrack();

private:

	std::thread* playThread = nullptr;
	std::thread* stopThread = nullptr;

};