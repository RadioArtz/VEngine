#include "IO/ImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

int ImageLoader::LoadImage(const std::string& path, unsigned char*& data, int& width, int& height, int& nrChannels)
{
	// FIXME: (Fixed, just a comment, remove me) You should references instead of pointers everywhere you possibly can. It's not only more readable, but it also fixed a bug here:
	// You checked data for not nullptr (line + 4), you probably wanted to check (*data) data was the pointer to the pointer to the image data, *data was the pointer to the image data
	// Effect: This always returned 0, or crashed because the input pointer is nullptr and stbi wrote to it before the check below would have executed.
	stbi__vertically_flip_on_load_global = 1;
	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
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