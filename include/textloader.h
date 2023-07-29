#ifndef REVISION2023_TEXTLOADER_H_
#define REVISION2023_TEXTLOADER_H_

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>



class textLoader
{
	unsigned int texture;
public:
	void loadImage(const char* image_path, int width, int height, int nrChannels, int req_comp);
	void Use(unsigned int unit);
};


#endif // !REVISION2023_TEXTLOADER_H_

