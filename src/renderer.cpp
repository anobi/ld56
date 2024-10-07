#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.hpp"

MeshID Renderer::AddMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    auto id = this->next_id++;
    auto mesh = Mesh(vertices, indices);
    this->meshes.emplace(id, mesh);
    return {id};
}

ShaderID Renderer::AddShader(const char *vertex_path, const char *fragment_path)
{
    auto id = this->next_id++;
    auto shader = Shader(vertex_path, fragment_path);
    this->shaders.emplace(id, shader);
    return {id};
}

void Renderer::Draw(std::vector<RenderObject> scene)
{
    ShaderID bound_shader{0};
    for (auto o : scene)
    {
        if (bound_shader != o.shader)
        {
            this->shaders.at(o.shader.id).use();
            bound_shader = o.shader;
        }
        auto tloc = glGetUniformLocation(this->shaders.at(bound_shader.id).ID, "transform");
        auto mvp = this->projection_matrix * o.transform;
        glUniformMatrix4fv(tloc, 1, GL_FALSE, glm::value_ptr(mvp));
        this->meshes.at(o.mesh.id).Draw();
    }
}

void Renderer::UpdatePerspective(float width, float height)
{
    float ratio = width / height;
    auto p = glm::ortho(ratio, -ratio, -1.0f, 1.0f, 1.0f, -1.0f);
    // auto p = glm::ortho(0.0f, width * ratio, 0.0f, height, 0.0f, 100.0f);
    this->projection_matrix = p;
}

Shader* Renderer::GetShaderRef(ShaderID shader)
{
    return &this->shaders.at(shader.id);
}
