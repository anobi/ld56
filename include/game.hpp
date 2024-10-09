#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "renderer.hpp"
#include "text_rendering.hpp"


enum GameplayState 
{
    ERROR = -1,
    NONE,
    NEW_GAME,
    RUNNING,
    GAME_OVER,
    QUIT_GAME
};


class Game {
public:
    Game() {};
    void Run();
    void Init();

private:
    int _tick = 0;

    GLFWwindow* _window;
    int _width = 800;
    int _height = 600;

    Renderer _renderer;
    TextRenderer _text_renderer;

    void Update();
    void Draw();
    void Shutdown();

    void load_game();
    void load_assets();

    //void input_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};