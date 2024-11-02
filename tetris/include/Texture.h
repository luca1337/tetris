#pragma once

#include <Color.h>

#include <tuple>
#include <string>

#include <glad/glad.h>
#include <glm/vec2.hpp>

class Texture
{
public:
	/**
	 * \brief Class destructor
	 */
	~Texture();

	/**
	 * \brief Class custom copy-constructor
	 */
	Texture(const Texture &);

	/**
	 * \brief Load texture from path (.jpg, .png. etc..)
	 * \param filePath where is the texture located?
	 */
	explicit Texture(const std::string &filePath);

	/**
	 * \brief Create a raw texture from memory with a color, width and height
	 * \param width width in pixels
	 * \param height height in pixels
	 * \param color color of the texture
	 */
	Texture(unsigned width, unsigned height, Color color);

	/**
	 * \brief Bind the texture
	 */
	void Bind() const;

	/**
	 * \brief Get ID associated to this Texture
	 */
	auto GetID() const { return m_TextureID; }

	auto GetWidth() const { return m_Width; }
	auto GetHeight() const { return m_Height; }

private:
	GLuint m_TextureID = {};
	unsigned m_Width = {};
	unsigned m_Height = {};

	void GenerateTexture(unsigned width, unsigned height, const unsigned char *data);
};