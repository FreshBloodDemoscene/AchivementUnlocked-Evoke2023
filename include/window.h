#ifndef REVISION2023__WINDOW_H
#define REVISION2023__WINDOW_H

#include <cstdint>
#include <string>

struct GLFWwindow;
struct GLFWmonitor;

class Window
{
	GLFWwindow*  m_window  = nullptr;
	GLFWmonitor* m_monitor = nullptr;

	uint32_t vao;
	uint32_t vertexBuffer;

	float positions[6];

	std::string m_vertexShader   = "";
	std::string m_fragmentShader = "";

	uint32_t m_shader = 0;
	uint32_t m_buffer = 0;

	static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	static unsigned int CompileShader(unsigned int type, const std::string& source);

	void ErrorCallBack(int error, const char* description);
	void ReadAndWrite_Shader(const char* vertexPath, const char* fragmentPath);
	void FullScreenMode();
	void PrintScreen();

	void CameraManager();

	static void s_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:	
	static const int32_t kDEFAULT_WIDTH  = 800,
	                     kDEFAULT_HEIGHT = 600;

	explicit Window(int32_t width = kDEFAULT_WIDTH, int32_t height = kDEFAULT_HEIGHT);
	~Window();

	void DoFrame();
};

#endif /* REVISION2023__WINDOW_H */
