#pragma once
#include <SDK_Scene.h>

class Cart : public SDK::Object {
private:
	bool FellDown{};
	SDK::Vector2 Position{};
	int Frame{};

public:
	Cart(bool Fell, SDK::Vector2 PositionValue);
	void UpdateFunc(float FrameTime);
	void RenderFunc();
};