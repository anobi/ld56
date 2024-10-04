#include "display.hpp"


int Display::Init(int width, int height) 
{
    if (!glfwInit()) {
        return -1;
    }

    this->_width = width;
    this->_height = height;
    this->_window = glfwCreateWindow(this->_width, this->_height, "LD56", NULL, NULL);
    if (!this->_window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(this->_window);
    int version = gladLoadGL(glfwGetProcAddress);

    return 0;
}

void Display::DrawStart()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Display::DrawEnd()
{
    glfwSwapBuffers(this->_window);
}

void Display::Shutdown()
{
    glfwTerminate();
}
