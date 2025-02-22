#include "SDK_ThreadTool.h"

SDK::SDK_ThreadTool SDK::ThreadTool;

void SDK::SDK_ThreadTool::Create(HANDLE& HandleValue, LPTHREAD_START_ROUTINE ThreadFunction, LPVOID Param) {
	HandleValue = CreateThread(NULL, 0, ThreadFunction, Param, 0, NULL);
}

bool SDK::SDK_ThreadTool::CheckAlive(HANDLE& HandleValue) {
	DWORD Result;
	GetExitCodeThread(HandleValue, &Result);

	if (Result == STILL_ACTIVE)
		return true;

	return false;
}

void SDK::SDK_ThreadTool::Close(HANDLE& HandleValue) {
	if(HandleValue)
		CloseHandle(HandleValue);
}

void SDK::SDK_ThreadTool::InitSection(CRITICAL_SECTION& Section) {
	InitializeCriticalSection(&Section);
}

bool SDK::SDK_ThreadTool::InitSectionAndCount(CRITICAL_SECTION& Section, int SpinCount) {
	return InitializeCriticalSectionAndSpinCount(&Section, SpinCount);
}

void SDK::SDK_ThreadTool::DeleteSection(CRITICAL_SECTION& Section) {
	DeleteCriticalSection(&Section);
}

void SDK::SDK_ThreadTool::Lock(CRITICAL_SECTION& Section) {
	EnterCriticalSection(&Section);
}

void SDK::SDK_ThreadTool::TryLock(CRITICAL_SECTION& Section) {
	TryEnterCriticalSection(&Section);
}

void SDK::SDK_ThreadTool::Unlock(CRITICAL_SECTION& Section) {
	LeaveCriticalSection(&Section);
}