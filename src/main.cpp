#include <cstdio>
#include <cstddef>
#include <random>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fmt/core.h>

#include "renderer.hpp"
#include "primitives.hpp"
#include "shader.hpp"
#include "ant.hpp"
#include "goop.hpp"
#include "baddie.hpp"


int width = 800;
int height = 600;
GLFWwindow* window;

Renderer renderer;

std::vector<Ant> goobers;
std::vector<Goop> goops;

ShaderID shader;
MeshID goop_mesh;


void input_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        printf("A");
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void input_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {
        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        fmt::println("Mouse 1 clicked at [{}, {}]", mouse_x, mouse_y);

        // Translate coordinates
        mouse_x = 1 - (mouse_x / width) * 2.0f;
        mouse_y = 1 - (mouse_y / height) * 2.0f;

        auto goop = Goop(goop_mesh, shader, glm::fvec3((float) mouse_x, (float) mouse_y, 0.0f));
        goops.push_back(goop);
    }
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

    window = glfwCreateWindow(width, height, "LD56 - Goops and Goobers", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    // Input setup
    glfwSetKeyCallback(window, input_key_callback);
    glfwSetMouseButtonCallback(window, input_mouse_button_callback);

    srand (static_cast <unsigned> (time(0)));

    // Load "content"
    shader = renderer.AddShader("shaders/basic.vert", "shaders/basic.frag");

    auto goop_plane = plane_col(glm::fvec3(0.4f, 0.7f, 0.4f));
    goop_mesh = renderer.AddMesh(goop_plane.vertices, goop_plane.indices);
    
    auto ant_plane = plane_col(glm::fvec3(0.1f));
    auto plane_mesh = renderer.AddMesh(ant_plane.vertices, ant_plane.indices);

    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(-1.0f, 1.0f);
    for (int i = 1; i < 10; i++) {
        auto goober = Ant(i, plane_mesh, shader);
        goober.Move(glm::fvec3(dis(e), dis(e), 0.0f));
        goobers.push_back(goober);
    }

    auto baddie_plane = plane_col(glm::fvec3(0.8f, 0.2f, 0.2f));
    auto baddie_mesh = renderer.AddMesh(baddie_plane.vertices, baddie_plane.indices);
    auto baddie = Baddie(baddie_mesh, shader);

    baddie.Move(glm::fvec3(dis(e), dis(e), 0.0f));

    // Run the game
    float delta_time = 0.0f;
    float last_frame = 0.0f;
    auto running = true;
    auto goober_count = goobers.size();
    while (!glfwWindowShouldClose(window)/*running*/) {
        glfwGetFramebufferSize(window, &width, &height);
        
        glViewport(0, 0, width, height);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.UpdatePerspective(width, height);

        auto scene = std::vector<RenderObject>();

        if (goober_count > 0)
        {
            auto goopies = std::vector<Goop*>();
            for (auto &goop : goops) 
            {
                goop.Update(delta_time);
                scene.push_back(goop.render_obj);
                goopies.push_back(&goop);
            }

            auto goober_locations = std::vector<glm::fvec3>();
            for (auto &goobie : goobers) 
            {
                goobie.Update(goopies, baddie.position);
                scene.push_back(goobie.render_obj);
                goober_locations.push_back(goobie.position);
            }

            baddie.Update(goober_locations);
            scene.push_back(baddie.render_obj);

            // Clear dead goops
            for (int i = 0; i < goops.size(); i++) {
                auto goop = &goops[i];
                if (goop->dead) {
                    goops.erase(goops.begin() + i);
                }
            }

            // Clear dead goobers
            for (int i = 0; i < goobers.size(); i++) {
                auto goober = &goobers[i];
                if (goober->dead) {
                    goobers.erase(goobers.begin() + i);
                }
            }
        }
        else // Game over
        {
            
        }

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


