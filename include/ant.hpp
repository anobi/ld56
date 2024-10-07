#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderable.hpp"
#include "shader.hpp"
#include "goop.hpp"


enum AntBehavior
{
    WANDER,
    GOOPING,
    FLEEING
};


// This was supposed to be an ant game
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
    //float speed;

    glm::fvec3 position;
    glm::fvec3 scale;
    float rotation_angle = 0.0f;

    RenderObject render_obj;

    unsigned int ID = 0;
    unsigned int tick = 0;
    bool dead = false;

private:
    float goop_interest_threshold = 0.5f;
    float goop_min_distance = 0.05f;
    float current_goop_score = 0.0f;
    AntBehavior current_goal = WANDER;
    
    glm::fvec3 wander();
    glm::fvec3 seek(glm::fvec3 location);
    glm::fvec3 flee(glm::fvec3 scary);
};