#pragma once
#include "SDK_Header.h"
#include <cmath>

namespace SDK {
	class Physics {
	private:
		float Gravity{};
		float Friction{};
		float RebounceReduce{};
		float MinRebounceValue{};

		float FloorHeight{};
		float HeightOffset{};

		float GravityAcc{};
		float MoveAcc{};

		bool    FallingState{};

	public:
		bool GetFallingState();
		void EnableFalling();
		void DisableFalling();
		void AddGravityAcc(float AccValue);
		void SetGravityAcc(float AccValue);
		void SetGravity(float Value);
		void SetFloorHeight(float Value);
		void SetFriction(float Value);
		void SetHeightOffset(float Value);
		void SetRebounceReduce(float Value);
		void SetMinimumRebounce(float Value);
		bool CheckFloorCollision(float& Position);
		bool CheckFloorCollision(SDK::Vector2& DestPosition);
		void LandOnFloor(float& Position);
		void LandOnFloor(SDK::Vector2& DestPosition);
		void UpdateFalling(float& HeightPosition, float FrameTime);
		void UpdateFalling(SDK::Vector2& DestPosition, float FrameTime);
		void UpdateBouncing(float& HeightPosition, float FrameTime);
		void UpdateBouncing(SDK::Vector2& DestPosition, float FrameTime);
		void LerpAcceleratation(float& Speed, float DestSpeed, float AccValue, float FT);
		void LerpDeceleration(float& Speed, float FT);
		void LinearAcceleratation(float& Speed, float DestSpeed, float AccValue, float FT);
		void LinearDeceleration(float& Speed, float FT);
		void AdjustSpeedEqual(float& SpeedX, float& SpeedY);
	};
}