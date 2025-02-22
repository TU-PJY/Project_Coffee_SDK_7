#pragma once
#include "SDK_Header.h"

namespace SDK {
class SDK_ColorClipping {
public:
	void First();
	void Second();
	void End();
};

class SDK_AlphaClipping {
public:
	void First();
	void Second();
	void End();
};

	extern SDK_ColorClipping ColorClip;
	extern SDK_AlphaClipping AlphaClip;
}