#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexCoord;

out vec2 oTexCoord;
out vec3 FragPos;

uniform mat4 transform;
uniform mat4 cameraTransformMat;
uniform mat4 projection;


void main()
{
	FragPos = vec3(transform * vec4(aPos, 1.0));
	vec4 glPos = projection * inverse(cameraTransformMat) * vec4(FragPos, 1.0);
    gl_Position = glPos;
	oTexCoord = TexCoord;
}