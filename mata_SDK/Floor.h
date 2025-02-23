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
		// �ڽ��� ��ġ�� ȭ�� �������� �鷯���� �ڽ��� �ٷ� �����ʿ� ���ο� ��ü�� �߰��Ѵ�
		if (!NewFloorGenerated && Position - 0.4 <= SDK::Camera.Position.x + SDK::ASP(1.0) + 0.4) {
			SDK::Scene.AddObject(new Floor(Position + 0.8, false), "floor", LAYER_BG);
			NewFloorGenerated = true;
		}

		// ȭ���� �ָ� ����� �����Ѵ�
		if (Position < SDK::Camera.Position.x - SDK::ASP(1.0) - 3.0)
			SDK::Scene.DeleteObject(this);
	}

	void RenderFunc() {
		// ȭ�鿡�� ������ �ʴ� �ٴ��� ���������� �ʴ´�
		if (Position > SDK::Camera.Position.x + SDK::ASP(1.0) + 0.4)
			return;

		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, Position, -0.85);
		SDK::Transform.Scale(SDK::MoveMatrix, 0.8, 0.8);
		SDK::ImageTool.RenderImage(SDK::IMAGE.FloorTile);
	} 
};