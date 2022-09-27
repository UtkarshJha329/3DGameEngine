#include "../HelpHeaders/Components/Camera.h"

Texture* CameraComponent::temp = nullptr;
unsigned int CameraComponent::mainCameraEntity = 1;

void CameraComponent::SerializeComponent() {
	SF(DelimiterType::NEW_LINE, projectionValue);
}

void CameraComponent::DeSerializeComponent() {
	DSF(&projectionValue);
}

void CameraComponent::UIfyComponent() {
	UIfyCombo("Projection", projectionValue, Perspective, Orthographic);
}