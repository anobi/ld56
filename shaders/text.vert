#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vCol;
layout (location = 2) in vec2 vUv;

out vec2 TexCoords;
out vec3 Color;
  
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vPos, 1.0f);
    TexCoords = vUv;
}