#include <random>
#include <iostream>
#include <fmt/core.h>

#include "baddie.hpp"

constexpr int AGGRESSION_COOLDOWN = 300;
constexpr float MIN_DISTANCE = 0.01f;
constexpr float BADDIE_CHASE_SPEED = 0.007f;
constexpr float BADDIE_WANDER_SPEED = 0.001f;


glm::fvec3 Baddie::wander()
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(-1.0f, 1.0f);
    auto jitter = 1.5f;
    auto ring_radius = 1.2;

    auto new_target = glm::normalize(glm::fvec3(dis(e) * jitter, dis(e) * jitter, 0.0f));
    new_target *= ring_radius;

    // Transform the target to a poit around the current position
    auto world_target = this->position + new_target;
    world_target = glm::clamp(world_target, glm::fvec3(-1.0f, -1.0f, 0.0f), glm::fvec3(1.0f, 1.0f, 0.0f));

    return world_target;
}

glm::fvec3 Baddie::chase(glm::fvec3 goober)
{
    auto new_velocity = glm::normalize(goober - this->position) * BADDIE_CHASE_SPEED;
    return new_velocity - this->velocity;
}

Baddie::Baddie(MeshID mesh_id, ShaderID shader_id)
{
    this->position = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->velocity = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::fvec3(0.06f);

    // this->speed = 0.001f;

    this->render_obj.mesh = mesh_id;
    this->render_obj.shader = shader_id;
    this->render_obj.is_dirty = true;

    fmt::println("Baddie spawned");
}

void Baddie::Update(std::vector<glm::fvec3> goobers)
{
    this->tick += 1;

    if (current_behavior == PROWL)
    {
        if (this->aggression_cooldown > 0) {
            this->aggression_cooldown -= 1;
        }

        glm::fvec3 closest_goober;
        float closest_goober_distance = 999.0f;
        for (auto g : goobers)
        {
            auto distance = glm::length(g - this->position);

            // Don't react if we're too close
            if (distance < closest_goober_distance) 
            {
                closest_goober = g;
            }
        }
        if (this->aggression_cooldown == 0) {
            this->current_behavior = HUNT;
            this->target = closest_goober;
            this->aggression_cooldown = AGGRESSION_COOLDOWN;
            fmt::println("Baddie [{}, {}]: found a new prey [{}, {}]", this->position.x, this->position.y, closest_goober.x, closest_goober.y);
        }
        else {
            // Reached current wander target, find a new spot to wander to
            auto distance_to_target = glm::length(this->target - this->position);
            if (distance_to_target < MIN_DISTANCE) {
                this->target = wander();
            }
        }
    }
    else if (current_behavior == HUNT)
    {
        if (glm::length(this->target - this->position) < MIN_DISTANCE)
        {
            this->current_behavior = PROWL;
            this->target = wander();
            fmt::println("Baddie is on the hunt");
        }
        // else
        // {
        //     this->velocity = chase(this->target);
        // }
    }

    // Calculate velocity towards the target
    auto move_speed = BADDIE_WANDER_SPEED;
    if (current_behavior == HUNT) {
        move_speed = BADDIE_CHASE_SPEED;
    }

    auto target_dir = (this->target - this->position) * move_speed;
    auto steering_vector = target_dir - this->velocity;
    if(glm::length(steering_vector) > 1.0f) {
        steering_vector *= 1 - 0.001 / glm::length(steering_vector);
    }

    this->velocity += steering_vector;
    this->velocity.z = 0.0f;

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

void Baddie::Move(glm::fvec3 new_position)
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

void Baddie::Rotate(float angle)
{
    this->rotation_angle = angle;
    this->render_obj.is_dirty = true;
}

void Baddie::Scale(glm::fvec3 new_scale)
{
    this->scale = new_scale;
    this->render_obj.is_dirty = true;
}
