#pragma once

#include <vector>
#include <glm/glm.hpp>


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


class Mesh 
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void Draw();
private:
    GpuMesh _gpu_mesh;
};