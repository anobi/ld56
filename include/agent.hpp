#pragma once

#include <vector>
#include <glm/glm.hpp>

enum AgentBehavior
{
    IDLE,
    WANDER,
    SEEK,
    FLEE
};

struct Interest {
    glm::fvec3 position;
    float interest_score;
    float threat_score;
};

class Agent {
public:
    void Update(glm::fvec3 position);
    glm::fvec3 GetTarget();
    glm::fvec3 GetDesiredVelocity();

private:
    AgentBehavior evaluate_situation();
    void wander();
    void seek();
    void evade();

    glm::fvec3 position;
    glm::fvec3 desired_velocity;
    AgentBehavior current_behavior = IDLE;

    std::vector<Interest> interests;
    Interest* current_interest = nullptr;
};