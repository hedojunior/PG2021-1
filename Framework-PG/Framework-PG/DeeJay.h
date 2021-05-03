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
	//Threading para iniciar e parar reprodução da trilha
	std::thread* playThread = nullptr;
	std::thread* stopThread = nullptr;
};