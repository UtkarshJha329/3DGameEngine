#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec4 vertColor;
out vec2 oTexCoord;
out vec3 normal;
out vec3 FragPos;
out vec4 glPos;

out mat4 camTransMat;
out mat4 proj;

out mat3 TBN;

uniform mat4 transform;
uniform mat4 cameraTransformMat;
uniform mat4 projection;


void main()
{
	FragPos = vec3(transform * vec4(aPos, 1.0));
	glPos = projection * inverse(cameraTransformMat) * vec4(FragPos, 1.0);
    gl_Position = glPos;
	oTexCoord = TexCoord;
	normal = mat3(transpose(inverse(transform))) * aNormal;
	

	vec3 T = normalize(vec3(transform * vec4(aTangent, 0.0)));
	vec3 N = -normalize(vec3(transform * vec4(aNormal, 0.0)));
	
	
	T = normalize(T - dot(T, N) * N);
	
	//vec3 B = normalize(cross(N, T));
	vec3 B = normalize(vec3(transform * vec4(aBitangent, 0.0)));
	
	if(dot(cross(B, T), N) > 0){
		T *= -1;
	}

	TBN = mat3(T, B, N);


	camTransMat = cameraTransformMat;
	proj = projection;
}