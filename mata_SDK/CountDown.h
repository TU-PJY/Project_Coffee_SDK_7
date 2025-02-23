#pragma once
#include <SDK_Scene.h>

class CountDown : public SDK::Object {
private:
	SDK::Timer Timer{};
	SDK::Timer DeleteTimer{};

	SDK::Text Text{};
	SDK::SoundChannel SndChannel{};

	float Size{};

	float Time = 1.0;

	int Count = 4;
	bool Running = true;

public:
	CountDown() {
		Text.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		Text.SetColorRGB(255, 213, 80);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.EnableShadow();
		Text.SetShadow(0.1, -0.1, 0.7);

		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);
	}

	void UpdateFunc(float FrameTime) {
		Time += FrameTime;
		if (Running && Time >= 1.0) {
			Count--;
			Size += 0.2;
			if (Count > 0) {
				SDK::SoundTool.Play(SDK::SOUND.CountDownReady, SndChannel);
			}
			else {
				SDK::SoundTool.Play(SDK::SOUND.CountDownGo, SndChannel);

				// ī��Ʈ�ٿ��� ��� ���� Ÿ�̸� ���� ����
				if (auto TimeWatch = SDK::Scene.Find("time_watch"); TimeWatch)
					TimeWatch->Start();

				// �̵� �Է� Ȱ��ȭ
				if (auto ED = SDK::Scene.Find("ed"); ED)
					ED->EnableInput();

				// bgm ���
				if (auto Manager = SDK::Scene.Find("play_mode_manager"); Manager)
					Manager->PlayBGM();

				Running = false;
			}

			Time = Time - 1.0;
		}

		// ī��Ʈ�� ��� �� �� 1�� �� ���� 
		if (Count == 0) {
			DeleteTimer.Update(FrameTime);
			if (DeleteTimer.Sec() >= 1) 
				SDK::Scene.DeleteObject(this);
		}

		SDK::Math.Lerp(Size, 0.0, 10.0, FrameTime);
	}

	void RenderFunc() {
		if (SDK::Scene.Mode() != SDK::MODE.Play)
			return;

		if (0 < Count && Count < 4)
			Text.Render(0.0, 0.0, 0.4 + Size, L"%d", Count);
		else if (Count == 0) {
			Text.SetColor(0.0, 1.0, 0.0);
			Text.Render(0.0, 0.0, 0.4 + Size, L"GO!");
		}
	}
};