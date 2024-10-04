#include "game.hpp"


Game::Game() 
{

}

void Game::Run() 
{
    bool running = true;
    while (running) {
        this->Update();
        this->Draw();
    }

    this->_display.Shutdown();
}

void Game::Update() 
{
    glfwPollEvents();
}

void Game::Draw() {
    this->_display.DrawStart();

    this->_display.DrawEnd();
}

void Game::Shutdown()
{
    this->_display.Shutdown();
}
