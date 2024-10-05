#include <glad/gl.h>
#include "renderable.hpp"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) 
{
    glGenVertexArrays(1, &this->_gpu_mesh.VAO);
    glGenBuffers(1, &this->_gpu_mesh.VBO);
    glGenBuffers(1, &this->_gpu_mesh.EBO);

    glBindVertexArray(this->_gpu_mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->_gpu_mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_gpu_mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    
    // TODO: Normal

    // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindVertexArray(0);
}

void Mesh::Draw() 
{
    glBindVertexArray(this->_gpu_mesh.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}