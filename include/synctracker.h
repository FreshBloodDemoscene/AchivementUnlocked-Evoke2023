#ifndef REVISION2023_SYNCTRACKER_H
#define REVISION2023_SYNCTRACKER_H

#include <string>
#include <unordered_map>

struct sync_device;
struct sync_track;
class SoundTrack;

class SyncTracker
{
	double			m_row;
	double			m_time;
	sync_device*	m_rocket;
	std::unordered_map<std::string, const sync_track*> m_tracks;

public:
	explicit SyncTracker();
	~SyncTracker() noexcept;

	void Update(SoundTrack& SoundTrack);
	float FetchValue(const std::string& name);
	float CurrentTime();

};

#endif // !REVISION2023_SYNCTRACKER_H
