#version 330 core

layout (location = 0) in vec4 vertex;

out vec2 TexCoords;
out vec3 Color;
  
uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vertex.xy, 0.0f, 1.0f);
    TexCoords = vertex.zw;
}