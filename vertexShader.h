#pragma once

const char* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 colour;
in vec3 normal;
out vec3 Colour;
uniform mat4 trans;
uniform mat4 view;
uniform mat4 proj;
void main()
{
vec3 camera = vec3(0 , -.5 , .8 );



float light = dot(camera , normal);
Colour = colour * ( light +.95);
gl_Position = proj*view*trans*vec4(position.x , position.y,position.z, 2);
gl_PointSize = 2;
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