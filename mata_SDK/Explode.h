#pragma once
#include <SDK_Scene.h>

typedef struct {
	SDK::Physics ps;
	SDK::Vector2 Position;
	float MoveSpeed;
	float Rotation;
	float RotateSpeed;
	bool DropState;
}DropStruct;

class Explode : public SDK::Object {
private:
	// Ŀ�� ����� �����͸� �����ϴ� ����
	std::vector<DropStruct> DropVec{};

	// �ùķ��̼��� ��� ����
	int NumDrop = 15;

	// ���� Ŀ�Ǹ� �μ� ��� Ȱ��ȭ �Ѵ� 
	bool IsStick{};

public:
	Explode(SDK::Vector2 Position, bool StickCoffee) {
		IsStick = StickCoffee;

		// ��ƽ Ŀ���� ��� ������ ���δ�
		if (StickCoffee)
			NumDrop = 8;

		for (int i = 0; i < NumDrop; ++i) {
			DropStruct DS{};
			DS.Position = Position;

			// �߷� ����
			DS.ps.SetGravity(10.0);

			// ���� �ٴ� ���� ����
			float RandomFloorHeight = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.85, -0.95);
			DS.ps.SetFloorHeight(RandomFloorHeight);

			// ���� �߷� ���ӵ� �߰�
			float RandomAcc = SDK::Random.Gen(RANDOM_TYPE_REAL, 3.0, 5.0);
			DS.ps.AddGravityAcc(RandomAcc);

			// ���� �ӵ� ����
			DS.MoveSpeed = SDK::Random.Gen(RANDOM_TYPE_REAL, -2.0, 2.0);

			// ���� Ŀ�Ǹ� �ν��� ��� ���� ȸ�� �ӵ��� �����Ѵ�
			if (StickCoffee)
				DS.RotateSpeed = SDK::Random.Gen(RANDOM_TYPE_REAL, 200.0, 1500.0);

			DropVec.emplace_back(DS);
		}
	}

	void UpdateFunc(float FrameTime) {
		// �� ����� ���� �ùķ��̼� ����
		for (auto& D : DropVec) {
			// ȭ�� ���� ��� ��ü�� ������Ʈ ���� �ʴ´�.
			if (D.Position.x > SDK::Camera.Position.x - SDK::ASP(1.0) - 0.12) {
				D.ps.UpdateFalling(D.Position.y, FrameTime);

				// �ٴڿ� ������������ �����δ�
				if (D.ps.GetFallingState()) {
					D.Position.x += D.MoveSpeed * FrameTime;

					// Ŀ�� ��ƽ�� ��� ȸ�� ������Ʈ
					if (IsStick)
						D.Rotation += D.RotateSpeed * FrameTime;
				}
			}
		}
	}

	void RenderFunc() {
		int Count{};

		// ȭ���� ����� �������� �ǳʶڴ�
		for (auto& D : DropVec) {
			if (IsStick) {
				if (D.Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - 0.12) {
					Count++;
					continue;
				}
			}
			else {
				if (D.Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - 0.06) {
					Count++;
					continue;
				}
			}

			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, D.Position);

			if (IsStick) {
				SDK::Transform.Scale(SDK::MoveMatrix, 0.2, 0.2);
				SDK::Transform.Rotate(SDK::MoveMatrix, D.Rotation);
				SDK::ImageTool.RenderImage(SDK::IMAGE.Stick);
			}
			else {
				// �ٴڿ� �������� �ٸ� ��������Ʈ�� �������Ѵ�
				if (D.ps.GetFallingState()) {
					SDK::Transform.Scale(SDK::MoveMatrix, 0.06, 0.06);
					SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Drop, 0);
				}
				else {
					SDK::Transform.Scale(SDK::MoveMatrix, 0.12, 0.12);
					SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Drop, 1);
				}
			}
		}

		// ��� ����� ȭ�� ���� ����� ������ �����Ǹ� ������ �����Ѵ�
		if (Count == NumDrop)
			SDK::Scene.DeleteObject(this);
	}
};