#include "SDK_Scene.h"
#include "SDK_SoundTool.h"
#include "SDK_EXTool.h"

SDK::SDK_SoundTool SDK::SoundTool;

void SDK::SDK_SoundTool::Init() {
	Result = FMOD::System_Create(&SoundSystem);

	if (Result != FMOD_OK)	
		exit(EXIT_FAILURE);

	SoundSystem->init(MAX_CHANNEL_SIZE, FMOD_INIT_NORMAL, ExtDvData);
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS, &LowPass);
	SoundSystem->set3DSettings(1.0, 1.0, 2.0); 

	FFTdata.reserve(FFT_SIZE);
}

void SDK::SDK_SoundTool::Load(SDK::Sound& Sound, std::string FileName, FMOD_MODE Option) {
	HRESULT Result = SoundSystem->createSound(FileName.c_str(), Option, 0, &Sound);
	if (Result != FMOD_OK)
		SDK::Scene.SetErrorScreen(ERROR_TYPE_SOUND_LOAD, FileName);
}

void SDK::SDK_SoundTool::Release(SDK::Sound& Sound) {
	Sound->release();
}

void SDK::SDK_SoundTool::Update() {
	SoundSystem->update();
}

void SDK::SDK_SoundTool::SetFastSoundChannel(SDK::FastSoundChannel& ChannelVar, int NumChannel) {
	ChannelVar.NumChannel = NumChannel;
	ChannelVar.Channel.assign(NumChannel, {});
	ChannelVar.PlayChannel = 0;
	ChannelVar.StopChannel = 1;
}

void SDK::SDK_SoundTool::AddChannelToGroup(SDK::SoundChannelGroup& Group, SDK::Sound& Sound, SDK::SoundChannel& ChannelVar) {
	SoundSystem->playSound(Sound, 0, true, &ChannelVar.Channel);
	Group.emplace_back(ChannelVar);
}

void SDK::SDK_SoundTool::DeleteChannelFromGroup(SDK::SoundChannelGroup& Group, SDK::SoundChannel& ChannelVar) {
	for (auto It = begin(Group); It != end(Group);) {
		if (It->Channel == ChannelVar.Channel)
			It = Group.erase(It);
		else
			It++;
	}
}

void SDK::SDK_SoundTool::ClearGroup(SDK::SoundChannelGroup& Group) {
	Group.clear();
}

void SDK::SDK_SoundTool::PlayGroup(SDK::SoundChannelGroup& Group) {
	for (auto& G : Group) {
		G.Channel->setPaused(false);
		G.Channel->setVolume(G.Volume);
		G.Channel->setPitch(G.PlaySpeed);
	}
}

void SDK::SDK_SoundTool::StopGroup(SDK::SoundChannelGroup& Group) {
	for (auto& G : Group) {
		G.Channel->setPaused(true);
		G.Channel->setPosition(0, FMOD_TIMEUNIT_MS);
	}
}

void SDK::SDK_SoundTool::SetGroupVolume(SDK::SoundChannelGroup& Group, float Volume) {
	for (auto& G : Group) 
		G.Channel->setVolume(Volume);
}

void SDK::SDK_SoundTool::SetGroupPlaySpeed(SDK::SoundChannelGroup& Group, float Speed) {
	for (auto& G : Group)
		G.Channel->setPitch(Speed);
}

void SDK::SDK_SoundTool::PauseGroup(SDK::SoundChannelGroup& Group, bool Flag) {
	for (auto& G : Group)
		G.Channel->setPaused(Flag);
}

void SDK::SDK_SoundTool::Play(SDK::Sound& Sound, SDK::SoundChannel& ChannelVar, float Time) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel);
	ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
	ChannelVar.Channel->setPitch(ChannelVar.PlaySpeed);
}

void SDK::SDK_SoundTool::Play(SDK::Sound& Sound, SDK::FastSoundChannel& ChannelVar, float Time) {
	SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel[ChannelVar.PlayChannel]);
	ChannelVar.Channel[ChannelVar.PlayChannel]->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
	ChannelVar.Channel[ChannelVar.PlayChannel]->setVolume(ChannelVar.Volume);
	ChannelVar.Channel[ChannelVar.PlayChannel++]->setPitch(ChannelVar.PlaySpeed);
	ChannelVar.Channel[ChannelVar.StopChannel++]->stop();

	SDK::EXTool.ClampValue(ChannelVar.PlayChannel, 0, ChannelVar.NumChannel, CLAMP_RETURN);
	SDK::EXTool.ClampValue(ChannelVar.StopChannel, 0, ChannelVar.NumChannel, CLAMP_RETURN);
}

void SDK::SDK_SoundTool::PlayOnce(SDK::Sound& Sound, SDK::SoundChannel& ChannelVar, bool& BoolValue, float Time) {
	if (!BoolValue) {
		SoundSystem->playSound(Sound, 0, false, &ChannelVar.Channel);
		ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
		ChannelVar.Channel->setVolume(ChannelVar.Volume);
		ChannelVar.Channel->setPitch(ChannelVar.PlaySpeed);
	}
}

void SDK::SDK_SoundTool::Pause(SDK::SoundChannel& ChannelVar, bool Flag) {
	ChannelVar.Channel->setPaused(Flag);
}

void SDK::SDK_SoundTool::Stop(SDK::SoundChannel& ChannelVar) {
	ChannelVar.Channel->stop();
}

void SDK::SDK_SoundTool::FadeOut(SDK::SoundChannel& ChannelVar, float Speed, float FrameTime) {
	ChannelVar.Volume -= Speed * FrameTime;
	SDK::EXTool.ClampValue(ChannelVar.Volume, 0.0, CLAMP_LESS);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
}

void SDK::SDK_SoundTool::FadeIn(SDK::SoundChannel& ChannelVar, float Speed, float FrameTime, float DestVolume) {
	ChannelVar.Volume += Speed * FrameTime;
	SDK::EXTool.ClampValue(ChannelVar.Volume, DestVolume, CLAMP_GREATER);
	ChannelVar.Channel->setVolume(ChannelVar.Volume);
}

unsigned int SDK::SDK_SoundTool::GetLength(SDK::Sound& Sound) {
	unsigned int Length{};
	Sound->getLength(&Length, FMOD_TIMEUNIT_MS);
	return Length;
}

unsigned int SDK::SDK_SoundTool::GetPlayTime(SDK::SoundChannel& ChannelVar) {
	unsigned int Position{};
	ChannelVar.Channel->getPosition(&Position, FMOD_TIMEUNIT_MS);
	return Position;
}

void SDK::SDK_SoundTool::SetVolume(SDK::SoundChannel& ChannelVar, float Volume) {
	ChannelVar.Volume = Volume;
	ChannelVar.Channel->setVolume(Volume);
}

void SDK::SDK_SoundTool::SetVolume(SDK::FastSoundChannel& ChannelVar, float Volume) {
	ChannelVar.Volume = Volume;
	for(int i = 0; i < ChannelVar.NumChannel; i++)
		ChannelVar.Channel[i]->setVolume(Volume);
}

void SDK::SDK_SoundTool::SetPlayTime(SDK::SoundChannel& ChannelVar, float Time) {
	ChannelVar.Channel->setPosition(Time * 1000, FMOD_TIMEUNIT_MS);
}

void SDK::SDK_SoundTool::SetPlaySpeed(SDK::SoundChannel& ChannelVar, float Speed) {
	ChannelVar.PlaySpeed = Speed;
	ChannelVar.Channel->setPitch(Speed);
}

void SDK::SDK_SoundTool::SetPlaySpeed(SDK::FastSoundChannel& ChannelVar, float Speed) {
	ChannelVar.PlaySpeed = Speed;
	for (int i = 0; i < ChannelVar.NumChannel; i++)
		ChannelVar.Channel[i]->setPitch(Speed);
}

void SDK::SDK_SoundTool::ResetPlaySpeed(SDK::SoundChannel& ChannelVar) {
	ChannelVar.PlaySpeed = 1.0;
	ChannelVar.Channel->setPitch(1.0);
}

void SDK::SDK_SoundTool::ResetPlaySpeed(SDK::FastSoundChannel& ChannelVar) {
	ChannelVar.PlaySpeed = 1.0;
	for (int i = 0; i < ChannelVar.NumChannel; i++)
		ChannelVar.Channel[i]->setPitch(1.0);
}

void SDK::SDK_SoundTool::SetFreqCutOff(SDK::SoundChannel& ChannelVar, float Frequency) {
	LowPass->setParameterFloat(FMOD_DSP_LOWPASS_CUTOFF, Frequency);
	ChannelVar.Channel->addDSP(0, LowPass);
}

void SDK::SDK_SoundTool::SetBeatDetect(SDK::SoundChannel& ChannelVar) {
	SoundSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &BeatDetector);
	ChannelVar.Channel->addDSP(0, BeatDetector);
}

float SDK::SDK_SoundTool::DetectBeat(float Threshold, int SamplingRate) {
	FMOD_DSP_PARAMETER_FFT* FFT = nullptr;
	BeatDetector->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&FFT, 0, 0, 0);

	if (FFT) {
		int NumChannels = FFT->numchannels;

		if (NumChannels > 0) {
			memcpy(FFTdata.data(), FFT->spectrum[0], FFT_SIZE * sizeof(float));

			float BassEnergy = 0.0f;

			for (int i = 0; i < SamplingRate; ++i)
				BassEnergy += FFTdata[i];

			if (BassEnergy > Threshold)
				return BassEnergy;
		}
	}

	return 0;
}

void SDK::SDK_SoundTool::DetectBeat(float& DestValue, float Threshold, int SamplingRate) {
	FMOD_DSP_PARAMETER_FFT* FFT = nullptr;
	BeatDetector->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&FFT, 0, 0, 0);

	if (FFT) {
		int NumChannels = FFT->numchannels;

		if (NumChannels > 0) {
			memcpy(FFTdata.data(), FFT->spectrum[0], FFT_SIZE * sizeof(float));

			float BassEnergy = 0.0f;

			for (int i = 0; i < SamplingRate; ++i)
				BassEnergy += FFTdata[i];

			if (BassEnergy > Threshold)
				DestValue = BassEnergy;
		}
	}

	DestValue = 0.0;
}

bool SDK::SDK_SoundTool::IsBeat(float Threshold, int SamplingRate) {
	FMOD_DSP_PARAMETER_FFT* FFT = nullptr;
	BeatDetector->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&FFT, 0, 0, 0);

	if (FFT) {
		int NumChannels = FFT->numchannels;

		if (NumChannels > 0) {
			memcpy(FFTdata.data(), FFT->spectrum[0], FFT_SIZE * sizeof(float));

			float BassEnergy = 0.0f;

			for (int i = 0; i < SamplingRate; ++i)
				BassEnergy += FFTdata[i];

			if (BassEnergy > Threshold)
				return true;
		}
	}

	return false;
}

void SDK::SDK_SoundTool::DisableBeatDetect(SDK::SoundChannel& ChannelVar) {
	ChannelVar.Channel->removeDSP(BeatDetector);
}

void SDK::SDK_SoundTool::DisableFreqCutOff(SDK::SoundChannel& ChannelVar) {
	ChannelVar.Channel->removeDSP(LowPass);
}

void SDK::SDK_SoundTool::StopAllSounds() {
	FMOD::ChannelGroup* MasterChannelGroup{};

	SoundSystem->getMasterChannelGroup(&MasterChannelGroup);
	Result = MasterChannelGroup->stop();
}

void SDK::SDK_SoundTool::SetDistance(SDK::SoundChannel& ChannelVar, float MinDist, float MaxDist) {
	ChannelVar.Channel->set3DMinMaxDistance(MinDist, MaxDist);
}

void SDK::SDK_SoundTool::SetListnerPosition(float X, float Y) {
	ListenerPosition.x = X;
	ListenerPosition.y = Y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
};

void SDK::SDK_SoundTool::SetListnerPosition(SDK::Vector2& Position) {
	ListenerPosition.x = Position.x;
	ListenerPosition.y = Position.y;
	ListenerPosition.z = 0.0f;

	SoundSystem->set3DListenerAttributes(0, &ListenerPosition, 0, 0, 0);
}

void SDK::SDK_SoundTool::SetSoundPosition(SDK::SoundChannel& ChannelVar, float X, float Y, float Diff) {
	SoundPosition.x = X;
	SoundPosition.y = Y;
	SoundPosition.z = Diff;

	ChannelVar.Channel->set3DAttributes(&SoundPosition, 0);
}

void SDK::SDK_SoundTool::SetSoundPosition(SDK::SoundChannel& ChannelVar, SDK::Vector2& Position, float Diff) {
	SoundPosition.x = Position.x;
	SoundPosition.y = Position.y;
	SoundPosition.z = Diff;

	ChannelVar.Channel->set3DAttributes(&SoundPosition, 0);
}