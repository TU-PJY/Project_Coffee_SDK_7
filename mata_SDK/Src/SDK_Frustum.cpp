#include "SDK_Frustum.h"
#include "SDK_Config.h"
#include "SDK_Camera.h"
#include "SDK_CameraController.h"

SDK::SDK_Frustum SDK::Frustum;

void SDK::SDK_Frustum::Update() {
	FrustumOOBB.Update(SDK::Camera.Position, (2.0 * SDK::ASPECT) / SDK::Camera.Zoom, 2.0 / SDK::Camera.Zoom, SDK::Camera.Rotation);

if(SHOW_FRUSTUM_BOUND)
	LineRect.Draw(SDK::Camera.Position.x, SDK::Camera.Position.y, (2.0 * SDK::ASPECT) / SDK::Camera.Zoom, 2.0 / SDK::Camera.Zoom, BOUND_BOX_THICKNESS, SDK::Camera.Rotation);
}

bool SDK::SDK_Frustum::Check(SDK::AABB& aabb) {
	if (FrustumOOBB.CheckCollision(aabb))
		return true;

	return false;
}

bool SDK::SDK_Frustum::Check(SDK::OOBB& oobb) {
	if (FrustumOOBB.CheckCollision(oobb))
		return true;

	return false;
}

bool SDK::SDK_Frustum::Check(SDK::BoundingCircle& circle) {
	if (FrustumOOBB.CheckCollision(circle))
		return true;

	return false;
}

SDK::SDK_Frustum::SDK_Frustum() {
	LineRect.SetRenderType(RENDER_TYPE_DEFAULT);
	LineRect.SetColor(1.0, 0.0, 0.0);
}