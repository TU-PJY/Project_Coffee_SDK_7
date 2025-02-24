#include "TitleMode.h"
#include "PlayMode.h"
#include "GameOverMode.h"
#include "PauseMode.h"
#include "CreditMode.h"
//////////////////////////////////// Start Mode Option
// Specifies the mode to run first when the program starts.
//You must include the mode header file, and declare the mode class.

TitleMode Title;
PlayMode Play;
GameOverMode GameOver;
PauseMode Pause;
CreditMode Credit;

// Map modes' Start() function pointer here
void ModeAttribute() {
	Title.Map(SDK::MODE.Title);
	Play.Map(SDK::MODE.Play);
	GameOver.Map(SDK::MODE.GameOver);
	Pause.Map(SDK::MODE.Pause);
	Credit.Map(SDK::MODE.Credit);

	// Set start mode
	SDK::START_MODE = SDK::MODE.Credit;
}