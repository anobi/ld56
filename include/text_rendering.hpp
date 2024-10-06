#pragma once

#include <glm/glm.hpp>
#include <map>

#include "renderable.hpp"
#include "shader.hpp"

struct Character {
    unsigned int textureId;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class TextRenderer
{
public:
    TextRenderer(unsigned int width, unsigned int height, Shader* shader);
    void LoadFont(const char* font, unsigned int size);
    void Draw(std::string text, glm::fvec2 position, float scale, glm::fvec3 color);

    std::map<char, Character> characters;

private:
    GpuMesh gpuMesh;
    // ShaderID shader;
    Shader* shader;
};