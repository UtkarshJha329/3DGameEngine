#version 330 core 

struct Material {

	sampler2D diffuseTex1;
	sampler2D specularTex1;
	sampler2D normalTex1;
	sampler2D emissionTex1;

	float shininess;
};

struct DirLight {

	vec3 direction;

	vec3 ambientColour;
	vec3 diffuseColour;
	vec3 specularColour;
};

struct PointLight{

	float radius;

	vec3 position;

	vec3 ambientColour;
	vec3 diffuseColour;
	vec3 specularColour;
};

struct SpotLight {

	float radius;

	vec3 position;
	vec3 direction;

	float minAngleCos;
	float maxAngleCos;

	vec3 ambientColour;
	vec3 diffuseColour;
	vec3 specularColour;
};

in vec4 vertColor;
in vec3 normal;
in vec2 oTexCoord;
in vec4 glPos;

in mat4 camTransMat;
in mat4 proj;

in mat3 TBN;
in vec3 norm;

in vec3 FragPos;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 ColourBuffer;
layout (location = 2) out int thisEntityID;

uniform vec3 viewerPos;

uniform float curEntity;
uniform float totalEntities;

uniform Material mat;
uniform sampler2D depthFrontTex;
uniform sampler2D depthBackTex;

uniform bool calculateLightingAndNormals;

#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 5
#define NUM_SPOT_LIGHTS 1

#define NUM_SHADOW_SAMPLES 2

uniform DirLight dirLights[NUM_DIR_LIGHTS];
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];

//uniform vec2 plScreenCoord[NUM_POINT_LIGHTS];

uniform float emissionStrength;

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample);
vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample);
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample);
vec4 DiffuseTotal(Material mat);

float near = 0.1; 
float far  = 1000.0; 

float gamma = 2.2;
bool gammaUnCorrectTextures = true;

void main()
{
	
	vec4 textureColour = DiffuseTotal(mat);
	if(textureColour.a < 0.5) {
		discard;
	}
	
	vec3 viewDir = normalize(viewerPos - FragPos);

	vec3 diffuseSample = DiffuseTotal(mat).rgb;
	
	vec3 specularSample = vec3(texture(mat.specularTex1, oTexCoord));
	vec3 emissionSample = vec3(texture(mat.emissionTex1, oTexCoord)) * emissionStrength;
			
	vec3 fragmentColour = vec3(0.0);

	if(calculateLightingAndNormals)
	{
		for(int i = 0; i < NUM_DIR_LIGHTS; i++){
			fragmentColour += CalcDirLight(dirLights[i], norm, viewDir, diffuseSample, specularSample, emissionSample);
		}
		
		for(int i = 0; i < NUM_POINT_LIGHTS; i++){
			fragmentColour += CalcPointLight(pointLights[i], norm, viewDir, diffuseSample, specularSample, emissionSample);

		}
		
		for(int i = 0; i < NUM_SPOT_LIGHTS; i++){
			fragmentColour += CalcSpotLight(spotLights[i], norm, viewDir, diffuseSample, specularSample, emissionSample);
		}
	}
	else
	{
		fragmentColour = diffuseSample;
	}

	fragmentColour = pow(fragmentColour, vec3(1.0/gamma));
	FragColor = vec4(fragmentColour, 1.0);
	//FragColor = vertColor;
	//FragColor = vec4(norm, 1.0);

	//float curEntityColour = (float)(curEntity) / (float)(totalEntities);
	ColourBuffer = vec4(vec3(curEntity/totalEntities), 1.0);
	thisEntityID = int(curEntity);
}


vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample){
	
	vec3 lightDir = normalize(-dirLight.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
	vec3 halfwayDir = normalize(viewDir + dirLight.direction);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), mat.shininess);

    // combine results
    vec3 ambient = dirLight.ambientColour * diffuseSample;
    vec3 diffuse = dirLight.diffuseColour * diff * diffuseSample;
    vec3 specular = dirLight.specularColour * spec * specularSample;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample){
	
	vec3 rayDir = normalize(pointLight.position - FragPos);

	float diff = max(dot(normal, rayDir), 0.0);

	//vec3 reflectedRay = reflect(-rayDir, normal);
	//float specAmount = pow(max(dot(viewDir, reflectedRay), 0.0), mat.shininess);
	vec3 lightDir = normalize(pointLight.position - FragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float specAmount = pow(max(dot(normal, halfwayDir), 0.0), mat.shininess);

	float distance = length(pointLight.position - FragPos);
	float attenuation = 1.0 / (1.0 + 2 * (distance/pointLight.radius) + pow((distance/pointLight.radius), 2));

	vec3 ambient = pointLight.ambientColour * diffuseSample;
	vec3 diffuse = diff * pointLight.diffuseColour * diffuseSample;
	vec3 specular = specAmount * pointLight.specularColour * specularSample;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample){

    vec3 rayDir = normalize(spotLight.position - FragPos);
	float dotRayDir_LightDir = dot(rayDir, normalize(-spotLight.direction));

	float epsilon = spotLight.minAngleCos - spotLight.maxAngleCos;
	float intensity = clamp((dotRayDir_LightDir - spotLight.maxAngleCos)/epsilon, 0.0, 1.0);

	// diffuse
	float diff = max(dot(normal, rayDir), 0.0);

	// specular
	//vec3 reflectedRayDir = reflect(-rayDir, normal);  
	//float specAmount = pow(max(dot(viewDir, reflectedRayDir), 0.0), 4);
	vec3 lightDir = normalize(spotLight.position - FragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float specAmount = pow(max(dot(normal, halfwayDir), 0.0), 4);

	float distance = length(spotLight.position - FragPos);
	float attenuation = 1.0 / (1.0 + 2 * (distance/spotLight.radius) + pow((distance/spotLight.radius), 2));


	vec3 ambient = spotLight.ambientColour * diffuseSample;
	vec3 diffuse = diff * spotLight.diffuseColour * diffuseSample;
	vec3 specular = specAmount * spotLight.specularColour * specularSample; 
	
	ambient *= attenuation;
	diffuse *= intensity * attenuation;
	specular *= intensity * attenuation;
	vec3 emission = diff * emissionSample * intensity * attenuation;

	return ambient + diffuse + specular/* + emission*/;
}

vec4 DiffuseTotal(Material mat){

	vec3 diffuseColor;

	if(gammaUnCorrectTextures){
		diffuseColor = pow(texture(mat.diffuseTex1, oTexCoord).rgb, vec3(gamma));
	}else{
		diffuseColor = texture(mat.diffuseTex1, oTexCoord).rgb;
	}
	return  vec4(diffuseColor, texture(mat.diffuseTex1, oTexCoord).a);

}