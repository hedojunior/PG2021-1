#include "DeeJay.h"

void _playSoundtrack()
{
	PlaySound(TEXT("../soundtrack/soundtrack.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void _stopSoundtrack()
{
	PlaySound(NULL, 0, 0);
}

void DeeJay::playSoundtrack()
{
	playThread = new std::thread(_playSoundtrack);
	playThread->detach();
}

void DeeJay::stopSoundtrack()
{
	stopThread = new std::thread(_stopSoundtrack);
	stopThread->detach();
}

