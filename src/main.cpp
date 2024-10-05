#include <cstdio>
#include <cstddef>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderer.hpp"
#include "primitives.hpp"
#include "shader.hpp"
#include "ant.hpp"


int width = 800;
int height = 600;
GLFWwindow* window;

Renderer renderer;


void input_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        printf("A");

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}


int Game() 
{
    if (!glfwInit()) {
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    window = glfwCreateWindow(width, height, "LD56", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    // Input setup
    glfwSetKeyCallback(window, input_key_callback);

    // Tringaly stuff
    auto shader = renderer.AddShader("shaders/basic.vert", "shaders/basic.frag");
    auto plane_mesh = renderer.AddMesh(plane.vertices, plane.indices);

    auto ant = Ant(plane_mesh, shader);


    auto running = true;
    while (!glfwWindowShouldClose(window)/*running*/) {
        glfwGetFramebufferSize(window, &width, &height);
        
        glViewport(0, 0, width, height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.UpdatePerspective(width, height);

        auto scene = std::vector<RenderObject>();
        ant.Rotate((float)glfwGetTime());
        ant.Update();
        scene.push_back(ant.render_obj);

        renderer.Draw(scene);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

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
    return Game();
}

#else
int main() 
{
    return Game();
}
#endif


