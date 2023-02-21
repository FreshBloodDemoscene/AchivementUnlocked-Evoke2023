#ifndef REVISION2023__SoundTrack_H_
#define REVISION2023__SoundTrack_H_

#include <iostream>


class SoundTrack
{
public:
	explicit SoundTrack();
	~SoundTrack();

	static void PlayMusic(std::string musicPath);
	static void* getFile(std::string* file);
};

#endif // !REVISION2023__SoundTrack_H_



