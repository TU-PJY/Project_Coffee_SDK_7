#include "SDK_Object.h"
#include "SDK_Camera.h"

void SDK::Object::Begin(int RenderType) {
	SDK::Transform.Identity(MoveMatrix);
	SDK::Transform.Identity(RotateMatrix);
	SDK::Transform.Identity(ScaleMatrix);
	SDK::Transform.Identity(FlipMatrix);

	ObjectOpacityValue = 1.0f;
	ObjectBlurValue = 0.0;
	ObjectColor = SDK::Color3(0.0, 0.0, 0.0);

	SDK::Camera.SetCamera(RenderType);
}

void SDK::Object::IdentityUnitMatrix() {
	SDK::Transform.Identity(UnitMoveMatrix);
	SDK::Transform.Identity(UnitRotateMatrix);
	SDK::Transform.Identity(UnitScaleMatrix);
	SDK::Transform.Identity(UnitFlipMatrix);

	UnitOpacityValue = 1.0f;
	UnitBlurValue = 0.0f;
}

void SDK::Object::ComputeViewportPosition(float& DestX, float& DestY, bool ApplyAspect) {
	if (ApplyAspect)
		DestX = ASP(ViewportPosition().x);
	else
		DestX = ViewportPosition().x;
	DestY = ViewportPosition().y;
}

void SDK::Object::ComputeViewportPosition(SDK::Vector2& DestValue, bool ApplyAspect) {
	if (ApplyAspect) {
		DestValue.x = ASP(ViewportPosition().x);
		DestValue.y = ViewportPosition().y;
	}
	else
		DestValue = ViewportPosition();
}

void SDK::Object::ComputeLocalPosition(float& DestX, float& DestY) {
	DestX = LocalPosition().x;
	DestY = LocalPosition().y;
}

void SDK::Object::ComputeLocalPosition(SDK::Vector2& DestPosition) {
	DestPosition = LocalPosition();
}

////////////////////////// private

glm::vec4 SDK::Object::ViewportPosition() {
	SDK::ComputeTool.ComputeMatrix(SDK::ViewportPositionMatrix, SDK::Camera.Projection, SDK::Camera.ViewMatrix, ResultMatrix);
	return SDK::ViewportPositionMatrix * SDK::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

glm::vec4 SDK::Object::LocalPosition() {
	return SDK::ResultMatrix * SDK::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}