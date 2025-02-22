#pragma once
#include "SDK_Header.h"
 
namespace SDK {
	class SDK_StringTool {
	public:
		void InputWString(std::wstring& Str, SDK::KeyEvent& Event, bool UseNewLine);
		void AddString(std::string& Str, std::string AddStr);
		void EraseString(std::string& Str);
		void RemoveString(std::string& Str, std::string RemoveStr);
		void AddWString(std::wstring& Str, std::wstring AddStr);
		void EraseWString(std::wstring& Str);
		void RemoveWstring(std::wstring& Str, std::wstring RemoveStr);
		std::wstring Wstring(const std::string& Str);
	};

	extern SDK::SDK_StringTool StringTool;
}