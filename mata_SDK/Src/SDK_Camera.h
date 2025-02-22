#pragma once
#include "SDK_Header.h"

namespace SDK {
	class SDK_Camera {
	private:
		SDK::Vector3 CamPos{}, CamDirection{}, CamUp{};
		bool      StaticRenderCommand{};
	
	public:
		SDK::Matrix4 ViewMatrix{ 1.0 }, Projection{ 1.0 }, CameraMatrix{ 1.0 };

		SDK::Vector2 Position{};
		float   Rotation{};
		float   Zoom{ 1.0f };

		void CalculateASPECT();
		void Init();
		void SetCamera(int RenderType);
		void PrepareRender(int ShaderType);
	};

	extern SDK_Camera Camera;
}