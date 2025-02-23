#pragma once
#include <SDK_Scene.h>

class BackgroundShelf : public SDK::Object{
private:
	float Position{};
	bool NextShelfGenerated{};

public:
	BackgroundShelf(float PositionValue) {
		Position = PositionValue;
	}

	void UpdateFunc(float FrameTime) {
		if (!NextShelfGenerated && Position < SDK::Camera.Position.x + SDK::ASP(1.0) - 2.5) {
			SDK::Scene.AddObject(new BackgroundShelf(Position + 5.0), "background_shelf", LAYER1);
			NextShelfGenerated = true;
		}

		if (Position < SDK::Camera.Position.x - SDK::ASP(1.0) - 2.5)
			SDK::Scene.DeleteObject(this);
	}

	void RenderFunc() {
		if (Position> SDK::Camera.Position.x + SDK::ASP(1.0) + 2.5)
			return;

		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, Position, 0.1);
		SDK::Transform.Scale(SDK::MoveMatrix, 5.0, 5.0);
		SDK::ImageTool.RenderImage(SDK::IMAGE.BackGroundShelf);
	}
};