#define SDL_MAIN_HANDLED

#include <game/Game.h>

int main(int argc, char** argv)
{
    auto& game = Game::GetInstance();
    game.Init();
    game.Update();

    return 0;
}