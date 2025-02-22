#pragma once
#include "SDK_Header.h"

namespace SDK {
	class SDK_ComputeTool {
	public:
		void ComputeMatrix(SDK::Matrix4& Dest, SDK::Matrix4& Matrix1, SDK::Matrix4& Matrix2 = SDK::Matrix4(1.0f), SDK::Matrix4& Matrix3 = SDK::Matrix4(1.0f), SDK::Matrix4& Matrix4 = SDK::Matrix4(1.0f), SDK::Matrix4& Matrix5 = SDK::Matrix4(1.0f));
	};

	extern SDK::SDK_ComputeTool ComputeTool;
}