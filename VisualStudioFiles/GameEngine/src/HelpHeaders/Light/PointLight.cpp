#include "PointLight.h"

void PointLight::SerializeComponent()
{
	SFvecs(ambient, diffuse, specular);
	//SerializeGlmVec3(diffuse);
	//SerializeGlmVec3(specular);

	SF(DelimiterType::NEW_LINE, radius);
}

void PointLight::DeSerializeComponent()
{
	DSFvecs(&ambient, &diffuse, &specular);
	DSF(&radius);
}

void PointLight::UIfyComponent()
{
	UIfyVec3(ambient, diffuse, specular);
	UIfyFloat(radius);
}