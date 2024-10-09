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

class Agent {
public:
    void Update(glm::fvec3 position);

    void SetTarget(glm::fvec3 position);
    void SetMovingTarget(glm::fvec3* position, glm::fvec3* velocity, glm::fvec3* heading);

    void Wander();
    void Seek();
    void Evade();

    glm::fvec3 GetSteeringDirection();

private:
    AgentBehavior evaluateSituation();
    void changeBehavior(AgentBehavior new_behavior);
    glm::fvec3 getTarget(float jitter, float radius);


    glm::fvec3 position;
    glm::fvec3 target;

    glm::fvec3* moving_target_position = nullptr;
    glm::fvec3* moving_target_velocity= nullptr;
    glm::fvec3* moving_target_heading = nullptr;
    
    AgentBehavior current_behavior = IDLE;

    
};