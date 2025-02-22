#include "SDK_StringTool.h"

SDK::SDK_StringTool SDK::StringTool;

void SDK::SDK_StringTool::InputWString(std::wstring& Str, SDK::KeyEvent& Event, bool UseNewLine) {
	if (Event.Type == WM_KEYDOWN) {
		switch (Event.Key) {
		case VK_SPACE:
			AddWString(Str, L" ");
			break;

		case VK_BACK:
			EraseWString(Str);
			break;

		case VK_RETURN:
			if (!UseNewLine) break;
			AddWString(Str, L"\n");
			break;
		}
	}

	else if(Event.Type == WM_CHAR && iswprint(Event.Key))
		Str.push_back(static_cast<wchar_t>(Event.Key));
}

void SDK::SDK_StringTool::AddString(std::string& Str, std::string AddStr) {
	Str += AddStr;
}

void SDK::SDK_StringTool::EraseString(std::string& Str) {
	if (!Str.empty()) {
		Str.pop_back();
		if (!Str.empty() && Str.back() == L'\n')
			Str.pop_back();
	}
}

void SDK::SDK_StringTool::RemoveString(std::string& Str, std::string RemoveStr) {
	if (Str.empty())
		return;

	size_t Pos = Str.find(RemoveStr);
	while (Pos != std::string::npos) {
		Str.erase(Pos, RemoveStr.length());
		Pos = Str.find(RemoveStr);
	}
}

void SDK::SDK_StringTool::AddWString(std::wstring& Str, std::wstring AddStr) {
	Str += AddStr;
}

void SDK::SDK_StringTool::EraseWString(std::wstring& Str) {
	if (!Str.empty()) {
		Str.pop_back();
		if (!Str.empty() && (Str.back() == L'\n'))
			Str.pop_back();
	}
}

void SDK::SDK_StringTool::RemoveWstring(std::wstring& Str, std::wstring RemoveStr) {
	if (Str.empty())
		return;

	size_t Pos = Str.find(RemoveStr);
	while (Pos != std::string::npos) {
		Str.erase(Pos, RemoveStr.length());
		Pos = Str.find(RemoveStr);
	}
}

std::wstring SDK::SDK_StringTool::Wstring(const std::string& Str) {
	if (Str.empty())
		return L"";

	int SizeNeed = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), (int)Str.size(), NULL, 0);
	if (SizeNeed <= 0) 
		return L"";
	
	std::wstring Wstr(SizeNeed, 0);
	MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), (int)Str.size(), &Wstr[0], SizeNeed);
	return Wstr;
}