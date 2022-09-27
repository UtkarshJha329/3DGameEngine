#include "../Headers/CameraMovement.h"

void CameraMovementComponent::SerializeComponent()  {

	SF(DelimiterType::NEW_LINE, cameraMvSpeed, sensitivity);
}

void CameraMovementComponent::DeSerializeComponent()  {

	DSF(&cameraMvSpeed, &sensitivity);
}

 void CameraMovementComponent::UIfyComponent()
{
	UIfyFloat(sensitivity, cameraMvSpeed);
}