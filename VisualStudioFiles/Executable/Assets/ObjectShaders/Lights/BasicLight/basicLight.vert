#version 330 core

layout (location = 0) in vec3 aPos;

out vec4 vertColor;

uniform vec3 lightColour;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
	vertColor = vec4(lightColour, 1.0);
}