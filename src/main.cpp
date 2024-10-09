#include "game.hpp"


int App() 
{
    auto game = Game();
    game.Init();
    game.Run();

    return 0;
}


#ifdef _WIN32
#include <Windows.h>
int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    return App();
}

#else
int main() 
{
    return App();
}
#endif


