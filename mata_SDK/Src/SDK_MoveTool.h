#pragma once
#include "SDK_Header.h"

namespace SDK {
	class SDK_MoveTool {
	public:
		void MoveStraight(float& Position, int MoveDirection, float Speed, float FT);
		void MoveStraight(float& Position, float Speed, float FT);
		void MoveForward(float& X, float& Y, float Speed, int MoveDirection, float RotationValue, float FT, bool Plus90Option);
		void MoveForward(SDK::Vector2& Position, float Speed, int MoveDirection, float RotationValue, float FrameTime, bool Plus90Option);
		void MoveForward(float& X, float& Y, float Speed, float RotationValue, float FT, bool Plus90Option);
		void MoveForward(SDK::Vector2& Position, float Speed, float RotationValue, float FrameTime, bool Plus90Option);
	};

	extern SDK::SDK_MoveTool MoveTool;
}