#include "TitleMode.h"
//////////////////////////////////// Start Mode Option
// Specifies the mode to run first when the program starts.
//You must include the mode header file, and declare the mode class.

TitleMode Title;

// Map modes' Start() function pointer here
void ModeAttribute() {
	Title.Map(SDK::MODE.Title);

	// Set start mode
	SDK::START_MODE = SDK::MODE.Title;
}