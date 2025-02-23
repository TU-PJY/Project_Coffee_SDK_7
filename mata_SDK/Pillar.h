#pragma once
#include <SDK_Scene.h>

class Pillar : public SDK::Object {
private:
	float Position{};
	bool NextPillarGenerated{};
	bool GenerateNextPillar{};

public:
	Pillar(float PositionValue, bool GenNextFlag=true) {
		GenerateNextPillar = GenNextFlag;
		Position = PositionValue;
	}

	void UpdateFunc(float FrameTime) {
		if (GenerateNextPillar) {
			if (!NextPillarGenerated && Position + SDK::Camera.Position.x * 0.5 < SDK::Camera.Position.x + SDK::ASP(0.5)) {
				SDK::Scene.AddObject(new Pillar(Position + 3.0), "pillar", LAYER_BG);
				NextPillarGenerated = true;
			}
		}

		if (Position + SDK::Camera.Position.x * 0.5 < SDK::Camera.Position.x - SDK::ASP(1.0) - 0.75)
			SDK::Scene.DeleteObject(this);
	}

	void RenderFunc() {
		if (Position + SDK::Camera.Position.x * 0.5 > SDK::Camera.Position.x + SDK::ASP(1.0) + 0.75)
			return;

		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, Position + SDK::Camera.Position.x * 0.5, 0.6);
		SDK::Transform.Scale(SDK::MoveMatrix, 3.0, 3.0);
		SDK::ImageTool.RenderImage(SDK::IMAGE.Pillar);
	}
};

