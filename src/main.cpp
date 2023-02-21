#include <window.h>
#include <renderer.h>

int main(int argc, char** argv)
{
	Window window;
	Renderer renderer(window);

	do
	{
		renderer.Render();
	} while (window.SwapBuffers());

	return 0;
}
