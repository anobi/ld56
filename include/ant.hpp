#pragma once

#include <glm/gtc/quaternion.hpp>

#include "renderable.hpp"
#include "shader.hpp"

class Ant 
{
public:
    Ant(unsigned int ant_id, MeshID mesh_id, ShaderID shader_id);
    void Update();

    void Move(glm::fvec3 new_position);
    void Rotate(float angle);
    void Scale(glm::fvec3 new_scale);

    glm::fvec3 target;
    glm::fvec3 velocity;
    float speed;

    glm::fvec3 position;
    glm::fvec3 scale;
    float rotation_angle;

    RenderObject render_obj;

    unsigned int ant_id;
    unsigned int tick;

private:
    glm::fvec3 wander();
    glm::fvec3 wander_ring();
};