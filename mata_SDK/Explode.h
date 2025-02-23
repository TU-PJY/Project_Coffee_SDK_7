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
	// 커피 방울의 데이터를 저장하는 벡터
	std::vector<DropStruct> DropVec{};

	// 시뮬레이션할 방울 개수
	int NumDrop = 15;

	// 종이 커피를 부술 경우 활성화 한다 
	bool IsStick{};

public:
	Explode(SDK::Vector2 Position, bool StickCoffee) {
		IsStick = StickCoffee;

		// 스틱 커피일 경우 개수를 줄인다
		if (StickCoffee)
			NumDrop = 8;

		for (int i = 0; i < NumDrop; ++i) {
			DropStruct DS{};
			DS.Position = Position;

			// 중력 지정
			DS.ps.SetGravity(10.0);

			// 랜덤 바닥 높이 지정
			float RandomFloorHeight = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.85, -0.95);
			DS.ps.SetFloorHeight(RandomFloorHeight);

			// 랜덤 중력 가속도 추가
			float RandomAcc = SDK::Random.Gen(RANDOM_TYPE_REAL, 3.0, 5.0);
			DS.ps.AddGravityAcc(RandomAcc);

			// 랜덤 속도 지정
			DS.MoveSpeed = SDK::Random.Gen(RANDOM_TYPE_REAL, -2.0, 2.0);

			// 종이 커피를 부쉈을 경우 랜덤 회전 속도를 지정한다
			if (StickCoffee)
				DS.RotateSpeed = SDK::Random.Gen(RANDOM_TYPE_REAL, 200.0, 1500.0);

			DropVec.emplace_back(DS);
		}
	}

	void UpdateFunc(float FrameTime) {
		// 각 방울의 물리 시뮬레이션 실행
		for (auto& D : DropVec) {
			// 화면 밖을 벗어난 객체는 업데이트 하지 않는다.
			if (D.Position.x > SDK::Camera.Position.x - SDK::ASP(1.0) - 0.12) {
				D.ps.UpdateFalling(D.Position.y, FrameTime);

				// 바닥에 떨어질때까지 움직인다
				if (D.ps.GetFallingState()) {
					D.Position.x += D.MoveSpeed * FrameTime;

					// 커피 스틱일 경우 회전 업데이트
					if (IsStick)
						D.Rotation += D.RotateSpeed * FrameTime;
				}
			}
		}
	}

	void RenderFunc() {
		int Count{};

		// 화면을 벗어나면 렌더링을 건너뛴다
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
				// 바닥에 떨어지면 다른 스프라이트를 렌더링한다
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

		// 모든 방울이 화면 밖을 벗어나는 것으로 판정되면 스스로 삭제한다
		if (Count == NumDrop)
			SDK::Scene.DeleteObject(this);
	}
};