#pragma once

#include <glm/gtc/quaternion.hpp>

#include "renderable.hpp"
#include "shader.hpp"


struct Goober
{
    float strength;
    glm::fvec3 position;
};

enum Behavior
{
    WANDER,
    GOOPING,
    FLEEING
};

class Ant 
{
public:
    Ant(unsigned int ant_id, MeshID mesh_id, ShaderID shader_id);
    void Update(std::vector<Goober> goops);

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
    float scoop_interest_threshold = 0.5;
    Behavior current_goal = WANDER;
    glm::fvec3 wander();
    glm::fvec3 seek(glm::fvec3 goop);
    glm::fvec3 flee(glm::fvec3 scary);
};