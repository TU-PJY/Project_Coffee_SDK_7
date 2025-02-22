#pragma once
#include "SDK_Header.h"

namespace SDK {
	extern SDK::Matrix4 MoveMatrix, RotateMatrix, ScaleMatrix, FlipMatrix, ResultMatrix;

	extern SDK::Matrix4 UnitMoveMatrix, UnitRotateMatrix, UnitScaleMatrix;
	extern SDK::Matrix4 UnitFlipMatrix;

	extern SDK::Matrix4 ViewportPositionMatrix;

	extern SDK::Color3 ObjectColor;

	extern float UnitBlurValue;
	extern float ObjectBlurValue;

	extern float ObjectOpacityValue;
	extern float UnitOpacityValue;
}