#version 330 core

in vec3 Color;
out vec4 fragment;

void main()
{
    fragment = vec4(Color, 1.0);
}