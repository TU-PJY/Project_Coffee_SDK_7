#pragma once
#include "SDK_Object.h"

class SDK_IntroScreen : public SDK::Object {
private:
	SDK::Timer        IntroTimer;
	SDK::SoundChannel IntroChannel{};

	SDK::SinMove      IntroMove{}, OpacityMove{};

	SDK::RectBrush    BackGround{};
	float             BackGroundOpacity{ 1.0 };

	float      LogoSize        = 1.0;
	float      LogoOpacity     = 0.0;
	float      LogoPosition    = -0.4;
	float      AnimationSpeed  = 6.0;

	int        SceneNumber{}; 

	bool       ExitState{};

	SDK::Timer TextTimer{};
	SDK::Text Text{};
	bool RenderText{true};

public:
	SDK_IntroScreen() {
		IntroMove.SetMovePoint(LogoPosition, 0.0);
		OpacityMove.SetMovePoint(0.0, 1.0);

		BackGround.SetColorRGB(31, 31, 31);
		BackGround.SetRenderType(RENDER_TYPE_STATIC);

		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.EnableFixMiddle();
		Text.SetColor(1.0, 1.0, 1.0);
		Text.SetLineGap(0.02);
	}

	void InputKey(SDK::KeyEvent& Event) {
		if (Event.Type == WM_KEYDOWN) {
			switch (Event.Key) {
			case VK_RETURN:
				SDK::SoundTool.Stop(IntroChannel);
				SDK::Scene.DeleteInputObject(this);
				SDK::Scene.SwitchMode(SDK::START_MODE);
				ExitState = true;
				break;
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		if (!ExitState) {

			if (TextTimer.UpdateAndCheckSec(7, CHECK_AND_STOP, FrameTime)) {
				RenderText = false;

				IntroTimer.Update(FrameTime);
				switch (SceneNumber) {
				case 0:
					if (IntroTimer.CheckMiliSec(1.0, 1, CHECK_AND_INTERPOLATE)) {
						SDK::SoundTool.Play(SDK::SYSRES.INTRO_SOUND, IntroChannel);
						++SceneNumber;
					}
					break;


				case 1:
					if (IntroTimer.MiliSec() < 2.5) {
						IntroMove.Update(LogoPosition, AnimationSpeed, FrameTime);
						OpacityMove.Update(LogoOpacity, AnimationSpeed, FrameTime);
					}

					if (IntroTimer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
						LogoOpacity -= FrameTime * 2;
						SDK::EXTool.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
					}

					if (IntroTimer.CheckMiliSec(4.0, 1, CHECK_AND_INTERPOLATE)) {
						LogoPosition = -0.4;

						IntroMove.SetMovePoint(LogoPosition, 0.0);
						OpacityMove.SetMovePoint(0.0, 1.0);
						IntroMove.Reset();
						OpacityMove.Reset();

						++SceneNumber;
					}
					break;


				case 2:
					if (IntroTimer.MiliSec() < 2.5) {
						IntroMove.Update(LogoPosition, AnimationSpeed, FrameTime);
						OpacityMove.Update(LogoOpacity, AnimationSpeed, FrameTime);
					}

					if (IntroTimer.CheckMiliSec(2.5, 1, CHECK_AND_RESUME)) {
						LogoOpacity -= FrameTime * 2;
						SDK::EXTool.ClampValue(LogoOpacity, 0.0, CLAMP_LESS);
					}

					if (IntroTimer.CheckMiliSec(4.0, 1, CHECK_AND_RESUME)) {
						SDK::Scene.DeleteInputObject(this);
						SDK::Scene.SwitchMode(SDK::START_MODE);
						ExitState = true;
					}

					break;
				}
			}
		}

		else {
			BackGroundOpacity -= FrameTime;
			if (SDK::EXTool.CheckClampValue(BackGroundOpacity, 0.0, CLAMP_LESS))
				SDK::Scene.DeleteObject(this);
		}
	}

	void RenderFunc() {
		BackGround.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, BackGroundOpacity);

		if (!RenderText) {
			Begin(RENDER_TYPE_STATIC);
			SDK::Transform.Move(SDK::MoveMatrix, 0.0, LogoPosition);
			SDK::ImageTool.SetColor(1.0, 1.0, 1.0);

			switch (SceneNumber) {
			case 1:
				SDK::ImageTool.RenderImage(SDK::SYSRES.SDK_LOGO, LogoOpacity * BackGroundOpacity);
				break;

			case 2: case 3:
				SDK::ImageTool.RenderImage(SDK::SYSRES.FMOD_LOGO, LogoOpacity * BackGroundOpacity);
				break;
			}
		}

		else {
			Text.SetOpacity(BackGroundOpacity);
			Text.Render(0.0, 0.0, 0.1, L"본 게임은 '트릭컬:리바이브' 팬게임으로, \n해당 게임의 캐릭터에 대한 모든 권한은\nEPIDGames가 소유함을 알립니다.\n\n(C)2025 EPIDGames. All Rights Reserved.");
		}

	}
};