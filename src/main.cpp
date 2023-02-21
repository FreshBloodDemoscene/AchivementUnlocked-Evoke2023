#include <window.h>
#include <renderer.h>
#include <soundTrack.h>

int main(int argc, char** argv)
{
	Window window;
	Renderer renderer(window);
	SoundTrack::PlayMusic("Audio/test.mp3");

	do
	{
		renderer.Render();

	} while (window.SwapBuffers());

	return 0;
}
