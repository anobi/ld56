#include <random>
#include <fmt/core.h>

#include "goop.hpp"

Goop::Goop(MeshID mesh_id, ShaderID shader_id, glm::fvec3 pos)
{
    this->position = pos;
    this->scale = glm::fvec3(this->radius);

    static std::default_random_engine e;
    static std::uniform_int_distribution<> dis(0, 359);
    this->rotation_angle = (float)dis(e);

    static std::uniform_real_distribution<> dirdis(-1.0f, 1.0f);
    this->rotation_dir = (float)dirdis(e);

    this->render_obj.mesh = mesh_id;
    this->render_obj.shader = shader_id;
    this->render_obj.is_dirty = true;
    //fmt::println("Goop spawned at [{}, {}]", pos.x, pos.y);
}

void Goop::Update(float dt)
{
    if (this->dead) {
        //fmt::println("Goop is dead");
        return;
    }

    this->strength -= this->decay;
    if (this->strength < 0.0f) {
        this->dead = true;
        //fmt::println("Goop died");
    }

    this->scale = glm::fvec3(this->radius * this->strength);
    this->rotation_angle += this->decay * this->rotation_dir * 4.0f;

    // Update render matrix
    this->render_obj.is_dirty = true;  // Goop is always dirty.
    if(this->render_obj.is_dirty)
    {
        auto transform = glm::mat4(1.0f);
        transform = glm::translate(transform, this->position);
        transform = glm::rotate(transform, this->rotation_angle, glm::vec3(0, 0, 1));
        transform = glm::scale(transform, this->scale);
        this->render_obj.transform = transform;
        //this->render_obj.is_dirty = false;
    }
}
