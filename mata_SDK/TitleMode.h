#pragma once
#include <SDK_ModeHeader.h>

#include "TitleScreen.h"

class TitleMode {
public:
	// define mode name and mode type here
	std::string ModeName { "TitleMode" };
	int         ModeType { MODE_TYPE_DEFAULT };

	static void Start() {
		SetUp();
		SDK::System.SetBackColorRGB(122, 138, 154);
		SDK::Scene.AddObject(new TitleScreen(false), "title_screen", LAYER1, true);
	}

	static void Destructor() {
	}

	/////////////////////////////////////////////////////////////
#pragma region FoldRegion 
	std::vector<SDK::Object*> InputObject{};
	inline static TitleMode* M_Inst;

	TitleMode() { M_Inst = this; }

	static void Map(SDK::MODE_PTR& Mode) {
		Mode = Start;
	}

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

		case WM_KEYDOWN: case WM_KEYUP: case WM_CHAR:
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