#include "SpotLight.h"

void SpotLight::SerializeComponent()
{
	SFvecs(ambient, diffuse, specular, direction);
	//SerializeGlmVec3(diffuse);
	//SerializeGlmVec3(specular);

	//SerializeGlmVec3(direction);

	SF(DelimiterType::NEW_LINE, radius, halfAngle, fallHalfAngle);
	//SerializeArgs(DelimiterType::NEW_LINE, halfAngle);
	//SerializeArgs(DelimiterType::NEW_LINE, fallHalfAngle);
}

void SpotLight::DeSerializeComponent()
{
	DSFvecs(&ambient, &diffuse, &specular, &direction);
	DSF(&radius, &halfAngle, &fallHalfAngle);
}

void SpotLight::UIfyComponent()
{
	UIfyVec3(ambient, diffuse, specular, direction);
	UIfyFloat(radius, halfAngle, fallHalfAngle);
}
