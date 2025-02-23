#include "Xion.h"

Xion::Xion(float PositionValue, float DestPositionValue, bool BoolMoveState, int FrameValue) {
	Position.x = PositionValue;
	DestPosition = DestPositionValue;
	MoveState = BoolMoveState;
	VerticalLoop.SetValue(SDK::Preset::HalfNegative);
	TiltLoop.SetValue(SDK::Preset::HalfPositive);
	Frame = FrameValue;

	SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);
}

void Xion::UpdateFunc(float FrameTime) {
	// 점수가 0점일 경우 표정에 변화가 생긴다
	if (SDK::GLOBAL.GameOver && SDK::GLOBAL.Score == 0) {
		ShakeValue.x = 0.0;
		ShakeValue.y = 0.0;

		VerticalLoop.Update(VerticalSize, 0.05, 5.0, FrameTime);
		Position.x = -1.3;
		Frame = Xion_Curious;
	}

	// 목표 지점까지 이동 한 후 멈춘다
	if (MoveState && !HitState && !PushState) 
		SDK::Math.Lerp(Position.x, DestPosition, 20.0, FrameTime);

	else if (PushState) {
		SDK::Math.Lerp(Position.x, DestPosition, 8.0, FrameTime);
		SDK::Math.Lerp(HRotation, DestHRotation, 8.0, FrameTime);

		if (HRotation >= 90.0)
			Frame = Xion_Cry2;
	}

	if (Frame == Xion_Blocking) {
		if (!HitState) {
			VerticalLoop.Update(VerticalSize, 0.05, 20.0, FrameTime);
			TiltLoop.Update(TiltValue, 0.1, 20.0, FrameTime);
		}
		else {
			if (!FellDown) {
				VerticalSize = 0.0;
				TiltValue = 0.0;

				Rotation += 250 * FrameTime;
				Position.x += 8.0 * FrameTime;
				Position.y -= 1.0 * FrameTime;

				// 시온이 바닥에 넘어지면 이드의 이성이 돌아오면서 게임오버 된다
				if (Position.y <= -1.3) {
					Position.y = -1.3;
					SDK::SoundTool.Play(SDK::SOUND.CartCrash, SndChannel);
					FellDown = true;
				}
			}

			// 1초 후 게임오버 된다
			else {
				GameOverTimer.Update(FrameTime);
				if (GameOverTimer.Sec() >= 1) {
					if(!SDK::GLOBAL.GameOver)
						SDK::SoundTool.Play(SDK::SOUND.GameOver, SDK::CHANNEL.BGM);
					SDK::GLOBAL.GameOver = true;
				}
			}
		}
	}

	else if (Frame == Xion_Cry1 || Frame == Xion_Cry2) {
		ShakeTimer.Update(FrameTime);
		if (ShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
			ShakeValue.x = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			ShakeValue.y = SDK::Random.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
		}
	}

	else if (Frame == Xion_Curious) {
		ShakeValue.x = 0.0;
		ShakeValue.y = 0.0;
	}
	
	if (Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - 1.0)
		SDK::Scene.DeleteObject(this);
}

void Xion::RenderFunc() {
	if (Position.x > SDK::Camera.Position.x + SDK::ASP(1.0) + 1.0)
		return;

	Begin();
	if(Frame == Xion_Cry1)
		SDK::Transform.Move(SDK::MoveMatrix, Position.x - 0.2, Position.y - 0.8);
	else
		SDK::Transform.Move(SDK::MoveMatrix, Position.x, Position.y - 0.8);
	SDK::Transform.RotateH(SDK::MoveMatrix, HRotation);
	SDK::Transform.Move(SDK::MoveMatrix, 0.1, 0.0);
	SDK::ImageTool.RenderImage(SDK::IMAGE.Shadow);

	Begin();
	SDK::Transform.Move(SDK::MoveMatrix, Position.x + TiltValue * 0.5 + ShakeValue.x, Position.y + VerticalSize * 0.5 + ShakeValue.y);
	SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0 + VerticalSize);
	SDK::Transform.Rotate(SDK::MoveMatrix, Rotation);
	SDK::Transform.RotateH(SDK::MoveMatrix, HRotation);

	if (Frame == Xion_Blocking || Frame == Xion_Cry2)
		SDK::Transform.Flip(FLIP_TYPE_H);

	SDK::Transform.Tilt (SDK::MoveMatrix, TiltValue, 0.0);

	if (ShowBack)
		SDK::ImageTool.RenderImage(SDK::IMAGE.XionBack);

	SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Xion, Frame);
}

void Xion::HitPeople() {
	if (auto TimeWatch = SDK::Scene.Find("time_watch"); TimeWatch)
		TimeWatch->Stop();

	if (auto Manager = SDK::Scene.Find("play_mode_manager"); Manager)
		Manager->StopBGM();

	SDK::GLOBAL.DestroyedItems[Item_Xion]++;

	// 게임오버 엔딩 지정
	SDK::GLOBAL.Ending = GameOver_HitXion;

	SDK::SoundTool.Play(SDK::SOUND.PeopleHit, SndChannel);

	HitState = true;
	ShowBack = false;
	ObjectTag = "";
}

// 시온을 뒤로 민다
void Xion::PushPeople() {
	SDK::SoundTool.Play(SDK::SOUND.Whoosh, SndChannel);

	DestPosition = Position.x - 2.5;
	DestHRotation = 180.0;
	ObjectTag = "";

	PushState = true;
	ShowBack = false;

	// 커피를 다시 부술 수 있는 상태로 전환한다
	if (auto Shelf = SDK::Scene.Find("shelf"); Shelf)
		Shelf->EnableCoffeeHit();
}

void Xion::SetFrame(int Value) {
	Frame = Value;
}

void Xion::EnableBack() {
	ShowBack = true;
}

void Xion::DisableBack() {
	ShowBack = false;
}
