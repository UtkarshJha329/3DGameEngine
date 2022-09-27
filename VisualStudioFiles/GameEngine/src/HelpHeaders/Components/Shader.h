#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../Component.h"

enum ShaderType {

	VertFrag,
	Compute
};

enum ShaderNumber {

	TexturedLit,
	Particles
};

class Shader : public Component<Shader>{
	
public:

	unsigned int ID;
	ShaderType type;

	ShaderNumber shadNum;

	std::string* _vertPath;
	std::string* _fragPath;

	Shader() = default;



	virtual void SerializeComponent() override {}
	virtual void DeSerializeComponent() override {}
	virtual void UIfyComponent() override 
	{
		UIfyString(*_vertPath, *_fragPath);
	}

};
#endif