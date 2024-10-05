#include "ant.hpp"

Ant::Ant(MeshID mesh_id, ShaderID shader_id)
{
    this->render_obj.mesh = mesh_id;
    this->render_obj.shader = shader_id;
    this->render_obj.is_dirty = true;
}

void Ant::Update()
{
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
