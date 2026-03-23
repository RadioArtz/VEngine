#include "IO/ImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

int ImageLoader::LoadImage(const std::string& path, unsigned char** data, int* width, int* height, int* nrChannels)
{
	stbi__vertically_flip_on_load_global = 1;
	*data = stbi_load(path.c_str(), width, height, nrChannels, 0);
	if (data)
		return 0;
	else
		return -1;
}

void ImageLoader::FreeImage(unsigned char* data)
{
	if(data)
		stbi_image_free(data);
}