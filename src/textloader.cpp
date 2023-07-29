#include <textloader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void textLoader::loadImage(const char* image_path, int width, int height, int nrChannels, int req_comp)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    unsigned char* data = stbi_load(image_path, &width, &height, &nrChannels, 4);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void textLoader::Use(unsigned int unit)
{
    glBindTextureUnit(unit, texture);
}