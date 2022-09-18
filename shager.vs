#version 150 core
in vec3 position;
in vec3 colour;
out vec3 Colour;
void main()
{
Colour = colour;
gl_Position = vec4(position.x , -position.y,position.z, 1);
}