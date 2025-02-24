#pragma once
#include <SDK_Scene.h>

class Tutorial : public SDK::Object {
public:
	bool RenderState{ true };

	SDK::RectBrush Rect{};
	float RectOpacity{1.0};

	Tutorial() {
		// 게임을 최초 실행했을 때만 튜토리얼을 보여주도록 한다
		SDK::GLOBAL.NeedTutorial = false;
		SDK::FILE.HighscoreData.UpdateDigitData("TutorialNeed", "Bool", 0);
		Rect.SetRenderType(RENDER_TYPE_STATIC);
		Rect.SetColor(0.0, 0.0, 0.0);
	}

	void UpdateFunc(float FrameTime) {
		RectOpacity -= 4.0 * FrameTime;
		SDK::EXTool.ClampValue(RectOpacity, 0.0, CLAMP_LESS);
	}

	void RenderFunc() {
		Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0);

		if (RenderState) {
			Begin(RENDER_TYPE_STATIC);
			float SizeX = 2.0 * SDK::ASPECT;
			float SizeY = 2.0 * SDK::ASPECT;
			float ImgAspect = 2560.0 / 1440.0;

			if (SizeY >= 2.0 * ImgAspect) {
				SizeY = 2.0 * ImgAspect;
				SizeX = 2.0 * ImgAspect;
			}

			SDK::Transform.Scale(SDK::MoveMatrix, SizeX, SizeY);
			SDK::ImageTool.RenderImage(SDK::IMAGE.Tutorial);

			Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, RectOpacity);
		}
	}

	void DisableRenderState() {
		RenderState = false;
	}
};