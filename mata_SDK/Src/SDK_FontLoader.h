#pragma once
#include "SDK_Header.h"
namespace SDK {
	class SDK_FontLoader {
	public:
		bool Load(const std::string& FontFilePath);
		bool LoadT(const std::string& FontFilePath);
	};

	extern SDK::SDK_FontLoader FontLoader;
}
