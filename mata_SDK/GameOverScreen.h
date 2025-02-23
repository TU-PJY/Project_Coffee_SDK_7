#pragma once
#include <SDK_Scene.h>

#include "Cover.h"
#include "TitleMode.h"

struct Reparations {
	int DestroyedType;
	int NumDestroyed;
};

class GameOverScreen : public SDK::Object {
private:
	// 클로에가 뺨을 때리는 간격 타이머
	SDK::Timer Timer{};

	// 클로에가 뺨을 때리는 간격
	float CheekInterval{};

	// 시온이 떠는 간격 타이머
	SDK::Timer ShiverTimer{};

	// 떠는 애니메이션 수치
	glm::vec2 ShiverValue{};
	glm::vec2 ShiverValue2{};

	// 상하 크기 수치
	float VerticalSize{};

	// 시온 상하 크기 수치
	float XionVerticalSize{};
	SDK::SinLoop XionVerticalSizeLoop{};

	// 시온 대사 루프
	float XionTextSize{};
	SDK::SinLoop XionTextLoop{};

	// 텍스트 크기
	float TextSize{};

	// 텍스트 크기 루프
	SDK::SinLoop TextSizeLoop{};

	// 프레임
	int ChloeFrame{};
	int EDFrame{};

	// 이드 프레임 타이머
	SDK::Timer EDFrameTimer{};

	// 홈모드 전환 상태
	bool ExitState{};

	std::wstring Str{};
	std::wstring Str2{};

	SDK::Text Text{}, ScoreText{};

	// 배상금 
	///////
	// 배상금 표시되는 상태
	bool ShowRep{};

	// 총 배상금 표시되는 상태
	bool ShowTotalRep{};
	 
	// 배상 품목을 저장하는 벡터
	std::vector <Reparations> RepVec{};
	size_t VecSize{};
	int CurrentSize{};

	// 총 배상금
	int TotalRep{};

	// 배상금액을 순차적으로 보여주기 위한 타이머
	SDK::Timer RepTimer{};

	// 배경
	SDK::RectBrush Rect{};

	// 총 배상금액 텍스트 흔들림 값
	glm::vec2 TextShake{};
	float TextShakeValue{0.05};
	SDK::Timer TextShakeTimer{};

	SDK::SoundChannel SndChannel1{};
	SDK::SoundChannel SndChannel2{};
	SDK::SoundChannel SndChannel3{};


	// 타이틀로 나가기 지연 타이머
	SDK::Timer DeleteTimer{};

	bool NewHighScore{};
	bool NewHighRep{};

	// 합계 스킵 여부
	bool RepSkip{};

public:
	GameOverScreen() {
		if(SDK::GLOBAL.Ending == GameOver_TimeOut || SDK::GLOBAL.Ending == GameOver_HitXion)
			SDK::System.SetBackColorRGB(74, 78, 101);
		else
			SDK::System.SetBackColorRGB(255, 178, 80);

		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.EnableShadow();
		Text.SetShadow(0.1, -0.1, 0.7);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.SetLineGap(0.01);
		Text.Rotate(-10.0);

		ScoreText.Init(L"픽셀로보로보체", FW_DONTCARE);
		ScoreText.SetAlign(ALIGN_MIDDLE);
		ScoreText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		ScoreText.EnableShadow();
		ScoreText.SetShadow(0.1, -0.1, 0.7);
		ScoreText.SetColor(1.0, 1.0, 1.0);
		ScoreText.SetLineGap(0.02);

		if (SDK::GLOBAL.Ending == GameOver_TimeOut) {
			CheekInterval = 0.7;
			Str = L"유죄!!!";
			Str2 = L"이드으!!!";
		}

		else if (SDK::GLOBAL.Ending == GameOver_HitXion) {
			CheekInterval = 0.25;
			Str = L"찐짜 유죄!!!";
			Str2 = L"이드으으으으으으으!!!!!";
		}

		else if (SDK::GLOBAL.Ending == GameOver_Suppressed) {
			Str = L"무죄!!!";
			Str2 = L"이드! 참아냈구나!";
		}

		SDK::SoundTool.Play(SDK::SOUND.GameEnd[SDK::GLOBAL.Ending], SDK::CHANNEL.BGM);

		// 파괴한 품목이 존재하면 벡터에 저장한다
		for (int i = 0; i < 6; i++) {
			Reparations Rep{};
			if (SDK::GLOBAL.DestroyedItems[i] > 0) {
				Rep.DestroyedType = i;
				Rep.NumDestroyed = SDK::GLOBAL.DestroyedItems[i];
				RepVec.emplace_back(Rep);
			}
		}

		VecSize = RepVec.size();

		// 총 배상액 계산
		TotalRep += SDK::GLOBAL.DestroyedItems[Item_BoxCoffee] * 25000;
		TotalRep += SDK::GLOBAL.DestroyedItems[Item_BottleCoffee] * 2500;
		TotalRep += SDK::GLOBAL.DestroyedItems[Item_CanCoffee] * 1200;
		TotalRep += SDK::GLOBAL.DestroyedItems[Item_People] * 2000000;
		TotalRep += SDK::GLOBAL.DestroyedItems[Item_Xion] * 1000000;
		TotalRep += SDK::GLOBAL.DestroyedItems[Item_HouseDoc] * 100000;

		if (SDK::GLOBAL.HighScore < SDK::GLOBAL.Score) {
			NewHighScore = true;
			SDK::GLOBAL.IsHighScore = true;
			SDK::FILE.HighscoreData.UpdateDigitData("HighScore", "Score", SDK::GLOBAL.Score);
			SDK::GLOBAL.HighScore = SDK::GLOBAL.Score;
		}

		if (SDK::GLOBAL.MaxRep < TotalRep) {
			NewHighRep = true;
			SDK::GLOBAL.IsHighRep = true;
			SDK::FILE.HighscoreData.UpdateDigitData("HighScore", "Rep", TotalRep);
			SDK::GLOBAL.MaxRep = TotalRep;
		}

		SDK::SoundTool.SetVolume(SndChannel2, SDK::GLOBAL.SFXVolume);
		SDK::SoundTool.SetVolume(SndChannel3, SDK::GLOBAL.SFXVolume);

		Rect.SetColor(0.0, 0.0, 0.0);
		Rect.SetRenderType(RENDER_TYPE_STATIC);
	}

	void InputKey(SDK::KeyEvent& Event) {
		if (Event.Type == WM_KEYDOWN && Event.Key == VK_RETURN) {
			if (SDK::GLOBAL.Ending == GameOver_TimeOut || SDK::GLOBAL .Ending == GameOver_HitXion) {
				// 첫 번쨰 입력은 총 배상금 화면으로 전환하고, 두 번째 입력은 타이틀 모드로 전환한다
				if (!ShowRep) {
					ShowRep = true;
					ScoreText.DisableShadow();
				}
				else if (ShowRep && !ShowTotalRep) {
					ShowTotalRep = true;
					RepSkip = true;
					CurrentSize = VecSize;
					SDK::SoundTool.Play(SDK::SOUND.RepTotal, SndChannel3);
				}
				else if (ShowTotalRep) {
					ExitState = true;
					SDK::Scene.AddObject(new Cover(0.5), "cover", LAYER7);
					// 나가는 상태를 활성화하고 더 이상 입력을 받지 않도록 한다
					SDK::Scene.DeleteInputObject(this);
				}
			}

			// 굿엔딩의 경우 바로 타이틀 모드로 전환한다
			else {
				ExitState = true;
				SDK::Scene.AddObject(new Cover(0.5), "cover", LAYER7);
				// 나가는 상태를 활성화하고 더 이상 입력을 받지 않도록 한다
				SDK::Scene.DeleteInputObject(this);
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		// 클로에 이드 시온 애니메이션 업데이트
		UpdateChloeEDXionAnimation(FrameTime);

		// 합계 업데이트
		UpdateRepResult(FrameTime);

		// 텍스트 애니메이션
		UpdateTextAnimation(FrameTime);

		// 나가기 업데이트
		UpdateExit(FrameTime);
	}

	void RenderFunc() {
		RenderObjects();
		if (!ShowRep)  RenderGameoverText();
		else		   RenderRepResult();
	}

	void UpdateChloeEDXionAnimation(float FrameTime) {
		if (SDK::GLOBAL.Ending == GameOver_TimeOut || SDK::GLOBAL.Ending == GameOver_HitXion) {
			Timer.Update(FrameTime);
			if (Timer.CheckMiliSec(CheekInterval, 2, CHECK_AND_INTERPOLATE)) {
				ChloeFrame++;

				// 일정 간격으로 클로에가 이드의 뺨을 때린다
				if (ChloeFrame == 1) {
					EDFrame = 1;
					EDFrameTimer.Reset();
					VerticalSize = 0.3;
					SDK::SoundTool.Play(SDK::SOUND.HitCheek, SndChannel2);
				}
				SDK::EXTool.ClampValue(ChloeFrame, 0, 1, CLAMP_RETURN);
			}

			// 떨림 애니메이션
			ShiverTimer.Update(FrameTime);
			if (ShiverTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
				ShiverValue.x = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue.y = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue2.x = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue2.y = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			}

			// 수직 크기 애니메이션
			SDK::Math.Lerp(VerticalSize, 0.0, 10.0, FrameTime);

			// 이드는 뺨을 맞은 후 조금 후에 다시 원래 프레임으로 복귀한다
			if (EDFrame == 1) {
				EDFrameTimer.Update(FrameTime);
				if (EDFrameTimer.CheckMiliSec(CheekInterval * 0.5, 2, CHECK_AND_RESET))
					EDFrame = 0;
			}
		}

		// 아무것도 부수지 않았을 시의 애니메이션 
		// 시온, 이드 값 공유
		else
			XionVerticalSizeLoop.Update(XionVerticalSize, 0.05, 20.0, FrameTime);
	}

	void UpdateTextAnimation(float FrameTime) {
		TextSizeLoop.Update(TextSize, 0.01, 10.0, FrameTime);
		XionTextLoop.Update(XionTextSize, 0.01, 20.0, FrameTime);
	}

	void UpdateRepResult(float FrameTime) {
		// 각 배상액을 순차적으로 보여준다
		if (ShowRep) {
			if (!RepSkip)
				RepTimer.Update(FrameTime);

			if (CurrentSize != VecSize && RepTimer.CheckMiliSec(0.5, 1, CHECK_AND_INTERPOLATE)) {
				CurrentSize++;
				SDK::SoundTool.Play(SDK::SOUND.Rep, SndChannel3);
			}

			else if (CurrentSize == VecSize && RepTimer.CheckMiliSec(1, 1, CHECK_AND_INTERPOLATE)) {
				ShowTotalRep = true;
				if (!RepSkip)
					SDK::SoundTool.Play(SDK::SOUND.RepTotal, SndChannel3);
				RepTimer.Stop();
				RepTimer.Reset();
			}

			// 배상액 총액은 흔들리는 애니메이션과 함께 보여준다
			if (ShowTotalRep) {
				TextShakeTimer.Update(FrameTime);
				if (TextShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
					TextShake.x = SDK::Random.Gen(RANDOM_TYPE_REAL, -TextShakeValue, TextShakeValue);
					TextShake.y = SDK::Random.Gen(RANDOM_TYPE_REAL, -TextShakeValue, TextShakeValue);
				}

				SDK::Math.Lerp(TextShakeValue, 0.0, 5.0, FrameTime);
			}
		}
	}

	void UpdateExit(float FrameTime) {
		// 화면이 어두워지면서 홈모드로 전환한다
		// 볼륨을 부드럽게 줄인다
		if (ExitState) {
			SDK::SoundTool.FadeOut(SDK::CHANNEL.BGM, 0.5, FrameTime);
			SDK::SoundTool.FadeOut(SndChannel2, 0.5, FrameTime);
			SDK::SoundTool.FadeOut(SndChannel3, 0.5, FrameTime);

			if (auto Cover = SDK::Scene.Find("cover"); Cover)
				if (Cover->GetState()) {
					SDK::System.SetBackColorRGB(122, 138, 154);
					DeleteTimer.Update(FrameTime);
					if (DeleteTimer.Sec() >= 1) {
						SDK::SoundTool.Stop(SDK::CHANNEL.BGM);
						SDK::SoundTool.SetVolume(SDK::CHANNEL.BGM, SDK::GLOBAL.BGMVolume);
						SDK::Scene.SwitchMode(SDK::MODE.Title);
						SDK::Scene.AddObject(new DisappearCover, "dis_cover", LAYER7);
					}
				}
		}
	}

	void RenderObjects() {
		// 배경 렌더링
		Begin();
		SDK::Transform.Scale(SDK::MoveMatrix, 3.0, 3.0);

		if (SDK::GLOBAL.Ending == GameOver_TimeOut || SDK::GLOBAL.Ending == GameOver_HitXion)
			SDK::ImageTool.RenderImage(SDK::IMAGE.GameOverBackGround, 0.7);
		else
			SDK::ImageTool.RenderImage(SDK::IMAGE.GameOverBackGround2, 0.7);

		// 시온 렌더링
		Begin();
		if (SDK::GLOBAL.Ending == GameOver_Suppressed || SDK::GLOBAL.Ending == GameOver_HitXion)
			SDK::Transform.Move(SDK::MoveMatrix, SDK::ASP(-1.0) + 0.5, -0.9);
		else
			SDK::Transform.Move(SDK::MoveMatrix, SDK::ASP(-1.0) + 0.5 - 0.075, -0.9);
		SDK::Transform.Scale(SDK::MoveMatrix, 0.75, 0.75);
		SDK::ImageTool.RenderImage(SDK::IMAGE.Shadow);

		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, SDK::ASP(-1.0) + 0.5 + ShiverValue.x, -0.3 + ShiverValue.y + XionVerticalSize * 0.5);
		SDK::Transform.Scale(SDK::MoveMatrix, 1.5, 1.5 + XionVerticalSize);

		// 엔딩마다 시온의 프레임이 달라진다
		if (SDK::GLOBAL.Ending == GameOver_HitXion)
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Xion, Xion_Hurt);
		else if (SDK::GLOBAL.Ending == GameOver_TimeOut)
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Xion, Xion_Cry1);
		else if (SDK::GLOBAL.Ending == GameOver_Suppressed) {
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Xion, Xion_Happy);
		}

		// 이드 렌더링
		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, SDK::ASP(1.0) - 1.03 + ShiverValue2.x - VerticalSize * 0.5, -0.7 - VerticalSize * 0.5 + ShiverValue2.y - XionVerticalSize * 0.5);
		SDK::Transform.Scale(SDK::MoveMatrix, 2.0 + VerticalSize, 2.0 - VerticalSize - XionVerticalSize);
		SDK::Transform.Tilt(SDK::MoveMatrix, -VerticalSize * 0.5, 0.0);

		if (SDK::GLOBAL.Ending == GameOver_TimeOut || SDK::GLOBAL.Ending == GameOver_HitXion)
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.ED_GameOver, EDFrame);
		else {
			SDK::Transform.Flip(FLIP_TYPE_H);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.ED, ED_Happy);
		}

		// 클로에 렌더링
		if (SDK::GLOBAL.Ending == GameOver_TimeOut || SDK::GLOBAL.Ending == GameOver_HitXion) {
			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, SDK::ASP(1.0) - 0.5 + ShiverValue2.x - VerticalSize * 0.5, -0.6 - VerticalSize * 0.5 + ShiverValue2.y);
			SDK::Transform.Scale(SDK::MoveMatrix, 2.2 + VerticalSize, 2.2 - VerticalSize);
			SDK::Transform.Tilt(SDK::MoveMatrix, -VerticalSize * 0.5, 0.0);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Chloe, ChloeFrame);
		}
	}

	void RenderGameoverText() {
		// 텍스트 출력
		ScoreText.SetColor(1.0, 1.0, 1.0);
		Text.Render(SDK::ASP(-1.0) + 0.8, 0.85, 0.2 + TextSize, Str.c_str());
		Text.Render(SDK::ASP(-1.0) + 0.6, 0.4, 0.07 + XionTextSize, Str2.c_str());

		// 점수 출력
		ScoreText.Render(0.0, 0.4, 0.2, L"SCORE\n%d", SDK::GLOBAL.Score);

		if (NewHighScore) {
			ScoreText.SetColorRGB(255, 213, 80);
			ScoreText.Render(0.0, 0.55, 0.1, L"HighScore!");
		}

		ScoreText.SetColor(1.0, 1.0, 1.0);
		ScoreText.SetAlign(ALIGN_LEFT);
		ScoreText.SetHeightAlign(HEIGHT_ALIGN_UNDER);
		ScoreText.Render(SDK::ASP(1.0) - 0.05, 0.95, 0.1, L"Enter를 눌러 계속");
		ScoreText.SetAlign(ALIGN_MIDDLE);
		ScoreText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
	}

	void RenderRepResult() {
		// 뒷 배경 출력
		Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, 0.6);

		// 배상금 계산
		float RenderHeight{ 0.8 };

		for (int i = 0; i < CurrentSize; i++) {
			ScoreText.SetColor(1.0, 1.0, 1.0);

			switch (RepVec[i].DestroyedType) {
			case Item_BoxCoffee:
				ScoreText.Render(0.0, RenderHeight, 0.1, L"커피 상자 배상: %d개 x 25000골드", SDK::GLOBAL.DestroyedItems[RepVec[i].DestroyedType]);
				break;

			case Item_BottleCoffee:
				ScoreText.Render(0.0, RenderHeight, 0.1, L"병 커피 배상: %d개 x 2500골드", SDK::GLOBAL.DestroyedItems[RepVec[i].DestroyedType]);
				break;

			case Item_CanCoffee:
				ScoreText.Render(0.0, RenderHeight, 0.1, L"캔 커피 배상: %d개 x 1200골드", SDK::GLOBAL.DestroyedItems[RepVec[i].DestroyedType]);
				break;

			case Item_People:
				ScoreText.Render(0.0, RenderHeight, 0.1, L"폭행 피해 합의금: %d명 x 2000000골드", SDK::GLOBAL.DestroyedItems[RepVec[i].DestroyedType]);
				break;

			case Item_Xion:
				ScoreText.Render(0.0, RenderHeight, 0.1, L"시온 치료비: 1000000골드");
				break;

			case Item_HouseDoc:
				ScoreText.Render(0.0, RenderHeight, 0.1, L"불 탄 집문서 배상: %d장 x 100000골드", SDK::GLOBAL.DestroyedItems[RepVec[i].DestroyedType]);
				break;
			}

			RenderHeight -= 0.2;
		}

		if (ShowTotalRep) {
			ScoreText.SetColorRGB(255, 213, 80);
			ScoreText.Render(TextShake.x, -0.7 + TextShake.y, 0.15, L"합계: %d 골드", TotalRep);

			if (NewHighRep)
				ScoreText.Render(TextShake.x, -0.5 + TextShake.y, 0.1, L"역대급!!");
		}

		ScoreText.SetColor(1.0, 1.0, 1.0);
		ScoreText.SetAlign(ALIGN_LEFT);
		ScoreText.SetHeightAlign(HEIGHT_ALIGN_DEFAULT);
		ScoreText.Render(SDK::ASP(1.0) - 0.05, -0.95, 0.1, L"Enter를 눌러 계속");
		ScoreText.SetAlign(ALIGN_MIDDLE);
		ScoreText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
	}
};