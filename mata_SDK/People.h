#pragma once
#include <SDK_Scene.h>
#include "Cart.h"
#include "Smoke.h"

int Num;

class People : public SDK::Object {
private:
	// �ε����� ���� �������Ǵ� ĳ���Ͱ� �޶�����
	int Frame{};

	// ��ġ
	SDK::Vector2 Position{};

	// ���ư� �Ÿ�
	float MoveDistance{};

	// ȸ����
	float Rotation{};

	// īƮ ��ġ
	SDK::Vector2 CartPosition{};

	// īƮ ȸ����
	float CartRotation{};

	// ���� �ϴ� Ƚ��
	int HitCount = 1;

	// �̵� �߿� ���������� ����
	bool HitState{};

	// �ٴڿ� ���������� ����
	bool FellDown{};

	// ���� �ִϸ��̼� ��ü
	SDK::SinLoop Loop{};
	SDK::SinLoop Loop2{};

	// ������ �ִϸ��̼� ��ġ
	float LoopSize{};

	// �Ѿ��� ���� �ִϸ��̼� ��ġ
	float FellDownSize{};

	// ���� ���� ����� ��ġ
	float TiltValue{};

	// ���� ä��
	SDK::SoundChannel SndChannel{};

	bool ShowBack{};

public:
	People(SDK::Vector2 PositionValue) {
		Position = PositionValue;
		CartPosition.x = PositionValue.x + 1.2;
		CartPosition.y = PositionValue.y - 0.3;

		int RandNum = SDK::Random.Gen(RANDOM_TYPE_INT, Listy, EOE - 1);

		// �� �� ���õ� ĳ���ʹ� ���� 2ȸ�� ���ȿ��� ���õ��� �ʴ´�.
		if (SDK::GLOBAL.CreateAvailable[RandNum]) {
			SDK::GLOBAL.CreateAvailable[RandNum] = false;
			Frame = RandNum * 2;
		}

		else {
			std::vector<int> AvailableNum{};

			// ���� �� �ִ� ĳ���͵��� ������ ��ȣ�� ���Ϳ� ����
			for (int i = Listy; i < EOE; i++) {
				if (SDK::GLOBAL.CreateAvailable[i])
					AvailableNum.emplace_back(i);
			}

			// ������ ��ȣ �� �ϳ��� �����Ͽ� ĳ���� �̱�
			int Rand = SDK::Random.Gen(RANDOM_TYPE_INT, 0, AvailableNum.size() - 1);
			Frame = AvailableNum[Rand] * 2;
		}

		// ���� ���� �� ���� ĳ���͵��� ī��Ʈ ���� 
		for (int i = Listy; i < EOE; i++) {
			if (!SDK::GLOBAL.CreateAvailable[i])
				SDK::GLOBAL.PrevChFrame[i]++;

			// ī��Ʈ�� 2�� �Ǹ� �ٽ� ���� �� �ִ� ���·� ��ȯ
			if (SDK::GLOBAL.PrevChFrame[i] > 2) {
				SDK::GLOBAL.CreateAvailable[i] = true;
				SDK::GLOBAL.PrevChFrame[i] = 0;
			}
		}

		// ��ǥ�� ��� 2�� ���� �Ѵ�
		if (Frame == Daepyo * 2)
			HitCount = 2;

		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);
	}

	void UpdateFunc(float FrameTime) {
		// �߿� ���� ���¿����� ������ ������.
		if (HitState) {
			// �Ѿ����� ��
			if (!FellDown) {
				Rotation += 250 * FrameTime;
				Position.x += 8.0 * FrameTime;
				Position.y -= 1.0 * FrameTime;

				CartPosition.x += 8.0 * FrameTime;
				CartPosition.y -= 0.75 * FrameTime;

				// -1.3���� �̵��ϸ� �Ѿ��� ���¸� Ȱ��ȭ �Ѵ�
				if (Position.y <= -1.3) {
					// ���̴̹� �������� ����Ѵ�
					if (Frame == Yumimi * 2) 
						SDK::SoundTool.Play(SDK::SOUND.Explode, SndChannel);
					else 
						SDK::SoundTool.Play(SDK::SOUND.CartCrash, SndChannel);

					if (Frame == Listy * 2 || Frame == Mayo * 2)
						Position.y = -1.35;
					else if (Frame == Silphir * 2)
						Position.y = -1.2;
					else if (Frame == Kidian * 2)
						Position.y = -0.8;
					else if (Frame == Melloon * 2)
						Position.y = -1.53;
					else if (Frame == Yumimi * 2)
						Position.y = -1.4;
					else if (Frame == Daepyo * 2)
						Position.y = -1.35;
					else
						Position.y = -1.3;

					CartPosition.x += 0.5;
					CartPosition.y = -0.35;
					                                    
					// 5�� ���̾�� �̵�

					// �Ѿ��� īƮ �߰�
					// ���̴̹� īƮ�� �߰����� �ʴ´�
					if (Frame != Yumimi * 2) {
						SDK::Scene.SwapLayer(this, LAYER6);
						SDK::Scene.AddObject(new Cart(true, CartPosition), "cart", LAYER4);
					}
					else {
						SDK::Scene.SwapLayer(this, LAYER2);
						SDK::Scene.AddObject(new Smoke(Position.x), "smoke", LAYER5);
					}

					// ���� �������� ���� ������ �Ѿ��� ������
					Frame++;

					// �Ѿ��� ���� Ȱ��ȭ
					FellDown = true;
				}
			}
			// �Ѿ��� ��
			else {
				// Ű����� ��� �౸���� �Ǿ� ��������
				if (Frame == Kidian * 2 + 1) {
					Rotation -= 360 * FrameTime;
					Position.x -= 4.0 * FrameTime;
				}

				// ���̹�, ����� ��� ������ �ִ´�
				else if (Frame == Melloon * 2 + 1) 
					Rotation = 180.0;
				else if (Frame == Yumimi * 2 + 1) 
					Rotation = 0.0;

				else {
					float TempRotation{};
					Loop.Update(TempRotation, 2.5, 20.0, FrameTime);
					Rotation = TempRotation - 25.0;
					Loop2.Update(FellDownSize, 0.1, 20.0, FrameTime);
				}
			}
		}

		// ������ �ִϸ��̼�
		else {
			if (Frame == Naia * 2)
				Loop.Update(LoopSize, 0.03, 30.0, FrameTime);

			// ����� ������ �ִ´�
			else if (Frame == Melloon * 2) {
				Loop.Update(LoopSize, 0.0, 0.0, 0.0);
			}
			else
				Loop.Update(LoopSize, 0.03, 4.0, FrameTime);
		}

		// ����� ��ġ ������Ʈ
		SDK::Math.Lerp(TiltValue, 0.0, 10.0, FrameTime);

		// ȭ�� �������� ����� �����Ѵ�.
		if (Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - 1.2)
			SDK::Scene.DeleteObject(this);
	}

	void RenderFunc() {
		// ȭ�麸�� �����ʿ� ���� �� �������� �������� �ʴ´�.
		if (Position.x > SDK::Camera.Position.x + SDK::ASP(1.0) + 1.2)
			return;

		// ��� ������
		Begin();
		if(Frame == Listy * 2)
			SDK::Transform.Move(SDK::MoveMatrix, Position.x + 0.3 + TiltValue * 0.5, Position.y + LoopSize * 0.5);
		else
			SDK::Transform.Move(SDK::MoveMatrix, Position.x + TiltValue * 0.5, Position.y + LoopSize * 0.5);
		SDK::Transform.Rotate(SDK::MoveMatrix, Rotation);
		if(Frame == Yumimi * 2 + 1)
			SDK::Transform.Scale(SDK::MoveMatrix, 3.0, 3.0 + LoopSize + FellDownSize);
		else
			SDK::Transform.Scale(SDK::MoveMatrix, 2.0, 2.0 + LoopSize + FellDownSize);

		SDK::Transform.Tilt(SDK::MoveMatrix, TiltValue, 0.0);

		if(ShowBack)
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.PeopleBack, Frame);
		SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.People, Frame);

		// īƮ ������
		// ���̴̹� īƮ�� ���������� �ʴ´�
		if (!FellDown && Frame != Yumimi * 2) {
			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, CartPosition.x, CartPosition.y);
			SDK::Transform.Rotate(SDK::MoveMatrix, CartRotation);
			SDK::Transform.Scale(SDK::MoveMatrix, 1.5, 1.5);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Cart, 0);
		}
	}
	
	void HitPeople() {
		SDK::SoundTool.Play(SDK::SOUND.PeopleHit, SndChannel);
		// ���� ���°� �Ǹ� �������� �����ϰ� ������Ʈ �±׸� ���� �� �̻� ��ȣ�ۿ����� �ʵ��� �Ѵ�
		HitCount--;

		// ����� HitCount�� 0�� �� ��� Ŀ�Ǹ� �ٽ� �μ� �� �ִ� ���·� ��ȯ�Ѵ� 
		if (HitCount == 0) {
			ShowBack = false;
			HitState = true;
			ObjectTag = "";
			Loop.SetValue(SDK::Preset::HalfPositive);                     
			Loop2.SetValue(SDK::Preset::HalfNegative);
			LoopSize = 0.0;

			if (auto Shelf = SDK::Scene.Find("shelf"); Shelf)
				Shelf->EnableCoffeeHit();

			SDK::Object* Score = SDK::Scene.Find("score_indicator");

			// ��ǥ�� ��� �ı��� ������ ������ ������Ų��
			if (Frame == Daepyo * 2) {
				Score->AddScore(118);
				SDK::GLOBAL.DestroyedItems[Item_HouseDoc]++;
			}
			else
				Score->AddScore(100);

			SDK::GLOBAL.DestroyedItems[Item_People]++;
		}

		// HitCount�� �������� ��� ����� ��ġ�� �߰��Ѵ�
		else 
			TiltValue = 2.0;
	}

	void EnableBack() {
		std::cout << "Enabled" << std::endl;
		ShowBack = true;
	}

	void DisableBack() {
		ShowBack = false;
	}
};