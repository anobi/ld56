#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <glm/glm.hpp>

#include "renderable.hpp"
#include "shader.hpp"



// Text rendering

struct Character {
    unsigned int textureId;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class TextRenderer
{
public:
    TextRenderer() {};
    TextRenderer(unsigned int width, unsigned int height, Shader* shader);
    void LoadFont(const char* font, unsigned int size);
    void Draw(std::string text, glm::fvec2 position, float scale, glm::fvec3 color);
    std::map<char, Character> characters;
private:
    GpuMesh gpuMesh;
    // ShaderID shader;
    Shader* shader;
};


// Rendering

class Renderer 
{
public:
    void Init(unsigned int width, unsigned int height);
    MeshID AddMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ShaderID AddShader(const char* vertex_path, const char* fragment_path);
    void Draw(std::vector<RenderObject> scene);
    void DrawText(std::string text, glm::fvec2 position, float scale, glm::fvec3 color);
    void UpdatePerspective(float width, float height);
    Shader* GetShaderRef(ShaderID shader);
private:
    glm::mat4 projection_matrix;
    unsigned int next_id = 1;
    std::unordered_map<unsigned int, Mesh> meshes;
    std::unordered_map<unsigned int, Shader> shaders;

    TextRenderer text_renderer;
    ShaderID text_shader;
};