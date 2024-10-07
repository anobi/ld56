#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderable.hpp"
#include "shader.hpp"

class Goop {
public:
    Goop(MeshID mesh_id, ShaderID shader_id, glm::fvec3 pos);

    void Update(float dt);

    glm::fvec3 position;
    glm::fvec3 scale;
    float rotation_angle = 0.0f;
    float rotation_dir = 0.0f;

    float lifetime = 100.0f;
    float radius = 0.10f;
    float strength = 1.0f;
    float decay = 0.001f;
    bool dead = false;

    RenderObject render_obj;
};