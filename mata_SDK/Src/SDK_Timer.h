#pragma once
#include "SDK_Header.h"
#undef GetCurrentTime

namespace SDK {
	class Timer {
	private:
		float Time{};
		bool  StopState{};

	public:
		void Update(float FrameTime);
		void Stop();
		void Resume();
		void Reset();
		int Sec();
		float MiliSec(int DemicalPlace = 1);
		void Interpolate(float Value);
		bool UpdateAndCheckSec(int DestSec, int CheckOption, float FrameTime);
		bool UpdateAndCheckMiliSec(float DestTime, int DemicalPlace, int CheckOption, float FrameTime);
		bool CheckSec(int DestTime, int CheckOption);
		bool CheckMiliSec(float DestTime, int DemicalPlace, int CheckOption);
		float GetCurrentTime();
	};



	class Metronome {
	private:
		float BeatInterval{};
		float Time{};
		bool  StopState{};
		bool  BeatState{};

	public:
		void SetBPM(int Value);
		void Update(float FrameTime);
		void Stop();
		void Resume();
		void Reset();
		bool IsBeat();
	};
}