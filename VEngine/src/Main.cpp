#include <Game.hpp>

//VEngine::Game* newGame;
int RunEngine()
{
    //newGame = new VEngine::Game{};
    return VEngine::Game::Get().EngineInit();
}
//static VEngine::Game* GetActiveGame() { return newGame; };

#ifdef _DEBUG
    int main() { return RunEngine(); }
#else
    int WinMain() { return RunEngine(); }
#endif