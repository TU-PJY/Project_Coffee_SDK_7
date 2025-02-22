#pragma once
#include "SDK_Collision.h"

namespace SDK {
	class SDK_Frustum {
	private:
		SDK::OOBB FrustumOOBB{};
		SDK::LineRectBrush LineRect{ false, true };

	public:
		SDK_Frustum();
		void Update();
		bool Check(SDK::AABB& aabb);
		bool Check(SDK::OOBB& oobb);
		bool Check(SDK::BoundingCircle& circle);
	};

	extern SDK::SDK_Frustum Frustum;
}