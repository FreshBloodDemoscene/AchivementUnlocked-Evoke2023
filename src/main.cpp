#include <iostream>

#include <window.h>
#include <renderer.h>
#include <soundTrack.h>
#include <synctracker.h>
#include <file.h>

int main(int argc, char** argv)
{
	File file;
	Window window;
	SoundTrack s;
	SyncTracker syncTracker;
	Renderer renderer(window);

	
	s.PlayMusic("Audio/revision.wav");


	do
	{
		float f = syncTracker.FetchValue("test");

		//std::cout << f << std::endl;
		syncTracker.Update(s);
		renderer.Render();
		file.UpdateFile(renderer, window);
	} while (window.SwapBuffers());

	return 0;
}
