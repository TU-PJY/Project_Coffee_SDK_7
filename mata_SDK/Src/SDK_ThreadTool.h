#pragma once
#include "SDK_Header.h"

namespace SDK {
	class SDK_ThreadTool {
	public:
		void Create(HANDLE& HandleValue, LPTHREAD_START_ROUTINE ThreadFunction, LPVOID Param = NULL);
		bool CheckAlive(HANDLE& HandleValue);
		void Close(HANDLE& HandleValue);
		void InitSection(CRITICAL_SECTION& Section);
		bool InitSectionAndCount(CRITICAL_SECTION& Section, int SpinCount);
		void DeleteSection(CRITICAL_SECTION& Section);
		void Lock(CRITICAL_SECTION& Section);
		void TryLock(CRITICAL_SECTION& Section);
		void Unlock(CRITICAL_SECTION& Section);
	};

	extern SDK::SDK_ThreadTool ThreadTool;
}