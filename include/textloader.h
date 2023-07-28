#ifndef REVISION2023_TEXTLOADER_H_
#define REVISION2023_TEXTLOADER_H_

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class textLoader
{
public:
	void loadImage(const char* image_path, int width, int height, int nrChannels, int req_comp);
};


#endif // !REVISION2023_TEXTLOADER_H_

