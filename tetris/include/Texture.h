#pragma once

#include <Color.h>

#include <tuple>
#include <string>
#include <SDL2/SDL.h>

class Texture
{
public:
	/**
	 * \brief Class destructor
	 */
	~Texture();

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

	auto SetPositionOnScreen(int x, int y) -> void;

	auto GetPositionOnScreen() const -> std::tuple<int, int> { return std::make_tuple(m_Quad.x, m_Quad.y); }

private:
	uint32_t m_Width = {};
	uint32_t m_Height = {};
	SDL_Texture* m_SdlTexture = {};
	SDL_Rect m_Quad = {};
};