#include <Block.h>

#include <Texture.h>

#include <Constants.h>

Block::Block(SDL_Renderer* sdlRenderer, int x, int y, Color color)
{
    m_Texture = std::make_shared<Texture>(sdlRenderer, BlockSize, BlockSize, color);
    m_Texture->SetPositionOnScreen(x, y);
}

auto Block::Draw(SDL_Renderer *sdlRenderer, uint8_t alpha) -> void
{
    if (!m_Texture)
        return;

    m_Texture->Draw(sdlRenderer, alpha);
}
