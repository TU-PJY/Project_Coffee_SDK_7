#pragma once
#include <SDK_Scene.h>

#include "Cover.h"

enum PausePageEnum {
	Pause_MainPage,
	RestartPage,
	ExitToTitlePage,
	ExitToDesktopPage
};

class PauseScreen : public SDK::Object {
private:
	float RectOpacity{};
	SDK::RectBrush Rect{};
	SDK::Text Text{};

	// 현재 가리키는 항목 인덱스
	int MenuIndex{};

	// 묻기 메뉴 항목 인덱스
	int QuestionIndex{};

	// 다시 시작하기 묻기

	// 페이지
	int MenuPage{};

	// 메뉴 항목
	std::wstring MenuItems[4] = {
		L"계속하기",
		L"다시 시작하기",
		L"타이틀로 나가기",
		L"바탕화면으로 나가기"
	};

	bool MenuFocused[4]{};

	std::wstring QuestionItems[2] = {
		L"예",
		L"아니오"
	};

	bool QuestionFocused[2]{};

	SDK::SoundChannel SndChannel{};

	// 타이틀로 나가는 상태
	bool ExitState{};
	// 다시 시작하는 상태
	bool RestartState{};

public:
	PauseScreen() {
		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);

		SDK::SoundTool.Play(SDK::SOUND.MenuSelect, SndChannel);
		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);
		SDK::SoundTool.SetFreqCutOff(SDK::CHANNEL.BGM, 300);

		Rect.SetColor(0.0, 0.0, 0.0);
		Rect.SetRenderType(RENDER_TYPE_STATIC);

		MenuFocused[0] = true;
		QuestionFocused[0] = true;
	}

	~PauseScreen() {
		SDK::SoundTool.DisableFreqCutOff(SDK::CHANNEL.BGM);
	}

	void InputKey(SDK::KeyEvent& Event) {
		if (Event.Type == WM_KEYDOWN) {
			auto BackToMainPage = [&]() {
				QuestionIndex = 0;
				QuestionFocused[0] = true;
				QuestionFocused[1] = false;
				MenuPage = Pause_MainPage;
				};

			if (Event.Key == VK_ESCAPE) {
				SDK::SoundTool.Play(SDK::SOUND.MenuSelect, SndChannel);

				switch (MenuPage) {
				case Pause_MainPage:
					SDK::Scene.EndFloatingMode();
					break;
				case ExitToTitlePage: case ExitToDesktopPage: case RestartPage:
					BackToMainPage();
					break;
				}
			}

			else if (Event.Key == VK_RETURN) {
				SDK::SoundTool.Play(SDK::SOUND.MenuSelect, SndChannel);

				if (MenuPage == Pause_MainPage) {
					switch (MenuIndex) {
					case 0:
						SDK::Scene.EndFloatingMode();  break;
					case 1:
						MenuPage = RestartPage;  break;
					case 2:
						MenuPage = ExitToTitlePage;  break;
					case 3:
						MenuPage = ExitToDesktopPage;  break;
					}
				}

				else if (MenuPage == RestartPage) {
					switch (QuestionIndex) {
					case 0:
						SDK::SoundTool.Stop(SDK::CHANNEL.BGM);
						RestartState = true;
						SDK::Scene.DeleteInputObject(this);
						break;
					case 1:
						BackToMainPage();
						break;
					}
				}

				else if (MenuPage == ExitToTitlePage) {
					switch (QuestionIndex) {
					case 0:
						SDK::SoundTool.Stop(SDK::CHANNEL.BGM);
						ExitState = true;
						SDK::Scene.DeleteInputObject(this);
						break;

					case 1:
						BackToMainPage();
						break;
					}
				}

				else if (MenuPage == ExitToDesktopPage) {
					switch (QuestionIndex) {
					case 0:
						SDK::System.Exit();
						break;

					case 1:
						BackToMainPage();
						break;
					}
				}
			}

			else if (Event.Key == VK_UP || Event.Key == VK_DOWN) {
				SDK::SoundTool.Play(SDK::SOUND.MenuSelect, SndChannel);

				if (MenuPage == Pause_MainPage) {
					MenuFocused[MenuIndex] = false;
					MenuIndex += (int)Event.Key - (int)VK_RIGHT;
					SDK::EXTool.ClampValue(MenuIndex, 0, 3, CLAMP_RETURN);
					MenuFocused[MenuIndex] = true;
				}
				else {
					QuestionFocused[QuestionIndex] = false;
					QuestionIndex += (int)Event.Key - (int)VK_RIGHT;
					SDK::EXTool.ClampValue(QuestionIndex, 0, 1, CLAMP_RETURN);
					QuestionFocused[QuestionIndex] = true;
				}
			}
		}
	}

	// 다시 시작이나 타이틀로 나가기 선택 시 화면이 어두워진 후 플레이 모드를 재시작하거나 타이틀 모드로 전환한다
	void UpdateFunc(float FrameTime) {
		if (ExitState || RestartState) {
			RectOpacity += FrameTime * 4.0;
			if (SDK::EXTool.CheckClampValue(RectOpacity, 1.0, CLAMP_GREATER)) {
				if (ExitState) {
					SDK::Scene.SwitchMode(SDK::MODE.Title);
					SDK::Scene.AddObject(new DisappearCover, "dis_cover", LAYER7);
				}
				else if (RestartState) {
					SDK::Scene.RestartMode(SDK::MODE.Play);
					SDK::Scene.AddObject(new DisappearCover, "dis_cover", LAYER7);
				}
			}
		}
	}

	void RenderFunc() {
		// 뒷배경 
		Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, 0.7);

		// 텍스트 출력
		if (MenuPage == Pause_MainPage) {
			Text.SetColor(1.0, 1.0, 1.0);
			Text.Render(0.0, 0.8, 0.15, L"일시정지");

			float RenderHeight = 0.375;
			for (int i = 0; i < 4; i++) {
				// 선택된 항목은 노란색으로 표시된다
				if (MenuFocused[i])
					Text.SetColorRGB(255, 213, 80);
				else
					Text.SetColor(1.0, 1.0, 1.0);

				Text.Render(0.0, RenderHeight, 0.1, MenuItems[i].c_str());
				RenderHeight -= 0.25;
			}
		}

		else {
			Text.SetColor(1.0, 1.0, 1.0);
			if(MenuPage == RestartPage)
				Text.Render(0.0, 0.8, 0.15, L"다시 시작할까요?");
			else if (MenuPage == ExitToTitlePage) 
				Text.Render(0.0, 0.8, 0.15, L"타이틀로 나갈까요?");
			else if(MenuPage == ExitToDesktopPage)
				Text.Render(0.0, 0.8, 0.15, L"바탕화면으로 나갈까요?");

			float RenderHeight = 0.125;
			for (int i = 0; i < 2; i++) {
				if(QuestionFocused[i])
					Text.SetColorRGB(255, 213, 80);
				else
					Text.SetColor(1.0, 1.0, 1.0);

				Text.Render(0.0, RenderHeight, 0.1, QuestionItems[i].c_str());
				RenderHeight -= 0.25;
			}
		}

		if (ExitState || RestartState)
			Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, RectOpacity);
	}
};