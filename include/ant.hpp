#pragma once

#include <glm/gtc/quaternion.hpp>

#include "renderable.hpp"
#include "shader.hpp"

class Ant 
{
public:
    Ant(MeshID mesh_id, ShaderID shader_id);
    void Update();

    void Move(glm::fvec3 new_position);
    void Rotate(float angle);
    void Scale(glm::fvec3 new_scale);

    glm::vec3 position;
    glm::vec3 scale;
    float rotation_angle;

    RenderObject render_obj;
};