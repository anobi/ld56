#pragma once

#include <glad/gl.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:

    unsigned int ID;

    Shader(const char* vertex_path, const char* fragment_path) {
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream vertex_file;
        std::ifstream fragment_file;

        vertex_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fragment_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            vertex_file.open(vertex_path);
            fragment_file.open(fragment_path);
            std::stringstream vertex_stream, fragment_stream;

            vertex_stream << vertex_file.rdbuf();
            fragment_stream << fragment_file.rdbuf();

            vertex_file.close();
            fragment_file.close();

            vertex_code = vertex_stream.str();
            fragment_code = fragment_stream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR: Could not read shader " << e.what() << std::endl;
        }

        const char* vs_code = vertex_code.c_str();
        const char* fs_code = fragment_code.c_str();

        const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vs_code, NULL);
        glCompileShader(vertex_shader);
        checkCompileErrors(vertex_shader, "vertex");

        const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fs_code, NULL);
        glCompileShader(fragment_shader);
        checkCompileErrors(fragment_shader, "fragment");

        this->ID = glCreateProgram();
        glAttachShader(this->ID, vertex_shader);
        glAttachShader(this->ID, fragment_shader);
        glLinkProgram(this->ID);
        checkCompileErrors(this->ID, "program");

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    void use() const { glUseProgram(this->ID); }
    GLuint getUniformLocation(const char* name) const { return glGetUniformLocation(this->ID, name); }

private:

    void checkCompileErrors(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "Error compiling " << type << " shader\n" << infoLog << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "Error linking " << type << " shader\n" << infoLog << std::endl;
            }
        }
    }
};