#pragma once

#include <Color.h>

#include <tuple>
#include <string>

#include <SDL2/SDL.h>
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
	Texture(const Texture&);

	/**
	 * \brief Load texture from path (.jpg, .png. etc..)
	 * \param sdlRenderer SDL Renderer handle
	 * \param filePath where is the texture located?
	 * \param useBlending whether the texture should be blended or not
	 */
	explicit Texture(SDL_Renderer* sdlRenderer, const std::string& filePath, bool useBlending);

	/**
	 * \brief Create a raw texture from memory with a color, width and height
	 * \param sdlRenderer SDL Renderer handle
	 * \param width width in pixels
	 * \param height height in pixels
	 * \param color color of the texture
	 */
	Texture(SDL_Renderer* sdlRenderer, unsigned width, unsigned height, Color color);

	/**
	 * \brief Draw texture on screen
	 * \param sdlRenderer SDL Renderer handle
	 * \param alpha alpha aka transparency (0 to 255 -> fully opaque)
	 */
	auto Draw(SDL_Renderer* sdlRenderer, uint8_t alpha) const -> void;

	auto SetSize(const glm::vec2& size) { m_Quad.w = size.x; m_Quad.h = size.y; }
	auto SetPositionOnScreen(int x, int y) -> void;

	auto GetPositionOnScreen() const -> std::tuple<int, int> { return std::make_tuple(m_Quad.x, m_Quad.y); }
	auto GetSizeInPixel() const { return std::make_tuple(m_Quad.w, m_Quad.h); }

private:
	int m_Width = {};
	int m_Height = {};
	SDL_Texture* m_SdlTexture = {};
	SDL_Renderer* m_SdlRenderer = {};
	SDL_FRect m_Quad = {};
	Color m_Color = {};
	bool m_IsLoadedFromFile = {};
	bool m_UseBlending = {};
	std::string m_Path = {};
	unsigned char* m_Pixels = {};
};