#pragma once
#include <SDK_Scene.h>

enum MenuPageEnum {
	ExitPage,
	MainPage,
	OptionPage,
	ResetPage
};

class TitleScreen : public SDK::Object {
private:
	// 타이틀 동작 여부
	bool TitleActivateState{};
	// 인트로 플레이 여부
	bool IntroPlayState{};
	// 게임 시작 여부
	bool GameStartState{};
	// 인트로 시작 여부
	bool IntroStartState{};

	// 타이틀 이미지 위치
	SDK::Vector2 TitleImagePosition{};
	// 타이틀 이미지 크기
	float TitleImageSize{};

	// 시온 상하 크기
	float XionVerticalSize{};
	// 시온 상하 크기 루프
	SDK::SinLoop XionVerticalSizeLoop{};

	// 이드 떨림 위치
	SDK::Vector2 EDShakeOffset{};
	// 이드 떨림 간격 타이머
	SDK::Timer EDShakeTimer{};

	// 카메라 위치 및 줌
	SDK::Vector2 TitleCameraPosition{};
	float TitleCameraZoom{};
	// 카메라 이동 애니메이션
	SDK::SinMove TitleCameraPositionMove{};
	// 카메라 줌 애니메이션
	SDK::SinMove TitleCameraZoomMove{};

	// 인트로를 제외한 나머지 카메라 위치 및 줌
	SDK::Vector2 EffectCameraPosition{};
	float EffectCameraZoom{};

	// 타이틀 화면 텍스트
	SDK::Text TitleText{};
	// 텍스트 가로 위치 오프셋
	float TitleTextHorizontalOffset{};
	// 텍스트 렌더링 여부
	bool TitleTextRenderState{};

	// 텍스트 목록
	std::wstring MenuItem[3] = {
		L"시작!",
		L"환경설정",
		L"바탕화면으로\n나가기"
	};

	std::wstring QuestionItem[2] = {
		L"예",
		L"아니오"
	};

	std::wstring QuestionItem2[2] = {
		L"아니오",
		L"예"
	};

	// 메뉴 텍스트 포커싱 
	bool ExitPageFocused[2]{ true, false };
	bool MainPageFocused[3]{ true, false, false };
	bool OptionPageFocused[6]{ true, false, false, false, false, false };
	bool ResetPageFocused[2]{ true, false };

	// 현재 가리키는 메뉴 항목
	int ExitPageIndex{};
	int MainPageIndex{};
	int OptionPageIndex{};
	int ResetPageIndex{};

	// 메뉴 페이지, 페이지 번호에 따라 표시되는 항목이 달라진다
	int MenuPage{ 1 };

	// 텍스트 배경
	SDK::RectBrush TitleRect{};

public:
	TitleScreen(bool IsIntroPlayed) {
		// 텍스트 초기화
		TitleText.Init(SDK::FONTNAME.Main);
		TitleText.SetAlign(ALIGN_LEFT);
		TitleText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		TitleText.SetShadow(0.1, -0.1, 0.8);
		TitleText.SetLineGap(0.02);

		// 타이틀 화면 세팅
		SetTitleScreen(IsIntroPlayed);

		// 텍스트 배경 설정
		TitleRect.SetColor(0.0, 0.0, 0.0);
		TitleRect.SetRenderType(RENDER_TYPE_STATIC);

		// BGM 재생
		SDK::SoundTool.Play(SDK::SOUND.TitleBgm, SDK::CHANNEL.BGM);
	}

	void InputKey(SDK::KeyEvent& Event) {
		if (Event.Type != WM_KEYDOWN)  return;

		// 타이틀 활성화 전 메뉴 입력
		if (!TitleActivateState)
			InputFrontPage(Event);

		else if (TitleActivateState && TitleTextRenderState) {
			switch (MenuPage) {
				case ExitPage: // 나가기 페이지 메뉴 입력
					InputExitPage(Event);  break;
				case MainPage: // 메인 페이지 메뉴 입력
					InputMainPage(Event);  break;
				case OptionPage:// 옵션 페이지 메뉴 입력
					InputOptionPage(Event);  break;
				case ResetPage: // 리셋 페이지 메뉴 입력
					InputResetPage(Event);  break;
			}
		}
	}

	// 타이틀 활성화 전 메뉴 입력
	void InputFrontPage(SDK::KeyEvent& Event) {
		auto Exit = [&]() {
			MenuPage = MainPage;
			ExitPageFocused[ExitPageIndex] = false;
			ExitPageFocused[0] = true;
			ExitPageIndex = 0;
			};

		if (Event.Key == VK_UP || Event.Key == VK_DOWN) {
			if (MenuPage == ExitPage) {
				// 방향키 위를 누르면 감소, 아래를 누르면 증가
				ExitPageFocused[ExitPageIndex] = false;
				ExitPageIndex += (int)Event.Key - (int)VK_RIGHT;
				SDK::EXTool.ClampValue(ExitPageIndex, 0, 1, CLAMP_RETURN);
				ExitPageFocused[ExitPageIndex] = true;
			}
		}

		else if (Event.Key == VK_RETURN) {
			switch (MenuPage) {
			case MainPage:
				TitleActivateState = true;
				return;

			case ExitPage:
				if (ExitPageFocused[0])
					SDK::System.Exit();
				else if (ExitPageFocused[1]) 
					Exit();
				
				break;
			}
		}

		else if (Event.Key == VK_ESCAPE) {
			switch (MenuPage) {
			case MainPage:
				MenuPage = ExitPage;
				break;

			case ExitPage:
				Exit();
				break;
			}
		}
	}
	
	// 나가기 페이지 메뉴 입력
	void InputExitPage(SDK::KeyEvent& Event) {
		auto Exit = [&]() {
			MenuPage = MainPage;
			ExitPageFocused[ExitPageIndex] = false;
			ExitPageFocused[0] = true;
			ExitPageIndex = 0;
			};

		if (Event.Key == VK_UP || Event.Key == VK_DOWN) {
			// 방향키 위를 누르면 감소, 아래를 누르면 증가
			ExitPageFocused[ExitPageIndex] = false;
			ExitPageIndex += (int)Event.Key - (int)VK_RIGHT;
			SDK::EXTool.ClampValue(ExitPageIndex, 0, 1, CLAMP_RETURN);
			ExitPageFocused[ExitPageIndex] = true;
		}

		else if (Event.Key == VK_RETURN) {
			if (ExitPageFocused[0])
				SDK::System.Exit();
			else if (ExitPageFocused[1]) 
				Exit();
		}

		else if (Event.Key == VK_ESCAPE) 
			Exit();
	}

	// 메인 페이지 메뉴 입력
	void InputMainPage(SDK::KeyEvent& Event) {
		if (Event.Key == VK_UP || Event.Key == VK_DOWN) {
			// 방향키 위를 누르면 감소, 아래를 누르면 증가
			MainPageFocused[MainPageIndex] = false;
			MainPageIndex += (int)Event.Key - (int)VK_RIGHT;
			SDK::EXTool.ClampValue(MainPageIndex, 0, 2, CLAMP_RETURN);
			MainPageFocused[MainPageIndex] = true;
		}

		else if (Event.Key == VK_RETURN) {
			switch (MainPageIndex) {
			case 0: break;
			case 1: MenuPage = OptionPage;  break;
			case 2: MenuPage = ExitPage;  break;
			}
		}

		else if (Event.Key == VK_ESCAPE)
			MenuPage = ExitPage;
	}

	// 옵션 페이지 메뉴 입력
	void InputOptionPage(SDK::KeyEvent& Event) {
		auto Exit = [&]() {
			MenuPage = MainPage;
			OptionPageFocused[OptionPageIndex] = false;
			OptionPageFocused[0] = true;
			OptionPageIndex = 0;
			};

		if (Event.Key == VK_UP || Event.Key == VK_DOWN) {
			OptionPageFocused[OptionPageIndex] = false;
			OptionPageIndex += (int)Event.Key - (int)VK_RIGHT;
			SDK::EXTool.ClampValue(OptionPageIndex, 0, 5, CLAMP_RETURN);
			OptionPageFocused[OptionPageIndex] = true;
		}

		else if (Event.Key == VK_RETURN) {
			switch (OptionPageIndex) {
			case 0:
				Exit();
				break;

			case 4:
				MenuPage = ResetPage;
				break;
			}
		}

		else if (Event.Key == VK_ESCAPE) 
			Exit();
	}

	// 리셋 페이지 메뉴 입력
	void InputResetPage(SDK::KeyEvent& Event) {
		auto Exit = [&]() {
			MenuPage = OptionPage;
			ResetPageFocused[ResetPageIndex] = false;
			ResetPageFocused[0] = true;
			ResetPageIndex = 0;
			};

		if (Event.Key == VK_UP || Event.Key == VK_DOWN) {
			ResetPageFocused[ResetPageIndex] = false;
			ResetPageIndex += (int)Event.Key - (int)VK_RIGHT;
			SDK::EXTool.ClampValue(ResetPageIndex, 0, 1, CLAMP_RETURN);
			ResetPageFocused[ResetPageIndex] = true;
		}

		else if (Event.Key == VK_RETURN) {
			if (ResetPageIndex == 1) 
				SDK::FILE.HighscoreData.ResetData();
			Exit();
		}

		else if (Event.Key == VK_ESCAPE) 
			Exit();
	}

	void UpdateFunc(float FrameTime) {
		// 인트로 업데이트
		UpdateIntroAnimation(FrameTime);

		// UI 업데이트
		UpdateUI(FrameTime);

		// 카메라 업데이트
		UpdateTitleCamera(FrameTime);

		// 시온 이드 애니메이션 업데이트
		UpdateXionEDAnimation(FrameTime);
	}

	void RenderFunc() {
		RenderObjects();
		RenderUI();
	}

	// 타이틀 화면 세팅
	void SetTitleScreen(bool IsIntroPlayed) {
		// 인트로 플레이 여부
		// 인트로를 이미 플레이 했다면 인트로 애니메이션을 실행하지 않는다.
		if (IsIntroPlayed) {
			IntroPlayState = false;

			// 인트로 애니메이션이 모두 진행된 상태로 초기화 한다.
			TitleActivateState = true;
			TitleTextRenderState = true;
			TitleTextHorizontalOffset = 0.0;
			TitleImagePosition = SDK::Vector2(SDK::ASP(1.0) - 0.45, -1.0 + 0.2);
			TitleImageSize = 1.0;

			// 카메라 위치 지정
			TitleCameraPosition = SDK::Vector2(0.7, 0.4);
			TitleCameraZoom = 1.5;
			SDK::CameraControl.Move(0.7, 0.4);
			SDK::CameraControl.SetZoom(1.5);
		}

		// 타이틀 모드로 최초 진입했다면 인트로 애니메이션을 플레이 한다
		else {
			IntroPlayState = true;

			TitleTextRenderState = false;
			TitleTextHorizontalOffset = 3.0;
			TitleImagePosition = SDK::Vector2(0.0, 0.5);
			TitleImageSize = 1.8;

			// 카메라 위치 지정
			TitleCameraPosition = SDK::Vector2(-1.3, 0.4);
			TitleCameraZoom = 2.0;
			SDK::CameraControl.Move(-1.3, 0.4);
			SDK::CameraControl.SetZoom(2.0);

			TitleCameraPositionMove.SetMovePoint(TitleCameraPosition, SDK::Vector2(0.7, 0.4));
			TitleCameraZoomMove.SetMovePoint(TitleCameraZoom, 1.5);
		}
	}

	// 인트로 애니메이션 업데이트 
	void UpdateIntroAnimation(float FrameTime) {
		// 인트로 활성화 시에만 업데이트
		if (IntroPlayState) {
			if (!GameStartState) {
				if (TitleActivateState) {
					TitleCameraZoomMove.Update(TitleCameraZoom, 0.5, FrameTime);
					TitleCameraPositionMove.Update(TitleCameraPosition, 0.5, FrameTime);

					SDK::Math.Lerp(TitleImagePosition.x, SDK::ASP(1.0) - 0.45, 5.0, FrameTime);
					SDK::Math.Lerp(TitleImagePosition.y, -1.0 + 0.2, 5.0, FrameTime);
					SDK::Math.Lerp(TitleImageSize, 1.0, 5.0, FrameTime);
				}

				if (TitleCameraPosition.x > 0.6)
					TitleTextRenderState = true;
			}
		}
		else {
			TitleCameraPosition.x = 0.7;
			TitleCameraZoom = 1.5;
			TitleImagePosition = SDK::Vector2(SDK::ASP(1.0) - 0.45, -1.0 + 0.2);
		}
	}

	// ui 업데이드
	void UpdateUI(float FrameTime) {
		if (TitleTextRenderState)
			SDK::Math.Lerp(TitleTextHorizontalOffset, 0.0, 5.0, FrameTime);
	}

	// 카메라 업데이트
	void UpdateTitleCamera(float FrameTime) {
		SDK::CameraControl.Move(TitleCameraPosition + EffectCameraPosition);
		SDK::CameraControl.SetZoom(TitleCameraZoom + EffectCameraZoom);
	}

	// 시온 이드 애니메이션 업데이트
	void UpdateXionEDAnimation(float FrameTime) {
		// 시온 상하 크기 루프 업데이트
		XionVerticalSizeLoop.Update(XionVerticalSize, 0.03, 15.0, FrameTime);

		// 이드 떨림 업데이트
		if (EDShakeTimer.UpdateAndCheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE, FrameTime)) {
			EDShakeOffset.x = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			EDShakeOffset.y = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
		}
	}

	//  타이틀 오브젝트 렌더링
	void RenderObjects() {
		// 시온
		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, -1.3, XionVerticalSize * 0.5);
		SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0 + XionVerticalSize);
		SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Xion, Xion_Nervous);

		// 이드
		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, EDShakeOffset);
		SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0);
		SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.ED, ED_Title);

		// 카트
		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, 0.7, -0.3);
		SDK::Transform.Scale(SDK::MoveMatrix, 1.5, 1.5);
		SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Cart, 0);
	}

	// ui 렌더링
	void RenderUI() {
		// 텍스트
		RenderText();

		// 타이틀 이미지
		if (MenuPage == MainPage) {
			Begin(RENDER_TYPE_STATIC);
			SDK::Transform.Move(SDK::MoveMatrix, TitleImagePosition);
			SDK::Transform.Scale(SDK::MoveMatrix, TitleImageSize, TitleImageSize);
			SDK::ImageTool.RenderImage(SDK::IMAGE.Title);
		}
	}

	// 텍스트 렌더링
	void RenderText() {
		switch (MenuPage) {
		case ExitPage: // 나가기 페이지 렌더링
			RenderExitPage();  break;
		case MainPage: // 메인 페이지 렌더링
			RenderMainPage();  break;
		case OptionPage: // 옵션 페이지 렌더링
			RenderOptionPage();  break;
		case ResetPage: // 리셋 페이지 렌더링
			RenderResetPage();  break;
		}
	}

	//메인 페이지 렌더링
	void RenderMainPage() {
		// 배경
		TitleRect.Draw(SDK::ASP(1.0) + TitleTextHorizontalOffset, 0.0, 1.8, 2.0, 0.0, 0.3);

		// 좌측 정렬, 가운데 높이 정렬로 변경
		TitleText.SetAlign(ALIGN_LEFT);
		TitleText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);

		// 텍스트 가로 위치
		float TextHorizontalPositon = SDK::ASP(1.0) - 0.1 + TitleTextHorizontalOffset;

		// 메뉴 항목 렌더링
		for (int i = 0; i < 3; i++) {
			if (MainPageFocused[i])  TitleText.SetColorRGB(255, 213, 80);
			else				     TitleText.SetColor(1.0, 1.0, 1.0);
			TitleText.Render(TextHorizontalPositon, 0.25 - 0.25 * i, 0.1, MenuItem[i].c_str());
		}

		// 최고 점수 렌더링, 갱신 시 초록색으로 하이라이트
		if (SDK::GLOBAL.IsHighScore)  TitleText.SetColor(0.0, 1.0, 0.0);
		else						  TitleText.SetColor(1.0, 1.0, 1.0);
		if (SDK::GLOBAL.HighScore > 0)
			TitleText.Render(TextHorizontalPositon, 0.85, 0.08, L"최고 점수\n%d", SDK::GLOBAL.HighScore);
		else
			TitleText.Render(TextHorizontalPositon, 0.85, 0.08, L"최고 점수\n-", SDK::GLOBAL.HighScore);

		// 최고 금액 렌더링, 갱신 시 초록색으로 하이라이트
		if (SDK::GLOBAL.IsHighRep)  TitleText.SetColor(0.0, 1.0, 0.0);
		else						TitleText.SetColor(1.0, 1.0, 1.0);
		if (SDK::GLOBAL.MaxRep > 0)
			TitleText.Render(TextHorizontalPositon, 0.6, 0.08, L"최고 금액\n%d 골드", SDK::GLOBAL.MaxRep);
		else
			TitleText.Render(TextHorizontalPositon, 0.6, 0.08, L"최고 금액\n- 골드", SDK::GLOBAL.MaxRep);
	}

	// 옵션 페이지 렌더링
	void RenderOptionPage() {
		TitleRect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, 0.7);

		TitleText.SetAlign(ALIGN_MIDDLE);
		TitleText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		TitleText.SetColor(1.0, 1.0, 1.0);
		TitleText.Render(0.0, 0.8, 0.15, L"환경설정");

		TitleText.SetAlign(ALIGN_LEFT);
		for (int i = 0; i < 6; i++) {
			if (OptionPageFocused[i]) TitleText.SetColorRGB(255, 213, 80);
			else					  TitleText.SetColor(1.0, 1.0, 1.0);

			switch (i) {
			case 0:
				TitleText.Render(SDK::ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"타이틀로 돌아가기");
				break;
			case 1:
				if (SDK::GLOBAL.FullscreenAcvivated)
					TitleText.Render(SDK::ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"화면 모드: 전체화면");
				else
					TitleText.Render(SDK::ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"화면 모드: 창");
				break;
			case 2:
				TitleText.Render(SDK::ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"배경음악 볼륨: %.1f", SDK::GLOBAL.BGMVolume);
				break;
			case 3:
				TitleText.Render(SDK::ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"효과음 볼륨: %.1f", SDK::GLOBAL.SFXVolume);
				break;
			case 4:
				TitleText.Render(SDK::ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"진행 상황 초기화");
				break;
			case 5:
				TitleText.Render(SDK::ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"크래딧");
				break;
			}
		}

		if (0 < OptionPageIndex && OptionPageIndex < 4) {
			TitleText.SetHeightAlign(HEIGHT_ALIGN_DEFAULT);
			TitleText.SetAlign(ALIGN_DEFAULT);
			TitleText.Render(SDK::ASP(-1.0) + 0.1, -0.9, 0.08, L"좌 우 방향키로 설정");
		}
	}

	// 리셋 페이지 렌더링
	void RenderResetPage() {
		TitleRect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, 0.7);

		TitleText.SetColor(1.0, 1.0, 1.0);
		TitleText.SetAlign(ALIGN_MIDDLE);
		TitleText.Render(0.0, 0.8, 0.15, L"정말인가요?");

		TitleText.SetAlign(ALIGN_LEFT);
		for (int i = 0; i < 2; i++) {
			if (ResetPageFocused[i])  TitleText.SetColorRGB(255, 213, 80);
			else				      TitleText.SetColor(1.0, 1.0, 1.0);
			TitleText.Render(SDK::ASP(1.0) - 0.1, 0.125 - i * 0.25, 0.1, QuestionItem2[i].c_str());
		}
	}

	// 나가기 페이지 렌더링
	void RenderExitPage() {
		// 배경
		TitleRect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, 0.7);

		// 나가기 묻기 렌더링
		TitleText.SetColor(1.0, 1.0, 1.0);
		TitleText.SetAlign(ALIGN_MIDDLE);
		TitleText.Render(0.0, 0.8, 0.15, L"바탕화면으로 나갈까요?");

		TitleText.SetAlign(ALIGN_LEFT);
		for (int i = 0; i < 2; i++) {
			if (ExitPageFocused[i]) TitleText.SetColorRGB(255, 213, 80);
			else				    TitleText.SetColor(1.0, 1.0, 1.0);
			TitleText.RenderWString(SDK::ASP(1.0) - 0.1, 0.125 - i * 0.25, 0.1, QuestionItem[i]);
		}
	}
};