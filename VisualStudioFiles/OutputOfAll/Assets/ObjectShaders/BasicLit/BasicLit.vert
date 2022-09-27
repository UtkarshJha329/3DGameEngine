#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 FragPos;

uniform mat4 transform;
uniform mat4 cameraTransformMat;
uniform mat4 projection;


void main()
{
    gl_Position = projection * inverse(cameraTransformMat) * transform * vec4(aPos, 1.0);
	FragPos = vec3(transform * vec4(aPos, 1.0));
	normal = mat3(transpose(inverse(transform))) * aNormal;
}