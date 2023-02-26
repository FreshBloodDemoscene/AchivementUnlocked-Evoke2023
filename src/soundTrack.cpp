#include "soundTrack.h"

#include <string>



sync_cb SoundTrack::s_syncLink =
{
	SoundTrack::ms_Pause,
	SoundTrack::ms_SetRow,
	SoundTrack::ms_IsPlaying
};

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
	m_streamHandle = BASS_StreamCreateFile(false, getFile(filePath), 0, 0, 0);
	BASS_ChannelPlay(m_streamHandle, true);
}

void* SoundTrack::getFile(std::string* file) {
	return file->data();
}

double SoundTrack::CurrentRow() const
{
	QWORD pos = BASS_ChannelGetPosition(m_streamHandle, BASS_POS_BYTE);
	double time = BASS_ChannelBytes2Seconds(m_streamHandle, pos);
	return time * m_rowRate;
}

double SoundTrack::CurrentTime() const
{
	QWORD pos = BASS_ChannelGetPosition(m_streamHandle, BASS_POS_BYTE);
	double time = BASS_ChannelBytes2Seconds(m_streamHandle, pos);
	return time;
}

void SoundTrack::ms_Pause(void* d, int flag)
{
	SoundTrack* self = static_cast<SoundTrack*>(d);
	if (flag)
		BASS_ChannelPause(self->m_streamHandle);
	else
		BASS_ChannelPlay(self->m_streamHandle, false);
}

void SoundTrack::ms_SetRow(void* d, int row)
{
	SoundTrack* self = static_cast<SoundTrack*>(d);
	QWORD pos = BASS_ChannelSeconds2Bytes(self->m_streamHandle, row / self->m_rowRate);
	BASS_ChannelSetPosition(self->m_streamHandle, pos, BASS_POS_BYTE);
}

int SoundTrack::ms_IsPlaying(void* d)
{
	SoundTrack* self = static_cast<SoundTrack*>(d);
	return BASS_ChannelIsActive(self->m_streamHandle) == BASS_ACTIVE_PLAYING;
}