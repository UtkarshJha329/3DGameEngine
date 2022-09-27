#pragma once

#include <iostream>

#include "../Component.h"

class Transform : public Component<Transform>{

public:

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::mat4 transformMat = glm::mat4(1.0f);

	static glm::vec3 worldFront;
	static glm::vec3 worldUp;
	static glm::vec3 worldRight;

	glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);

	glm::quat rotQuat;
	glm::mat4 rotMt;
	
	virtual void SerializeComponent() override;
	virtual void DeSerializeComponent() override;
	virtual void UIfyComponent() override;


	glm::vec3 OpenGLTransform(glm::vec3 vec) {
		return glm::vec3(vec.x, vec.y, -vec.z);
	}

	glm::mat4 CalculateTransformMatr() {

		transformMat = glm::mat4(1.0f);

		//TRANSLATE
		transformMat = glm::translate(transformMat, position);

		//ROTATE
		rotQuat = glm::quat(glm::radians(rotation));
		
		front = rotQuat * worldFront;
		up = rotQuat * worldUp;
		right = rotQuat * worldRight;

		rotMt = glm::mat4_cast(rotQuat);
		transformMat = transformMat * rotMt;

		//SCALE
		transformMat = glm::scale(transformMat, scale);
		return transformMat;
	}
	
	glm::mat4 CalculateTransformMatr(glm::vec3 _rotation) {

		transformMat = glm::mat4(1.0f);

		//TRANSLATE
		transformMat = glm::translate(transformMat, position);

		//ROTATE
		rotQuat = glm::quat(glm::radians(_rotation));

		front = rotQuat * worldFront;
		up = rotQuat * worldUp;
		right = rotQuat * worldRight;

		rotMt = glm::mat4_cast(rotQuat);
		transformMat = transformMat * rotMt;

		//SCALE
		transformMat = glm::scale(transformMat, scale);
		return transformMat;
	}

	glm::mat4 CalculateTransformMatr(glm::quat _rotQuat) {

		transformMat = glm::mat4(1.0f);

		//TRANSLATE
		transformMat = glm::translate(transformMat, position);

		//ROTATE
		rotQuat = glm::quat(_rotQuat);

		front = rotQuat * worldFront;
		up = rotQuat * worldUp;
		right = rotQuat * worldRight;

		rotMt = glm::mat4_cast(rotQuat);
		transformMat = transformMat * rotMt;

		//SCALE
		transformMat = glm::scale(transformMat, scale);
		return transformMat;
	}

	void PrintTransformMat() {
		CalculateTransformMatr();
		for (int i = 0; i < 16; i++)
		{
			std::cout << glm::value_ptr(transformMat)[i] << "   ";
			if ((i + 1) % 4 == 0) {
				std::cout << std::endl;
			}
		}
		std::cout << std::endl;

	}
};
