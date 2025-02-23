#pragma once
#include <SDK_ModeHeader.h>

#include "ED.h"
#include "Shelf.h"
#include "Cart.h"
#include "Xion.h"
#include "Floor.h"
#include "TimeWatch.h"
#include "Score.h"
#include "CountDown.h"
#include "PlayModeManager.h"
#include "BackgroundShelf.h"
#include "Pillar.h"


class PlayMode {
public:
	std::string ModeName{ "PlayMode" };
	int         ModeType{ MODE_TYPE_DEFAULT };

	/////////////////////////////////////////////////////////////

	static void Start() {
		SetUp();

		SDK::System.SetBackColorRGB(122, 138, 154);

		// 이전에 파괴했던 개수 초기화
		for (int i = 0; i < 6; i++)
			SDK::GLOBAL.DestroyedItems[i] = 0;

		// 점수 초기화
		SDK::GLOBAL.Score = 0;

		// 최고 기록 달성 여부 초기화
		SDK::GLOBAL.IsHighScore = false;
		SDK::GLOBAL.IsHighRep = false;

		// 사람 생성 가능 여부 초기화
		for (int i = 0; i < 10; i++) {
			SDK::GLOBAL.PrevChFrame[i] = 0;
			SDK::GLOBAL.CreateAvailable[i] = true;
		}

		// 게임 오버 상태 초기화
		SDK::GLOBAL.GameOver = false;

		SDK::Scene.AddObject(new PlayModeManager, "play_mode_manager", LAYER1, true);

		SDK::Scene.AddObject(new Pillar(1.0), "pillar", LAYER_BG);

		// 타일 추가
		GLfloat Position = SDK::ASP(-1.0) - 1.6;
		while (true) {
			SDK::Scene.AddObject(new Floor(Position, true), "floor", LAYER_BG);
			Position += 0.8;

			if (Position > SDK::ASP(1.0) + 0.8) {
				SDK::Scene.AddObject(new Floor(Position, false), "floor", LAYER_BG);
				break;
			}
		}

		SDK::Scene.AddObject(new BackgroundShelf(0.0), "background_shelf", LAYER1);

		SDK::Scene.AddObject(new Shelf(2, 1.75), "shelf", LAYER2);
		SDK::Scene.AddObject(new Xion(-1.0, 0.0, false, Xion_Cry1), "xion", LAYER3);
		SDK::Scene.AddObject(new ED, "ed", LAYER3, true);
		SDK::Scene.AddObject(new Cart(true, glm::vec2(0.8, -0.35)), "cart", LAYER4);

		SDK::Scene.AddObject(new TimeWatch, "time_watch", LAYER7);
		SDK::Scene.AddObject(new ScoreIndicator, "score_indicator", LAYER7);
		SDK::Scene.AddObject(new CountDown, "countdown", LAYER7);
	}

	static void Destructor() {
	}

	/////////////////////////////////////////////////////////////
#pragma region FoldRegion 
	std::vector<SDK::Object*> InputObject{};
	inline static PlayMode* M_Inst;

	PlayMode() { M_Inst = this; }

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