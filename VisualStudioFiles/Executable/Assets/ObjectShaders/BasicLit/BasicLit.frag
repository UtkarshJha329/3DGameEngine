#version 330 core 

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

in vec3 normal;
in vec2 oTexCoord;

in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewerPos;

#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 5
#define NUM_SPOT_LIGHTS 1

uniform DirLight dirLights[NUM_DIR_LIGHTS];
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLights[NUM_SPOT_LIGHTS];

uniform float emissionStrength;

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample);
vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample);
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample);

vec3 diffuseColour = vec3(0.5);
vec3 specularColour = vec3(1.0);
vec3 emmissionColour = vec3(0.0);

float shininess = 16.0;

void main()
{

	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewerPos - FragPos);

	vec3 fragmentColour = vec3(0.0);
	for(int i = 0; i < NUM_DIR_LIGHTS; i++){
		fragmentColour += CalcDirLight(dirLights[i], norm, viewDir, diffuseColour, specularColour, emmissionColour);
	}
	
	for(int i = 0; i < NUM_POINT_LIGHTS; i++){
		fragmentColour += CalcPointLight(pointLights[i], norm, viewDir, diffuseColour, specularColour, emmissionColour);
	}

	for(int i = 0; i < NUM_SPOT_LIGHTS; i++){
		fragmentColour += CalcSpotLight(spotLights[i], norm, viewDir, diffuseColour, specularColour, emmissionColour);
	}

	FragColor = vec4(fragmentColour, 1.0);
}


vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample){
	
	vec3 lightDir = normalize(-dirLight.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient = dirLight.ambientColour * diffuseSample;
    vec3 diffuse = dirLight.diffuseColour * diff * diffuseSample;
    vec3 specular = dirLight.specularColour * spec * specularSample;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir, vec3 diffuseSample, vec3 specularSample, vec3 emissionSample){
	
	vec3 rayDir = normalize(pointLight.position - FragPos);

	float diff = max(dot(normal, rayDir), 0.0);

	vec3 reflectedRay = reflect(-rayDir, normal);
	float specAmount = pow(max(dot(viewDir, reflectedRay), 0.0), shininess);

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
	vec3 reflectedRayDir = reflect(-rayDir, normal);  
	float specAmount = pow(max(dot(viewDir, reflectedRayDir), 0.0), 4);
	
	float distance = length(spotLight.position - FragPos);
	float attenuation = 1.0 / (1.0 + 2 * (distance/spotLight.radius) + pow((distance/spotLight.radius), 2));


	vec3 ambient = spotLight.ambientColour * diffuseSample;
	vec3 diffuse = diff * spotLight.diffuseColour * diffuseSample;
	vec3 specular = specAmount * spotLight.specularColour * specularSample; 
	
	ambient *= attenuation;
	diffuse *= intensity * attenuation;
	specular *= intensity * attenuation;
	vec3 emission = diff * emissionSample * intensity * attenuation;

	return ambient + diffuse + specular + emission;
}