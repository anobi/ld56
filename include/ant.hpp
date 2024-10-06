#pragma once

#include <glm/gtc/quaternion.hpp>

#include "renderable.hpp"
#include "shader.hpp"
#include "goop.hpp"


enum AntBehavior
{
    WANDER,
    GOOPING,
    FLEEING
};

class Ant 
{
public:
    Ant(unsigned int ID, MeshID mesh_id, ShaderID shader_id);
    void Update(std::vector<Goop*> goops, glm::fvec3 baddie);

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

    unsigned int ID;
    unsigned int tick;
    bool dead = false;

private:
    float scoop_interest_threshold = 0.5;
    float scoop_min_distance = 0.05;
    AntBehavior current_goal = WANDER;
    Goop* target_goop = nullptr;
    
    glm::fvec3 wander();
    glm::fvec3 seek(glm::fvec3 location);
    glm::fvec3 flee(glm::fvec3 scary);
};