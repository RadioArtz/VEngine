#include <Rendering/Texture.h>
#include <string>
#include <IO/ImageLoader.h>
#include <glad/glad.h>


int Texture::SetupGLTexture(bool guh)
{
    if(guh)
        imagePath = "C:/Users/mathi/Pictures/contain soundcloud.png";
    ImageLoader::LoadImage(imagePath, &data, &width, &height, &nrChannels);

    if (!data)
        return -1;
    glGenTextures(1, &glTexture);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    if(nrChannels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    if(nrChannels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    ImageLoader::FreeImage(data);
    return 0;
}

void Texture::ActivateTexture() 
{
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glActiveTexture(GL_TEXTURE0);
}   