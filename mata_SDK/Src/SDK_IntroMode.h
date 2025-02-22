#pragma once
#include "SDK_Scene.h"
#include "SDK_Mouse.h"

#include "SDK_IntroScreen.h"

class SDK_IntroMode {
public:
	std::string ModeName{ "IntroMode" };
	int         ModeType{ MODE_TYPE_DEFAULT };

	static void Start() {
		SetUp();
		SDK::Scene.AddObject(new SDK_IntroScreen, "intro_screen", LAYER1, true, OBJECT_TYPE_STATIC);
	}

	static void Destructor() {

	}

	/////////////////////////////////////////////////////////////
#pragma region FoldRegion 
	std::vector<SDK::Object*> InputObject{};
	inline static SDK_IntroMode* M_Inst;

	SDK_IntroMode() { M_Inst = this; }

	static void SetUp() {
		M_Inst->InputObject.clear();
		SDK::Scene.RegisterInputObjectList(M_Inst->InputObject);
		SDK::Scene.RegisterDestructor(Destructor);
		SDK::Scene.RegisterController(ModeController, M_Inst->ModeType);
		SDK::Scene.RegisterModeName(M_Inst->ModeName);
		SDK::Scene.RegisterModePtr(M_Inst->Start);
	}

	static LRESULT CALLBACK ModeController(HWND Hwnd, UINT Message, WPARAM wParam, LPARAM lParam, UINT_PTR SubClassID, DWORD_PTR RefData) {
		int ProcEvent{};
		int WheelDelta{};
		int MouseEvent{ EVENT_NONE };

		POINT CursorPosition;
		if (GetCursorPos(&CursorPosition)) {
			ScreenToClient(Hwnd, &CursorPosition);
			SDK::Mouse.ConvertPosition(CursorPosition.x, CursorPosition.y);
		}

		switch (Message) {
		case WM_LBUTTONDOWN:
			MouseEvent = LEFT_BUTTON_DOWN; break;
		case WM_RBUTTONDOWN:
			MouseEvent = RIGHT_BUTTON_DOWN; break;
		case WM_MBUTTONDOWN:
			MouseEvent = MIDDLE_BUTTON_DOWN; break;
		case WM_LBUTTONUP:
			MouseEvent = LEFT_BUTTON_UP; break;
		case WM_RBUTTONUP:
			MouseEvent = RIGHT_BUTTON_UP; break;
		case WM_MBUTTONUP:
			MouseEvent = MIDDLE_BUTTON_UP; break;

		case WM_XBUTTONDOWN:
			ProcEvent = GET_XBUTTON_WPARAM(wParam);
			if (ProcEvent == XBUTTON1)
				MouseEvent = BACKWARD_BUTTON_DOWN;
			else if (ProcEvent == XBUTTON2)
				MouseEvent = FORWARD_BUTTON_DOWN;
			break;

		case WM_XBUTTONUP:
			ProcEvent = GET_XBUTTON_WPARAM(wParam);
			if (ProcEvent == XBUTTON1)
				MouseEvent = BACKWARD_BUTTON_UP;
			else if (ProcEvent == XBUTTON2)
				MouseEvent = FORWARD_BUTTON_UP;
			break;

		case WM_MOUSEWHEEL:
			WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (WheelDelta > 0)
				MouseEvent = WHEEL_UP;
			else if (WheelDelta < 0)
				MouseEvent = WHEEL_DOWN;
			break;

		case WM_KEYDOWN: case WM_KEYUP:
			if (ENABLE_DEV_EXIT && wParam == VK_ESCAPE)
				SDK::System.Exit();

			SDK::KeyEvent Event{};
			Event.Type = Message;
			Event.Key = wParam;

			for (auto const& Object : M_Inst->InputObject)
				if (Object)  Object->InputKey(Event);

			return DefSubclassProc(Hwnd, Message, wParam, lParam);
			break;
		}

		for (auto const& Object : M_Inst->InputObject)
			if (Object)  Object->InputMouse(MouseEvent);

		return DefSubclassProc(Hwnd, Message, wParam, lParam);
	}
#pragma endregion
};
extern SDK_IntroMode IntroMode;