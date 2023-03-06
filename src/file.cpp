#include <file.h>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

File::File()
{
	fs_changed = std::filesystem::last_write_time(fragment_shader_file);
	vs_changed = std::filesystem::last_write_time(vertex_shader_file);

}

File::~File() noexcept
{
}

void File::CheckModification()
{	
	auto check_fs_modification = std::filesystem::last_write_time(fragment_shader_file);
	auto check_vs_modification = std::filesystem::last_write_time(vertex_shader_file);

	if (fs_changed != check_fs_modification || vs_changed != check_vs_modification)
	{
		status = FileStatus::modified;

		fs_changed = check_fs_modification;
		vs_changed = check_vs_modification;

	}
	else
	{
		status = FileStatus::nothingchanged;
	}

}

void File::UpdateFile(Renderer& render, Window& window, float t)
{
	CheckModification();

	switch (status)
	{
	case FileStatus::modified:
	{	
		render.ReadAndWrite_Shader("ShaderFiles/Shader.vs", "ShaderFiles/Shader.fs");
		render.m_shader = Renderer::CreateShader(render.m_vertexShader, render.m_fragmentShader);
		glUseProgram(render.m_shader);
		glProgramUniform2f(render.m_shader, 0, float(window.Size().x), float(window.Size().y));
	}
	break;
	case FileStatus::nothingchanged:
		break;
	default:
		break;
	}
	glProgramUniform1f(render.m_shader, 1, t);
}





