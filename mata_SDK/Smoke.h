#pragma once
#include <SDK_Scene.h>

class Smoke : public SDK::Object {
private:
	SDK::Vector2 Position{};

	// 새로운 연기 객체 추가 여부
	bool NewSmokeAdded{};

	// 크기
	float Size { 2.5 };
	
public:
	Smoke(float PositionValue) {
		Position.x = PositionValue;
		Position.y = -1.0 - Size * 0.5;
	}

	void UpdateFunc(float FrameTime) {
		Position.y += FrameTime * 0.8;

		// 연기가 화면 위쪽까지 도달하면 아래쪽에 새로운 객체를 생성한다
		if (!NewSmokeAdded && Position.y - Size * 0.5 >= -1.0) {
			Position.y = -1.0 + Size * 0.5;
			SDK::Scene.AddObject(new Smoke(Position.x), "smoke", LAYER6);
			NewSmokeAdded = true;
		}

		if(Position.y - Size * 0.5 >= 1.0 || Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - Size * 0.5)
			SDK::Scene.DeleteObject(this);
	}

	void RenderFunc() {
		if (Position.x > SDK::Camera.Position.x + SDK::ASP(1.0) + Size * 0.5)
			return;

		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, Position.x + 0.2, Position.y);
		SDK::Transform.Scale(SDK::MoveMatrix, Size, Size);
		SDK::ImageTool.RenderImage(SDK::IMAGE.Smoke, 0.5);
	}
};