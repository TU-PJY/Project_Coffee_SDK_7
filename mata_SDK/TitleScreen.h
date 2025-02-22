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
	// Ÿ��Ʋ ���� ����
	bool TitleActivateState{};
	// ��Ʈ�� �÷��� ����
	bool IntroPlayState{};
	// ���� ���� ����
	bool GameStartState{};
	// ��Ʈ�� ���� ����
	bool IntroStartState{};

	// Ÿ��Ʋ �̹��� ��ġ
	SDK::Vector2 TitleImagePosition{};
	// Ÿ��Ʋ �̹��� ũ��
	float TitleImageSize{};

	// �ÿ� ���� ũ��
	float XionVerticalSize{};
	// �ÿ� ���� ũ�� ����
	SDK::SinLoop XionVerticalSizeLoop{};

	// �̵� ���� ��ġ
	SDK::Vector2 EDShakeOffset{};
	// �̵� ���� ���� Ÿ�̸�
	SDK::Timer EDShakeTimer{};

	// ī�޶� ��ġ �� ��
	SDK::Vector2 TitleCameraPosition{};
	float TitleCameraZoom{};
	// ī�޶� �̵� �ִϸ��̼�
	SDK::SinMove TitleCameraPositionMove{};
	// ī�޶� �� �ִϸ��̼�
	SDK::SinMove TitleCameraZoomMove{};

	// ��Ʈ�θ� ������ ������ ī�޶� ��ġ �� ��
	SDK::Vector2 EffectCameraPosition{};
	float EffectCameraZoom{};

	// Ÿ��Ʋ ȭ�� �ؽ�Ʈ
	SDK::Text TitleText{};
	// �ؽ�Ʈ ���� ��ġ ������
	float TitleTextHorizontalOffset{};
	// �ؽ�Ʈ ������ ����
	bool TitleTextRenderState{};

	// �ؽ�Ʈ ���
	std::wstring MenuItem[3] = {
		L"����!",
		L"ȯ�漳��",
		L"����ȭ������\n������"
	};

	std::wstring QuestionItem[2] = {
		L"��",
		L"�ƴϿ�"
	};

	// �޴� �ؽ�Ʈ ��Ŀ�� 
	bool ExitPageFocused[2]{ true, false };
	bool MainPageFocused[3]{ true, false, false };
	bool OptionPageFocused[6]{ true, false, false, false, false, false };
	bool ResetPageFocused[2]{ false, true };

	// ���� ����Ű�� �޴� �׸�
	int ExitPageIndex{};
	int MainPageIndex{};
	int OptionPageIndex{};
	int ResetPageIndex{};

	// �޴� ������, ������ ��ȣ�� ���� ǥ�õǴ� �׸��� �޶�����
	int MenuPage{ 1 };

	// �ؽ�Ʈ ���
	SDK::RectBrush TitleRect{};

public:
	TitleScreen(bool IsIntroPlayed) {
		// �ؽ�Ʈ �ʱ�ȭ
		TitleText.Init(SDK::FONTNAME.Main);
		TitleText.SetAlign(ALIGN_LEFT);
		TitleText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		TitleText.SetShadow(0.1, -0.1, 0.8);
		TitleText.SetLineGap(0.02);

		// Ÿ��Ʋ ȭ�� ����
		SetTitleScreen(IsIntroPlayed);

		// �ؽ�Ʈ ��� ����
		TitleRect.SetColor(0.0, 0.0, 0.0);
		TitleRect.SetRenderType(RENDER_TYPE_STATIC);

		// BGM ���
		SDK::SoundTool.Play(SDK::SOUND.TitleBgm, SDK::CHANNEL.BGM);
	}

	void InputKey(SDK::KeyEvent& Event) {
		if (Event.Type != WM_KEYDOWN)  return;

		// Ÿ��Ʋ Ȱ��ȭ �� �޴� �Է�
		if (!TitleActivateState)
			InputFrontPage(Event);

		else {
			switch (MenuPage) {
			case MainPage: // ���� ������ �޴� �Է�
				InputMainPage(Event);  break;
			case ExitPage: // ������ ������ �޴� �Է�
				InputExitPage(Event);  break;
			}
		}
	}

	// Ÿ��Ʋ Ȱ��ȭ �� �޴� �Է�
	void InputFrontPage(SDK::KeyEvent& Event) {
		if (Event.Key == VK_UP || Event.Key == VK_DOWN) {
			if (MenuPage == ExitPage) {
				// ����Ű ���� ������ ����, �Ʒ��� ������ ����
				ExitPageIndex += (int)Event.Key - (int)VK_RIGHT;
				SDK::EXTool.ClampValue(ExitPageIndex, 0, 1, CLAMP_RETURN);
				for (int i = 0; i < 2; i++)  
					ExitPageFocused[i] = false;
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
				else if (ExitPageFocused[1]) {
					MenuPage = MainPage;
					for (int i = 0; i < 2; i++)  
						ExitPageFocused[i] = false;
					ExitPageFocused[0] = true;
					ExitPageIndex = 0;
				}
				break;
			}
		}

		else if (Event.Key == VK_ESCAPE) {
			switch (MenuPage) {
			case MainPage:
				MenuPage = ExitPage;
				break;

			case ExitPage:
				MenuPage = MainPage;
				for (int i = 0; i < 2; i++) 
					ExitPageFocused[i] = false;
				ExitPageFocused[0] = true;
				ExitPageIndex = 0;
				break;
			}
		}
	}

	// ���� ������ �޴� �Է�
	void InputMainPage(SDK::KeyEvent& Event) {
		if (Event.Key == VK_UP || Event.Key == VK_DOWN) {
			// ����Ű ���� ������ ����, �Ʒ��� ������ ����
			MainPageIndex += (int)Event.Key - (int)VK_RIGHT;
			SDK::EXTool.ClampValue(MainPageIndex, 0, 2, CLAMP_RETURN);
			for (int i = 0; i < 3; i++) 
				MainPageFocused[i] = false;
			MainPageFocused[MainPageIndex] = true;
		}

		else if (Event.Key == VK_ESCAPE)
			MenuPage = ExitPage;
	}

	// ������ ������ �޴� �Է�
	void InputExitPage(SDK::KeyEvent& Event) {
		if (Event.Key == VK_UP || Event.Key == VK_DOWN) {
			// ����Ű ���� ������ ����, �Ʒ��� ������ ����
			ExitPageIndex += (int)Event.Key - (int)VK_RIGHT;
			SDK::EXTool.ClampValue(ExitPageIndex, 0, 1, CLAMP_RETURN);
			for (int i = 0; i < 2; i++)  
				ExitPageFocused[i] = false;
			ExitPageFocused[ExitPageIndex] = true;
		}

		else if (Event.Key == VK_RETURN) {
			if (ExitPageFocused[0])
				SDK::System.Exit();
			else if (ExitPageFocused[1]) {
				MenuPage = MainPage;
				for (int i = 0; i < 2; i++)
					ExitPageFocused[i] = false;
				ExitPageFocused[0] = true;
				ExitPageIndex = 0;
			}
		}

		else if (Event.Key == VK_ESCAPE) {
			for (int i = 0; i < 2; i++)  
				ExitPageFocused[i] = false;
			ExitPageFocused[0] = true;
			ExitPageIndex = 0;
			MenuPage = MainPage;
		}
	}

	void UpdateFunc(float FrameTime) {
		// ��Ʈ�� ������Ʈ
		UpdateIntroAnimation(FrameTime);

		// UI ������Ʈ
		UpdateUI(FrameTime);

		// ī�޶� ������Ʈ
		UpdateTitleCamera(FrameTime);

		// �ÿ� �̵� �ִϸ��̼� ������Ʈ
		UpdateXionEDAnimation(FrameTime);
	}

	void RenderFunc() {
		RenderObjects();
		RenderUI();
	}

	// Ÿ��Ʋ ȭ�� ����
	void SetTitleScreen(bool IsIntroPlayed) {
		// ��Ʈ�� �÷��� ����
		// ��Ʈ�θ� �̹� �÷��� �ߴٸ� ��Ʈ�� �ִϸ��̼��� �������� �ʴ´�.
		if (IsIntroPlayed) {
			IntroPlayState = false;

			// ��Ʈ�� �ִϸ��̼��� ��� ����� ���·� �ʱ�ȭ �Ѵ�.
			TitleTextRenderState = true;
			TitleTextHorizontalOffset = 0.0;
			TitleImagePosition = SDK::Vector2(SDK::ASP(1.0) - 0.45, -1.0 + 0.2);
			TitleImageSize = 1.0;

			// ī�޶� ��ġ ����
			TitleCameraPosition = SDK::Vector2(0.7, 0.4);
			TitleCameraZoom = 1.5;
			SDK::CameraControl.Move(0.7, 0.4);
			SDK::CameraControl.SetZoom(1.5);
		}

		// Ÿ��Ʋ ���� ���� �����ߴٸ� ��Ʈ�� �ִϸ��̼��� �÷��� �Ѵ�
		else {
			IntroPlayState = true;

			TitleTextRenderState = false;
			TitleTextHorizontalOffset = 3.0;
			TitleImagePosition = SDK::Vector2(0.0, 0.5);
			TitleImageSize = 1.8;

			// ī�޶� ��ġ ����
			TitleCameraPosition = SDK::Vector2(-1.3, 0.4);
			TitleCameraZoom = 2.0;
			SDK::CameraControl.Move(-1.3, 0.4);
			SDK::CameraControl.SetZoom(2.0);

			TitleCameraPositionMove.SetMovePoint(TitleCameraPosition, SDK::Vector2(0.7, 0.4));
			TitleCameraZoomMove.SetMovePoint(TitleCameraZoom, 1.5);
		}
	}

	// ��Ʈ�� �ִϸ��̼� ������Ʈ 
	void UpdateIntroAnimation(float FrameTime) {
		// ��Ʈ�� Ȱ��ȭ �ÿ��� ������Ʈ
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

	// ui �����̵�
	void UpdateUI(float FrameTime) {
		if (TitleTextRenderState)
			SDK::Math.Lerp(TitleTextHorizontalOffset, 0.0, 5.0, FrameTime);
	}

	// ī�޶� ������Ʈ
	void UpdateTitleCamera(float FrameTime) {
		SDK::CameraControl.Move(TitleCameraPosition + EffectCameraPosition);
		SDK::CameraControl.SetZoom(TitleCameraZoom + EffectCameraZoom);
	}

	// �ÿ� �̵� �ִϸ��̼� ������Ʈ
	void UpdateXionEDAnimation(float FrameTime) {
		// �ÿ� ���� ũ�� ���� ������Ʈ
		XionVerticalSizeLoop.Update(XionVerticalSize, 0.03, 15.0, FrameTime);

		// �̵� ���� ������Ʈ
		if (EDShakeTimer.UpdateAndCheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE, FrameTime)) {
			EDShakeOffset.x = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			EDShakeOffset.y = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
		}
	}

	//  Ÿ��Ʋ ������Ʈ ������
	void RenderObjects() {
		// �ÿ�
		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, -1.3, XionVerticalSize * 0.5);
		SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0 + XionVerticalSize);
		SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Xion, Xion_Nervous);

		// �̵�
		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, EDShakeOffset);
		SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0);
		SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.ED, ED_Title);

		// īƮ
		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, 0.7, -0.3);
		SDK::Transform.Scale(SDK::MoveMatrix, 1.5, 1.5);
		SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Cart, 0);
	}

	// ui ������
	void RenderUI() {
		// �ؽ�Ʈ
		RenderText();

		// Ÿ��Ʋ �̹���
		if (MenuPage == MainPage) {
			Begin(RENDER_TYPE_STATIC);
			SDK::Transform.Move(SDK::MoveMatrix, TitleImagePosition);
			SDK::Transform.Scale(SDK::MoveMatrix, TitleImageSize, TitleImageSize);
			SDK::ImageTool.RenderImage(SDK::IMAGE.Title);
		}
	}

	// �ؽ�Ʈ ������
	void RenderText() {
		switch (MenuPage) {
		case ExitPage: // ������ ������ ������
			RenderExitPage();  break;

		case MainPage: // ���� ������ ������
			RenderMainPage();  break;

		case OptionPage: // �ɼ� ������ ������
			RenderOptionPage();  break;
		}
	}

	//���� ������ ������
	void RenderMainPage() {
		// ���
		TitleRect.Draw(SDK::ASP(1.0) + TitleTextHorizontalOffset, 0.0, 1.8, 2.0, 0.0, 0.3);

		// ���� ����, ��� ���� ���ķ� ����
		TitleText.SetAlign(ALIGN_LEFT);
		TitleText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);

		// �ؽ�Ʈ ���� ��ġ
		float TextHorizontalPositon = SDK::ASP(1.0) - 0.1 + TitleTextHorizontalOffset;

		// �޴� �׸� ������
		for (int i = 0; i < 3; i++) {
			if (MainPageFocused[i])  TitleText.SetColorRGB(255, 213, 80);
			else				 TitleText.SetColor(1.0, 1.0, 1.0);
			TitleText.Render(TextHorizontalPositon, 0.25 - 0.25 * i, 0.1, MenuItem[i].c_str());
		}

		// �ְ� ���� ������, ���� �� �ʷϻ����� ���̶���Ʈ
		if (SDK::GLOBAL.IsHighScore)  TitleText.SetColor(0.0, 1.0, 0.0);
		else						  TitleText.SetColor(1.0, 1.0, 1.0);
		if (SDK::GLOBAL.HighScore > 0)
			TitleText.Render(TextHorizontalPositon, 0.85, 0.08, L"�ְ� ����\n%d", SDK::GLOBAL.HighScore);
		else
			TitleText.Render(TextHorizontalPositon, 0.85, 0.08, L"�ְ� ����\n-", SDK::GLOBAL.HighScore);

		// �ְ� �ݾ� ������, ���� �� �ʷϻ����� ���̶���Ʈ
		if (SDK::GLOBAL.IsHighRep)  TitleText.SetColor(0.0, 1.0, 0.0);
		else						TitleText.SetColor(1.0, 1.0, 1.0);
		if (SDK::GLOBAL.MaxRep > 0)
			TitleText.Render(TextHorizontalPositon, 0.6, 0.08, L"�ְ� �ݾ�\n%d ���", SDK::GLOBAL.MaxRep);
		else
			TitleText.Render(TextHorizontalPositon, 0.6, 0.08, L"�ְ� �ݾ�\n- ���", SDK::GLOBAL.MaxRep);
	}

	// �ɼ� ������ ������
	void RenderOptionPage() {

	}

	// ������ ������ ������
	void RenderExitPage() {
		// ���
		TitleRect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0, 0.0, 0.7);

		// ������ ���� ������
		TitleText.SetColor(1.0, 1.0, 1.0);
		TitleText.SetAlign(ALIGN_MIDDLE);
		TitleText.Render(0.0, 0.8, 0.15, L"����ȭ������ �������?");

		TitleText.SetAlign(ALIGN_LEFT);
		for (int i = 0; i < 2; i++) {
			if (ExitPageFocused[i]) TitleText.SetColorRGB(255, 213, 80);
			else				    TitleText.SetColor(1.0, 1.0, 1.0);
			TitleText.RenderWString(SDK::ASP(1.0) - 0.1, 0.125 - i * 0.25, 0.1, QuestionItem[i]);
		}
	}
};