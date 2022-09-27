#include "../Header/CameraMovementSystem.h"
#include "HelpHeaders/Components/Camera.h"

#include "Components/Transform.h"
#include "Components/Input.h"
#include "HelpHeaders/TimeVals.h"

void CameraMovementSystem::PreAwake() {

}

void CameraMovementSystem::Awake() {

	archetype.priorityType = HAS_ALL_COMP;
	archetype.compSparseIdsNdPriority.push_back(Transform::compSetIndex);
	archetype.compSparseIdsNdPriority.push_back(CameraComponent::compSetIndex);
	archetype.compSparseIdsNdPriority.push_back(CameraMovementComponent::compSetIndex);

}

void CameraMovementSystem::Start() {

}

void CameraMovementSystem::Update() {

	//std::cout << curSmallestSetSize << std::endl;

	for (unsigned int i = 0; i < curSmallestSetSize; i++)
	{
		CameraMovementComponent* curCamMv = curScene->GetCompOfTArray<CameraMovementComponent>(i);

		if (Input::Get().KeyHeld(KeyCode::I)) {
			curCamMv->cameraMvSpeed += 1.0f;
		}
		else if (Input::Get().KeyHeld(KeyCode::O)) {
			curCamMv->cameraMvSpeed -= 1.0f;
		}

		if (Input::Get().MouseButtonHeld(MouseButtonCode::RIGHT)) {
			
			Input::cursorVisible = false;

			float curCameraSpeed = curCamMv->cameraMvSpeed;

			Transform* curTrans = curScene->GetCompOfTArray<Transform>(i);

			if (Input::Get().KeyHeld(KeyCode::W))
			{
				curTrans->position += curTrans->front * curCameraSpeed * Time::deltaTime;
			}
			if (Input::Get().KeyHeld(KeyCode::S))
			{
				curTrans->position -= curTrans->front * curCameraSpeed * Time::deltaTime;
			}
			if (Input::Get().KeyHeld(KeyCode::A))
			{
				curTrans->position -= curTrans->right * curCameraSpeed * Time::deltaTime;
			}
			if (Input::Get().KeyHeld(KeyCode::D))
			{
				curTrans->position += curTrans->right * curCameraSpeed * Time::deltaTime;
			}

			float curSensitity = curScene->GetCompOfTArray<CameraMovementComponent>(i)->sensitivity;

			curTrans->rotation.y -= (Input::Get().mouseDeltaX * curSensitity * Time::deltaTime);
			curTrans->rotation.x += (Input::Get().mouseDeltaY * -curSensitity * Time::deltaTime);

			if (curTrans->rotation.x > 89.0f)
				curTrans->rotation.x = 89.0f;
			if (curTrans->rotation.x < -89.0f)
				curTrans->rotation.x = -89.0f;
		}
		else {
			Input::cursorVisible = true;
		}

	}

}

void CameraMovementSystem::LateUpdate()
{
}

void CameraMovementSystem::Clear() {

}