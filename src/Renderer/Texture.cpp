#include "Texture.h"

Texture::Texture()
{
	// Generate Texture
	glGenTextures(1, &ID);
}
Texture::Texture(const char* path)
{
	// Generate Texture
	glGenTextures(1, &ID);
	this->SetTexture(path);
}

void Texture::SetTexture(const char* path)
{
	// Create width & height to handle image size
	int width, height, nrChannels;
	// Load image to *data (unsigned char array)
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	// If cannot load data(wrong source file)
	assert(data != NULL);

	this->m_Width = width;
	this->m_Height = height;

	GLenum format;
	if (nrChannels == 1)
	{
		format = GL_RED;
	}
	else if (nrChannels == 3)
	{
		format = GL_RGB;
	}
	else if (nrChannels == 4)
	{
		format = GL_RGBA;
	}

	// Binding and setting-up texture
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Specific Texture with image data
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);


	// free data
	stbi_image_free(data);
}

void Texture::Activate(GLenum TextureSlot)
{
	// Activate TextureSlot and Bind it
	glActiveTexture(TextureSlot);
	glBindTexture(GL_TEXTURE_2D, ID);
}
void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}
void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}

// ----------------- Getter -----------------
glm::vec2 Texture::GetImageSize() const
{
	return glm::ivec2(this->m_Width, this->m_Height);
}
GLuint Texture::GetImageWidth() const
{
	return this->m_Width;
}
GLuint Texture::GetImageHeight() const
{
	return this->m_Height;
}