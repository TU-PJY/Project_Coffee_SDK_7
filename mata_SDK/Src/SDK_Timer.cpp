#include "SDK_Timer.h"
#include <cmath>

void SDK::Timer::Update(float FrameTime) {
	if(!StopState)
		Time += FrameTime;
}

void SDK::Timer::Stop() {
	StopState = true;
}

void SDK::Timer::Resume() {
	StopState = false;
}

void SDK::Timer::Reset() {
	Time = 0.0;
}

float SDK::Timer::GetCurrentTime() {
	return Time;
}

int SDK::Timer::Sec() {
	return (int)Time;
}

float SDK::Timer::MiliSec(int DemicalPlace) {
	float TruncateValue = pow(10, DemicalPlace);
	float TruncatedNum = std::floor(Time * TruncateValue) / TruncateValue;
	return TruncatedNum;
}

void SDK::Timer::Interpolate(float Value) {
	float OverTime = Time - Value;
	Time = OverTime;
}

bool SDK::Timer::UpdateAndCheckSec(int DestTime, int CheckOption, float FrameTime) {
	Time += FrameTime;

	if (Sec() >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			break;

		case CHECK_AND_RESET:
			Reset();
			break;

		case CHECK_AND_INTERPOLATE:
			Interpolate((float)DestTime);
			break;

		case CHECK_AND_STOP:
			Stop();
			break;
		}

		return true;
	}

	return false;
}

bool SDK::Timer::UpdateAndCheckMiliSec(int DestTime, int DemicalPlace, int CheckOption, float FrameTime) {
	Time += FrameTime;

	if (MiliSec(DemicalPlace) >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			break;

		case CHECK_AND_RESET:
			Reset();
			break;

		case CHECK_AND_INTERPOLATE:
			Interpolate(DestTime);
			break;

		case CHECK_AND_STOP:
			Stop();
			break;
		}

		return true;
	}

	return false;
}

bool SDK::Timer::CheckSec(int DestTime, int CheckOption) {
	if (Sec() >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			return true;

		case CHECK_AND_RESET:
			Reset();
			return true;

		case CHECK_AND_INTERPOLATE:
			Interpolate((float)DestTime);
			return true;

		case CHECK_AND_STOP:
			Stop();
			return true;
		}
	}

	return false;
}

bool SDK::Timer::CheckMiliSec(float DestTime, int DemicalPlace, int CheckOption) {
	if (MiliSec(DemicalPlace) >= DestTime) {
		switch (CheckOption) {
		case CHECK_AND_RESUME:
			return true;

		case CHECK_AND_RESET:
			Reset();
			return true;

		case CHECK_AND_INTERPOLATE:
			Interpolate(DestTime);
			return true;

		case CHECK_AND_STOP:
			Stop();
			return true;
		}
	}
	return false;
}


void SDK::Metronome::SetBPM(int BPM) {
	BeatInterval = 60.0 / (float)BPM;
}

void SDK::Metronome::Update(float FrameTime) {
	if (!StopState) {
		Time += FrameTime;

		if (Time >= BeatInterval) {
			float OverTime = Time - BeatInterval;
			Time = OverTime;
			BeatState = true;
		}

		else
			BeatState = false;
	}
}

void SDK::Metronome::Stop() {
	StopState = true;
	BeatState = false;
}

void SDK::Metronome::Resume() {
	StopState = false;
}

void SDK::Metronome::Reset() {
	Time = 0.0;
}

bool SDK::Metronome::IsBeat() {
	return BeatState;
}