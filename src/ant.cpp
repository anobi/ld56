#include <random>
#include <iostream>
#include <fmt/core.h>

#include "ant.hpp"


glm::fvec3 Ant::wander()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(-1.0f, 1.0f);
    auto jitter = 1.5f;
    auto ring_distance = 1.0f;
    auto ring_radius = 0.2;

    auto new_target = glm::fvec3(dis(e) * jitter, dis(e) * jitter, 0.0f);
    new_target = glm::normalize(new_target);
    new_target *= ring_radius;
    new_target -= this->position;

    fmt::println("Ant {} : new target at [{}, {}]", this->ant_id, new_target.x, new_target.y);

    return new_target;
}

glm::fvec3 Ant::seek(glm::fvec3 goop)
{
    auto new_velocity = glm::normalize(goop - this->position) * this->speed;
    return new_velocity - this->velocity;
}

glm::fvec3 Ant::flee(glm::fvec3 scary)
{
    
}




Ant::Ant(unsigned int ant_id, MeshID mesh_id, ShaderID shader_id)
{
    this->ant_id = ant_id;
    this->tick = 0;
    this->position = glm::fvec3(0.0f, 0.0f, 1.0f);
    this->scale = glm::fvec3(0.02f);

    this->velocity = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->target = wander();
    this->speed = 0.001f;

    this->render_obj.mesh = mesh_id;
    this->render_obj.shader = shader_id;
    this->render_obj.is_dirty = true;
}

void Ant::Update(std::vector<Goober> goops)
{
    this->tick++;

    if (current_goal == WANDER)
    {
        auto most_important_goop_position = glm::fvec3(0.0f);
        auto most_important_goop_score = 0.0f;
        for (auto goop : goops)
        {
            auto distance = glm::length(goop.position - this->position);
            auto score = goop.strength * distance;
            if ( score > most_important_goop_score )
            {
                most_important_goop_score = score;
                most_important_goop_position = goop.position;
            }
        }
        if (most_important_goop_score) {
            this->current_goal = GOOPING;
            this->target = most_important_goop_position;
            fmt::println("Ant {} : chasing goober at [{}, {}]", this->ant_id, this->target.x, this->target.y);
        }
        else {
            // Wander to a new target if at destination
            if (glm::length(this->target - this->position) < 0.05f)
            {
                this->target = wander();
            }

            auto target_dir = (this->target - this->position) * this->speed;
            auto steering_vector = target_dir - this->velocity;
            if(glm::length(steering_vector) > 1.0f) {
                steering_vector *= 1 - 0.001 / glm::length(steering_vector);
            }

            this->velocity += steering_vector;
        }
    }
    else if (current_goal == GOOPING) {
        this->velocity = seek(this->target);
        if (glm::length(this->target - this->position) < 0.05f)
        {
            this->current_goal = WANDER;
            fmt::println("Ant {} wandering", this->ant_id);
        }
    }

    this->velocity.z = 0.0f;
    auto new_position = this->position + this->velocity;
    new_position.z = 0.0f;
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
