#pragma once
#include <string>
class Texture
{
private:
	unsigned char* data;
	int width, height, nrChannels;
	std::string imagePath = "game/content/textures/test.jpg";
	
	unsigned int glTexture;
public:
	int SetupGLTexture(int guh);
	void ActivateTexture(int TextureUnit);
	int GetGLTexture() { return glTexture; }
};

enum class EMipMode 
{

};