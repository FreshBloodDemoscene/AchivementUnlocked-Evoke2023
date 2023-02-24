#include <window.h>

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glew32s.lib")

#ifndef NDEBUG
# pragma comment(lib, "glfw3-s-d.lib")
#else
# pragma comment(lib, "glfw3-s.lib")
#endif

Window::Window(const glm::ivec2& size /* = glm::ivec2(kDEFAULT_WIDTH, kDEFAULT_HEIGHT) */)
	: m_size(size)
{
	glfwInit();

	if (!glfwInit())
	{
		ErrorCallBack(1, "Window didn't openned properly.");
		glfwTerminate();
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	m_window = glfwCreateWindow(size.x, size.y, "Revision2023", NULL, NULL);

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

	glfwSetKeyCallback(m_window, s_KeyCallback);

#pragma endregion
}

Window::~Window()
{
	glfwDestroyWindow(m_window);
}

void Window::ErrorCallBack(int error, const char* description)
{
	std::cout << stderr << "Error : " << description << std::endl;
}

bool Window::SwapBuffers()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();

	return !glfwWindowShouldClose(m_window);
}

void Window::s_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
		FullScreenMode(window);
};

void Window::FullScreenMode(GLFWwindow* window)
{
	GLFWmonitor* temp_monitor;

	temp_monitor = glfwGetPrimaryMonitor();

	if (temp_monitor != NULL)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(temp_monitor);

		glfwSetWindowMonitor(window, temp_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
}

glm::ivec2 Window::Size() const
{
	return m_size;
}