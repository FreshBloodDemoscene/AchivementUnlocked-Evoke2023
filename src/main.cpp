#include <window.h>
#include <renderer.h>
#include <soundTrack.h>

int main(int argc, char** argv)
{
	Window window;
	Renderer renderer(window);

	do
	{
		renderer.Render();
		SoundTrack::PlayMusic("Audio/test.mp3");
	} while (window.SwapBuffers());

	return 0;
}
