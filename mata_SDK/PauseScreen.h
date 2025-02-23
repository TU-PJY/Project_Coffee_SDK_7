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

	// ���� ����Ű�� �׸� �ε���
	int MenuIndex{};

	// ���� �޴� �׸� �ε���
	int QuestionIndex{};

	// �ٽ� �����ϱ� ����

	// ������
	int MenuPage{};

	// �޴� �׸�
	std::wstring MenuItems[4] = {
		L"����ϱ�",
		L"�ٽ� �����ϱ�",
		L"Ÿ��Ʋ�� ������",
		L"����ȭ������ ������"
	};

	bool MenuFocused[4]{};

	std::wstring QuestionItems[2] = {
		L"��",
		L"�ƴϿ�"
	};

	bool QuestionFocused[2]{};

	SDK::SoundChannel SndChannel{};

	// Ÿ��Ʋ�� ������ ����
	bool ExitState{};
	// �ٽ� �����ϴ� ����
	bool RestartState{};

public:
	PauseScreen() {
		Text.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
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

	// �ٽ� �����̳� Ÿ��Ʋ�� ������ ���� �� ȭ���� ��ο��� �� �÷��� ��带 ������ϰų� Ÿ��Ʋ ���� ��ȯ�Ѵ�
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
		// �޹�� 
		Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, 0.7);

		// �ؽ�Ʈ ���
		if (MenuPage == Pause_MainPage) {
			Text.SetColor(1.0, 1.0, 1.0);
			Text.Render(0.0, 0.8, 0.15, L"�Ͻ�����");

			float RenderHeight = 0.375;
			for (int i = 0; i < 4; i++) {
				// ���õ� �׸��� ��������� ǥ�õȴ�
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
				Text.Render(0.0, 0.8, 0.15, L"�ٽ� �����ұ��?");
			else if (MenuPage == ExitToTitlePage) 
				Text.Render(0.0, 0.8, 0.15, L"Ÿ��Ʋ�� �������?");
			else if(MenuPage == ExitToDesktopPage)
				Text.Render(0.0, 0.8, 0.15, L"����ȭ������ �������?");

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