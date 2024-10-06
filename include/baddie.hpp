#pragma once

#include <glm/gtc/quaternion.hpp>

#include "renderable.hpp"
#include "shader.hpp"

enum BaddieBehavior
{
    PROWL,
    HUNT
};

class Baddie 
{
public:
    Baddie(MeshID mesh_id, ShaderID shader_id);
    void Update(std::vector<glm::fvec3> goobers);
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

private:
    int tick;
    int aggression_cooldown;
    float scoop_interest_threshold = 0.5;
    float scoop_min_distance = 0.05;
    BaddieBehavior current_behavior = PROWL;

    glm::fvec3 wander();
    glm::fvec3 chase(glm::fvec3 goober);
};