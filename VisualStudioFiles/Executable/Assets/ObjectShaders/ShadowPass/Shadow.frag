#version 330 core 

in vec4 vertColor;
in vec2 oTexCoord;
in vec3 normal;
in vec3 FragPos;

in vec4 glPos;
in float calcDep;

layout (location = 0) out vec4 FragColor;

float near = 0.1; 
float far  = 1000.0; 

uniform sampler2D entityTexture;

uniform float entityID;
uniform float totalEntities;


float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{	
	float colourFrac = entityID / totalEntities;

    //float depth = LinearizeDepth(gl_FragCoord.z) / far;
	//FragColor = vec4(vec3(glPos.z / far), 1.0);
	FragColor = vec4(vec3(colourFrac), 1.0);
}
