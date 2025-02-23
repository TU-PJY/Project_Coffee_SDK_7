#include "Cover.h"

Cover::Cover(float Speed) {
	Rect.SetColor(0.0, 0.0, 0.0);
	Rect.SetRenderType(RENDER_TYPE_STATIC);
	OpacitySpeed = Speed;
}

void Cover::UpdateFunc(float FrameTime) {
	Opacity += FrameTime * OpacitySpeed;
	SDK::EXTool.ClampValue(Opacity, 1.0, CLAMP_GREATER);
}

void Cover::RenderFunc(){
	Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, Opacity);
}

bool Cover::GetState() {
	if (Opacity >= 1.0)
		return true;
	
	return false;
}
