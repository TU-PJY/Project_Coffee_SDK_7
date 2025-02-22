#pragma once
#include "SDK_Header.h"

namespace SDK {
	class SDK_Math {
	public:
		void LookAt(float& DestRotation, float FromX, float FromY, float ToX, float ToY, float RotationSpeed = 0.0, float FrameTime = 0.0);
		void LookAt(float& DestRotation, SDK::Vector2& Position1, SDK::Vector2& Position2, float Rotationpeed = 0.0, float FrameTime = 0.0);
		void LookAt(float& DestRotation, float Rotation, float RotationSpeed = 0.0, float FrameTime = 0.0);
		float ComputeDistance(float FromX, float FromY, float ToX, float ToY);
		float ComputeDistance(SDK::Vector2& Position1, SDK::Vector2& Position2);
		SDK::Vector2 ComputeMidPoint(SDK::Vector2& Position1, SDK::Vector2& Position2);
		void Lerp(float& DestValue, float Dest, float Speed, float FrameTime);
		void Lerp(SDK::Vector2& DestValue, SDK::Vector2& Dest, float Speed, float FrameTime);
		void Lerp(SDK::Vector3& DestValue, SDK::Vector3& Dest, float Speed, float FrameTime);
		float Computedegree(float FromX, float FromY, float ToX, float ToY);
		float Computedegree(SDK::Vector2& Position1, SDK::Vector2& Position2);
		float ComputeRadians(float FromX, float FromY, float ToX, float ToY);
		float ComputeRadians(SDK::Vector2& Position1, SDK::Vector2& Position2);
		SDK::RayVector ComputeRay(float FromX, float FromY, float ToX, float ToY);
		SDK::RayVector ComputeRay(SDK::Vector2& From, SDK::Vector2& To);
		SDK::RayVector ComputeRayWithDegree(float OriginX, float OriginY, float Degree, float LengthValue = 0.0);
		SDK::RayVector ComputeRayWithDegree(SDK::Vector2& Origin, float Degree, float LengthValue = 0.0);
	private:
		float NormalizeDegree(float Degree);
		float ComputeShortestRotation(float CurrentDegree, float DegreeDest);
	};

	extern SDK::SDK_Math Math;
}