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

		// �߷� ����
		ps.SetGravity(15.0);

		// ���� �ٴ� ���� ����
		GLfloat RandomFloorHeight = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.85, -0.95);
		ps.SetFloorHeight(RandomFloorHeight);

		// ���� �߷� ���ӵ� ����
		GLfloat RandomAcc = SDK::Random.Gen(RANDOM_TYPE_REAL, 5.0, 8.0);
		ps.AddGravityAcc(RandomAcc);

		// �ּ� �ٿ ��ġ�� �ٿ�� ���� ��ġ ����
		ps.SetMinimumRebounce(1.0);
		ps.SetRebounceReduce(2.0);

		// ���� �ӵ� ����
		MoveSpeed = SDK::Random.Gen(RANDOM_TYPE_REAL, 1.0, 3.0);

		// ���� ȸ�� �ӵ� ����
		RotateSpeed = SDK::Random.Gen(RANDOM_TYPE_REAL, 800.0, 1500.0);

		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);
	}

	void UpdateFunc(float FrameTime) {
		// �������鼭 �ٴڿ� ƨ��� ���� ���� �ùķ��̼� ����
		if (ps.GetFallingState()) {
			Position.x += MoveSpeed * FrameTime;
			Rotation += RotateSpeed * FrameTime;

			// �ٴڿ� ƨ�涧���� �Ҹ��� ����Ѵ�
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

		// ȭ���� ��� ��� ����
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