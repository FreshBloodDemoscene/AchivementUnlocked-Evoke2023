#include "soundTrack.h"

#include <string>

#include "bass.h"

#pragma comment(lib, "bass.lib")

SoundTrack::SoundTrack()
{
	BASS_Init(-1, 65000, BASS_DEVICE_STEREO, 0, NULL);

	if (!BASS_Init)
	{
		MessageBox(0, (LPCWSTR)"BASS_Init could not be executed.", 0, MB_ICONERROR);
	}

	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		MessageBox(0, (LPCWSTR)"An incorrect version of BASS.DLL was loaded.", 0, MB_ICONERROR);
	}
}

SoundTrack::~SoundTrack()
{

}

void SoundTrack::PlayMusic(std::string musicPath)
{
	SoundTrack();
	std::string* filePath = new std::string(musicPath);
	HSTREAM streamHandle = BASS_StreamCreateFile(false, getFile(filePath), 0, 0, 0);
	BASS_ChannelPlay(streamHandle, true);
}


void* SoundTrack::getFile(std::string* file) {
	return file->data();
}