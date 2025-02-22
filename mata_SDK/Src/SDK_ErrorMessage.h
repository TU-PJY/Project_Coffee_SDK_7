#pragma once
#include "SDK_Object.h"

class SDK_ErrorMessage : public SDK::Object {
private:
	SDK::Text Message{};
	std::string Result{};

	std::string ErrorValue1{};
	std::string ErrorValue2{};
	int ErrorType{};

	SDK::RectBrush Rect{};
	SDK::LineRectBrush LineRect{};
	float HeightOffset = 0.1;

	bool InitState{};

public:
	SDK_ErrorMessage(int Type, std::string Value) {
		ErrorValue1 = Value;
		ErrorType = Type;
	}

	SDK_ErrorMessage(int Type, std::string Value1, std::string Value2) {
		ErrorValue1 = Value1;
		ErrorValue2 = Value2;
		ErrorType = Type;
	}

	void RenderFunc() {
		if (!InitState)
			InitErrorMessage();

		else {
			Rect.SetColorRGB(31, 31, 31);
			Rect.Draw(0.0, 0.0, SDK::ASP(2.0), 2.0);

			Rect.SetColorRGB(45, 45, 45);
			Rect.Draw(0.0, HeightOffset, 0.8, 0.2, -45.0f);
			Rect.Draw(0.0, HeightOffset, 0.8, 0.2, 45.0f);

			LineRect.Draw(0.0, HeightOffset, 1.5, 1.0, 0.1);
			Rect.Draw(0.0, -0.65 + HeightOffset, 0.4, 0.3);
			Rect.Draw(0.0, -0.75 + HeightOffset, 0.7, 0.1);

			Message.RenderString(SDK::ASP(-1.0) + 0.01, 1.0 - 0.06, 0.06, Result);
			Message.Render(SDK::ASP(-1.0) + 0.01, -1.0 + 0.02, 0.06, L"Press ESC or Enter to terminate system...");
		}
	}


	void InitErrorMessage() {
		Rect.SetRenderType(RENDER_TYPE_STATIC);
		LineRect.SetRenderType(RENDER_TYPE_STATIC);
		LineRect.SetColorRGB(45, 45, 45);

		Message.Init(SDK::SYSTEM_FONT, FW_DONTCARE);
		Message.SetColor(1.0, 1.0, 1.0);
		Message.SetLineGap(0.01);

		if (ERROR_TYPE_IMAGE_LOAD <= ErrorType && ErrorType <= ERROR_TYPE_DATA_FILE_CATEGORY) {
			PlaySoundW(TEXT("SDKResource\\Sound\\sound-error.wav"), NULL, SND_FILENAME | SND_ASYNC);
			Result = "An error occurred in the mata_SDK system.\n\n";
		}
		else {
			PlaySoundW(TEXT("SDKResource\\Sound\\sound-critical-error.wav"), NULL, SND_FILENAME | SND_ASYNC);
			Result = "A critical error occurred in the mata_SDK system.\n\n";
		}

		

		if (ErrorValue2.empty()) {
			switch (ErrorType) {
			case ERROR_TYPE_IMAGE_LOAD:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_IMAGE_LOAD\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the image file is correct.");
				break;

			case ERROR_TYPE_SOUND_LOAD:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_SOUND_LOAD\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the audio file is correct.");
				break;

			case ERROR_TYPE_FONT_LOAD:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_FONT_LOAD\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the font file is correct.");
				break;

			case ERROR_TYPE_SCRIPT_LOAD:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_SCRIPT_LOAD\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be missing or damaged.\nMake sure the script file is correct.");
				break;

			case ERROR_TYPE_SCRIPT_PARSE:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_SCRIPT_PARSE\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be damaged or have an incorrect structure.\nMake sure the script file is correct.");
				break;

			case ERROR_TYPE_SCRIPT_CATEGORY:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_SCRIPT_CATEGORY\nCategory with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The category may be missed.\nMake sure the script file's category is correct.");
				break;

			case ERROR_TYPE_DATA_FILE_PARSE:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_DATA_FILE_PARSE\nFile with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The file may be damaged or have an incorrect structure.\nMake sure the data file is correct.");
				break;

			case ERROR_TYPE_DATA_FILE_CATEGORY:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_DATA_FILE_CATEGORY\nCategory with problem: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "The category may be missed.\nMake sure the data file's category is correct.");
				break;

			case ERROR_TYPE_EXECUTED_MODE_EXECUTION:
				SDK::StringTool.AddString(Result, "Error Type: ERROR_TYPE_EXECUTED_MODE_EXECUTION\nWhile running mode: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "You have attempted to execute a mode that has already been executed.\nCheck which part overlaps with the currently running mode.");
				break;

			case ERROR_TYPE_EXECUTED_FLOATING_MODE_EXECUTUION:
				SDK::StringTool.AddString(Result, "Error Type: ERROR_TYPE_EXECUTED_FLOATING_MODE_EXECUTUION\nWhile running floating mode: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "You have attempted to execute a floating mode that has already been executed.\nCheck which part overlaps with the currently running floating mode.");
				break;

			case ERROR_TYPE_TERMINATED_FLOATING_MODE_TERMINATION:
				SDK::StringTool.AddString(Result, "Error Type: ERROR_TYPE_TERMINATED_FLOATING_MODE_TERMINATION\nWhile running mode: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "You have attempted to terminate a floating mode that has already been terminated.\nCheck which part attempts to terminate the floating mode that terminated.");
				break;

			case ERROR_TYPE_OBJECT_ADD_OUT_OF_SCENE_LAYER:
				SDK::StringTool.AddString(Result, "Error Type: ERROR_TYPE_OBJECT_ADD_OUT_OF_SCENE_LAYER\nWhile running mode: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "You attempted to add an object at a location that exceeds the maximum layers.\nCheck which part adds the object in the wrong location.");
				break;

			case ERROR_TYPE_OBJECT_SWAP_OUT_OF_SCENE_LAYER:
				SDK::StringTool.AddString(Result, "Error Type: ERROR_TYPE_OBJECT_SWAP_OUT_OF_SCENE_LAYER\nWhile running mode: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "You attempted to swap an object's layer to a location that exceeds the maximum layers.\nCheck which part swaps the object's layer to the wrong location.");
				break;

			case ERROR_TYPE_OBJECT_FIND_OUT_OF_SCENE_LAYER:
				SDK::StringTool.AddString(Result, "Error Type: ERROR_TYPE_OBJECT_FIND_OUT_OF_SCENE_LAYER\nWhile running mode: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "You attempted to find an object at a location that exceeds the maximum layers.\nCheck which part finds the object in the wrong location.");
				break;

			case ERROR_TYPE_RETRIEVE_LAYER_SIZE_OUT_OF_SCENE_LAYER:
				SDK::StringTool.AddString(Result, "Error Type: ERROR_TYPE_RETRIEVE_LAYER_SIZE_OUT_OF_SCENE_LAYER\nWhile running mode: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "You attempted to retrieve the number of objects at a location that exceeds the maximum layers.\nCheck which part retrieves the number of objects in a location that exceeds the maximum layer.");
				break;

			case ERROR_TYPE_PERMISSION_VIOLATION_SYSTEM_LAYER_ACCESS:
				SDK::StringTool.AddString(Result, "Error Type: ERROR_TYPE_PERMISSION_VIOLATION_SYSTEM_LAYER_ACCESS\nWhile running mode: ");
				SDK::StringTool.AddString(Result, ErrorValue1 + "\n\n");
				SDK::StringTool.AddString(Result, "You do not have permission to access the system layer.\nTo ensure safe system operation, access to the system interior is prohibited.");
				break;
			}
		}

		else {
			switch (ErrorType) {
			case ERROR_TYPE_SCRIPT_DATA:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_SCRIPT_DATA\nData with problem: ");
				SDK::StringTool.AddString(Result, "Category: " + ErrorValue1 + " | Data Name: " + ErrorValue2 + "\n\n");
				SDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.");
				break;

			case ERROR_TYPE_DATA_FILE_DATA_LOAD:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_DATA_FILE_DATA_LOAD\nData with problem: ");
				SDK::StringTool.AddString(Result, "Category: " + ErrorValue1 + " | Data Name: " + ErrorValue2 + "\n\n");
				SDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.");
				break;

			case ERROR_TYPE_DATA_FILE_DATA_WRITE:
				SDK::StringTool.AddString(Result, "Error Type : ERROR_TYPE_DATA_FILE_DATA_WRITE\nData with problem: ");
				SDK::StringTool.AddString(Result, "Category: " + ErrorValue1 + " | Data Name: " + ErrorValue2 + "\n\n");
				SDK::StringTool.AddString(Result, "Data may be missing.\nMake sure the category's data is correct.");
				break;
			}
		}

		InitState = true;
	}
};