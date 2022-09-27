#ifndef CAMERA_H
#define CAMERA_H

#include "../WindowProperties.h"
#include "../TextureHandeling/Texture_H.h"

class CameraComponent : public Component<CameraComponent> {

public:

	CameraComponent()
	{
		//transform.front = glm::vec3(0.0f, 0.0f, -1.0f);
		projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), WindowProperties::instance->windowWidth / WindowProperties::instance->windowHeight, 0.1f, 1000.0f);
	}

	enum ProjectionType
	{
		Perspective,
		Orthographic
	};

	ProjectionType projectionType;
	int projectionValue = 0;

	glm::mat4 projection;

	static Texture* temp;

	static unsigned int mainCameraEntity;

	virtual void SerializeComponent() override;
	virtual void DeSerializeComponent() override;
	virtual void UIfyComponent() override;

	unsigned int renderTextureID = 0;
	unsigned int entityColourTextureID = 0;

	unsigned int entityTextureID = 0;
	unsigned int depthTextureFrontID = 0;
	unsigned int depthTextureBackID = 0;
	unsigned int entityIDTextureID = 0;

	int entityFromTexture = -1;
	unsigned int FBO = 0;

private:

	friend class MeshRenderingSystem;
	friend class CameraHandelingSystem;

	unsigned int RBO = 0;

	unsigned int FBO_PickingColour = 0;
	unsigned int RBO_PickingColour = 0;

	unsigned int FBO_FrontDepthPass = 0;
	unsigned int RBO_FrontDepthPass = 0;

	unsigned int FBO_BackDepthPass = 0;
	unsigned int RBO_BackDepthPass = 0;

	glm::mat4 view = glm::mat4(1.0f);
};

#endif