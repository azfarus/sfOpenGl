#pragma once

const char* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 colour;
out vec3 Colour;
uniform mat4 trans;
void main()
{
Colour = colour;
gl_Position = trans*vec4(position.x , position.y,position.z, 1);
}
)glsl";

const char* fragmentSource = R"glsl(
#version 150 core
in vec3 Colour;
out vec4 outColor;
void main()
{
outColor = vec4(Colour, 1);
}
)glsl";