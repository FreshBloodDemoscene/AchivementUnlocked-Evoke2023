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

	
	s.PlayMusic("Audio/Evoke2023.mp3");

	glm::vec3 origin, target;

	glm::vec3	color = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3	light_Position(2, 2, 7);

	glm::vec3 lpIntro, lpIntro2;

	float		propagation = 10.0f;

	float		scene2laser;
	float		sceneId;
	glm::vec3 rotationObj;


	int fullSquare = 0;
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
	
		fullSquare = syncTracker.FetchValue("fullSquare");

		glProgramUniform1i(renderer.m_shader, 5, fullSquare);


		propagation = syncTracker.FetchValue("propagation");

		glProgramUniform1f(renderer.m_shader, 6, propagation);

		scene2laser = syncTracker.FetchValue("scene2Laser");
		glProgramUniform1f(renderer.m_shader, 7, scene2laser);

		sceneId = syncTracker.FetchValue("sceneId");
		glProgramUniform1f(renderer.m_shader, 8, sceneId);

		rotationObj.x = syncTracker.FetchValue("rotationObj:x");
		rotationObj.y = syncTracker.FetchValue("rotationObj:y");
		rotationObj.z = syncTracker.FetchValue("rotationObj:z");
		glProgramUniform3f(renderer.m_shader, 9, rotationObj.x, rotationObj.y, rotationObj.z);

		lpIntro.x = syncTracker.FetchValue("lpIntro:X");
		lpIntro.y = syncTracker.FetchValue("lpIntro:Y");
		lpIntro.z = syncTracker.FetchValue("lpIntro:Z");

		glProgramUniform3f(renderer.m_shader, 15, lpIntro.x, lpIntro.y, lpIntro.z);

		lpIntro2.x = syncTracker.FetchValue("lpIntro2:X");
		lpIntro2.y = syncTracker.FetchValue("lpIntro2:Y");
		lpIntro2.z = syncTracker.FetchValue("lpIntro2:Z");

		glProgramUniform3f(renderer.m_shader, 16, lpIntro2.x, lpIntro2.y, lpIntro2.z);



		//std::cout << ro.x << ro.y << ro.z << std::endl;
		syncTracker.Update(s);
		renderer.Render();
		file.UpdateFile(renderer, window, float(s.CurrentTime()));


	} while (window.SwapBuffers());

	return 0;
}
