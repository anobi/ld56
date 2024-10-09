#include <cstdio>
#include <cstddef>


#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/core.h>

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


