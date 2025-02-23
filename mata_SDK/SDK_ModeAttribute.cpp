#include "TitleMode.h"
#include "PlayMode.h"
#include "GameOverMode.h"
#include "PauseMode.h"
//////////////////////////////////// Start Mode Option
// Specifies the mode to run first when the program starts.
//You must include the mode header file, and declare the mode class.

TitleMode Title;
PlayMode Play;
GameOverMode GameOver;
PauseMode Pause;

// Map modes' Start() function pointer here
void ModeAttribute() {
	Title.Map(SDK::MODE.Title);
	Play.Map(SDK::MODE.Play);
	GameOver.Map(SDK::MODE.GameOver);
	Pause.Map(SDK::MODE.Pause);

	// Set start mode
	SDK::START_MODE = SDK::MODE.Title;
}