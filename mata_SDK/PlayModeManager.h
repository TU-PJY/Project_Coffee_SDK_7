#pragma once
#include <SDK_Scene.h>

#include "Cover.h"

class PlayModeManager : public SDK::Object {
private:
	SDK::Timer Timer1{}, Timer2{};
	bool GameStart{};

	// 타이틀 화면으로 돌아가기
	bool GoToTitle{};
	bool CoverAdded{};

public:
	// 일시정지 모드로 전환
	void InputKey(SDK::KeyEvent& Event) {
		if (Event.Type == WM_KEYDOWN && Event.Key == VK_ESCAPE)
			SDK::Scene.StartFloatingMode(SDK::MODE.Pause, true);
	}

	void UpdateFunc(float FrameTime) {
		// 3초 후 화면이 어두워진다
		if (SDK::GLOBAL.GameOver) {
			Timer1.Update(FrameTime);
			if (Timer1.Sec() >= 3) {
				SDK::Scene.AddObject(new Cover(0.5), "cover", LAYER7);
				Timer1.Reset();
				Timer1.Stop();
			}

			//  화면이 완전히 어두워진 후 1초 뒤 게임오버 스크린으로 전환한다
			if (auto Cover = SDK::Scene.Find("cover"); Cover) {
				if (Cover->GetState()) {
					Timer2.Update(FrameTime);

					if (Timer2.Sec() >= 1) {
						// 게임오버 모드로 전환
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

			//  화면이 완전히 어두워진 후 0.5초 뒤 타이틀 화면으로 전환한다
			if (auto Cover = SDK::Scene.Find("cover"); Cover) {
				if (Cover->GetState()) {
					Timer2.Update(FrameTime);

					if (Timer2.MiliSec() >= 0.5) {
						// 타이틀 모드로 전환
						SDK::Scene.SwitchMode(SDK::MODE.Title);
					}
				}
			}
		}
	}

	// Bgm 재생
	void PlayBGM() {
		int RandomIndex = SDK::Random.Gen(RANDOM_TYPE_INT, 0, 2);
		SDK::SoundTool.Play(SDK::SOUND.PlayBgm[RandomIndex], SDK::CHANNEL.BGM);
		GameStart = true;
	}

	// Bgm 정지
	void StopBGM() {
		SDK::SoundTool.Stop(SDK::CHANNEL.BGM);
		GameStart = false;
	}

	void SetGoToTitle() {
		GoToTitle = true;
	}
};