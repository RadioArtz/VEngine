#include <Rendering/Texture.h>
#include <string>
#include <IO/ImageLoader.h>
#include <glad/glad.h>


int Texture::SetupGLTexture(int guh)
{
    if(guh == 1)
        imagePath = "C:/Users/mathi/Pictures/contain soundcloud.png";
    if(guh == 2)
        imagePath = "C:/Users/mathi/Pictures/how do i fit him sfw.png";
    ImageLoader::LoadImage(imagePath, &data, &width, &height, &nrChannels);

    if (!data)
        return -1;
    glGenTextures(1, &glTexture);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    if(nrChannels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else if(nrChannels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
    ImageLoader::FreeImage(data);
    return 0;
}

void Texture::ActivateTexture(int TextureUnit) 
{
    glActiveTexture(GL_TEXTURE0 + TextureUnit);
    glBindTexture(GL_TEXTURE_2D, glTexture);
}