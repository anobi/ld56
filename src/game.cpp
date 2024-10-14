#include "game.hpp"

#include <random>
#include <fmt/core.h>

#include "primitives.hpp"
#include "ant.hpp"
#include "goop.hpp"
#include "baddie.hpp"


GameplayState gameState = GameplayState::NONE;
ShaderID shader;
ShaderID text_shader;

Baddie baddie;
MeshID baddie_mesh;

std::vector<Ant> goobers;
MeshID goober_mesh;
int goober_count = 0;


std::vector<Goop> goops;
MeshID goop_mesh;



void error_callback(int error, const char* description) {
    fmt::system_error(errno, "Error: {}", description);
}

void input_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        fmt::println("A");
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        gameState = GameplayState::QUIT_GAME;
    }

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        gameState = GameplayState::NEW_GAME;
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
        mouse_x = 1 - (mouse_x / 800) * 2.0f;
        mouse_y = 1 - (mouse_y / 600) * 2.0f;

        auto goop = Goop(goop_mesh, shader, glm::fvec3((float) mouse_x, (float) mouse_y, 0.0f));
        goops.push_back(goop);
    }
}

void Game::load_assets()
{
    shader = this->_renderer.AddShader("shaders/basic.vert", "shaders/basic.frag");
    
    auto goop_plane = plane_col(glm::fvec3(0.4f, 0.7f, 0.4f));
    goop_mesh = this->_renderer.AddMesh(goop_plane.vertices, goop_plane.indices);

    auto goober_plane = plane_col(glm::fvec3(0.1f));
    goober_mesh= this->_renderer.AddMesh(goober_plane.vertices, goober_plane.indices);

    auto baddie_plane = plane_col(glm::fvec3(0.8f, 0.2f, 0.2f));
    baddie_mesh = this->_renderer.AddMesh(baddie_plane.vertices, baddie_plane.indices);
}

void Game::load_game()
{
    goobers.clear();
    goops.clear();

    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(-1.0f, 1.0f);
    for (int i = 0; i < 10; i++) {
        auto goober = Ant(i+1, goober_mesh, shader);
        goober.Move(glm::fvec3(dis(e), dis(e), 0.0f));
        goobers.push_back(goober);
    }

    baddie = Baddie(baddie_mesh, shader);
    baddie.Move(glm::fvec3(dis(e), dis(e), 0.0f));

    goober_count = goobers.size();
}


void Game::Init()
{
    if (!glfwInit()) {
        throw fmt::system_error(errno, "Error: could not init glfw");
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    this->_window = glfwCreateWindow(this->_width, this->_height, "LD56 - Goop and Goobers", NULL, NULL);
    if (!this->_window) {
        glfwTerminate();
        throw fmt::system_error(errno, "Error: could not create glfw window");
    }
    glfwMakeContextCurrent(this->_window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glfwSetWindowShouldClose(this->_window, GLFW_FALSE);

    // Input setup
    glfwSetKeyCallback(this->_window, input_key_callback);
    glfwSetMouseButtonCallback(this->_window, input_mouse_button_callback);

    this->_renderer.Init(this->_width, this->_height);
}

void Game::Run()
{

    load_assets();

    // Set gamestate to load game data
    gameState = GameplayState::NEW_GAME;

    float delta_time = 0.0f;
    float last_frame = 0.0f;
    while (!glfwWindowShouldClose(this->_window)/*running*/) 
    {
        if (gameState == GameplayState::QUIT_GAME)
        {
            glfwSetWindowShouldClose(this->_window, GLFW_TRUE);
            continue;
        }

        // Load the game
        if (gameState == GameplayState::NEW_GAME) {
            this->load_game();
            gameState = GameplayState::RUNNING;
            continue;
        }

        // TODO: Separate into display update or start rendering or something
        this->Update();
        
        if (gameState == GameplayState::RUNNING)
        {
            // TODO: Renderer should hold this data?
            //       And then delete on demand instead of recreating on every frame.
            auto render_scene = std::vector<RenderObject>();

            auto goopies = std::vector<Goop*>();
            for (auto &goop : goops) 
            {
                goop.Update(delta_time);
                render_scene.push_back(goop.render_obj);
                goopies.push_back(&goop);
            }

            auto goober_locations = std::vector<glm::fvec3>();
            for (auto &goobie : goobers) 
            {
                goobie.Update(goopies, baddie.position);
                render_scene.push_back(goobie.render_obj);
                goober_locations.push_back(goobie.position);
            }

            baddie.Update(goober_locations);
            render_scene.push_back(baddie.render_obj);

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
            goober_count = goobers.size();
            if (goober_count <= 0)
            {
                gameState = GameplayState::GAME_OVER;
            }

            this->_renderer.Draw(render_scene);
            auto l_score = fmt::format("Goobers alive: {}", goober_count);
            this->_renderer.DrawText(l_score, glm::fvec2(10.0f, 10.0f), 0.25f, glm::fvec3(0.1f));
        }
        else if (gameState == GameplayState::GAME_OVER) // Game over
        {
            this->_renderer.DrawText("Game Over!", glm::fvec2(120.0f, 250.0f), 1.0f, glm::fvec3(0.1f));
            this->_renderer.DrawText("Press F1 to restart, ESC to quit", glm::fvec2(200.0f, 400.0f), 0.25f, glm::fvec3(0.1f));
            fmt::println("All goobers eaten! Game Over!");
        }

        glfwSwapBuffers(this->_window);
        glfwPollEvents();
    }

    // Tear down the game
    glfwDestroyWindow(this->_window);
    glfwTerminate();
}

void Game::Update() 
{
    glfwGetFramebufferSize(this->_window, &this->_width, &this->_height);
    
    glViewport(0, 0, this->_width, this->_height);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    this->_renderer.UpdatePerspective((float)this->_width, (float)this->_height);
}

void Game::Draw() {
}

void Game::Shutdown()
{
}
