#include <synctracker.h>
#include <soundTrack.h>

#define SYNC_PLAYER

#include <rocket/sync.h>

#include <exception>

//#ifndef SYNC_PLAYER
#pragma comment(lib, "ws2_32.lib")
//#endif // SYNC_PLAYER

#ifndef NDEBUG
# pragma comment(lib, "sync-s-d.lib")
#else /* !NDEBUG */
# pragma comment(lib, "sync-s.lib")
#endif /* !NDEBUG */

SyncTracker::SyncTracker()
	:m_row(0.0)
{
	m_rocket = sync_create_device("sync");
    if (!m_rocket)
        throw std::exception("SyncTracker: Unable to create Rocket sync device");

#ifndef SYNC_PLAYER
    if (sync_tcp_connect(m_rocket, "localhost", SYNC_DEFAULT_PORT))
        throw std::exception("SyncTracker: Unable to connect to host");
#endif /* !SYNC_PLAYER */
}

SyncTracker::~SyncTracker() noexcept
{
#ifndef SYNC_PLAYER
    sync_save_tracks(m_rocket);
#endif /* SYNC_PLAYER */
    sync_destroy_device(m_rocket);
}

void SyncTracker::Update(SoundTrack& soundtrack)
{
    m_row = soundtrack.CurrentRow();
    m_time = soundtrack.CurrentTime();

#ifndef SYNC_PLAYER
    if (sync_update(m_rocket, (int)floor(m_row), &SoundTrack::s_syncLink, (void*)&soundtrack))
        sync_tcp_connect(m_rocket, "localhost", SYNC_DEFAULT_PORT);
#endif
}

float SyncTracker::FetchValue(const std::string& name)
{
    if (!m_tracks.contains(name))
        m_tracks[name] = sync_get_track(m_rocket, name.c_str());

    if (m_tracks[name] == nullptr)
        return 0.0f;

    return sync_get_val(m_tracks[name], m_row);
}

float SyncTracker::CurrentTime()
{
    return float(m_time);
}


