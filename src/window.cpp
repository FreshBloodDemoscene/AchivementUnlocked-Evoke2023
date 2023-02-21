#include <window.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glew32s.lib")

#ifndef NDEBUG
# pragma comment(lib, "glfw3-s-d.lib")
#else
# pragma comment(lib, "glfw3-s.lib")
#endif

Window::Window(int32_t width /* = kDEFAULT_WIDTH */, int32_t height /* = kDEFAULT_HEIGHT */)
{

#pragma region GLFW INIT
	glfwInit();

	if (!glfwInit())
	{
		ErrorCallBack(1, "Window didn't openned properly.");
		glfwTerminate();
	}

#pragma endregion

#pragma region Window INIT
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4.5);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4.5);

	m_window = glfwCreateWindow(width, height, "Fractal", NULL, NULL);

	if (!m_window)
	{
		ErrorCallBack(2, "Window or OpenGL context creation failed.");
		glfwTerminate();
	}

	glfwMakeContextCurrent(m_window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		ErrorCallBack(3, "Unable to initialize GLEW.");
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
		glViewport(0, 0, w, h);
		});

#pragma endregion


	float positions[] = {
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glCreateBuffers(1, &vertexBuffer);
	glNamedBufferStorage(vertexBuffer, 8 * sizeof(float), positions, 0);

	glCreateBuffers(1, &EBO);
	glNamedBufferStorage(EBO, 6 * sizeof(unsigned int), indices, 0);

	glCreateVertexArrays(1, &vao);
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayVertexBuffer(vao, 0, vertexBuffer, 0, 2 * sizeof(float));
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, false, 0);
	glVertexArrayElementBuffer(vao, EBO);


	//READ SHADER FILE AND LOAD IT IN SHADER RENDERER
	ReadAndWrite_Shader("../Shader.vs", "../Shader.fs");
	m_shader = CreateShader(m_vertexShader, m_fragmentShader);
	glUseProgram(m_shader);

	glProgramUniform2f(m_shader, 0, 1920.0f, 1080.0f);
	//RUN TIME
	DoFrame();
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

void Window::ErrorCallBack(int error, const char* description)
{
	std::cout << stderr << "Error : " << description << std::endl;
}

#pragma region RunTime
void Window::DoFrame()
{
	while (!glfwWindowShouldClose(m_window))
	{
		//Make FullScreen On
		FullScreenMode();

		//Set inputs
		glfwSetKeyCallback(m_window, s_KeyCallback);
		//KeepRunning

		//print GL_TRIANGLES in screen
		PrintScreen();
	}

	glDeleteShader(m_shader);
}
#pragma endregion


void Window::s_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
};

void Window::FullScreenMode()
{
	m_monitor = glfwGetPrimaryMonitor();

	if (m_monitor != NULL)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);

		glfwSetWindowMonitor(m_window, m_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}

void Window::PrintScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);

	float timeValue = glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(m_shader, "ourColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glfwSwapBuffers(m_window);

	glfwPollEvents();
}

unsigned int Window::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();

	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int Window::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = (char*)_malloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Window::ReadAndWrite_Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		__debugbreak();
	}

	m_vertexShader = vertexCode.c_str();
	m_fragmentShader = fragmentCode.c_str();
}

void Window::CameraManager()
{
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

	glm::vec3 cameraTarget = glm::vec3(0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);


}