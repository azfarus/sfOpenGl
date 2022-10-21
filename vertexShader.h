#pragma once

const char* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 colour;
in vec3 normal;
in vec2 uv;
out vec3 Colour;
out vec2 UV;
uniform vec3 camera;
uniform mat4 trans;
uniform mat4 view;
uniform mat4 proj;
void main()
{


UV = uv;
vec3 pos = vec3(trans*vec4(position.x , position.y,position.z,1));
vec3 norm = normalize(vec3(trans* vec4(normal,1)));
vec3 crntpos = normalize(camera - pos);

float light = max(dot(crntpos , norm),0) ;
float x = .2;
Colour = colour*(light+x);
gl_Position = proj*view*trans*vec4(position.x , position.y,position.z, 1);
gl_PointSize = 2;
}
)glsl";

const char* fragmentSource = R"glsl(
#version 150 core
in vec3 Colour;
in vec2 UV;
out vec4 outColor;
uniform  sampler2D glsmp;
void main()
{
outColor =  texture2D(glsmp ,UV ) * vec4(Colour, 1);
}
)glsl";