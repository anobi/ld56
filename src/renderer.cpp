#include <glad/gl.h>
#include <fmt/core.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "renderer.hpp"

void Renderer::Init(unsigned int width, unsigned int height)
{
    this->text_shader = this->AddShader("shaders/text.vert", "shaders/text.frag");
    this->text_renderer = TextRenderer(width, height, this->GetShaderRef(text_shader));
    this->text_renderer.LoadFont("assets/Tiny5-Regular.ttf", 120);
}

MeshID Renderer::AddMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    auto id = this->next_id++;
    auto mesh = Mesh(vertices, indices);
    this->meshes.emplace(id, mesh);
    return {id};
}

ShaderID Renderer::AddShader(const char *vertex_path, const char *fragment_path)
{
    auto id = this->next_id++;
    auto shader = Shader(vertex_path, fragment_path);
    this->shaders.emplace(id, shader);
    return {id};
}

void Renderer::Draw(std::vector<RenderObject> scene)
{
    ShaderID bound_shader{0};
    for (auto o : scene)
    {
        if (bound_shader != o.shader)
        {
            this->shaders.at(o.shader.id).use();
            bound_shader = o.shader;
        }
        auto tloc = glGetUniformLocation(this->shaders.at(bound_shader.id).ID, "transform");
        auto mvp = this->projection_matrix * o.transform;
        glUniformMatrix4fv(tloc, 1, GL_FALSE, glm::value_ptr(mvp));
        this->meshes.at(o.mesh.id).Draw();
    }
}

void Renderer::DrawText(std::string text, glm::fvec2 position, float scale, glm::fvec3 color)
{
    this->text_renderer.Draw(text, position, scale, color);
}


void Renderer::UpdatePerspective(float width, float height)
{
    float ratio = width / height;
    auto p = glm::ortho(ratio, -ratio, -1.0f, 1.0f, 1.0f, -1.0f);
    // auto p = glm::ortho(0.0f, width * ratio, 0.0f, height, 0.0f, 100.0f);
    this->projection_matrix = p;
}

Shader* Renderer::GetShaderRef(ShaderID shader)
{
    return &this->shaders.at(shader.id);
}




// Text rendering


TextRenderer::TextRenderer(unsigned int width, unsigned int height, Shader* shader)
{
    // Generate
    this->shader = shader;

    float ratio = (float)width / (float)height;
    auto p = glm::ortho(ratio, -ratio, -1.0f, 1.0f, 1.0f, -1.0f);
    
    this->shader->SetMat4("transform", glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f), true);
    this->shader->SetInt("text", 0, true);

    glGenVertexArrays(1, &this->gpuMesh.VAO);
    glGenBuffers(1, &this->gpuMesh.VBO);

    // Bind
    glBindVertexArray(this->gpuMesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->gpuMesh.VBO);

    // Set GL shenanigans
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    
    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::LoadFont(const char *font, unsigned int fontSize)
{
    this->characters.clear();

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) 
    {
        fmt::println("ERROR TextRenderer: Could not init FreeType");
    }

    FT_Face face;
    if (FT_New_Face(ft, font, 0, &face))
    {
        fmt::println("ERROR TextRenderer: Could not load font {}", font);
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Disable byte alignment restrictions
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load ASCII characters
    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            fmt::println("ERROR TextRenderer: Failed to load glyph {}", c);
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (unsigned int)face->glyph->advance.x
        };
        this->characters.insert(std::pair<char, Character>(c, character));
    }

    // Unbind and free resources
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::Draw(std::string text, glm::fvec2 pos, float scale, glm::fvec3 color)
{
    // TODO: Activate shader and set color attribute
    this->shader->SetMat4("transform", glm::ortho(0.0f, 800.0f, 600.0f, 0.0f), true);
    this->shader->SetVec3f("textColor", color, true);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->gpuMesh.VAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = this->characters[*c];

        float xpos = pos.x + ch.bearing.x * scale;
        float ypos = pos.y + (this->characters['H'].bearing.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h, 0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 0.0f },
            { xpos,     ypos,     0.0f, 0.0f },

            { xpos,     ypos + h, 0.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureId);
        glBindBuffer(GL_ARRAY_BUFFER, this->gpuMesh.VBO);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        pos.x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
