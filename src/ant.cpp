#include <random>
#include <iostream>
#include <fmt/core.h>

float get_random()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(0.0f, 1.0f);
    return dis(e);
}

#include "ant.hpp"

glm::fvec3 Ant::wander() {
    static std::default_random_engine e;
    static std::uniform_real_distribution<> disx(0.0f, 2.0f);
    static std::uniform_real_distribution<> disy(0.0f, 2.0f);
    auto new_target = glm::fvec3(1.0f-disx(e), 1.0f-disy(e), 0.0f);

    fmt::println("Ant {} : new target at [{}, {}]", this->ant_id, new_target.x, new_target.y);

    return new_target;
}


Ant::Ant(unsigned int ant_id, MeshID mesh_id, ShaderID shader_id)
{
    this->ant_id = ant_id;
    this->tick = 0;
    this->position = glm::fvec3(0.0f);
    this->scale = glm::fvec3(0.02f);

    this->velocity = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->target = wander();
    this->speed = 0.001f;

    this->render_obj.mesh = mesh_id;
    this->render_obj.shader = shader_id;
    this->render_obj.is_dirty = true;
}

void Ant::Update()
{
    this->tick++;

    // Wander
    if (this->tick % 240 == 0)
    {
        this->target = wander();
    }
    
    auto target_dir = (this->target - this->position) * this->speed;
    auto steering_vector = target_dir - this->velocity;
    this->velocity += steering_vector;
    auto new_position = this->position + this->velocity;
    Move(new_position);

    // Update render matrix
    if(this->render_obj.is_dirty)
    {
        auto transform = glm::mat4(1.0f);
        transform = glm::translate(transform, this->position);
        transform = glm::rotate(transform, this->rotation_angle, glm::vec3(0, 0, 1));
        transform = glm::scale(transform, this->scale);
        this->render_obj.transform = transform;
        this->render_obj.is_dirty = false;
    }
}

void Ant::Move(glm::fvec3 new_position)
{
    if(new_position.x > 1.0f) {
        new_position.x = -1.0f;
    }
    if(new_position.x < -1.0f) {
        new_position.x = 1.0f;
    }
    if(new_position.y > 1.0f) {
        new_position.y = -1.0f;
    }
    if(new_position.y < -1.0f) {
        new_position.y = 1.0f;
    }

    this->position = new_position;
    this->render_obj.is_dirty = true;
}

void Ant::Rotate(float angle)
{
    this->rotation_angle = angle;
    this->render_obj.is_dirty = true;
}

void Ant::Scale(glm::fvec3 new_scale)
{
    this->scale = new_scale;
    this->render_obj.is_dirty = true;
}
