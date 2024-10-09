#include "agent.hpp"

#include <fmt/core.h>
#include <random>


constexpr float INTEREST_MIN_DISTANCE = 0.01;

void Agent::SetTarget(glm::fvec3 target_position) {
    this->target = target_position;
}

void Agent::SetMovingTarget(glm::fvec3* position, glm::fvec3* velocity, glm::fvec3* heading) {
    glm::fvec3* moving_target_position = position;
    glm::fvec3* moving_target_velocity= velocity;
    glm::fvec3* moving_target_heading = heading;
}

void Agent::Wander() {

}

void Agent::Seek() {

}

void Agent::Evade() {

}

void Agent::changeBehavior(AgentBehavior new_behavior) {
}

void Agent::Update(glm::fvec3 position)
{
    this->position = position;
}

AgentBehavior evaluate_situation()
{
    return IDLE;
}

glm::fvec3 Agent::getTarget(float jitter, float radius) 
{
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(-1.0f, 1.0f);
    auto jitter = 1.5f;
    auto ring_radius = 1.2;

    auto new_target = glm::normalize(glm::fvec3(dis(e) * jitter, dis(e) * jitter, 0.0f));
    new_target *= ring_radius;

    auto world_target = this->target + new_target;

    // TODO: Remove once proper coordinates are in
    world_target = glm::clamp(world_target, glm::fvec3(-1.0f, -1.0f, 0.0f), glm::fvec3(1.0f, 1.0f, 0.0f));

    return world_target;
}