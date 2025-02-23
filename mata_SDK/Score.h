#pragma once
#include <SDK_Scene.h>

class ScoreIndicator : public SDK::Object {
private:
	float FeedbackHeight{};
	float FeedbackOpacity{};
	int AddedScore{};

	SDK::Timer Timer{};

	SDK::Text Text{};

public:
	ScoreIndicator() {
		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.EnableShadow();
		Text.SetShadow(0.1, -0.1, 0.7);
	}

	void UpdateFunc(float FrameTime) {
		if (SDK::Scene.Mode() != SDK::MODE.Play)
			return;

		SDK::Math.Lerp(FeedbackHeight, 0.0, 8.0, FrameTime);

		if(FeedbackOpacity > 0.0) {
			Timer.Update(FrameTime);
			if (Timer.MiliSec(1) >= 0.5)
				FeedbackOpacity -= FrameTime * 2.0;
			SDK::EXTool.ClampValue(FeedbackOpacity, 0.0, CLAMP_LESS);
		}
		else 
			Timer.Reset();
	}

	void RenderFunc() {
		if (SDK::Scene.Mode() != SDK::MODE.Play)
			return;
		Text.SetColorRGB(255, 213, 80);
		Text.SetOpacity(1.0);
		Text.Render((SDK::ASP(-1.0) + 0.07), (-1.0 + 0.1 + FeedbackHeight), 0.15, L"SCORE %d", SDK::GLOBAL.Score);
		if (FeedbackOpacity > 0.0) {
			Text.SetOpacity(FeedbackOpacity);
			Text.Render((SDK::ASP(-1.0) + 0.07), (-1.0 + 0.3 + FeedbackHeight), 0.1, L"+%d", AddedScore);
		}
	}

	void AddScore(int Value) {
		Timer.Reset();
		SDK::GLOBAL.Score += Value;
		AddedScore = Value;
		FeedbackHeight = 0.1;
		FeedbackOpacity = 1.0;
	}
};