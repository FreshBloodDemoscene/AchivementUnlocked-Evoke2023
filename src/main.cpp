#include <iostream>

#include <window.h>
#include <renderer.h>
#include <soundTrack.h>
#include <synctracker.h>
#include <file.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>


int main(int argc, char** argv)
{
	File file;
	Window window;
	SoundTrack s;
	SyncTracker syncTracker;
	Renderer renderer(window);

	
	s.PlayMusic("Audio/revision.wav");

	glm::vec3 ro = glm::vec3(0.0f, 1.0f, 4.0f);
	glm::vec3 origin, target;

	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
;
	do
	{
		origin.x = syncTracker.FetchValue("origin:X");
		origin.y = syncTracker.FetchValue("origin:Y");
		origin.z = syncTracker.FetchValue("origin:Z");

		target.x = syncTracker.FetchValue("target:X");
		target.y = syncTracker.FetchValue("target:Y");
		target.z = syncTracker.FetchValue("target:Z");

		glm::mat4 viewMatrix = glm::inverse(glm::lookAt(origin, target, glm::vec3(0.0, 1.0, 0.0)));
		glProgramUniformMatrix4fv(renderer.m_shader, 3, 1, false, &viewMatrix[0][0]);

		color.x = syncTracker.FetchValue("color:X");
		color.y = syncTracker.FetchValue("color:Y");
		color.z = syncTracker.FetchValue("color:Z");

		glProgramUniform3f(renderer.m_shader, 4, color.x, color.y, color.z);

		//std::cout << ro.x << ro.y << ro.z << std::endl;
		syncTracker.Update(s);
		renderer.Render();
		file.UpdateFile(renderer, window, float(s.CurrentTime()));


	} while (window.SwapBuffers());

	return 0;
}
