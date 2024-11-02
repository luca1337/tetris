#include <Texture.h>

#include <Logger.h>
#include <GLUtils.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::~Texture()
{
    glDeleteTextures(1, &m_TextureID);
}

Texture::Texture(const Texture &other) : m_Width(other.m_Width), m_Height(other.m_Height)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glCopyImageSubData(other.m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0, m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0, m_Width, m_Height, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(const std::string &filePath)
{
    int width, height, nrChannels;
    const auto data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        m_Width = width;
        m_Height = height;

        const auto format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        m_TextureID = glutils::GenerateOpenGLTexture(m_Width, m_Height, data, format, format, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        stbi_image_free(data);
    }
    else
    {
        LOG_CRITICAL("Failed to load texture");
    }
}

Texture::Texture(unsigned width, unsigned height, Color color) : m_Width(width), m_Height(height)
{
    auto data = new unsigned char[width * height * 4];
    for (unsigned i = 0; i < width * height; ++i)
    {
        data[i * 4 + 0] = color.r;
        data[i * 4 + 1] = color.g;
        data[i * 4 + 2] = color.b;
        data[i * 4 + 3] = color.a;
    }

    m_TextureID = glutils::GenerateOpenGLTexture(width, height, data, GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, true);

    delete[] data;
}