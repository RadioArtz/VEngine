#pragma once
#include <string>

class ImageLoader
{
public:
	static int LoadImage(const  std::string& path, unsigned char*& data, int& width, int& height, int& nrChannels);
	static void FreeImage(unsigned char* data);
};