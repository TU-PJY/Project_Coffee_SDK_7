#include "SDK_Mouse.h"
#include "SDK_Camera.h"

SDK::SDK_Mouse SDK::Mouse;

void SDK::SDK_Mouse::ConvertPosition(int X, int Y) {
	x = (float)((X - (float)SDK::WIDTH / 2.0) * (float)(1.0 / (float)(SDK::WIDTH / 2.0))) * SDK::ASPECT;
	y = -(float)(Y - (float)SDK::HEIGHT / 2.0) * (float)(1.0 / (float)(SDK::HEIGHT / 2.0));
}
