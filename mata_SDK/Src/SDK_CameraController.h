#pragma once
#include "SDK_Camera.h"

namespace SDK {
	class SDK_CameraController {
	public:
		void Update(float FrameTime);
		void Move(float X, float Y);
		void Move(SDK::Vector2& PositionValue);
		void Rotate(float Degree);
		void Zoom(int ZoomType, float ZoomValue);
		void SetZoom(float ZoomValue);
		float ComputeNextZoom(int ZoomType, float ZoomValue);

	private:
		SDK::Vector2 Position{};
		float Rotation{};

		void ComputeCameraMatrix();
	};

	extern SDK::SDK_CameraController CameraControl;
}