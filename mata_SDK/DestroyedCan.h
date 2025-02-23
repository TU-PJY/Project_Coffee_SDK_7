#pragma once
#include <SDK_Scene.h>

class DestroyedCan : public SDK::Object {
private:
	SDK::Physics ps{};
	SDK::Vector2 Position{};
	float MoveSpeed{};

	float Rotation{};
	float RotateSpeed{};

	SDK::SoundChannel SndChannel{};

	SDK::Timer Timer{};

	bool SoundPlayed{};

public:
	DestroyedCan(SDK::Vector2 PositionValue) {
		Position = PositionValue;

		// 중력 지정
		ps.SetGravity(15.0);

		// 랜덤 바닥 높이 설정
		GLfloat RandomFloorHeight = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.85, -0.95);
		ps.SetFloorHeight(RandomFloorHeight);

		// 랜덤 중력 가속도 설정
		GLfloat RandomAcc = SDK::Random.Gen(RANDOM_TYPE_REAL, 5.0, 8.0);
		ps.AddGravityAcc(RandomAcc);

		// 최소 바운스 수치와 바운드 감소 수치 설정
		ps.SetMinimumRebounce(1.0);
		ps.SetRebounceReduce(2.0);

		// 랜덤 속도 지정
		MoveSpeed = SDK::Random.Gen(RANDOM_TYPE_REAL, 1.0, 3.0);

		// 랜덤 회전 속도 지정
		RotateSpeed = SDK::Random.Gen(RANDOM_TYPE_REAL, 800.0, 1500.0);

		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);
	}

	void UpdateFunc(float FrameTime) {
		// 떨어지면서 바닥에 튕기는 물리 엔진 시뮬레이션 실행
		if (ps.GetFallingState()) {
			Position.x += MoveSpeed * FrameTime;
			Rotation += RotateSpeed * FrameTime;

			// 바닥에 튕길때마다 소리를 재생한다
			if (ps.CheckFloorCollision(Position)) {
				RotateSpeed *= 0.5;
				MoveSpeed *= 0.7;
				SDK::SoundTool.Stop(SndChannel);
				SDK::SoundTool.Play(SDK::SOUND.CanDrop, SndChannel);
			}
		}
		else {
			SDK::SoundTool.PlayOnce(SDK::SOUND.CanDrop, SndChannel, SoundPlayed);
		}

		ps.UpdateBouncing(Position, FrameTime);

		// 화면을 벗어날 경우 삭제
		if (Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - 0.255)
			SDK::Scene.DeleteObject(this);
	}

	void RenderFunc() {
		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, Position);
		SDK::Transform.Rotate(SDK::MoveMatrix, Rotation);
		SDK::Transform.Scale(SDK::MoveMatrix, 0.45, 0.45);
		SDK::ImageTool.RenderImage(SDK::IMAGE.DestroyedCan);
	}
};