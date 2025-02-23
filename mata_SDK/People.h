#pragma once
#include <SDK_Scene.h>
#include "Cart.h"
#include "Smoke.h"

int Num;

class People : public SDK::Object {
private:
	// 인덱스에 따라 렌더링되는 캐릭터가 달라진다
	int Frame{};

	// 위치
	SDK::Vector2 Position{};

	// 날아간 거리
	float MoveDistance{};

	// 회전값
	float Rotation{};

	// 카트 위치
	SDK::Vector2 CartPosition{};

	// 카트 회전값
	float CartRotation{};

	// 차야 하는 횟수
	int HitCount = 1;

	// 이드 발에 차였는지의 여부
	bool HitState{};

	// 바닥에 엎어졌는지 여부
	bool FellDown{};

	// 루프 애니메이션 객체
	SDK::SinLoop Loop{};
	SDK::SinLoop Loop2{};

	// 숨쉬기 애니메이션 수치
	float LoopSize{};

	// 넘어진 후의 애니메이션 수치
	float FellDownSize{};

	// 차인 후의 기울임 수치
	float TiltValue{};

	// 사운드 채널
	SDK::SoundChannel SndChannel{};

	bool ShowBack{};

public:
	People(SDK::Vector2 PositionValue) {
		Position = PositionValue;
		CartPosition.x = PositionValue.x + 1.2;
		CartPosition.y = PositionValue.y - 0.3;

		int RandNum = SDK::Random.Gen(RANDOM_TYPE_INT, Listy, EOE - 1);

		// 한 번 선택된 캐릭터는 다음 2회차 동안에는 선택되지 않는다.
		if (SDK::GLOBAL.CreateAvailable[RandNum]) {
			SDK::GLOBAL.CreateAvailable[RandNum] = false;
			Frame = RandNum * 2;
		}

		else {
			std::vector<int> AvailableNum{};

			// 뽑을 수 있는 캐릭터들의 프레임 번호를 벡터에 저장
			for (int i = Listy; i < EOE; i++) {
				if (SDK::GLOBAL.CreateAvailable[i])
					AvailableNum.emplace_back(i);
			}

			// 벡터의 번호 중 하나를 선택하여 캐릭터 뽑기
			int Rand = SDK::Random.Gen(RANDOM_TYPE_INT, 0, AvailableNum.size() - 1);
			Frame = AvailableNum[Rand] * 2;
		}

		// 현재 뽑을 수 없는 캐릭터들은 카운트 증가 
		for (int i = Listy; i < EOE; i++) {
			if (!SDK::GLOBAL.CreateAvailable[i])
				SDK::GLOBAL.PrevChFrame[i]++;

			// 카운트가 2가 되면 다시 뽑을 수 있는 상태로 전환
			if (SDK::GLOBAL.PrevChFrame[i] > 2) {
				SDK::GLOBAL.CreateAvailable[i] = true;
				SDK::GLOBAL.PrevChFrame[i] = 0;
			}
		}

		// 대표의 경우 2번 차야 한다
		if (Frame == Daepyo * 2)
			HitCount = 2;

		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);
	}

	void UpdateFunc(float FrameTime) {
		// 발에 차인 상태에서는 앞으로 구른다.
		if (HitState) {
			// 넘어지기 전
			if (!FellDown) {
				Rotation += 250 * FrameTime;
				Position.x += 8.0 * FrameTime;
				Position.y -= 1.0 * FrameTime;

				CartPosition.x += 8.0 * FrameTime;
				CartPosition.y -= 0.75 * FrameTime;

				// -1.3까지 이동하면 넘어짐 상태를 활성화 한다
				if (Position.y <= -1.3) {
					// 유미미는 폭발음을 재생한다
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
					                                    
					// 5번 레이어로 이동

					// 넘어진 카트 추가
					// 유미미는 카트를 추가하지 않는다
					if (Frame != Yumimi * 2) {
						SDK::Scene.SwapLayer(this, LAYER6);
						SDK::Scene.AddObject(new Cart(true, CartPosition), "cart", LAYER4);
					}
					else {
						SDK::Scene.SwapLayer(this, LAYER2);
						SDK::Scene.AddObject(new Smoke(Position.x), "smoke", LAYER5);
					}

					// 현재 프레임의 다음 프레임 넘어진 프레임
					Frame++;

					// 넘어짐 상태 활성화
					FellDown = true;
				}
			}
			// 넘어진 후
			else {
				// 키디언의 경우 축구공이 되어 굴러간다
				if (Frame == Kidian * 2 + 1) {
					Rotation -= 360 * FrameTime;
					Position.x -= 4.0 * FrameTime;
				}

				// 유미미, 멜룬의 경우 가만히 있는다
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

		// 숨쉬기 애니메이션
		else {
			if (Frame == Naia * 2)
				Loop.Update(LoopSize, 0.03, 30.0, FrameTime);

			// 멜룬은 가만히 있는다
			else if (Frame == Melloon * 2) {
				Loop.Update(LoopSize, 0.0, 0.0, 0.0);
			}
			else
				Loop.Update(LoopSize, 0.03, 4.0, FrameTime);
		}

		// 기울임 수치 업데이트
		SDK::Math.Lerp(TiltValue, 0.0, 10.0, FrameTime);

		// 화면 왼쪽으로 벗어나면 삭제한다.
		if (Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - 1.2)
			SDK::Scene.DeleteObject(this);
	}

	void RenderFunc() {
		// 화면보다 오른쪽에 있을 때 랜더링을 진행하지 않는다.
		if (Position.x > SDK::Camera.Position.x + SDK::ASP(1.0) + 1.2)
			return;

		// 사람 렌더링
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

		// 카트 렌더링
		// 유미미는 카트를 렌더링하지 않는다
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
		// 때린 상태가 되면 프레임을 변경하고 오브젝트 태그를 지워 더 이상 상호작용하지 않도록 한다
		HitCount--;

		// 사람의 HitCount가 0이 될 경우 커피를 다시 부술 수 있는 상태로 전환한다 
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

			// 대표의 경우 파괴된 집문서 개수를 증가시킨다
			if (Frame == Daepyo * 2) {
				Score->AddScore(118);
				SDK::GLOBAL.DestroyedItems[Item_HouseDoc]++;
			}
			else
				Score->AddScore(100);

			SDK::GLOBAL.DestroyedItems[Item_People]++;
		}

		// HitCount가 남아있을 경우 기울임 수치를 추가한다
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