#pragma once
#include "SDK_Object.h"

class SDK_FPS_Indicator : public SDK::Object {
private:
	SDK::Timer FPS_IND_REFRESH_TIMER{};
	float CurrentDeltaTime = 1.0;

	SDK::Text IndicatorText{};

public:
	SDK_FPS_Indicator() {
		IndicatorText.Init(SDK::SYSTEM_FONT, FW_BOLD);
		IndicatorText.SetColor(1.0, 1.0, 0.0);
		IndicatorText.SetHeightAlign(HEIGHT_ALIGN_UNDER);
		IndicatorText.EnableShadow();
		IndicatorText.SetShadow(0.07, -0.07, 1.0);
	}

	void UpdateFunc(float FrameTime) {
		FPS_IND_REFRESH_TIMER.Update(FrameTime);
		if (FPS_IND_REFRESH_TIMER.CheckMiliSec(0.5, 1, CHECK_AND_INTERPOLATE))
			CurrentDeltaTime = FrameTime;
	}

	void RenderFunc() {
		IndicatorText.Render(SDK::WindowRect.LeftX + 0.01, SDK::WindowRect.RightY, 0.1, L"FPS: %d", (int)(round((1.0 / CurrentDeltaTime))));
	}
};