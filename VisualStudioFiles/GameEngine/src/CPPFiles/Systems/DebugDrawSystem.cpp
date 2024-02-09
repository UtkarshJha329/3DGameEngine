#include "../../HelpHeaders/Systems/DebugDrawSystem.h"

void DebugDrawSystem::PreAwake()
{
}

void DebugDrawSystem::Awake()
{
}

void DebugDrawSystem::Start()
{
	//debugShader = new DebugShader(glm::vec3(1.0f, 0.0f, 1.0f));
}

void DebugDrawSystem::Update()
{

	if (RandomProps::lastSelectedEntity >= 0 && RandomProps::lastSelectedEntity < curScene->sceneTabler.entity_Set.sparse_Index.size()) {

		MeshData* curSelectedMesh = curScene->GetCompOfEntity<MeshData>(RandomProps::lastSelectedEntity);
		ParentChild* curParChi = curScene->GetCompOfEntity<ParentChild>(RandomProps::lastSelectedEntity);

		if(curSelectedMesh != nullptr && curParChi != nullptr){

			CameraComponent* curMainCamera = curScene->GetCompOfEntity<CameraComponent>(CameraComponent::mainCameraEntity);
			Transform* curCameraTrans = curScene->GetCompOfEntity<Transform>(CameraComponent::mainCameraEntity);

			Transform* curTrans = curScene->GetCompOfEntity<Transform>(RandomProps::lastSelectedEntity);

			if (curTrans == nullptr) {
				curTrans = curScene->GetCompOfEntity<Transform>(curParChi->parEntity);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, curMainCamera->FBO);
			glEnable(GL_DEPTH_TEST);


			int sp = curSelectedMesh->aabb.debugShader->shaderProgram;

			glUseProgram(sp);

			glUniformMatrix4fv(glGetUniformLocation(sp, "transform"), 1, GL_FALSE, glm::value_ptr(curTrans->CalculateTransformMatr()));


			glUniformMatrix4fv(glGetUniformLocation(sp, "cameraTransformMat"), 1, GL_FALSE, glm::value_ptr(curCameraTrans->CalculateTransformMatr()));
			glUniformMatrix4fv(glGetUniformLocation(sp, "projection"), 1, GL_FALSE, glm::value_ptr(curMainCamera->projection));

			curSelectedMesh->aabb.debugShader->draw(curSelectedMesh->aabb.debugShader->VAO);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}


	}

}

void DebugDrawSystem::LateUpdate()
{
}

void DebugDrawSystem::Clear()
{
}
