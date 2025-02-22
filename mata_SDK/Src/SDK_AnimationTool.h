#pragma once
#include "SDK_Header.h"
#include <cmath>

namespace SDK {
	class SinLoop {
	private:
		float Num{ 0.0 };
		SDK::Vector2 Num2{ 0.0, 0.0 };

	public:
		void Update(float& Value, float MoveScale, float Speed, float FrameTime);
		void Update(SDK::Vector2& Value, SDK::Vector2& MoveScale, SDK::Vector2& Speed, float FrameTime);
		void SetValue(float Value);
		void SetValue(SDK::Vector2& Value);
		void Reset();
	};

	class SinMove {
	private:
		float Num{SDK::Preset::MaxNegative};
		float SingleStartPosition{};
		float SingleDistance{};

		SDK::Vector2 StartPosition{};
		SDK::Vector2 Distance{};

	public:
		void SetMovePoint(float StartPoint, float EndPoint);
		void SetMovePoint(SDK::Vector2& StartPoint, SDK::Vector2& EndPoint);
		void SetValue(float Value);
		void Update(float& Value, float Speed, float FrameTime);
		void Update(SDK::Vector2& Value, float Speed, float FrameTime);
		void Reset();
	};

	class PopBounce {
	private:
		float Num1{}, Num2{}, Num3{};

	public:
		void Update(float& Value, float SizeDest, float ShakeValue, float SizeIncreaseSpeed, float ShakeSpeed, float ShakeReduceSpeed, float FrameTime);
		void Reset();
	};

	class ReverseLerp {
	private:
		bool  SingleMoveState{ true };
		float SingleVelocity{};
		float SingleAcc{};
		float SingleDistance{};
		float SingleStartPosition{};

		bool MoveState[2] { true, true };
		SDK::Vector2 Velocity{};
		SDK::Vector2 Acc{};
		SDK::Vector2 Distance{};
		SDK::Vector2 StartPosition{};

	public:
		void Update(float& Value, float Speed, float IncreaseSpeed, float FrameTime);
		void Update(SDK::Vector2& Value, float Speed, float IncreaseSpeed, float FrameTime);
		void SetMovePoint(float StartPoint, float EndPoint);
		void SetMovePoint(SDK::Vector2& StartPoint, SDK::Vector2& EndPoint);
		void Reset();
	};
}
