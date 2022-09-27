#include "../../HelpHeaders/Systems/CameraHandelingSystem.h"

void CameraHandelingSystem::PreAwake() {

	archetype.priorityType = HAS_ALL_COMP;
	archetype.compSparseIdsNdPriority.push_back(CameraComponent::compSetIndex);
}

void CameraHandelingSystem::Awake() {

	CameraComponent::mainCameraEntity = 1;
	SetCameraFrameBuffers(WindowProperties::instance->windowWidth, WindowProperties::instance->windowHeight);
}

void CameraHandelingSystem::Start() {

}

void CameraHandelingSystem::Update() {

	if (WindowProperties::instance->viewportResized) {

		glViewport(0, 0, WindowProperties::instance->viewportSize.x, WindowProperties::instance->viewportSize.y);

		SetCameraFrameBuffers(WindowProperties::instance->viewportSize.x, WindowProperties::instance->viewportSize.y);
		WindowProperties::instance->viewportResized = false;
	}

	UpdateCameraProjection();
}

void CameraHandelingSystem::LateUpdate() {

}

void CameraHandelingSystem::Clear() {
	ClearCameraBuffers();
}

void CameraHandelingSystem::UpdateCameraProjection() {

	CompSparseSet<CameraComponent>* camSet = curScene->GetCompSparseSet<CameraComponent>();

	for (int i = 0; i < camSet->denseTArray.size(); i++)
	{
		CameraComponent* curComp = &camSet->denseTArray[i];

		CameraComponent::ProjectionType UIVal = (CameraComponent::ProjectionType)curComp->projectionValue;

		if (UIVal != curComp->projectionType) {

			curComp->projectionType = UIVal;

			if (UIVal == CameraComponent::ProjectionType::Perspective) {
				curComp->projection = glm::perspective(glm::radians(45.0f), WindowProperties::instance->viewportSize.x / WindowProperties::instance->viewportSize.y, 0.1f, 1000.0f);
			}
			else {
				curComp->projection = glm::ortho(0.0f, 25.0f, 0.0f, 25.0f, 0.01f, 1000.0f);
			}

		}

	}
}


void CameraHandelingSystem::CreateCameraFrameBuffers() {

	CompSparseSet<CameraComponent>* camSet = curScene->GetCompSparseSet<CameraComponent>();

	for (int i = 0; i < camSet->denseTArray.size(); i++)
	{
		std::cout << "CREATED NEW CAMERA BUFFER OBJECTS !" << std::endl;
		CameraComponent* curComp = &camSet->denseTArray[i];
		ClearCameraBufSpecific(curComp);
		CreateCameraFrameBufSpecific(curComp);
	}
}

void CameraHandelingSystem::CreateCameraFrameBufSpecific(CameraComponent* curComp) {

	std::cout << curComp->FBO << std::endl;
	glGenFramebuffers(1, &(curComp->FBO));
	glGenFramebuffers(1, &(curComp->FBO_FrontDepthPass));
	glGenFramebuffers(1, &(curComp->FBO_BackDepthPass));
	std::cout << "FBO CREATED." << std::endl;
	glGenRenderbuffers(1, &(curComp->RBO));
	glGenRenderbuffers(1, &(curComp->RBO_FrontDepthPass));
	glGenRenderbuffers(1, &(curComp->RBO_BackDepthPass));
}

void CameraHandelingSystem::SetCameraFrameBuffers(float width, float height) {

	//CompSparseSet<CameraComponent>* camSet = curScene->GetCompSparseSet<CameraComponent>();

	/*for (int i = 0; i < camSet->denseTArray.size(); i++)
	{
	}*/

	CameraComponent* mainCam = curScene->GetCompOfEntity<CameraComponent>(CameraComponent::mainCameraEntity);

	if (mainCam->FBO) {
		ClearCameraBufSpecific(mainCam);
		CreateCameraFrameBufSpecific(mainCam);
	}
	else {
		CreateCameraFrameBufSpecific(mainCam);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, mainCam->FBO);

	glGenTextures(1, &(mainCam->renderTextureID));
	glBindTexture(GL_TEXTURE_2D, mainCam->renderTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mainCam->renderTextureID, 0);

	glGenTextures(1, &(mainCam->entityColourTextureID));
	glBindTexture(GL_TEXTURE_2D, mainCam->entityColourTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mainCam->entityColourTextureID, 0);

	glGenTextures(1, &(mainCam->entityIDTextureID));
	glBindTexture(GL_TEXTURE_2D, mainCam->entityIDTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mainCam->entityIDTextureID, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, mainCam->RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mainCam->RBO);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	//unsigned int attachments2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(3, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, mainCam->FBO_FrontDepthPass);

	glGenTextures(1, &(mainCam->depthTextureFrontID));
	glBindTexture(GL_TEXTURE_2D, mainCam->depthTextureFrontID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mainCam->depthTextureFrontID, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, mainCam->RBO_FrontDepthPass);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mainCam->RBO_FrontDepthPass);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, mainCam->FBO_BackDepthPass);

	glGenTextures(1, &(mainCam->depthTextureBackID));
	glBindTexture(GL_TEXTURE_2D, mainCam->depthTextureBackID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mainCam->depthTextureBackID, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, mainCam->RBO_BackDepthPass);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mainCam->RBO_BackDepthPass);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

void CameraHandelingSystem::ClearCameraBuffers() {

	CompSparseSet<CameraComponent>* camSet = curScene->GetCompSparseSet<CameraComponent>();

	for (int i = 0; i < camSet->denseTArray.size(); i++)
	{
		CameraComponent* curComp = &camSet->denseTArray[i];

		ClearCameraBufSpecific(curComp);
	}
}

void CameraHandelingSystem::ClearCameraBufSpecific(CameraComponent* curComp) {
	glDeleteRenderbuffers(1, &(curComp->RBO));
	glDeleteRenderbuffers(1, &(curComp->RBO_FrontDepthPass));
	glDeleteRenderbuffers(1, &(curComp->RBO_BackDepthPass));
	std::cout << "DELETED RBO" << std::endl;
	glDeleteTextures(1, &(curComp->renderTextureID));
	glDeleteTextures(1, &(curComp->depthTextureFrontID));
	glDeleteTextures(1, &(curComp->depthTextureBackID));
	glDeleteTextures(1, &(curComp->entityColourTextureID));
	glDeleteFramebuffers(1, &(curComp->FBO));
	glDeleteFramebuffers(1, &(curComp->FBO_FrontDepthPass));
	glDeleteFramebuffers(1, &(curComp->FBO_BackDepthPass));


}
