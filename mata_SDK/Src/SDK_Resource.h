#pragma once
#include "SDK_Header.h"
#include "SDK_EnumValue.h"
#include "SDK_EnumValue.h"
#include "SDK_ImageTool.h"
#include "SDK_SoundTool.h"
#include "SDK_Script.h"
#include "SDK_Data.h"
#include "SDK_FontLoader.h"
#include "SDK_Text.h"

namespace SDK {
	// Declare the sprite resource here. 
	struct SDK_IMAGE_RESOURCE {
		SpriteSheet ED{};
		SpriteSheet Xion{};
		Image	    XionBack{};
		SpriteSheet ED_GameOver{};
		SpriteSheet Chloe{};

		SpriteSheet Coffee{};
		SpriteSheet CoffeeBack{};
		SpriteSheet DestroyedCoffee{};
		Image       DestroyedCan{};
		SpriteSheet Shelf{};
		SpriteSheet Other{};
		SpriteSheet Drop{};
		Image       Stick{};

		SpriteSheet Cart{};
		SpriteSheet People{};
		SpriteSheet PeopleBack{};

		Image Smoke{};
		Image FloorTile{};

		Image GameOverBackGround{};
		Image GameOverBackGround2{};

		Image BackGroundShelf{};
		Image Pillar{};

		SpriteSheet CreditED{};
		SpriteSheet CreditXion{};
		SpriteSheet Sofa{};

		Image TV{};
		Image TVLight{};
		Image Chair{};

		Image Title{};
		Image Tutorial{};

		Image LoadingED{};

		Image ZZZ{};
	};

	// Declare the sound resource here. 
	struct SDK_SOUND_RESOURCE {
		Sound Whoosh{};
		Sound MissWhoosh{};
		Sound NextWhoosh{};

		Sound BoxBreak[3]{}, Bottle[3]{}, CanBreak[3]{};
		Sound CanDrop{};
		Sound PeopleHit{};

		Sound CartCrash{};
		Sound Explode{};

		Sound CountDownReady{};
		Sound CountDownGo{};
		Sound AddTime{};
		Sound GameOver{};
		Sound HitCheek{};

		Sound GameEnd[3]{};
		Sound Rep{};
		Sound RepTotal{};

		Sound PlayBgm[3]{};
		Sound TitleBgm{};
		Sound CreditBGM{};
		Sound ShutDown{};

		Sound MenuSelect{};
	};

	// Declare the sound channel resource here.
	struct SDK_SOUND_CHANNEL_RESOURCE {
		SoundChannel BGM{};
		SoundChannel SFX{};
	};

	// Data Format list
	// Write your data format here.
	// {  Data Type, Category Name, Data Name, Digit Value, String Value }
	struct SDK_FORMAT_RESOURCE {
		DataFormat HighScore = {
		{DATA_TYPE_DIGIT, "HighScore", "Score", 0, ""},
		{DATA_TYPE_DIGIT, "HighScore", "Rep", 0, ""},
		{DATA_TYPE_DIGIT, "TutorialNeed", "Bool", 1, ""}
		};

		DataFormat UserSettings = {
			{DATA_TYPE_DIGIT, "Setting", "Fullscreen", 1, ""},
			{DATA_TYPE_DIGIT, "Setting", "BGMVolume", 1.0, ""},
			{DATA_TYPE_DIGIT, "Setting", "SFXVolume", 1.0, ""}
		};
	};

	// Decalre the file resource here.
	struct SDK_FILE_RESOURCE {
		Data HighscoreData{};
		Data UserSettingData{};
	};

	// Declare the global value here.
	struct SDK_GLOBAL_RESOURCE {
		// 이전 캐릭터 프레임 및 캐릭터 프레임 사용 가능 여부
		int PrevChFrame[10]{};
		bool CreateAvailable[10]{ true, true, true, true, true, true, true, true, true, true };

		// 타이틀 인트로 재생 여부
		bool TitleIntroPlayed{};

		// 게임 오버 상태
		bool GameOver{};

		// 점수
		int Score{};

		// 파괴하거나 때린 숫자
		int DestroyedItems[6]{};

		// 게임오버 엔딩
		int Ending{};

		// 유저 설정
		float BGMVolume{};
		float SFXVolume{};

		// 전체화면 여부
		bool FullscreenAcvivated{};

		// 유저 하이스코어
		int HighScore{};

		//  유저 최대 배상금
		int MaxRep{};

		// 이전 최고 기록 달성 여부
		bool IsHighScore{};
		bool IsHighRep{};

		// 튜토리얼 열람 여부
		bool NeedTutorial{};
	};

	// Declare the global font name here.
	struct SDK_FONT_NAME_RESOURCE {
		FontName Main = L"픽셀로보로보";
	};

	extern SDK_IMAGE_RESOURCE IMAGE;
	extern SDK_SOUND_RESOURCE SOUND;
	extern SDK_SOUND_CHANNEL_RESOURCE CHANNEL;
	extern SDK_GLOBAL_RESOURCE GLOBAL;
	extern SDK_FORMAT_RESOURCE FORMAT;
	extern SDK_FILE_RESOURCE FILE;
	extern SDK_FONT_NAME_RESOURCE FONTNAME;
}

DWORD WINAPI ImageResourceLoader(LPVOID lpParam);
DWORD WINAPI SoundResourceLoader(LPVOID lpParam);
DWORD WINAPI FileResourceLoader(LPVOID lpParam);
DWORD WINAPI FontResourceLoader(LPVOID lpParam);

///////////////////////////////
// 물건 구조체
typedef struct {
	// 커피 종류
	int Type;

	// 커피 위치
	SDK::Vector2 Position;

	// 위 아래 위치 여부
	bool IsUpside;

	// 파괴 상태
	bool Destroyed;

	// 앞에 사람이 서있는지 여부
	bool IsPeopleFront;

	// 시온이 막아설 위치인지 여부
	bool IsXionFront;
}ItemStruct;