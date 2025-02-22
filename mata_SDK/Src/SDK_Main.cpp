#include "SDK_Header.h"
#include "SDK_CameraController.h"
#include "SDK_FPSIndicator.h"
#include "SDK_Scene.h"
#include "SDK_Frustum.h"
#include <thread>
#include <chrono>

SDK::SDK_System SDK::System;
SDK::SDK_System* SDK::SDK_System::S_Inst;

void SDK::SDK_System::Main() {
	glClearColor(SDK::ViewportColor.r, SDK::ViewportColor.g, SDK::ViewportColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (S_Inst->UpdateActivateCommand) {
		SDK::Scene.Update();
		SDK::CameraControl.Update(S_Inst->DeltaTime);
		SDK::Frustum.Update();
		SDK::SoundTool.Update();
		SDK::Scene.Render();
		SDK::Scene.CompleteCommand();
	}

	if (S_Inst->FPSLimit > 0) {
		std::chrono::duration<double, std::milli> Duration(S_Inst->DestFPS);
		std::this_thread::sleep_for(Duration);
	}

	S_Inst->CurrentTime = float(glutGet(GLUT_ELAPSED_TIME));
	S_Inst->DeltaTime = (S_Inst->CurrentTime - S_Inst->PrevTime) / 1000.0;
	SDK::Scene.InputFrameTime(S_Inst->DeltaTime);

	S_Inst->PrevTime = S_Inst->CurrentTime;

	glutSwapBuffers();
	glutPostRedisplay();

	S_Inst->UpdateActivateCommand = true;
}

void main(int argc, char** argv) {
	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale());
	std::cout.imbue(std::locale());

	if (SHOW_CONSOLE) {
		if (AllocConsole()) {
			FILE* FP{};

			freopen_s(&FP, "CONOUT$", "w", stdout);
			freopen_s(&FP, "CONIN$", "r", stdin);
			freopen_s(&FP, "CONERR$", "w", stderr);

			std::cout << "Console initialized successfully." << std::endl;
		}
	}

	wchar_t LocaleName[LOCALE_NAME_MAX_LENGTH];
	if (GetUserDefaultLocaleName(LocaleName, LOCALE_NAME_MAX_LENGTH)) {
		SDK::LOCALE = LocaleName;
		std::wcout << L"Windows System Locale: " << SDK::LOCALE << std::endl;
	}
		
	SDK::System.SetupSystem(argc, argv);
	glutDisplayFunc(SDK::System.Main);
	glutReshapeFunc(SDK::System.DisplayReshape);
	glutMainLoop();
}

void SDK::SDK_System::Exit() {
	glutDestroyWindow(1);
}

float SDK::ASP(float Value) {
	return Value * SDK::ASPECT;
}