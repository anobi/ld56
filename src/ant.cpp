#include <random>
#include <iostream>
#include <fmt/core.h>

#include "ant.hpp"


Ant::Ant(unsigned int ant_id, MeshID mesh_id, ShaderID shader_id)
{
    this->ant_id = ant_id;
    this->tick = 0;
    this->position = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->velocity = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::fvec3(0.02f);

    this->speed = 0.001f;

    this->render_obj.mesh = mesh_id;
    this->render_obj.shader = shader_id;
    this->render_obj.is_dirty = true;
}

void Ant::Update(std::vector<Goop*> goops)
{
    this->tick++;

    if (current_goal == WANDER)
    {
        if (this->target == glm::zero<glm::fvec3>()) {
            this->target = wander();
        }

        Goop* most_important_goop = nullptr;
        auto most_important_goop_score = 0.0f;
        for (auto goop : goops)
        {
            auto distance = glm::length(goop->position - this->position);

            // Don't react if we're too close
            if (distance < this->scoop_min_distance)
            {
                continue;
            }

            auto score = goop->strength * distance;
            if ( score > most_important_goop_score )
            {
                most_important_goop_score = score;
                most_important_goop = goop;
            }
        }
        if (most_important_goop_score) {
            this->current_goal = GOOPING;
            this->target_goop = most_important_goop;
            this->target = most_important_goop->position;
            fmt::println("Ant {} : chasing goober at [{}, {}]", this->ant_id, this->target.x, this->target.y);
        }
        else {
            // Wander to a new target if at destination
            if (glm::length(this->target - this->position) < 0.05f)
            {
                this->target = wander();
            }

            // Calculate velocity towards the wander
            auto target_dir = (this->target - this->position) * this->speed;
            auto steering_vector = target_dir - this->velocity;
            if(glm::length(steering_vector) > 1.0f) {
                steering_vector *= 1 - 0.001 / glm::length(steering_vector);
            }

            this->velocity += steering_vector;
        }
    }
    else if (current_goal == GOOPING) 
    {
        if (glm::length(this->target - this->position) < 0.05f || this->target_goop->dead)
        {
            this->current_goal = WANDER;
            this->target = wander();
            fmt::println("Ant {} wandering", this->ant_id);
        }
        else
        {
            this->velocity = seek(this->target);
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
