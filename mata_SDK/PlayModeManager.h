#pragma once
#include <SDK_Scene.h>

#include "Cover.h"

class PlayModeManager : public SDK::Object {
private:
	SDK::Timer Timer1{}, Timer2{};
	bool GameStart{};

	// Ÿ��Ʋ ȭ������ ���ư���
	bool GoToTitle{};
	bool CoverAdded{};

public:
	// �Ͻ����� ���� ��ȯ
	void InputKey(SDK::KeyEvent& Event) {
		if (Event.Type == WM_KEYDOWN && Event.Key == VK_ESCAPE)
			SDK::Scene.StartFloatingMode(SDK::MODE.Pause, true);
	}

	void UpdateFunc(float FrameTime) {
		// 3�� �� ȭ���� ��ο�����
		if (SDK::GLOBAL.GameOver) {
			Timer1.Update(FrameTime);
			if (Timer1.Sec() >= 3) {
				SDK::Scene.AddObject(new Cover(0.5), "cover", LAYER7);
				Timer1.Reset();
				Timer1.Stop();
			}

			//  ȭ���� ������ ��ο��� �� 1�� �� ���ӿ��� ��ũ������ ��ȯ�Ѵ�
			if (auto Cover = SDK::Scene.Find("cover"); Cover) {
				if (Cover->GetState()) {
					Timer2.Update(FrameTime);

					if (Timer2.Sec() >= 1) {
						// ���ӿ��� ���� ��ȯ
						SDK::Scene.SwitchMode(SDK::MODE.GameOver);
						SDK::Scene.AddObject(new DisappearCover, "dis_cover", LAYER7);
					}
				}
			}
		}

		else if (GoToTitle) {
			if (!CoverAdded) {
				SDK::Scene.AddObject(new Cover(1.5), "cover", LAYER7);
				CoverAdded = true;
			}

			//  ȭ���� ������ ��ο��� �� 0.5�� �� Ÿ��Ʋ ȭ������ ��ȯ�Ѵ�
			if (auto Cover = SDK::Scene.Find("cover"); Cover) {
				if (Cover->GetState()) {
					Timer2.Update(FrameTime);

					if (Timer2.MiliSec() >= 0.5) {
						// Ÿ��Ʋ ���� ��ȯ
						SDK::Scene.SwitchMode(SDK::MODE.Title);
					}
				}
			}
		}
	}

	// Bgm ���
	void PlayBGM() {
		int RandomIndex = SDK::Random.Gen(RANDOM_TYPE_INT, 0, 2);
		SDK::SoundTool.Play(SDK::SOUND.PlayBgm[RandomIndex], SDK::CHANNEL.BGM);
		GameStart = true;
	}

	// Bgm ����
	void StopBGM() {
		SDK::SoundTool.Stop(SDK::CHANNEL.BGM);
		GameStart = false;
	}

	void SetGoToTitle() {
		GoToTitle = true;
	}
};