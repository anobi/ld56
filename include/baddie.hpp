#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    // float speed;

    glm::fvec3 position;
    glm::fvec3 scale;
    float rotation_angle = 0.0f;

    RenderObject render_obj;

private:
    int tick = 0;
    int aggression_cooldown = 0;
    float scoop_interest_threshold = 0.5f;
    float scoop_min_distance = 0.05f;
    BaddieBehavior current_behavior = PROWL;

    glm::fvec3 wander();
    glm::fvec3 chase(glm::fvec3 goober);
};