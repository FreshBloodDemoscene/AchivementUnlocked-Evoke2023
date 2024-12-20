#include <iostream>

#include <window.h>
#include <renderer.h>
#include <soundTrack.h>
#include <synctracker.h>
#include <file.h>
#include <textloader.h>

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
    textLoader tw, credits;

	
	s.PlayMusic("Evoke2023.mp3");

	glm::vec3 origin, target;

	glm::vec3	color = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3	light_Position(2, 2, 7);

	glm::vec3 lpIntro, lpIntro2;

	glm::vec3 flashColor;
	
	glm::vec3 rotationObj;

	glm::vec3 cPosition;

	glm::vec3 lPosition;
	glm::vec3 lPosition2;

	float		propagation = 10.0f;

	float		scene2laser;
	float		sceneId;
	float		inflation;

	float		UseTw = 0.0f;
	float		UseCredits = 0.0f;

	tw.loadImage("triggerWarning.png", 225, 225, 1, 0);
	credits.loadImage("credits.png", 225, 225, 1, 0);

	do
	{
		//need whole new file for this one for engine recreation

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

		flashColor.x = syncTracker.FetchValue("fColor:X");
		flashColor.y = syncTracker.FetchValue("fColor:Y");
		flashColor.z = syncTracker.FetchValue("fColor:Z");

		glProgramUniform3f(renderer.m_shader, 17, flashColor.x, flashColor.y, flashColor.z);

		inflation = syncTracker.FetchValue("inflation");
		glProgramUniform1f(renderer.m_shader, 10, inflation);

		cPosition.x = syncTracker.FetchValue("cPosition:X");
		cPosition.y = syncTracker.FetchValue("cPosition:Y");
		cPosition.z = syncTracker.FetchValue("cPosition:Z");

		glProgramUniform3f(renderer.m_shader, 11, cPosition.x, cPosition.y, cPosition.z);

		lPosition.x = syncTracker.FetchValue("lPosition:X");
		lPosition.y = syncTracker.FetchValue("lPosition:Y");
		lPosition.z = syncTracker.FetchValue("lPosition:Z");

		glProgramUniform3f(renderer.m_shader, 13, lPosition.x, lPosition.y, lPosition.z);
		
		lPosition2.x = syncTracker.FetchValue("lPosition2:X");
		lPosition2.y = syncTracker.FetchValue("lPosition2:Y");
		lPosition2.z = syncTracker.FetchValue("lPosition2:Z");

		glProgramUniform3f(renderer.m_shader, 14, lPosition2.x, lPosition2.y, lPosition2.z);

		tw.Use(0);
		credits.Use(1);

		UseTw = syncTracker.FetchValue("UseTw");
		UseCredits = syncTracker.FetchValue("UseCredits");

		glProgramUniform1f(renderer.m_shader, 18, UseTw);
		glProgramUniform1f(renderer.m_shader, 19, UseCredits);

		//std::cout << ro.x << ro.y << ro.z << std::endl;
		syncTracker.Update(s);
		renderer.Render();
		file.UpdateFile(renderer, window, float(s.CurrentTime()));


	} while (window.SwapBuffers() && (SoundTrack::ms_IsPlaying(&s)));

	return 0;
}

int CALLBACK WinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	LPSTR       lpCmdLine,
	int         nCmdShow
)
{
	return main(0, nullptr);
}
