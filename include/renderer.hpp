#pragma once

#include <vector>
#include <unordered_map>

#include "renderable.hpp"
#include "shader.hpp"


class Renderer 
{
public:
    MeshID AddMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    ShaderID AddShader(const char* vertex_path, const char* fragment_path);
    void Draw(std::vector<RenderObject> scene);
    void UpdatePerspective(float width, float height);
private:
    glm::mat4 projection_matrix;
    unsigned int next_id = 1;
    std::unordered_map<unsigned int, Mesh> meshes;
    std::unordered_map<unsigned int, Shader> shaders;
};