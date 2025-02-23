#pragma once
#include <SDK_Scene.h>

class Cover : public SDK::Object {
private:
	float Opacity{ 0.0 };
	SDK::RectBrush Rect{};
	float OpacitySpeed{};

public:
	Cover(float Speed=1.0);
	void UpdateFunc(float FrameTime);
	void RenderFunc();
	bool GetState();
};


class DisappearCover : public SDK::Object {
private:
	SDK::RectBrush Rect{};
	float Opacity{ 1.0 };

public:
	DisappearCover() {
		Rect.SetRenderType(RENDER_TYPE_STATIC);
		Rect.SetColor(0.0, 0.0, 0.0);
	}

	void UpdateFunc(float FrameTime) {
		Opacity -= FrameTime * 4.0;
		SDK::EXTool.ClampValue(Opacity, 0.0, CLAMP_LESS);
		if (Opacity <= 0.0)
			SDK::Scene.DeleteObject(this);
	}

	void RenderFunc() {
		Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, Opacity);
	}
};