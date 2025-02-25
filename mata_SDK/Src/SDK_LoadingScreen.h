#pragma once
#include "SDK_Scene.h"
#include "SDK_ImageTool.h"
#include "SDK_SoundTool.h"
#include "SDK_Camera.h"
#include "SDK_FontLoader.h"
#include "SDK_IntroMode.h"
#include "SDK_FPSIndicator.h"

class SDK_LoadingScreen : public SDK::Object {
private:
	HANDLE  SystemResourceLoadHandle{};
	bool    LoadStart{};
	bool    SystemResourceLoadEnd{};

	HANDLE  ImageResourceLoadHandle{};
	HANDLE  SoundResourceLoadHandle{};
	HANDLE  DataResourceLoadHandle{};
	HANDLE  FontResourceLoadHandle{};

	bool    ImageResourceLoadEnd{};
	bool    SoundResourceLoadEnd{};
	bool    DataResourceLoadEnd{};
	bool    FontResourceLoadEnd{};
	bool    InitializationEnd{};

	float Rotation{};
	float SpinnerOpacity{ 1.0 };


public:
	void UpdateFunc(float FrameTime) {
		if (!LoadStart) {
			SDK::Camera.Init();
			SDK::ImageTool.Init();
			SDK::SoundTool.Init();

			SDK::FontLoader.LoadT(SDK::SYSRES.SDK_FONT_DIRECTORY);
			SDK::ImageTool.LoadImage(SDK::SYSRES.LOADING_SPINNER, SDK::SYSRES.SDK_LOADING_SPINNER_DIRECTORY, IMAGE_TYPE_LINEAR);
			SDK::ImageTool.LoadImage(SDK::SYSRES.MATA_LOGO, SDK::SYSRES.MATA_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
			
			SDK::ImageTool.LoadImage(SDK::IMAGE.Title, "Assets//Sprite//UI//title.png");
			SDK::ImageTool.LoadImage(SDK::IMAGE.LoadingED, "Assets//Sprite//People//ed-loading.png");

			SDK::ThreadTool.Create(SystemResourceLoadHandle, SystemResourceLoader);

			LoadStart = true;
		}

		else {
			Rotation += 200 * FrameTime;

			if (LoadResources()) {
				if (!InitializationEnd) {
					SDK::ImageTool.Map();
					std::cout << "All of Image resources mapped." << std::endl;

					ModeAttribute();
					std::cout << "All of Mode pointers mapped." << std::endl;

					InitializationEnd = true;
				}

				else {
					if (SDK::GLOBAL.FullscreenAcvivated)
						SDK::System.SwitchScreenState();
					else
						SDK::System.ChangeScreenSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

					if(ENABLE_INTRO_SCREEN) {
						SpinnerOpacity -= FrameTime * 2.0;
						if (SDK::EXTool.CheckClampValue(SpinnerOpacity, 0.0, CLAMP_LESS)) {
							if (SHOW_FPS)  AddFPSIndicator();
							SDK::Scene.SwitchMode(IntroMode.Start);
						}
					}

					else {
						if (SHOW_FPS)  AddFPSIndicator();
						SDK::Scene.SwitchMode(SDK::START_MODE);
					}
				}
			}
		}
	}

	void RenderFunc() {
		Begin(RENDER_TYPE_STATIC);
		SDK::Transform.Scale(SDK::MoveMatrix, 0.6, 0.6);
		SDK::ImageTool.SetColor(1.0, 1.0, 1.0);
		SDK::ImageTool.RenderImage(SDK::SYSRES.MATA_LOGO, SpinnerOpacity);

		Begin(RENDER_TYPE_STATIC);
		SDK::Transform.Scale(SDK::MoveMatrix, 1.0, 1.0);
		SDK::Transform.Rotate(SDK::MoveMatrix, Rotation);
		SDK::ImageTool.SetColor(1.0, 1.0, 1.0);
		SDK::ImageTool.RenderImage(SDK::SYSRES.LOADING_SPINNER, SpinnerOpacity);
	}

	void AddFPSIndicator() {
		SDK::Scene.AddSystemObject(new SDK_FPS_Indicator);
	}

	bool LoadResources() {
		if (!SDK::ThreadTool.CheckAlive(SystemResourceLoadHandle) && !SystemResourceLoadEnd) {
			SDK::ThreadTool.Close(SystemResourceLoadHandle);
			SDK::ThreadTool.Create(ImageResourceLoadHandle, ImageResourceLoader);
			SDK::ThreadTool.Create(SoundResourceLoadHandle, SoundResourceLoader);
			SDK::ThreadTool.Create(DataResourceLoadHandle, FileResourceLoader);
			SDK::ThreadTool.Create(FontResourceLoadHandle, FontResourceLoader);
			std::cout << "System resource load completed." << std::endl;

			SystemResourceLoadEnd = true;
		}

		if (!ImageResourceLoadEnd && !SDK::ThreadTool.CheckAlive(ImageResourceLoadHandle)) {
			SDK::ThreadTool.Close(ImageResourceLoadHandle);
			std::cout << "Image resource load completed." << std::endl;
			ImageResourceLoadEnd = true;
		}

		if (!SoundResourceLoadEnd && !SDK::ThreadTool.CheckAlive(SoundResourceLoadHandle)) {
			SDK::ThreadTool.Close(SoundResourceLoadHandle);
			std::cout << "Sound resource load completed." << std::endl;
			SoundResourceLoadEnd = true;
		}

		if (!DataResourceLoadEnd && !SDK::ThreadTool.CheckAlive(DataResourceLoadHandle)) {
			SDK::ThreadTool.Close(DataResourceLoadHandle);
			std::cout << "Data resource load completed." << std::endl;
			DataResourceLoadEnd = true;
		}

		if (!FontResourceLoadEnd && !SDK::ThreadTool.CheckAlive(FontResourceLoadHandle)) {
			SDK::ThreadTool.Close(FontResourceLoadHandle);
			std::cout << "Font resource load completed." << std::endl;
			FontResourceLoadEnd = true;
		}

		if (SystemResourceLoadEnd && ImageResourceLoadEnd && SoundResourceLoadEnd && DataResourceLoadEnd && FontResourceLoadEnd)
			return true;

		return false;
	}

	static DWORD WINAPI SystemResourceLoader(LPVOID Param) {
		SDK::ImageTool.LoadImageT(SDK::SYSRES.SDK_LOGO, SDK::SYSRES.SDK_LOGO_IMAGE_DIRECTORY, IMAGE_TYPE_LINEAR);
		SDK::ImageTool.LoadImageT(SDK::SYSRES.FMOD_LOGO, SDK::SYSRES.FMOD_LOGO_DIRECTORY, IMAGE_TYPE_LINEAR);
		SDK::ImageTool.LoadImageT(SDK::SYSRES.COLOR_TEXTURE, SDK::SYSRES.COLOR_TEXTURE_DIRECTORY);
		SDK::SoundTool.Load(SDK::SYSRES.INTRO_SOUND, SDK::SYSRES.SDK_LOGO_SOUND_DIRECTORY);

		SDK::SYSRES.GLU_CIRCLE = gluNewQuadric();
		SDK::SYSRES.GLU_LINE_CIRCLE = gluNewQuadric();
		gluQuadricDrawStyle(SDK::SYSRES.GLU_CIRCLE, GLU_FILL);
		gluQuadricDrawStyle(SDK::SYSRES.GLU_LINE_CIRCLE, GLU_FILL);

		return 0;
	}
};

