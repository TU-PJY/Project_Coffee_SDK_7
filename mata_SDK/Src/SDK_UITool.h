#pragma once
#include "SDK_Header.h"

namespace SDK {
	class SDK_UITool {
	public:
		void ClampMatrixToCorner(int EdgePosition, SDK::Matrix4& Matrix, float Width, float Height, float GapX, float GapY);
		void ClampMatrixToPosition(SDK::Matrix4& Matrix, float PositionX, float PositionY, float Width, float Height, float GapX, float GapY);
		void ClampPositionToCorner(int EdgePosition, float& PositionX, float& PositionY, float Width, float Height, float GapX, float GapY);
		void ClampPositionToCoord(float& ThisPositionX, float& ThisPositionY, float OtherPositionX, float OtherPositionY, float Width, float Height, float GapX, float GapY);
	};

	extern SDK::SDK_UITool UITool;
}