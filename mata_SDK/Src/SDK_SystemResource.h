#pragma once
#include "SDK_Header.h"
namespace SDK {
	struct SDK_SYSTEM_RESOURCE {
		const char* SDK_LOGO_SOUND_DIRECTORY = "SDKResource//Sound//sound-logo.wav";
		const char* SDK_LOGO_IMAGE_DIRECTORY = "SDKResource//Image//Common//logo-SDK.png";
		const char* MATA_LOGO_IMAGE_DIRECTORY = "SDKResource//Image//Common//logo-mata.png";
		const char* SDK_LOADING_SPINNER_DIRECTORY = "SDKResource//Image//Common//spinner.png";
		const char* FMOD_LOGO_DIRECTORY = "SDKResource//Image//Common//logo-FMOD.png";
		const char* COLOR_TEXTURE_DIRECTORY = "SDKResource//Image//ShapeUtil//ColorTexture.png";
		const char* SDK_FONT_DIRECTORY = "SDKResource//Font//Roboto-Regular.ttf";

		SDK::Image SDK_LOGO{};
		SDK::Image MATA_LOGO{};
		SDK::Image FMOD_LOGO{};
		SDK::Image COLOR_TEXTURE{};
		SDK::Image LOADING_SPINNER{};

		GLUquadric* GLU_CIRCLE{};
		GLUquadric* GLU_LINE_CIRCLE{};

		SDK::Sound INTRO_SOUND{};
	};

	extern SDK_SYSTEM_RESOURCE SYSRES;
}