#pragma once
#include <SDK_Scene.h>

class PauseScreen : public SDK::Object {
private:
	SDK::RectBrush Rect{};

	SDK::Text Text{};

	// ���� ����Ű�� �׸� �ε���
	int MenuIndex{};

	// ���� �޴� �׸� �ε���
	int QuestionIndex{};

	// ���� ����
	bool QuestionState{};

	// ����ȭ������ ������ ����
	bool QuestionToDesktop{};

	// ����ȭ������ ������ ����
	bool QuestionToTitle{};

	// �޴� �׸�
	std::wstring MenuItems[3] = {
		L"����ϱ�",
		L"Ÿ��Ʋ�� ������",
		L"����ȭ������ ������"
	};

	bool MenuFocused[3]{};

	std::wstring QuestionItems[2] = {
		L"��",
		L"�ƴϿ�"
	};

	bool QuestionFocused[2]{};

	SDK::SoundChannel SndChannel{};

public:
	PauseScreen() {
		Text.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);

		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);
		SDK::SoundTool.SetFreqCutOff(SDK::CHANNEL.BGM, 200);

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
			if (Event.Key == VK_ESCAPE) {
				SDK::SoundTool.Play(SDK::SOUND.MenuSelect, SndChannel);

				if (!QuestionState)
					SDK::Scene.EndFloatingMode();
				else {
					QuestionState = false;
					QuestionToDesktop = false;
					QuestionToTitle = false;
					QuestionIndex = 0;
					QuestionFocused[0] = true;
					QuestionFocused[1] = false;
				}
			}

			else if (Event.Key == VK_RETURN) {
				SDK::SoundTool.Play(SDK::SOUND.MenuSelect, SndChannel);

				if (!QuestionState) {
					switch (MenuIndex) {
					case 0:
						SDK::Scene.EndFloatingMode();
						break;
					case 1:
						QuestionState = true;
						QuestionToTitle = true;
						break;
					case 2:
						QuestionState = true;
						QuestionToDesktop = true;
						break;
					}
				}

				else {
					switch (QuestionIndex) {
					case 0:
						if (QuestionToTitle) {
							if (auto ED = SDK::Scene.Find("ed"); ED)
								ED->DisableInput();
							if (auto TimeWatch = SDK::Scene.Find("time_watch"); TimeWatch)
								TimeWatch->Stop();
							if (auto Manager = SDK::Scene.Find("play_mode_manager"); Manager) {
								Manager->StopBGM();
								Manager->SetGoToTitle();
							}
							if (auto CountDown = SDK::Scene.Find("countdown"); CountDown)
								SDK::Scene.DeleteObject(CountDown);
							SDK::Scene.EndFloatingMode();
						}
						
						else if (QuestionToDesktop)
							SDK::System.Exit();
						break;

					case 1:
						QuestionState = false;
						QuestionIndex = 0;
						QuestionFocused[0] = true;
						QuestionFocused[1] = false;

						if (QuestionToTitle) 
							QuestionToTitle = false;

						else if (QuestionToDesktop) 
							QuestionToDesktop = false;
						break;
					}
				}
			}

			else if (Event.Key == VK_UP || Event.Key == VK_DOWN) {
				SDK::SoundTool.Play(SDK::SOUND.MenuSelect, SndChannel);

				if (!QuestionState) {
					MenuFocused[MenuIndex] = false;
					MenuIndex += (int)Event.Key - (int)VK_RIGHT;
					SDK::EXTool.ClampValue(MenuIndex, 0, 2, CLAMP_RETURN);
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

	void RenderFunc() {
		// �޹�� 
		Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, 0.7);

		// �ؽ�Ʈ ���
		if (!QuestionState) {
			Text.SetColor(1.0, 1.0, 1.0);
			Text.Render(0.0, 0.8, 0.15, L"�Ͻ�����");

			GLfloat RenderHeight = 0.25;
			for (int i = 0; i < 3; i++) {
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
			if (QuestionToTitle) 
				Text.Render(0.0, 0.8, 0.15, L"Ÿ��Ʋ�� �������?");
			else if(QuestionToDesktop)
				Text.Render(0.0, 0.8, 0.15, L" ����ȭ������ �������?");

			GLfloat RenderHeight = 0.125;
			for (int i = 0; i < 2; i++) {
				if(QuestionFocused[i])
					Text.SetColorRGB(255, 213, 80);
				else
					Text.SetColor(1.0, 1.0, 1.0);

				Text.Render(0.0, RenderHeight, 0.1, QuestionItems[i].c_str());
				RenderHeight -= 0.25;
			}
		}
	}
};