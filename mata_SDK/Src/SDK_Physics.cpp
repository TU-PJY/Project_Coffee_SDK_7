#include "SDK_Physics.h"
#include "SDK_Math.h"
#include "SDK_EXTool.h"

void SDK::Physics::SetGravity(float Value) {
	if (Value < 0.0) return;
	Gravity = Value;
}

void SDK::Physics::SetFriction(float Value) {
	if (Value < 0.0) return;
	Friction = Value;
}

void SDK::Physics::SetRebounceReduce(float Value) {
	if (Value < 0.0) return;
	RebounceReduce = Value;
}

void SDK::Physics::SetMinimumRebounce(float Value) {
	if (Value < 0.0) return;
	MinRebounceValue = Value;
}

void SDK::Physics::SetFloorHeight(float Value) {
	FloorHeight = Value;
}

void SDK::Physics::SetHeightOffset(float Value) {
	HeightOffset = Value;
}

bool SDK::Physics::GetFallingState() {
	return FallingState;
}

void SDK::Physics::AddGravityAcc(float Value) {
	GravityAcc += Value;
	FallingState = true;
}

void SDK::Physics::SetGravityAcc(float Value) {
	GravityAcc = Value;
	FallingState = true;
}

void SDK::Physics::EnableFalling() {
	FallingState = true;
}

void SDK::Physics::DisableFalling() {
	FallingState = false;
}

bool SDK::Physics::CheckFloorCollision(float& HeightPosition) {
	if (HeightPosition <= FloorHeight + HeightOffset)
		return true;
	return false;
}

bool SDK::Physics::CheckFloorCollision(SDK::Vector2& DestPosition) {
	if (DestPosition.y <= FloorHeight + HeightOffset)
		return true;
	return false;
}

void SDK::Physics::LandOnFloor(float& HeightPosition) {
	HeightPosition = FloorHeight + HeightOffset;
	FallingState = false;
}

void SDK::Physics::LandOnFloor(SDK::Vector2& DestPosition) {
	DestPosition.y = FloorHeight + HeightOffset;
	FallingState = false;
}

void SDK::Physics::UpdateFalling(float& HeightPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		HeightPosition += GravityAcc * FrameTime;
		if (CheckFloorCollision(HeightPosition))
			LandOnFloor(HeightPosition);
	}
}

void SDK::Physics::UpdateFalling(SDK::Vector2& DestPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		DestPosition.y += GravityAcc * FrameTime;
		if (CheckFloorCollision(DestPosition.y))
			LandOnFloor(DestPosition.y);
	}
}

void SDK::Physics::UpdateBouncing(float& HeightPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		HeightPosition += GravityAcc * FrameTime;

		if (CheckFloorCollision(HeightPosition)) {
			HeightPosition = FloorHeight + HeightOffset;
			GravityAcc *= -1;
			GravityAcc -= RebounceReduce;

			if (fabs(GravityAcc) <= MinRebounceValue) {
				GravityAcc = 0.0;
				LandOnFloor(HeightPosition);
			}
		}
	}
}

void SDK::Physics::UpdateBouncing(SDK::Vector2& DestPosition, float FrameTime) {
	if (FallingState) {
		GravityAcc -= Gravity * FrameTime;
		DestPosition.y += GravityAcc * FrameTime;

		if (CheckFloorCollision(DestPosition.y)) {
			DestPosition.y = FloorHeight + HeightOffset;
			GravityAcc *= -1;
			GravityAcc -= RebounceReduce;

			if (fabs(GravityAcc) <= MinRebounceValue) {
				GravityAcc = 0.0;
				LandOnFloor(DestPosition.y);
			}
		}
	}
}


void SDK::Physics::LerpAcceleratation(float& Speed, float DestSpeed, float AccValue, float FT) {
	SDK::Math.Lerp(Speed, DestSpeed, AccValue * (1.0 - Friction), FT);
}

void SDK::Physics::LerpDeceleration(float& Speed, float FT) {
	SDK::Math.Lerp(Speed, 0.0, Friction, FT);
}

void SDK::Physics::LinearAcceleratation(float& Speed, float DestSpeed, float AccValue, float FT) {
	if (DestSpeed > 0.0) {
		Speed += AccValue * (1.0 - Friction) * FT;
		EXTool.ClampValue(Speed, DestSpeed, CLAMP_GREATER);
	}
	else {
		Speed -= AccValue * (1.0 - Friction) * FT;
		EXTool.ClampValue(Speed, DestSpeed, CLAMP_LESS);
	}
}

void SDK::Physics::LinearDeceleration(float& Speed, float FT) {
	if (Speed > 0) {
		Speed -= Friction * FT;
		EXTool.ClampValue(Speed, 0.0, CLAMP_LESS);
	}
	else if (Speed < 0) {
		Speed += Friction * FT;
		EXTool.ClampValue(Speed, 0.0, CLAMP_GREATER);
	}
}

void SDK::Physics::AdjustSpeedEqual(float& SpeedX, float& SpeedY) {
	float Length = std::sqrt(SpeedX * SpeedX + SpeedY * SpeedY);
	if (Length > 1.0) {
		SpeedX /= Length;
		SpeedY /= Length;
	}
}