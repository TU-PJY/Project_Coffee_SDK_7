#pragma once
#include <SDK_Scene.h>

#include "Cover.h"

class ZZZ : public SDK::Object {
private:
	SDK::Vector2 Position{};
	float Opacity{1.0};
	float Size{};
	SDK::SinLoop SizeLoop{};

public:
	ZZZ(SDK::Vector2 PositionValue) {
		Position = PositionValue;
		SizeLoop.SetValue(0.0);
	}
	
	void RenderFunc() {
		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, Position);
		SDK::Transform.Scale(SDK::MoveMatrix, Size, Size);
		SDK::ImageTool.RenderImage(SDK::IMAGE.ZZZ, Opacity);
	}

	void UpdateFunc(float FrameTime) {
		Position.x += 0.3 * FrameTime;
		Position.y += 0.2 * FrameTime;
		Opacity -= 0.5 * FrameTime;
		SizeLoop.Update(Size, 0.2, 1.5, FrameTime);
		SDK::EXTool.ClampValue(Opacity, 0.0, CLAMP_LESS);
		if (Opacity <= 0.0)
			SDK::Scene.DeleteObject(this);
	}
};

class CreditScreen : public SDK::Object {
private:
	float CreditZoom{ 2.2 };
	float CreditHeight{ 0.3 };
	float CreditPosition{};

	float EDPosition = -0.35;
	int EDFrame{};

	float XionPosition = 0.4;

	SDK::Timer EDTimer{};
	float BlinkInterval{};

	SDK::SinLoop EDLoop{};
	SDK::SinLoop XionLoop{};
	SDK::SinLoop LightLoop{};
	float LightOpacity{};

	float EDSize{};
	float XionSize{};

	SDK::Timer XionTimer{};

	SDK::Text Text{};

	std::vector<std::wstring> CreditStr = {
		L" ",
		L"- Original Game BY -",
		L"- A Computer game BY -",
		L"- Programming & Art -",
		L"- Music -",
		L"- Effect sound -",
		L"- Made using FMOD API BY -",
		L" ",
		L"플레이 해주셔서 감사합니다!"
	};

	std::vector<std::wstring> Compositor = {
		L" ",
		L"EPID Games",
		L"mata_",
		L"mata_",
		L"Rob0ne\nContext Sensitive\nPixabay",
		L"Context Sensitive\nPixabay",
		L"Firelight Technologies Pty Ltd.",
		L" ",
		L" ",
	};

	bool StartTextRender{};
	bool TextRenderState{};
	SDK::Timer CreditTimer{};
	int CurrentIndex{};

	SDK::Timer ExitEDTimer{};
	bool EDExitState{};
	bool ExitState{};

	SDK::RectBrush Rect{};

public:
	CreditScreen() {
		SDK::System.SetBackColorRGB(78, 99, 151);
		SDK::CameraControl.SetZoom(2.2);
		SDK::CameraControl.Move(0.0, 0.3);

		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetLineGap(0.01);

		SDK::SoundTool.SetVolume(SDK::CHANNEL.BGM, SDK::GLOBAL.BGMVolume);
		SDK::SoundTool.Play(SDK::SOUND.CreditBGM, SDK::CHANNEL.BGM);

		Rect.SetRenderType(RENDER_TYPE_STATIC);
		Rect.SetColor(0.0, 0.0, 0.0);
	}

	void InputKey(SDK::KeyEvent& Event) {
		if (Event.Type == WM_KEYDOWN && Event.Key == VK_RETURN) {
			EDExitState = true;
			SDK::Scene.DeleteInputObject(this);
		}
	}

	void UpdateFunc(float FrameTime) {
		EDLoop.Update(EDSize, 0.005, 4.0, FrameTime);
		XionLoop.Update(XionSize, 0.03, 1.0, FrameTime);
		LightLoop.Update(LightOpacity, 0.2, 20.0, FrameTime);

		XionTimer.Update(FrameTime);
		if (XionTimer.CheckMiliSec(0.6, 1, CHECK_AND_INTERPOLATE))
			SDK::Scene.AddObject(new ZZZ(SDK::Vector2(XionPosition + 0.3, 0.5)), "zzz", LAYER2);

		
		if (!EDExitState) {
			EDTimer.Update(FrameTime);
			if (EDTimer.CheckMiliSec(BlinkInterval, 1, CHECK_AND_INTERPOLATE)) {
				if (EDFrame == 0) {
					BlinkInterval = 0.2;
					EDFrame = 1;
				}
				else {
					BlinkInterval = 2.0;
					EDFrame = 0;
				}
			}
		}

		else {
			EDFrame = 0;
		}


		CreditTimer.Update(FrameTime);
		if (!StartTextRender && CreditTimer.CheckMiliSec(12.96, 2, CHECK_AND_INTERPOLATE)) {
			StartTextRender = true;
			TextRenderState = true;
		}

		if (StartTextRender && !TextRenderState && CreditTimer.CheckMiliSec(0.81, 3, CHECK_AND_INTERPOLATE)) 
			TextRenderState = true;

		if (CurrentIndex < CreditStr.size() - 1 && StartTextRender && TextRenderState && CreditTimer.CheckMiliSec(5.67, 3, CHECK_AND_INTERPOLATE)) {
			TextRenderState = false;
			CurrentIndex++;
		}

		if (!EDExitState) {
			CreditZoom -= 1.4 * 0.1 * FrameTime;
			CreditHeight -= 0.2 * 0.1 * FrameTime;
			SDK::EXTool.ClampValue(CreditZoom, 0.8, CLAMP_LESS);
			SDK::EXTool.ClampValue(CreditHeight, 0.1, CLAMP_LESS);
		}

		if (EDExitState) {
			SDK::SoundTool.FadeOut(SDK::CHANNEL.BGM, 1.0, FrameTime);

			SDK::Math.Lerp(CreditPosition, EDPosition, 3.0, FrameTime);
			SDK::Math.Lerp(CreditHeight, 0.2, 3.0, FrameTime);
			SDK::Math.Lerp(CreditZoom, 1.8, 3.0, FrameTime);

			ExitEDTimer.Update(FrameTime);
			if (ExitEDTimer.CheckSec(1, CHECK_AND_RESUME))
				EDFrame = 2;

			if (ExitEDTimer.CheckSec(2, CHECK_AND_RESUME)) {
				if (!ExitState) {
					SDK::SoundTool.Play(SDK::SOUND.ShutDown, SDK::CHANNEL.SFX);
				}
				ExitState = true;
			}

			if (ExitEDTimer.CheckSec(4, CHECK_AND_RESUME)) {
				SDK::GLOBAL.TitleIntroPlayed = false;
				SDK::SoundTool.Stop(SDK::CHANNEL.BGM);
				SDK::Scene.SwitchMode(SDK::MODE.Title);
				SDK::Scene.AddObject(new DisappearCover, "dis_cover", LAYER7);
			}
		}

		SDK::CameraControl.SetZoom(CreditZoom);
		SDK::CameraControl.Move(CreditPosition, CreditHeight);
	}

	void RenderFunc() {
		if (ExitState)
			Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0);

		else {
			// 총
			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, 1.8, 0.5);
			SDK::Transform.Scale(SDK::MoveMatrix, 1.6, 1.6);
			SDK::ImageTool.RenderImage(SDK::IMAGE.Gun);

			// 바닥
			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, CreditPosition, -1.5);
			SDK::Transform.Scale(SDK::MoveMatrix, SDK::ASP(2.0) / SDK::Camera.Zoom, 2.0);
			SDK::ImageTool.SetColorRGB(67, 76, 99);
			SDK::ImageTool.RenderImage(SDK::SYSRES.COLOR_TEXTURE);

			// 소파 뒷 부분
			Begin();
			SDK::Transform.Scale(SDK::MoveMatrix, 4.0, 4.0);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Sofa, 0);

			// 시온 몸통
			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, XionPosition, XionSize * 0.5);
			SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0 + XionSize);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.CreditXion, 0);

			// 이드 몸통
			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, EDPosition, EDSize * 0.5);
			SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0 + EDSize);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.CreditED, EDFrame);

			// 소파 쿠션 부분
			Begin();
			SDK::Transform.Scale(SDK::MoveMatrix, 4.0, 4.0);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Sofa, 2);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Sofa, 1);

			// 시온 다리
			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, XionPosition, XionSize * 0.5);
			SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0 + XionSize);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.CreditXion, 1);

			// 의자
			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, XionPosition, 0.0);
			SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0);
			SDK::ImageTool.RenderImage(SDK::IMAGE.Chair);

			// 이드 다리
			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, EDPosition, EDSize * 0.5);
			SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0 + EDSize);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.CreditED, 3);

			// TV 불빛
			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, 0.0, -1.4);
			SDK::Transform.Scale(SDK::MoveMatrix, 4.0, 4.0);
			SDK::ImageTool.SetColor(1.0, 1.0, 1.0);
			SDK::ImageTool.RenderImage(SDK::IMAGE.TVLight, 0.5 - LightOpacity);

			SDK::ImageTool.SetColor(0.0, 0.0, 0.0);
			SDK::ImageTool.RenderImage(SDK::IMAGE.TV);

			// 텍스트
			if (!EDExitState) {
				if (TextRenderState) {
					if (CurrentIndex == 0) {
						Begin(RENDER_TYPE_STATIC);
						SDK::Transform.Move(SDK::MoveMatrix, 0.0, 0.7);
						SDK::Transform.Scale(SDK::MoveMatrix, 1.2, 1.2);
						SDK::ImageTool.RenderImage(SDK::IMAGE.Title);
					}

					if (CurrentIndex == 6) {
						Begin(RENDER_TYPE_STATIC);
						SDK::Transform.Move(SDK::MoveMatrix, 0.0, 0.6);
						SDK::Transform.Scale(SDK::MoveMatrix, 0.7, 0.7);
						SDK::ImageTool.SetColor(1.0, 1.0, 1.0);
						SDK::ImageTool.RenderImage(SDK::SYSRES.FMOD_LOGO);
					}

					if (CurrentIndex == 7) {
						Begin(RENDER_TYPE_STATIC);
						SDK::Transform.Move(SDK::MoveMatrix, 0.0, 0.75);
						SDK::Transform.Scale(SDK::MoveMatrix, 0.7, 0.7);
						SDK::ImageTool.SetColor(1.0, 1.0, 1.0);
						SDK::ImageTool.RenderImage(SDK::SYSRES.SDK_LOGO);
					}

					else {
						Text.SetAlign(ALIGN_MIDDLE);
						Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);

						Text.SetColorRGB(255, 213, 80);
						if (CurrentIndex == 8)
							Text.Render(0.0, 0.85, 0.15, CreditStr[CurrentIndex].c_str());
						else
							Text.Render(0.0, 0.95, 0.15, CreditStr[CurrentIndex].c_str());

						Text.SetColor(1.0, 1.0, 1.0);
						Text.Render(0.0, 0.8, 0.08, Compositor[CurrentIndex].c_str());
					}
				}

				Text.SetAlign(ALIGN_LEFT);
				Text.SetHeightAlign(HEIGHT_ALIGN_DEFAULT);
				Text.SetColorRGB(255, 213, 80);
				Text.Render(SDK::ASP(1.0) - 0.05, -0.95, 0.1, L"Enter를 눌러 계속");
			}
		}
	}
};