#pragma once
#include <SDK_Scene.h>

#include "Explode.h"
#include "DestroyedCan.h"
#include "People.h"
#include "Xion.h"

class Shelf : public SDK::Object {
private:
	//////////////////////// ����
	// Ÿ��Ʋ ���� ����
	bool ForTitle{};
	// 
	// ���� ����
	int NumShelf{};

	// ���� ���� ��ġ
	float Position{};

	// ���� �ϳ��� ����
	float Length = 2.0;

	// ���� �߰����� ��ġ
	float MiddlePoint{};
	           
	// ���� ������ ��ġ
	float EndPoint{};

	// ���� �������� �����ϴ� �ε���
	int StartShelfIndex{};

	// ���� ���� ���� ����
	bool NextShelfGenerated{};

	//////////////////////// ����
	// Ŀ�ǵ��� ��ġ �� ������ �����ϴ� ����
	std::vector<ItemStruct> CoffeeVec{};

	// Ŀ�� �������� �����ϴ� �ε���
	int StartCoffeeIndex{};

	// ���� ���� ������ Ŀ�� �ε���
	int CurrentCoffeeIndex{};

	// Ŀ�ǰ� �ƴ� �ٸ� ���ǵ��� ��ġ �� ������ �����ϴ� ����
	std::vector<ItemStruct> OtherVec{}; 

	// Ŀ�ǰ� �ƴ� �ٸ� ���ǵ��� �������� �����ϴ� �ε���
	int StartOtherIndex{};

	// ����� �߰��� �������� ����
	bool PeopleAddActivated{};

	// ����� �߰��� �ڸ� �ε��� ��ȣ
	int AddedIndex{};

	// �ÿ��� ��ġ�� �������� ����
	bool XionAddActivated{};

	// �ÿ��� ������ ��ġ ���� ����
	bool AddedXionPosition{};

	// ��� ��ġ�� ���������� ����
	bool AvailableAddPeople{};

	// �ÿ��� ������ �ڸ�
	int XionIndex{};

	// �ÿ� ��ǥ �̵� ��ġ
	float XionDestPosition{};

	// �ÿ� �߰� ����
	bool XionGenerated{};

	// ��� ä��
	SDK::SoundChannel SndChannel{};

public:
	Shelf(int Num, float PositionValue, bool TitleRender=false) {
		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);

		NumShelf = Num;
		Position = PositionValue; 
		ForTitle = TitleRender;

		// �� ���ݸ��� 50�ۼ�Ʈ�� Ȯ���� �ÿ� ��ġ�� Ȱ��ȭ�ȴ�
		// �߰� ���� �� �� ���� ���� ��� 
		MiddlePoint = Position + Length * (float)(Num - 1) * 0.5;
		EndPoint = Position + Length * (float)(Num - 1) + Length * 0.5;

		if (!ForTitle) {
			if (NumShelf > 2 && SDK::Random.Probability(50))
				XionAddActivated = true;

			// ���� �� ĭ�� 4���� Ŀ�ǵ��� �������� ��ġ�Ѵ�.
			// ������ ĭ�� 3���� ��ġ�Ѵ�.
			int GenTime = Num * 4 - 1;

			for (int i = 0; i < GenTime; ++i) {
				AvailableAddPeople = true;

				ItemStruct Coffee{};
				ItemStruct Other{};

				// �ּ� 4ĭ �������� ��ġ�Ѵ�
				if (i - AddedIndex > 4 && NumShelf > 2) {
					// �� Ŀ�� ĭ���� 10�ۼ�Ʈ�� Ȯ���� �ÿ� ��ġ�� �����Ѵ�.
					// �ÿ� ��ġ�� ������ �ڸ����� ����� ��ġ�� �� ���� �� �� �����ϸ� �ٽ� �������� �ʴ´�.
					if (!AddedXionPosition && XionAddActivated && SDK::Random.Probability(10)) {
						AvailableAddPeople = false;
						AddedXionPosition = true;

						// �ÿ� ����
						float Position = PositionValue - 0.75 + 0.5 * i + 0.35;
						SDK::Scene.AddObject(new Xion(Position, 0.0, false, Xion_Blocking), "xion", LAYER3);

						// �ÿ��� ��ġ�� �ڸ� ���� ǥ��
						Coffee.IsXionFront = true;

						// �ε��� ���
						AddedIndex = i;

						// �ÿ��� ������ �ڸ� ���
						XionIndex = i;
					}

					//�� Ŀ�� ĭ ���� 5�ۼ�Ʈ�� Ȯ���� ����� ��ġ�Ѵ�
					if (AvailableAddPeople && SDK::Random.Probability(10)) {
						SDK::Vector2 AddPosition = glm::vec2(PositionValue - 0.75 + 0.5 * i, 0.0);
						SDK::Scene.AddObject(new People(AddPosition), "people", LAYER3);

						// ����� ��ġ�� �ڸ��� Ŀ�Ǵ� ���� ǥ���Ѵ�
						Coffee.IsPeopleFront = true;

						// ���������� ����� �߰��� �ε��� ���
						AddedIndex = i;
					}
				}

				// Ÿ�� ����
				Coffee.Type = SDK::Random.Gen(RANDOM_TYPE_INT, 0, 2);
				Other.Type = SDK::Random.Gen(RANDOM_TYPE_INT, 0, 2);

				if (NumShelf == 2 && i == 0) {
					if (i == 0) {
						Coffee.Type = Coffee_Bottle;
						Coffee.IsUpside = true;
						Coffee.Position.y = 0.14;

						Other.Type = 0;
						Other.Position.y = -0.27;
					}
				}

				// ��ġ ����
				// 1�̸� ��ĭ, 0�̸� �Ʒ�ĭ
				// Ŀ�ǿ� �ٸ� ������ ��ġ�� �ʵ��� ��ġ�Ѵ�
				else {
					int RandomNum = SDK::Random.Gen(RANDOM_TYPE_INT, 0, 1);
					if (RandomNum == 1) {
						Coffee.Position.y = 0.14;
						Coffee.IsUpside = true;
						Other.Position.y = -0.27;
					}
					else {
						Coffee.Position.y = -0.27;
						Coffee.IsUpside = false;
						Other.Position.y = 0.14;
					}
				}

				// x ��ġ�� Ŀ�ǿ� �ٸ� ������ ������ �Ѵ�
				Coffee.Position.x = PositionValue - 0.75 + 0.5 * i;
				Other.Position.x = Coffee.Position.x;

				CoffeeVec.emplace_back(Coffee);
				OtherVec.emplace_back(Other);
			}
		}

		else {
			int GenTime = Num * 4 - 1;
			for (int i = 0; i < GenTime; i++) {
				ItemStruct Coffee{};
				ItemStruct Other{};

				if (i == 0) {
					Coffee.Type = Coffee_Bottle;
					Coffee.IsUpside = true;
					Coffee.Position.y = 0.14;

					Other.Type = 0;
					Other.Position.y = -0.27;
				}

				else {
					Coffee.Type = SDK::Random.Gen(RANDOM_TYPE_INT, 0, 2);
					Other.Type = SDK::Random.Gen(RANDOM_TYPE_INT, 0, 2);

					int RandomNum = SDK::Random.Gen(RANDOM_TYPE_INT, 0, 1);
					if (RandomNum == 1) {
						Coffee.Position.y = 0.14;
						Coffee.IsUpside = true;
						Other.Position.y = -0.27;
					}
					else {
						Coffee.Position.y = -0.27;
						Coffee.IsUpside = false;
						Other.Position.y = 0.14;
					}
				}

				Coffee.Position.x = PositionValue - 0.75 + 0.5 * i;
				Other.Position.x = Coffee.Position.x;

				CoffeeVec.emplace_back(Coffee);
				OtherVec.emplace_back(Other);
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		if (!ForTitle) {
			if (auto ED = SDK::Scene.Find("ed"); ED) {
				// ī�޶� ��ġ�� �߰� ������ �����ϸ� ���� ������ �̸� �����Ѵ�
				if (!NextShelfGenerated && MiddlePoint <= SDK::Camera.Position.x) {
					NextShelfGenerated = true;
					SDK::Scene.AddObject(new Shelf(NumShelf + 1, EndPoint + Length * 2.0), "shelf", LAYER2);

					// �̵尡 �̵��ؾ� �� ���� ��ġ�� �˸���
					ED->TellNextPosition(EndPoint + Length * 2.0 - 1.75);
				}
			}

			// ������ ������ ȭ�鿡�� ������ �ʰ� �Ǹ� ������ �����Ѵ�
			if (EndPoint < SDK::Camera.Position.x - SDK::ASP(1.0))
				SDK::Scene.DeleteObject(this);
		}
	}

	void RenderFunc() {
		// ���� ������
		for (int i = StartShelfIndex; i < NumShelf; i++) {
			GLfloat ShelfPosition = Position + Length * i;

			// ȭ�麸�� �����ʿ� �ִ� ������ �������� �ǳʶڴ�
			if (ShelfPosition > SDK::Camera.Position.x + SDK::ASP(1.0) + Length * 0.5)
				break;

			// �� �̻� ȭ�鿡�� ������ ���� ������ �������� �� �̻� ���� �ʴ´�.
			else if (Position + Length * StartShelfIndex < SDK::Camera.Position.x - SDK::ASP(1.0) - Length * 0.5) {
				StartShelfIndex++;
				continue;
			}

			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, ShelfPosition, 0.0);
			SDK::Transform.Scale(SDK::MoveMatrix, Length, Length);

			// �� �պκа� �� �� �κ��� ������ ������ �κ��� ���� ���������� �������Ѵ�.
			if(i == 0) // �պκ�
				SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Shelf, 0);

			else if(i == NumShelf - 1) // �޺κ�
				SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Shelf, 2);

			else // �߰� �κ�
				SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Shelf, 1);
		}


		// Ŀ�� ������
		size_t CoffeeVecSize = CoffeeVec.size();
		for (int i = StartCoffeeIndex; i < CoffeeVecSize; i++) {
			// ȭ�麸�� �����ʿ� �ִ� Ŀ�Ǵ� �������� �ǳʶڴ�.
			if (CoffeeVec[i].Position.x > SDK::Camera.Position.x + SDK::ASP(1.0) + 0.255)
				break;

			// �� �̻� ������ ���� Ŀ�Ǵ� �� �̻� ������ ���� �ʴ´�.
			else if (CoffeeVec[i].Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - 0.255) {
				StartCoffeeIndex++;
				continue;
			}

			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, CoffeeVec[i].Position);
			SDK::Transform.Scale(SDK::MoveMatrix, 0.45, 0.45);

			// �ı��� Ŀ�Ǵ� �ı��� ��������Ʈ�� �������Ѵ�
			if(CoffeeVec[i].Destroyed)
				SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.DestroyedCoffee, CoffeeVec[i].Type);
			else {
				if(i == CurrentCoffeeIndex && !CoffeeVec[CurrentCoffeeIndex].IsPeopleFront && !CoffeeVec[CurrentCoffeeIndex].IsXionFront && !ForTitle)
					SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.CoffeeBack, CoffeeVec[i].Type);
				SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Coffee, CoffeeVec[i].Type);
			}
		}
	

		// �ٸ� ���� ������
		size_t OtherVecSize = OtherVec.size();
		for (int i = StartOtherIndex; i < OtherVecSize; i++) {
			// ȭ�麸�� �����ʿ� �ִ� ������ �������� �ǳʶڴ�.
			if (OtherVec[i].Position.x > SDK::Camera.Position.x + SDK::ASP(1.0) + 0.255)
				break;

			// �� �̻� ������ ���� ������ �� �̻� ������ ���� �ʴ´�.
			else if (OtherVec[i].Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - 0.255) {
				StartOtherIndex++;
				continue;
			}

			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, OtherVec[i].Position);
			SDK::Transform.Scale(SDK::MoveMatrix, 0.45, 0.45);
			SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Other, OtherVec[i].Type);
		}
	}

	// ���� �տ� �ִ� Ŀ���� �� �Ǵ� �Ʒ��� ��ġ ���θ� ��´�
	// ���� ���� �� true,  �Ʒ��� ���� �� false
	ItemStruct GetFrontCoffee() {
		return CoffeeVec[CurrentCoffeeIndex];
	}

	ItemStruct GetNextCoffee() {
		return CoffeeVec[CurrentCoffeeIndex + 1];
	}

	int GetCurrentIndex() {
		return CurrentCoffeeIndex;
	}

	// Ŀ�Ǹ� �μ� �� �ִ� ���·� ��ȯ�Ѵ�
	void EnableCoffeeHit() {
		CoffeeVec[CurrentCoffeeIndex].IsPeopleFront = false;
		CoffeeVec[CurrentCoffeeIndex].IsXionFront = false;
	}

	int GetCoffeeSize() { 
		return CoffeeVec.size();
	}

	// ���� �տ� �ִ� Ŀ�Ǹ� �μ���.
	void BreakCoffee() {
		// ���� �߰�
		if(auto Score = SDK::Scene.Find("score_indicator"); Score)
			Score->AddScore(10);

		// ���� 3���� �� 1���� ���� ����
		int RandomNum = SDK::Random.Gen(RANDOM_TYPE_INT, 0, 2);

		if(CoffeeVec[CurrentCoffeeIndex].Type == Coffee_Box)
			SDK::SoundTool.Play(SDK::SOUND.BoxBreak[RandomNum], SndChannel);

		else if(CoffeeVec[CurrentCoffeeIndex].Type == Coffee_Bottle)
			SDK::SoundTool.Play(SDK::SOUND.Bottle[RandomNum], SndChannel);

		else if(CoffeeVec[CurrentCoffeeIndex].Type == Coffee_Can)
			SDK::SoundTool.Play(SDK::SOUND.CanBreak[RandomNum], SndChannel);

		// �ı��� ǰ�� �� ������Ʈ
		SDK::GLOBAL.DestroyedItems[CoffeeVec[CurrentCoffeeIndex].Type]++;

		// ���� Ŀ�� �̿��� Ŀ�Ǹ� �μ��� Ŀ�ǰ� ���������� �ִϸ��̼� ��ü�� �߰��Ѵ�
		if(CoffeeVec[CurrentCoffeeIndex].Type != Coffee_Box)
			SDK::Scene.AddObject(new Explode(CoffeeVec[CurrentCoffeeIndex].Position, false), "explode", LAYER5);
		// ���� Ŀ�Ƕ�� Ŀ�� ��ƽ�� ���������� �ִϸ��̼� ��ü�� �߰��Ѵ�
		else
			SDK::Scene.AddObject(new Explode(CoffeeVec[CurrentCoffeeIndex].Position, true), "explode", LAYER5);

		//ĵĿ�Ƕ�� ��׷��� ĵ�� �߰��Ѵ�
		if (CoffeeVec[CurrentCoffeeIndex].Type == Coffee_Can)
			SDK::Scene.AddObject(new DestroyedCan(CoffeeVec[CurrentCoffeeIndex].Position), "destroyed_can", LAYER5);

		// Ŀ�Ǵ� �ı� ���°� �ǰ� �� �̻� �̵�� ��ȣ�ۿ����� �ʴ´�.
		CoffeeVec[CurrentCoffeeIndex].Destroyed = true;

		// ���� �ε��� ����
		CurrentCoffeeIndex++;

		// Ŀ�ǰ� ��� �ν����� �ڽ��� �±׸� ���� ���� ������ �˻��ǵ��� �Ѵ�.
		if (CurrentCoffeeIndex == NumShelf * 4 - 1)
			ObjectTag = "";
	}
};