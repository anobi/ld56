#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Game {
public:
    Game();
    void Run();


private:
    int _tick = 0;
    // Display _display;

    GLFWwindow* _window;
    int _width = 800;
    int _height = 600;

    void Update();
    void Draw();
    void Shutdown();
};