#ifndef REVISION2023__SoundTrack_H
#define REVISION2023__SoundTrack_H

#include <iostream>

class SoundTrack
{
public:
	explicit SoundTrack();
	~SoundTrack();

	static void PlayMusic(std::string musicPath);
	static void* getFile(std::string* file);
};

#endif // !REVISION2023__SoundTrack_H



