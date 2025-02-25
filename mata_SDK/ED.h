#pragma once
#include <SDK_Scene.h>
#include "Cover.h"

class ED : public SDK::Object {
private:
	// 현재 위치, 커피를 부술 때 마다 오른쪽으로 이동한다.
	float Position{};

	// 커피를 부순 후 목표 이동 위치
	float DestPosition{};

	// 카메라 위치
	float EDCameraPosition{};
	float EDCameraHeight{};

	// 인트로 카메라 위치
	SDK::Vector2 EDIntroCameraPosition{-1.95, 0.4};
	float EDIntroCameraZoom{1.0};

	SDK::SinMove IntroPositionMove{};
	SDK::SinMove IntroZoomMove{};

	// 카메라 줌 
	float EDCameraZoom{1.0};

	// 카메라 오프셋
	float CameraOffset{ 0.95 };

	// 각 상태마다 다른 프레임을 출력한다
	int Frame = ED_Idle;

	float PrevGeneratedFrame{};

	// 현재 프레임과 이전 프레임
	int PrevFrame = Frame;

	// 앞뒤로 늘어나는 효과를 준다
	float AnimationSize{};

	// 앞으로 기울어지는 효과를 준다
	float TiltValue{};

	// 이드 숨쉬기 애니메이션
	float BreatheSize{};
	SDK::SinLoop BreatheLoop{};

	// 수평회전값
	float HRotation{};

	// 상태가 변경되면 일정 시간 이후 다시 Idle 상태로 북귀하도록 한다
	SDK::Timer StateTimer{};

	// 떨림 애니메이션
	glm::vec2 ShakeValue{};
	SDK::Timer ShakeTimer{};

	// 게임오버 지연 타이머
	SDK::Timer GameOverTimer{};

	// 이드 사운드 채널
	SDK::SoundChannel SndChannel{};
	SDK::SoundChannel SndChannel2{};

	// 이드가 커피를 부순 횟수
	int BreakCount{};

	// 커피를 부숴야하는 개수, 초기값 7개
	int MaxBreak{ 7 };

	// 다음 선반의 시작 지점
	float NextPosition{};

	// 키가 눌린 상태
	// 하나라도 눌린 키가 있으면 조작할 수 없다
	bool KeyPressed[4]{};

	// 시온을 뒤로 미는 상태
	bool PushState{};

	// 뒤로 미는 상태 카메라 위치
	float PushCameraOffset{};

	// 조작키 입력 가능한 상태, 게임오버 되면 비활성화 되어 더 이상 키를 입력할 수 없다
	bool InputAvailable{ false };

	// 게임오버 사운드 재생 여부
	bool SoundPlayed{};

	// 이드가 부순 총 커피 개수, 개수에 따라 커피 얼룩의 정도가 달라진다
	int TotalBreakCount{};

	// 커피 얼룩 레벨, 레벨이 높을 수록 더 얼룩진다
	int CoffeeStatinLevel{};

public:
	ED() {
		EDCameraPosition = DestPosition + CameraOffset;
		SDK::CameraControl.Move(EDCameraPosition + EDIntroCameraPosition.x, EDIntroCameraPosition.y);
		SDK::CameraControl.SetZoom(EDCameraZoom + EDIntroCameraZoom);

		IntroPositionMove.SetMovePoint(EDIntroCameraPosition, SDK::Vector2(0.0, 0.0));
		IntroZoomMove.SetMovePoint(EDIntroCameraZoom, 0.0);

		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);
		SDK::SoundTool.SetVolume(SndChannel2, SDK::GLOBAL.SFXVolume);
	}

	void InputKey(SDK::KeyEvent& Event) {
		// 비활성화 상태에서는 입력을 받지 않는다.
		if (!InputAvailable)
			return;

		if (Event.Type != WM_KEYDOWN)
			return;

		if (Event.Key == VK_LEFT || Event.Key == VK_RIGHT || Event.Key == VK_DOWN || Event.Key == VK_UP || Event.Key == VK_SPACE) {
			// 가장 앞에 있는 커피를 부순다.
			if (auto Shelf = SDK::Scene.Find("shelf"); Shelf) {
				ItemStruct Item = Shelf->GetFrontCoffee();

				// 앞에 사람이 서있다면 커피와 상호작용 할 수 없다.
				if (Item.IsPeopleFront)
					InteractWithPeople(Event, Shelf, Item);

				// 시온이 서있다면 커피와 상호작용 할 수 없지만 시온과 상호작용 할 수 있다.
				else if (Item.IsXionFront)
					InteractWithXion(Event, Shelf, Item);
				
				// 사람 또는 시온이 자리에 없을 경우
				else 
					InteractWithCoffee(Event, Shelf, Item);
			}
		}
	}

	void InteractWithCoffee(SDK::KeyEvent& Event, SDK::Object* Shelf, ItemStruct& Item) {
		if (Event.Key == VK_UP || Event.Key == VK_SPACE)
			return;

		if (PushState)
			PushState = false;

		StateTimer.Reset();
		AnimationSize = 1.0;

		bool IsCorrect{};

		if (Item.Type == Coffee_Box && Event.Key == VK_DOWN)
			IsCorrect = true;
		else if (Item.Type == Coffee_Bottle && Event.Key == VK_RIGHT)
			IsCorrect = true;
		else if (Item.Type == Coffee_Can && Event.Key == VK_LEFT)
			IsCorrect = true;

		// 종류에 맞는 키를 눌러야 부술 수 있다.
		if (IsCorrect) {
			SDK::SoundTool.Play(SDK::SOUND.Whoosh, SndChannel);

			if (Item.IsUpside)
				Frame = SDK::Random.Gen(RANDOM_TYPE_INT, ED_HitHigh1, ED_HitHigh2);
			else
				Frame = SDK::Random.Gen(RANDOM_TYPE_INT, ED_HitLow1, ED_HitLow2);

			DestPosition += 0.5;

			if ((BreakCount < MaxBreak - 1)) {
				ItemStruct NextItem = Shelf->GetNextCoffee();

				// 다음 자리에 사람이 있다면 사람 강조 표시를 활성화 한다
				if (NextItem.IsPeopleFront) {
					if (auto People = SDK::Scene.Find("people"); People)
						People->EnableBack();
				}

				// 다음 자리에 시온이 있다면 시온 강조 표시를 활성화 한다
				else if (NextItem.IsXionFront) {
					if (auto Xion = SDK::Scene.Find("xion"); Xion)
						Xion->EnableBack();
				}
			}

			Shelf->BreakCoffee();

			BreakCount++;

			// 캔 커피 또는 뱡 커피를 부술 때 얼룩 레벨이 증가한다
			if (Item.Type == Coffee_Can || Coffee_Bottle)
				TotalBreakCount++;

			// 50개를 부술 때마다 커피 얼룩 레벨이 증가한다
			if (TotalBreakCount >= 50) {
				CoffeeStatinLevel++;
				SDK::EXTool.ClampValue(CoffeeStatinLevel, 5, CLAMP_GREATER);
				TotalBreakCount = 0;
			}


			// 커피를 다 부수면 다음 선반으로 이동한다
			// 부숴야할 커피는 4개 증가한다
			if (BreakCount == MaxBreak) {
				DestPosition = NextPosition;
				BreakCount = 0;
				MaxBreak += 4;

				// 하나의 선반을 지날 때마다 10초의 추가 시간을 얻는다.
				if (auto TimeWatch = SDK::Scene.Find("time_watch"); TimeWatch)
					TimeWatch->AddTime();

				if (auto Score = SDK::Scene.Find("score_indicator"); Score)
					Score->AddScore(200);

				TiltValue = 3.0;

				SDK::SoundTool.Play(SDK::SOUND.NextWhoosh, SndChannel2);
			}
		}

		// 맞는 키가 아닐 경우 엉뚱한 곳을 친다.
		else {
			SDK::SoundTool.Play(SDK::SOUND.MissWhoosh, SndChannel);

			if (Item.IsUpside)
				Frame = SDK::Random.Gen(RANDOM_TYPE_INT, ED_HitLow1, ED_HitLow2);
			else
				Frame = SDK::Random.Gen(RANDOM_TYPE_INT, ED_HitHigh1, ED_HitHigh2);

			if (auto TimeWatch = SDK::Scene.Find("time_watch"); TimeWatch)
				TimeWatch->DeleteTime();
		}
	}

	void InteractWithPeople(SDK::KeyEvent& Event, SDK::Object* Shelf, ItemStruct& Item) {
		if (Event.Key == VK_SPACE) {
			if (auto People = SDK::Scene.Find("people"); People) {
				SDK::SoundTool.Play(SDK::SOUND.Whoosh, SndChannel);

				StateTimer.Reset();
				AnimationSize = 2.0;

				People->HitPeople();
				Frame = ED_KickPeople;
			}
		}
	}

	void InteractWithXion(SDK::KeyEvent& Event, SDK::Object* Shelf, ItemStruct& Item) {
		// 아무키나 누르면 시온을 때린다
		if (auto Xion = SDK::Scene.Find("xion"); Xion) {
			if (Event.Key != VK_UP) {
				Xion->HitPeople();
				InputAvailable = false;

				StateTimer.Reset();
				AnimationSize = 2.0;

				if (Item.IsUpside)
					Frame = SDK::Random.Gen(RANDOM_TYPE_INT, ED_HitLow1, ED_HitLow2);
				else
					Frame = SDK::Random.Gen(RANDOM_TYPE_INT, ED_HitHigh1, ED_HitHigh2);
			}
			else {
				StateTimer.Reset();
				PushState = true;
				AnimationSize = 1.0;

				// 시온을 뒤로 민다.
				Xion->PushPeople();

				if (auto Score = SDK::Scene.Find("score_indicator"); Score)
					Score->AddScore(150);

				Frame = ED_HitHigh2;
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		// 프레임
		// 이전 프레임과 현재 프레임이 다를 경우 이전 프레임을 갱신하고 애니메이션 출력하도록 한다
		if (!SDK::GLOBAL.GameOver) {
			UpdateFrame(FrameTime);
			UpdatePush(FrameTime);
			UpdateAnimation(FrameTime);
			UpdateCamera(FrameTime);
		}

		else
			UpdateGameOverAnimation(FrameTime);
	}

	void UpdateFrame(float FrameTime) {
		if (PrevFrame != Frame)
			PrevFrame = Frame;

		// 현재 상태가 Idle이 아니라면 0.3초 후 다시 Idle 상태로 복귀시킨다
		if (Frame != ED_Idle) {
			StateTimer.Update(FrameTime);
			if (StateTimer.CheckMiliSec(0.3, 2, CHECK_AND_RESET)) {
				Frame = ED_Idle;
				// 시온 밀기 상태 해제
				PushState = false;
			}
		}
	}

	void UpdatePush(float FrameTime) {
		if (PushState) {
			SDK::Math.Lerp(HRotation, 180.0, 15.0, FrameTime);
			SDK::Math.Lerp(PushCameraOffset, -0.8, 10.0, FrameTime);
		}
		else {
			SDK::Math.Lerp(HRotation, 0.0, 15.0, FrameTime);
			SDK::Math.Lerp(PushCameraOffset, 0.0, 10.0, FrameTime);
		}
	}

	void UpdateAnimation(float FrameTime) {
		// 애니메이션
		// AnimationSize가 0.0보다 작다면 다시 0.0으로 복귀시킨다
		SDK::Math.Lerp(AnimationSize, 0.0, 15.0, FrameTime);

		// TiltValue가 0.0보다 크다면 다시 0.0으로 복귀시킨다
		SDK::Math.Lerp(TiltValue, 0.0, 5.0, FrameTime);

		// 이드의 숨쉬기 애니메이션을 업데이트 한다
		BreatheLoop.Update(BreatheSize, 0.03, 6.0, FrameTime);
	}

	void UpdateCamera(float FrameTime) {
		// 이동
		// 목표 위치로 이동하도록 한다
		SDK::Math.Lerp(Position, DestPosition, 20.0, FrameTime);

		// 게임 시작 시 인트로 연출
		IntroPositionMove.Update(EDIntroCameraPosition, 1.2, FrameTime);
		IntroZoomMove.Update(EDIntroCameraZoom, 1.2, FrameTime);

		// 카메라가 이드를 부드럽게 따라오도록 한다
		SDK::Math.Lerp(EDCameraPosition, DestPosition + CameraOffset, 7.0, FrameTime);

		// 이드를 약간 오른쪽에서 바라보도록 한다
		SDK::CameraControl.Move(EDCameraPosition + PushCameraOffset + EDIntroCameraPosition.x, EDIntroCameraPosition.y);
		SDK::CameraControl.SetZoom(1.0 + EDIntroCameraZoom);
	}

	void UpdateGameOverAnimation(float FrameTime) {
		// 입력 가능 상태 비활성화
		InputAvailable = false;

		// 이드에 포커싱 한다.
		SDK::Math.Lerp(EDCameraPosition, Position, 5.0, FrameTime);
		SDK::Math.Lerp(EDCameraHeight, 0.4, 5.0, FrameTime);
		SDK::CameraControl.Move(EDCameraPosition, EDCameraHeight);

		if (SDK::GLOBAL.Score > 0) {
			TiltValue = 0.0;
			AnimationSize = 0.0;
			BreatheSize = 0.0;

			Frame = ED_GameOver;

			ShakeTimer.Update(FrameTime);
			if (ShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
				ShakeValue.x = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShakeValue.y = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			}

			SDK::Math.Lerp(EDCameraZoom, 1.8, 5.0, FrameTime);
			SDK::CameraControl.SetZoom(EDCameraZoom);
		}

		// 점수가 0점일 경우 프레임에 변화를 주지 않는다.
		else {
			TiltValue = 0.0;
			AnimationSize = 0.0;

			Frame = ED_Idle;

			// 이드의 숨쉬기 애니메이션을 업데이트 한다
			BreatheLoop.Update(BreatheSize, 0.03, 6.0, FrameTime);
		}
	}

	void RenderFunc() {
		SDK::Vector2 FinalPosition{ Position + AnimationSize * 0.5 + TiltValue * 0.5 + ShakeValue.x , BreatheSize * 0.5 - AnimationSize * 0.25 + ShakeValue.y };
		SDK::Vector2 FinalSize{ 2.0 + AnimationSize, 2.0 + BreatheSize - AnimationSize * 0.5 };
		
		// 그림자
		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, Position - 0.1 + AnimationSize * 0.5, -0.8);
		SDK::Transform.Scale(SDK::MoveMatrix, 1.0 + AnimationSize, 1.0);
		SDK::ImageTool.RenderImage(SDK::IMAGE.Shadow);

		Begin();
		SDK::Transform.Move(SDK::MoveMatrix, FinalPosition);
		SDK::Transform.Scale(SDK::MoveMatrix, FinalSize);
		SDK::Transform.RotateH(SDK::MoveMatrix, HRotation);
		SDK::Transform.Tilt(SDK::MoveMatrix, TiltValue, 0.0);
		SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.ED, Frame + CoffeeStatinLevel * 9);
	}

	float GetPosition() {
		return Position;
	}

	// 다음 선반의 시작 지점을 미리 알린다
	void TellNextPosition(float Position) {
		NextPosition = Position;
	}

	// 조작키 입력 비활성화
	void DisableInput() {
		InputAvailable = false;
	}

	// 조작키 입력 활성화
	void EnableInput() {
		InputAvailable = true;
	}
};