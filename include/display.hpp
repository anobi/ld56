#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>




class Display {
public:    
    Display() {};

    int Init(int width, int height);
    void DrawStart();
    void DrawEnd();
    void Shutdown();

private:

};