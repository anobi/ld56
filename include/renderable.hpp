#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "shader.hpp"

struct ShaderID
{
    operator bool() const noexcept { return id != 0; }
    unsigned int id {};
};

struct MeshID
{
    operator bool() const noexcept { return id != 0; }
    unsigned int id {};
};

struct GpuMesh
{
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
};

struct Vertex {
    glm::fvec3 position;
    glm::fvec3 color;
};

struct RenderObjectID 
{
    operator bool() const noexcept { return id != 0; }
    unsigned int id {};
};

struct RenderObject 
{
    glm::fmat4 transform;
    bool is_dirty;

    ShaderID shader;
    MeshID mesh;
};

class Mesh 
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void Draw();
private:
    GpuMesh _gpu_mesh;
};