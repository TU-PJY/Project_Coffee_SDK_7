#pragma once
#include "SDK_Header.h"
#include "SDK_ObjectValue.h"

namespace SDK {
	class SDK_Transform {
	public:
		void Identity(SDK::Matrix4& Matrix);
		void Move(SDK::Matrix4& Matrix, float X, float Y);
		void Move(SDK::Matrix4& Matrix, SDK::Vector2& Position);
		void Rotate(SDK::Matrix4& Matrix, float Degree);
		void RotateRadians(SDK::Matrix4& Matrix, float Radians);
		void Scale(SDK::Matrix4& Matrix, float X, float Y);
		void Scale(SDK::Matrix4& Matrix, SDK::Vector2& Size);
		void Tilt(SDK::Matrix4& Matrix, float X, float Y);
		void Tilt(SDK::Matrix4& Matrix, SDK::Vector2& Value);
		void ImageScale(SDK::Matrix4& Matrix, float Width, float Height);
		void Flip(int FlipFlag);
		void UnitFlip(int FlipFlag);
		bool CheckIdentity(SDK::Matrix4& Matrix);
		void RotateV(SDK::Matrix4& Matrix, float Degree);
		void RotateH(SDK::Matrix4& Matrix, float Degree);
	};

	extern SDK::SDK_Transform Transform;
}