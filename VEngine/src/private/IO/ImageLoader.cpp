#include "IO/ImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

int ImageLoader::LoadImage(const std::string& path, unsigned char** data, int* width, int* height, int* nrChannels)
{
	stbi__vertically_flip_on_load_global = 1;
	*data = stbi_load(path.c_str(), width, height, nrChannels, 0);
	return 0;
}
