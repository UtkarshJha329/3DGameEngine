#include "DirectionalLight.h"

void DirectionalLight::SerializeComponent()
{
	SFvecs(ambient, diffuse, specular, direction);
	//SerializeGlmVec3(diffuse);
	//SerializeGlmVec3(specular);
	//
	//SerializeGlmVec3(direction);
}

void DirectionalLight::DeSerializeComponent()
{
	DSFvecs(&ambient, &diffuse, &specular, &direction);
}

void DirectionalLight::UIfyComponent()
{
	UIfyVec3(ambient, diffuse, specular, direction);
}
