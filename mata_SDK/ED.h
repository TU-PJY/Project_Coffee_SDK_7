#pragma once
#include <SDK_Scene.h>
#include "Cover.h"

class ED : public SDK::Object {
private:
	// ���� ��ġ, Ŀ�Ǹ� �μ� �� ���� ���������� �̵��Ѵ�.
	float Position{};

	// Ŀ�Ǹ� �μ� �� ��ǥ �̵� ��ġ
	float DestPosition{};

	// ī�޶� ��ġ
	float EDCameraPosition{};
	float EDCameraHeight{};

	// ��Ʈ�� ī�޶� ��ġ
	SDK::Vector2 EDIntroCameraPosition{-1.95, 0.4};
	float EDIntroCameraZoom{1.0};

	SDK::SinMove IntroPositionMove{};
	SDK::SinMove IntroZoomMove{};

	// ī�޶� �� 
	float EDCameraZoom{1.0};

	// ī�޶� ������
	float CameraOffset{ 0.95 };

	// �� ���¸��� �ٸ� �������� ����Ѵ�
	int Frame = ED_Idle;

	float PrevGeneratedFrame{};

	// ���� �����Ӱ� ���� ������
	int PrevFrame = Frame;

	// �յڷ� �þ�� ȿ���� �ش�
	float AnimationSize{};

	// ������ �������� ȿ���� �ش�
	float TiltValue{};

	// �̵� ������ �ִϸ��̼�
	float BreatheSize{};
	SDK::SinLoop BreatheLoop{};

	// ����ȸ����
	float HRotation{};

	// ���°� ����Ǹ� ���� �ð� ���� �ٽ� Idle ���·� �ϱ��ϵ��� �Ѵ�
	SDK::Timer StateTimer{};

	// ���� �ִϸ��̼�
	glm::vec2 ShakeValue{};
	SDK::Timer ShakeTimer{};

	// ���ӿ��� ���� Ÿ�̸�
	SDK::Timer GameOverTimer{};

	// �̵� ���� ä��
	SDK::SoundChannel SndChannel{};
	SDK::SoundChannel SndChannel2{};

	// �̵尡 Ŀ�Ǹ� �μ� Ƚ��
	int BreakCount{};

	// Ŀ�Ǹ� �ν����ϴ� ����, �ʱⰪ 7��
	int MaxBreak{ 7 };

	// ���� ������ ���� ����
	float NextPosition{};

	// Ű�� ���� ����
	// �ϳ��� ���� Ű�� ������ ������ �� ����
	bool KeyPressed[4]{};

	// �ÿ��� �ڷ� �̴� ����
	bool PushState{};

	// �ڷ� �̴� ���� ī�޶� ��ġ
	float PushCameraOffset{};

	// ����Ű �Է� ������ ����, ���ӿ��� �Ǹ� ��Ȱ��ȭ �Ǿ� �� �̻� Ű�� �Է��� �� ����
	bool InputAvailable{ false };

	// ���ӿ��� ���� ��� ����
	bool SoundPlayed{};

	// �̵尡 �μ� �� Ŀ�� ����, ������ ���� Ŀ�� ����� ������ �޶�����
	int TotalBreakCount{};

	// Ŀ�� ��� ����, ������ ���� ���� �� �������
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
		// ��Ȱ��ȭ ���¿����� �Է��� ���� �ʴ´�.
		if (!InputAvailable)
			return;

		if (Event.Type != WM_KEYDOWN)
			return;

		if (Event.Key == VK_LEFT || Event.Key == VK_RIGHT || Event.Key == VK_DOWN || Event.Key == VK_UP || Event.Key == VK_SPACE) {
			// ���� �տ� �ִ� Ŀ�Ǹ� �μ���.
			if (auto Shelf = SDK::Scene.Find("shelf"); Shelf) {
				ItemStruct Item = Shelf->GetFrontCoffee();

				// �տ� ����� ���ִٸ� Ŀ�ǿ� ��ȣ�ۿ� �� �� ����.
				if (Item.IsPeopleFront)
					InteractWithPeople(Event, Shelf, Item);

				// �ÿ��� ���ִٸ� Ŀ�ǿ� ��ȣ�ۿ� �� �� ������ �ÿ°� ��ȣ�ۿ� �� �� �ִ�.
				else if (Item.IsXionFront)
					InteractWithXion(Event, Shelf, Item);
				
				// ��� �Ǵ� �ÿ��� �ڸ��� ���� ���
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

		// ������ �´� Ű�� ������ �μ� �� �ִ�.
		if (IsCorrect) {
			SDK::SoundTool.Play(SDK::SOUND.Whoosh, SndChannel);

			if (Item.IsUpside)
				Frame = SDK::Random.Gen(RANDOM_TYPE_INT, ED_HitHigh1, ED_HitHigh2);
			else
				Frame = SDK::Random.Gen(RANDOM_TYPE_INT, ED_HitLow1, ED_HitLow2);

			DestPosition += 0.5;

			if ((BreakCount < MaxBreak - 1)) {
				ItemStruct NextItem = Shelf->GetNextCoffee();

				// ���� �ڸ��� ����� �ִٸ� ��� ���� ǥ�ø� Ȱ��ȭ �Ѵ�
				if (NextItem.IsPeopleFront) {
					if (auto People = SDK::Scene.Find("people"); People)
						People->EnableBack();
				}

				// ���� �ڸ��� �ÿ��� �ִٸ� �ÿ� ���� ǥ�ø� Ȱ��ȭ �Ѵ�
				else if (NextItem.IsXionFront) {
					if (auto Xion = SDK::Scene.Find("xion"); Xion)
						Xion->EnableBack();
				}
			}

			Shelf->BreakCoffee();

			BreakCount++;

			// ĵ Ŀ�� �Ǵ� �� Ŀ�Ǹ� �μ� �� ��� ������ �����Ѵ�
			if (Item.Type == Coffee_Can || Coffee_Bottle)
				TotalBreakCount++;

			// 50���� �μ� ������ Ŀ�� ��� ������ �����Ѵ�
			if (TotalBreakCount >= 50) {
				CoffeeStatinLevel++;
				SDK::EXTool.ClampValue(CoffeeStatinLevel, 5, CLAMP_GREATER);
				TotalBreakCount = 0;
			}


			// Ŀ�Ǹ� �� �μ��� ���� �������� �̵��Ѵ�
			// �ν����� Ŀ�Ǵ� 4�� �����Ѵ�
			if (BreakCount == MaxBreak) {
				DestPosition = NextPosition;
				BreakCount = 0;
				MaxBreak += 4;

				// �ϳ��� ������ ���� ������ 10���� �߰� �ð��� ��´�.
				if (auto TimeWatch = SDK::Scene.Find("time_watch"); TimeWatch)
					TimeWatch->AddTime();

				if (auto Score = SDK::Scene.Find("score_indicator"); Score)
					Score->AddScore(200);

				TiltValue = 3.0;

				SDK::SoundTool.Play(SDK::SOUND.NextWhoosh, SndChannel2);
			}
		}

		// �´� Ű�� �ƴ� ��� ������ ���� ģ��.
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
		// �ƹ�Ű�� ������ �ÿ��� ������
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

				// �ÿ��� �ڷ� �δ�.
				Xion->PushPeople();

				if (auto Score = SDK::Scene.Find("score_indicator"); Score)
					Score->AddScore(150);

				Frame = ED_HitHigh2;
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		// ������
		// ���� �����Ӱ� ���� �������� �ٸ� ��� ���� �������� �����ϰ� �ִϸ��̼� ����ϵ��� �Ѵ�
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

		// ���� ���°� Idle�� �ƴ϶�� 0.3�� �� �ٽ� Idle ���·� ���ͽ�Ų��
		if (Frame != ED_Idle) {
			StateTimer.Update(FrameTime);
			if (StateTimer.CheckMiliSec(0.3, 2, CHECK_AND_RESET)) {
				Frame = ED_Idle;
				// �ÿ� �б� ���� ����
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
		// �ִϸ��̼�
		// AnimationSize�� 0.0���� �۴ٸ� �ٽ� 0.0���� ���ͽ�Ų��
		SDK::Math.Lerp(AnimationSize, 0.0, 15.0, FrameTime);

		// TiltValue�� 0.0���� ũ�ٸ� �ٽ� 0.0���� ���ͽ�Ų��
		SDK::Math.Lerp(TiltValue, 0.0, 5.0, FrameTime);

		// �̵��� ������ �ִϸ��̼��� ������Ʈ �Ѵ�
		BreatheLoop.Update(BreatheSize, 0.03, 6.0, FrameTime);
	}

	void UpdateCamera(float FrameTime) {
		// �̵�
		// ��ǥ ��ġ�� �̵��ϵ��� �Ѵ�
		SDK::Math.Lerp(Position, DestPosition, 20.0, FrameTime);

		// ���� ���� �� ��Ʈ�� ����
		IntroPositionMove.Update(EDIntroCameraPosition, 1.2, FrameTime);
		IntroZoomMove.Update(EDIntroCameraZoom, 1.2, FrameTime);

		// ī�޶� �̵带 �ε巴�� ��������� �Ѵ�
		SDK::Math.Lerp(EDCameraPosition, DestPosition + CameraOffset, 7.0, FrameTime);

		// �̵带 �ణ �����ʿ��� �ٶ󺸵��� �Ѵ�
		SDK::CameraControl.Move(EDCameraPosition + PushCameraOffset + EDIntroCameraPosition.x, EDIntroCameraPosition.y);
		SDK::CameraControl.SetZoom(1.0 + EDIntroCameraZoom);
	}

	void UpdateGameOverAnimation(float FrameTime) {
		// �Է� ���� ���� ��Ȱ��ȭ
		InputAvailable = false;

		// �̵忡 ��Ŀ�� �Ѵ�.
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

		// ������ 0���� ��� �����ӿ� ��ȭ�� ���� �ʴ´�.
		else {
			TiltValue = 0.0;
			AnimationSize = 0.0;

			Frame = ED_Idle;

			// �̵��� ������ �ִϸ��̼��� ������Ʈ �Ѵ�
			BreatheLoop.Update(BreatheSize, 0.03, 6.0, FrameTime);
		}
	}

	void RenderFunc() {
		SDK::Vector2 FinalPosition{ Position + AnimationSize * 0.5 + TiltValue * 0.5 + ShakeValue.x , BreatheSize * 0.5 - AnimationSize * 0.25 + ShakeValue.y };
		SDK::Vector2 FinalSize{ 2.0 + AnimationSize, 2.0 + BreatheSize - AnimationSize * 0.5 };
		
		// �׸���
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

	// ���� ������ ���� ������ �̸� �˸���
	void TellNextPosition(float Position) {
		NextPosition = Position;
	}

	// ����Ű �Է� ��Ȱ��ȭ
	void DisableInput() {
		InputAvailable = false;
	}

	// ����Ű �Է� Ȱ��ȭ
	void EnableInput() {
		InputAvailable = true;
	}
};