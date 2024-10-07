#include <random>
#include <iostream>
#include <fmt/core.h>

#include "ant.hpp"

constexpr float BE_EATEN_DISTANCE = 0.05f;


glm::fvec3 Ant::wander()
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

    fmt::println("Goober {} [{}, {}]: wandering to [{}, {}]", this->ID, this->position.x, this->position.y, new_target.x, new_target.y);

    return world_target;
}

glm::fvec3 Ant::seek(glm::fvec3 goop)
{
    auto new_velocity = glm::normalize(goop - this->position) * this->speed;
    return new_velocity - this->velocity;
}

glm::fvec3 Ant::flee(glm::fvec3 scary)
{
    return glm::fvec3();
}

Ant::Ant(unsigned int ID, MeshID mesh_id, ShaderID shader_id)
{
    this->ID = ID;
    this->tick = 0;
    this->position = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->velocity = glm::fvec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::fvec3(0.02f);

    this->speed = 0.001f;

    this->render_obj.mesh = mesh_id;
    this->render_obj.shader = shader_id;
    this->render_obj.is_dirty = true;
}

void Ant::Update(std::vector<Goop*> goops, glm::fvec3 baddie)
{
    this->tick++;

    auto distance_to_baddie = glm::length(baddie - this->position);
    if (distance_to_baddie < BE_EATEN_DISTANCE)
    {
        this->dead = true;
        fmt::println("Goober {} was eaten!", this->ID);
    }

    if (this->target == glm::zero<glm::fvec3>()) {
        this->target = wander();
    }

    if (current_goal == WANDER)
    {
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

        // Check if current goop is alive and/or there are more interesting goops nearby
        auto current_goop_score = 0.0f;
        if (this->target_goop) {
            auto target_goop_pos = target_goop->position;
            auto current_goop_distance = glm::length(target_goop_pos - this->position);
            if (current_goop_distance < 0.05f || this->target_goop->dead)
            {
                this->current_goal = WANDER;
                this->target = wander();
                this->target_goop = nullptr;
                fmt::println("Goober {} wandering", this->ID);
            }
            else {
                current_goop_score = this->target_goop->strength *current_goop_distance;
            }
        }
        
        // Go after more interesting goop
        if (most_important_goop_score > current_goop_score) {
            // this->current_goal = GOOPING;
            this->target_goop = most_important_goop;
            this->target = most_important_goop->position;
            fmt::println("Goober {} : chasing goop at [{}, {}]", this->ID, this->target.x, this->target.y);
        }
        else {
            // Wander to a new target if at destination
            if (glm::length(this->target - this->position) < 0.05f)
            {
                this->target = wander();
                fmt::println("Goober {} wandering", this->ID);
            }
        }
    }
    // else if (current_goal == GOOPING) 
    // {
    //     if (glm::length(this->target - this->position) < 0.05f || this->target_goop->dead)
    //     {
    //         this->current_goal = WANDER;
    //         this->target = wander();
    //         fmt::println("Goober {} wandering", this->ID);
    //     }
    // }


    // Calculate velocity towards the target
    auto target_dir = (this->target - this->position) * this->speed;
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
