#pragma once
#include <SDK_Scene.h>

class Cover : public SDK::Object {
private:
	float Opacity{ 0.0 };
	SDK::RectBrush Rect{};

public:
	Cover();
	void UpdateFunc(float FrameTime);
	void RenderFunc();
	bool GetState();
};