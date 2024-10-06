#include "agent.hpp"

#include <random>


constexpr float INTEREST_MIN_DISTANCE = 0.01;


void Agent::Update(glm::fvec3 position)
{
    this->position = position;
    if (this->current_behavior == IDLE) 
    {
        this->current_behavior = WANDER;
    }
    else if (this->current_behavior == WANDER)
    {
        /* code */
    } 
    else if (this->current_behavior == SEEK)
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
    else if (this->current_behavior == FLEE)
    {
        /* code */
    }
    else if (!this->current_interest) {
        this->current_behavior = IDLE;
    }
}

glm::fvec3 Agent::GetTarget()
{
    return glm::fvec3();
}

Behavior evaluate_situation()
{
    return IDLE;
}

void Agent::wander()
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

    fmt::println("Ant {} [{}, {}]: new target at [{}, {}]", this->ant_id, this->position.x, this->position.y, new_target.x, new_target.y);

    return world_target;
}

void Agent::seek()
{
    auto new_velocity = glm::normalize(goop - this->position) * this->speed;
    return new_velocity - this->velocity;
}

void Agent::evade()
{
}