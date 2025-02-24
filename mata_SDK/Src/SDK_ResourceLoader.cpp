#include "SDK_Resource.h"


// Use LoadImageT(), LoadClipT(), and LoadSpriteSheetT() to load image files and sprite sheets in this thread.
DWORD WINAPI ImageResourceLoader(LPVOID lpParam) {
	SDK::ImageTool.SetSpriteSheetSize(256, 256, 9, 6);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.ED, "Assets//Sprite//People//sheet-ed.png");

	SDK::ImageTool.SetSpriteSheetSize(256, 256, 7, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.Xion, "Assets//Sprite//People//sheet-xion.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.XionBack, "Assets//Sprite//People//xion-back.png");

	SDK::ImageTool.SetSpriteSheetSize(256, 256, 2, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.ED_GameOver, "Assets//Sprite//People//sheet-ed-gameover.png");
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.Chloe, "Assets//Sprite//People//sheet-chloe.png");

	SDK::ImageTool.SetSpriteSheetSize(256, 256, 10, 2);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.People, "Assets//Sprite//People//sheet-people.png");
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.PeopleBack, "Assets//Sprite//People//sheet-people-back.png");

	SDK::ImageTool.LoadImageT(SDK::IMAGE.Shadow, "Assets//Sprite//People//shadow.png");

	SDK::ImageTool.SetSpriteSheetSize(256, 256, 4, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.CreditED, "Assets//Sprite//People//sheet-ed-credit.png");

	SDK::ImageTool.SetSpriteSheetSize(256, 256, 2, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.CreditXion, "Assets//Sprite//People//sheet-xion-credit.png");

	SDK::ImageTool.SetSpriteSheetSize(256, 256, 4, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.CreditEDBack, "Assets//Sprite//People//sheet-ed-credit-back.png");

	SDK::ImageTool.SetSpriteSheetSize(256, 256, 2, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.CreditXionBack, "Assets//Sprite//People//sheet-xion-credit-back.png");

	SDK::ImageTool.SetSpriteSheetSize(256, 256, 3, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.Shelf, "Assets//Sprite//Object//sheet-shelf.png");

	SDK::ImageTool.SetSpriteSheetSize(128, 128, 3, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.Coffee, "Assets//Sprite//Object//sheet-coffee.png");
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.CoffeeBack, "Assets//Sprite//Object//sheet-coffee-back.png");
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.DestroyedCoffee, "Assets//Sprite//Object//sheet-destroyed.png");
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.Other, "Assets//Sprite//Object//sheet-other.png");

	SDK::ImageTool.SetSpriteSheetSize(30, 30, 2, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.Drop, "Assets//Sprite//Object//sheet-drop.png");

	SDK::ImageTool.LoadImageT(SDK::IMAGE.DestroyedCan, "Assets//Sprite//Object//can-destroyed.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.Stick, "Assets//Sprite//Object//stick.png");

	SDK::ImageTool.SetSpriteSheetSize(256, 256, 2, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.Cart, "Assets//Sprite//Object//sheet-cart.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.Smoke, "Assets//Sprite//Object//smoke.png");

	SDK::ImageTool.SetSpriteSheetSize(512, 256, 3, 1);
	SDK::ImageTool.LoadSpriteSheetT(SDK::IMAGE.Sofa, "Assets//Sprite//Object//sheet-sofa.png");

	SDK::ImageTool.LoadImageT(SDK::IMAGE.TV, "Assets///Sprite//Object//TV.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.TVLight, "Assets//Sprite//Object//TV-light.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.Chair, "Assets//Sprite//Object//chair.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.Gun, "Assets//Sprite//Object//gun.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.Machine, "Assets//Sprite//Object//machine.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.Nata, "Assets//Sprite//Object//nata.png");


	SDK::ImageTool.LoadImageT(SDK::IMAGE.FloorTile, "Assets//Sprite//Background//floor.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.GameOverBackGround, "Assets//Sprite//Background//background-bad.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.GameOverBackGround2, "Assets//Sprite//Background//background-good.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.BackGroundShelf, "Assets//Sprite//Background//background-shelf.png");
	SDK::ImageTool.LoadImageT(SDK::IMAGE.Pillar, "Assets//Sprite//Background//pillar.png");

	SDK::ImageTool.LoadImageT(SDK::IMAGE.ZZZ, "Assets//Sprite//Object//z.png");

	SDK::ImageTool.LoadImageT(SDK::IMAGE.Tutorial, "Assets//Sprite//UI//tutorial.png");

	return 0;
}

DWORD WINAPI SoundResourceLoader(LPVOID lpParam) {
	SDK::SoundTool.Load(SDK::SOUND.Whoosh, "Assets//Sound//SFX//whoosh.mp3");
	SDK::SoundTool.Load(SDK::SOUND.MissWhoosh, "Assets//Sound//SFX//whoosh-miss.mp3");
	SDK::SoundTool.Load(SDK::SOUND.NextWhoosh, "Assets//Sound//SFX//next-shelf.mp3");

	for (int i = 0; i < 3; ++i) {
		std::string FileName = "Assets//Sound//SFX//box-hit-";
		FileName += std::to_string(i + 1) + ".mp3";
		SDK::SoundTool.Load(SDK::SOUND.BoxBreak[i], FileName);
	}

	for (int i = 0; i < 3; ++i) {
		std::string FileName = "Assets//Sound//SFX//glass-hit-";
		FileName += std::to_string(i + 1) + ".mp3";
		SDK::SoundTool.Load(SDK::SOUND.Bottle[i], FileName);
	}

	for (int i = 0; i < 3; ++i) {
		std::string FileName = "Assets//Sound//SFX//metal-hit-";
		FileName += std::to_string(i + 1) + ".mp3";
		SDK::SoundTool.Load(SDK::SOUND.CanBreak[i], FileName);
	}
	SDK::SoundTool.Load(SDK::SOUND.CanDrop, "Assets//Sound//SFX//metal-drop.mp3");

	SDK::SoundTool.Load(SDK::SOUND.PeopleHit, "Assets//Sound//SFX//people-hit.mp3");
	SDK::SoundTool.Load(SDK::SOUND.CartCrash, "Assets//Sound//SFX//cart-crash.mp3");
	SDK::SoundTool.Load(SDK::SOUND.Explode, "Assets//Sound//SFX//explode.ogg");

	SDK::SoundTool.Load(SDK::SOUND.CountDownReady, "Assets//Sound//SFX//countdown-ready.mp3");
	SDK::SoundTool.Load(SDK::SOUND.CountDownGo, "Assets//Sound//SFX//countdown-go.mp3");
	SDK::SoundTool.Load(SDK::SOUND.AddTime, "Assets//Sound//SFX//time-add.mp3");

	SDK::SoundTool.Load(SDK::SOUND.HitCheek, "Assets//Sound//SFX//hit-cheek.mp3");
	SDK::SoundTool.Load(SDK::SOUND.Rep, "Assets//Sound//SFX//rep.mp3");
	SDK::SoundTool.Load(SDK::SOUND.RepTotal, "Assets//Sound//SFX//rep-total.mp3");

	SDK::SoundTool.Load(SDK::SOUND.MenuSelect, "Assets//Sound//SFX//menu-select.mp3");

	SDK::SoundTool.Load(SDK::SOUND.ShutDown, "Assets//Sound//SFX//shutdown.mp3");


	SDK::SoundTool.Load(SDK::SOUND.GameOver, "Assets//Sound//BGM//gameover.mp3");
	SDK::SoundTool.Load(SDK::SOUND.GameEnd[0], "Assets//Sound//BGM//gameover-timeout.mp3");
	SDK::SoundTool.Load(SDK::SOUND.GameEnd[1], "Assets//Sound//BGM//gameover-hit-xion.mp3");
	SDK::SoundTool.Load(SDK::SOUND.GameEnd[2], "Assets//Sound//BGM//gameover-suppress.mp3");

	for (int i = 0; i < 3; i++) {
		std::string FileName = "Assets//Sound//BGM//play-bgm-" + std::to_string(i + 1) + ".mp3";
		SDK::SoundTool.Load(SDK::SOUND.PlayBgm[i], FileName, FMOD_LOOP_NORMAL);
	}

	SDK::SoundTool.Load(SDK::SOUND.TitleBgm, "Assets//Sound//BGM//title-bgm.mp3", FMOD_LOOP_NORMAL);
	SDK::SoundTool.Load(SDK::SOUND.CreditBGM, "Assets//Sound//BGM//credit-bgm.mp3", FMOD_LOOP_NORMAL);

	return 0;
}

DWORD WINAPI FileResourceLoader(LPVOID lpParam) {
	SDK::FILE.HighscoreData.Load("GameData//HighScore", SDK::FORMAT.HighScore);
	SDK::FILE.UserSettingData.Load("GameData//UserSettings", SDK::FORMAT.UserSettings);

	SDK::GLOBAL.FullscreenAcvivated = SDK::FILE.UserSettingData.LoadDigitData("Setting", "Fullscreen");
	SDK::GLOBAL.BGMVolume = SDK::FILE.UserSettingData.LoadDigitData("Setting", "BGMVolume");
	SDK::GLOBAL.SFXVolume = SDK::FILE.UserSettingData.LoadDigitData("Setting", "SFXVolume");

	SDK::GLOBAL.HighScore = SDK::FILE.HighscoreData.LoadDigitData("HighScore", "Score");
	SDK::GLOBAL.MaxRep = SDK::FILE.HighscoreData.LoadDigitData("HighScore", "Rep");
	SDK::GLOBAL.NeedTutorial = SDK::FILE.HighscoreData.LoadDigitData("TutorialNeed", "Bool");

	SDK::SoundTool.SetVolume(SDK::CHANNEL.SFX, SDK::GLOBAL.SFXVolume);

	return 0;
}

DWORD WINAPI FontResourceLoader(LPVOID lpParam) {
	SDK::FontLoader.LoadT("Assets//Font//pixelroborobo.otf");

	return 0;
} 