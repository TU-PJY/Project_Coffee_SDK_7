#pragma once
#include <SDK_Scene.h>

class Floor : public SDK::Object {
	float Position{};
	bool NewFloorGenerated{};

public:
	Floor(float PositionValue, bool GenFlag) {
		Position = PositionValue;
		NewFloorGenerated = GenFlag;
	}

	void UpdateFunc(float FrameTime) {
		// 자신의 위치가 화면 안쪽으로 들러오면 자신의 바로 오른쪽에 새로운 객체를 추가한다
		if (!NewFloorGenerated && Position - 0.4 <= SDK::Camera.Position.x + SDK::ASP(1.0) + 0.4) {
			SDK::Scene.AddObject(new Floor(Position + 0.8, false), "floor", LAYER_BG);
			NewFloorGenerated = true;
		}

		// 화면을 멀리 벗어나면 삭제한다
		if (Position < SDK::Camera.Position.x - SDK::ASP(1.0) - 3.0)
			SDK::Scene.DeleteObject(this);
	}

	void RenderFunc() {
		// 화면에서 보이지 않는 바닥은 렌더링하지 않는다
		if (Position > SDK::Camera.Position.x + SDK::ASP(1.0) + 0.4)
			return;

		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, Position, -0.85);
		SDK::Transform.Scale(SDK::MoveMatrix, 0.8, 0.8);
		SDK::ImageTool.RenderImage(SDK::IMAGE.FloorTile);
	} 
};