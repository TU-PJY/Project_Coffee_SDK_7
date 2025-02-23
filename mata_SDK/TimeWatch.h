#pragma once
#include <SDK_Scene.h>

class TimeAdded : public SDK::Object {
private:
	SDK::Timer Timer{};
	float Rotation{-25.0};
	float Opacity{0.0};
	float Size = 0.05;
	
	// 애니메이션 시퀀스
	int Seq = 1;

	SDK::SoundChannel SndChannel{};

	SDK::Text Text{};

public:
	TimeAdded() {
		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);
		SDK::SoundTool.Play(SDK::SOUND.AddTime, SndChannel);

		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.EnableShadow();
		Text.SetShadow(0.1, -0.1, 0.7);
		Text.SetColorRGB(255, 213, 80);
	}

	void UpdateFunc(float FrameTime) {
		if (Seq == 1) {
			SDK::Math.Lerp(Rotation, 25.0, 10.0, FrameTime);
			SDK::Math.Lerp(Opacity, 1.0, 10.0, FrameTime);
			SDK::Math.Lerp(Size, 0.25, 10.0, FrameTime);

			if (Rotation >= 24.999)
				Seq++;
		}

		else if (Seq == 2) {
			SDK::Math.Lerp(Opacity, 0.0, 10.0, FrameTime);
			SDK::Math.Lerp(Size, 0.0, 10.0, FrameTime);

			if (Opacity <= 0.001)
				SDK::Scene.DeleteObject(this);
		}
	}

	void RenderFunc() {
		if (SDK::Scene.Mode() != SDK::MODE.Play)
			return;

		Text.Rotate(Rotation);
		Text.Render(SDK::ASP(1.0) - 0.6, 0.3, Size, L"+10");
	}
};

class TimeWatch : public SDK::Object {
private:
	// 초기 시간 10초
	float Time = 10;
	float AddValue{};

	bool Running{};

	SDK::SinLoop Loop{};
	float Size{};

	SDK::Text Text{};

	SDK::Color3 Color = { 1, 0.84, 0.31 };

public:
	TimeWatch() {
		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_UNDER);
		Text.EnableShadow();
		Text.SetShadow(0.1, -0.1, 0.7);
	}

	void UpdateFunc(float FrameTime) {
		// 시간 감소
		if(Running)
			Time -= FrameTime;
		SDK::EXTool.ClampValue(Time, 0.0, CLAMP_LESS);

		if (AddValue > 0.0) {
			Time += FrameTime * 20.0;
			AddValue -= FrameTime * 20.0;
			SDK::EXTool.ClampValue(AddValue, 0.0, CLAMP_LESS);
		}

		// 시간이 끝나면 게임오버 된다
		if (Time <= 0.0) {
			SDK::GLOBAL.GameOver = true;

			if (auto Manager = SDK::Scene.Find("play_mode_manager"); Manager)
				Manager->StopBGM();

			// 게임오버 엔딩 지정
			if(SDK::GLOBAL.Score > 0)
				SDK::GLOBAL.Ending = GameOver_TimeOut;
			else
				SDK::GLOBAL.Ending = GameOver_Suppressed;
		}
		
		if (SDK::GLOBAL.GameOver)
			Loop.Update(Size, 0.025, 5.0, FrameTime);

		SDK::Math.Lerp(Color, glm::vec3(1, 0.84, 0.31), 5.0, FrameTime);
	}

	void RenderFunc() {
		if (SDK::Scene.Mode() != SDK::MODE.Play)
			return;

		// 시간 출력
		if (!SDK::GLOBAL.GameOver) {
			if(Time < 6.0)
				Text.SetColor(1.0, 0.0, 0.0);

			else {
				if(AddValue > 0.0)
					Text.SetColor(0.0, 1.0, 0.0);
				else
					Text.SetColor(Color);
			}

			Text.Render(0.0, 1.0, 0.25, L"%d", (int)Time);
		}

		else {
			Text.SetColorRGB(255, 213, 80);

			if (SDK::GLOBAL.GameOver && Time <= 0.0)
				Text.Render(0.0, 1.0, 0.3 + Size, L"TIME OUT!");

			else if (SDK::GLOBAL.GameOver && Time > 0.0)
				Text.Render(0.0, 1.0, 0.3 + Size, L"GAME OVER");
		}
	}

	void Stop() {
		Running = false;
	}

	void Start() {
		Running = true;
	}

	void AddTime() {
		AddValue += 10;
		SDK::Scene.AddObject(new TimeAdded, "time_added", LAYER7);
	}

	void DeleteTime() {
		Color = glm::vec3(1.0, 0.0, 0.0);
		Time -= 2;
		SDK::EXTool.ClampValue(Time, 0.0, CLAMP_LESS);
	}
};