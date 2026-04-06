#pragma once
#include <string>
class Texture
{
public:
	Texture() = default;
	Texture(std::string inPath) { imagePath = inPath; }
private:
	int width, height, nrChannels;
	std::string imagePath = "game/content/textures/test.jpg";
	
	unsigned int glTexture;
public:
	int SetupGLTexture();
	void ActivateTexture(int TextureUnit);
	int GetGLTexture() { return glTexture; }
};

enum class EMipMode 
{

};