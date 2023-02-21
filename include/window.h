#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <optional>


//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class windowManager
{
	GLFWwindow* m_window = NULL;
	GLFWmonitor* m_monitor = NULL;

	uint32_t vao;
	uint32_t vertexBuffer;

	float positions[6];

	std::string m_vertexShader = "";
	std::string m_fragmentShader = "";

	unsigned int m_shader = 0;
	unsigned int m_buffer = 0;


	static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	static unsigned int CompileShader(unsigned int type, const std::string& source);

	const GLint WIDTH = 800, HEIGHT = 600;

	void ErrorCallBack(int error, const char* description);
	void ReadAndWrite_Shader(const char* vertexPath, const char* fragmentPath);
	void FullScreenMode();
	void PrintScreen();

	void CameraManager();


	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	explicit windowManager();
	~windowManager();

	void windowRunTime();
};
