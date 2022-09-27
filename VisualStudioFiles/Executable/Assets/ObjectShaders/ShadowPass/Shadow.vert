#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 TexCoord;

out vec4 vertColor;
out vec2 oTexCoord;
out vec3 normal;
out vec3 FragPos;

out vec4 glPos;

uniform mat4 transform;
uniform mat4 cameraTransformMat;
uniform mat4 projection;


void main()
{
	glPos = projection * inverse(cameraTransformMat) * transform * vec4(aPos, 1.0);
    gl_Position = glPos;
	FragPos = vec3(transform * vec4(aPos, 1.0));
	oTexCoord = TexCoord;
	normal = mat3(transpose(inverse(transform))) * aNormal;
}