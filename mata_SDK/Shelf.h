#pragma once
#include <SDK_Scene.h>

#include "Explode.h"
#include "DestroyedCan.h"
#include "People.h"
#include "Xion.h"

class Shelf : public SDK::Object {
private:
	//////////////////////// 선반
	// 타이틀 렌더 여부
	bool ForTitle{};
	// 
	// 선반 개수
	int NumShelf{};

	// 생성 기준 위치
	float Position{};

	// 선반 하나의 길이
	float Length = 2.0;

	// 선반 중간지점 위치
	float MiddlePoint{};
	           
	// 선반 끝지점 위치
	float EndPoint{};

	// 선반 렌더링을 시작하는 인덱스
	int StartShelfIndex{};

	// 다음 선반 생성 여부
	bool NextShelfGenerated{};

	//////////////////////// 물건
	// 커피들의 위치 및 종류를 저장하는 벡터
	std::vector<ItemStruct> CoffeeVec{};

	// 커피 렌더링을 시작하는 인덱스
	int StartCoffeeIndex{};

	// 가장 먼저 참조할 커피 인덱스
	int CurrentCoffeeIndex{};

	// 커피가 아닌 다른 물건들의 위치 및 종류를 저장하는 벡터
	std::vector<ItemStruct> OtherVec{}; 

	// 커피가 아닌 다른 물건들의 렌더링을 시작하는 인덱스
	int StartOtherIndex{};

	// 사람을 추가할 것인지의 여부
	bool PeopleAddActivated{};

	// 사람이 추가된 자리 인덱스 번호
	int AddedIndex{};

	// 시온을 배치할 것인지의 여부
	bool XionAddActivated{};

	// 시온이 등장할 위치 지정 여부
	bool AddedXionPosition{};

	// 사람 배치가 가능한지의 여부
	bool AvailableAddPeople{};

	// 시온이 등장할 자리
	int XionIndex{};

	// 시온 목표 이동 위치
	float XionDestPosition{};

	// 시온 추가 여부
	bool XionGenerated{};

	// 고속 채널
	SDK::SoundChannel SndChannel{};

public:
	Shelf(int Num, float PositionValue, bool TitleRender=false) {
		SDK::SoundTool.SetVolume(SndChannel, SDK::GLOBAL.SFXVolume);

		NumShelf = Num;
		Position = PositionValue; 
		ForTitle = TitleRender;

		// 각 선반마다 50퍼센트의 확률로 시온 배치가 활성화된다
		// 중간 지점 및 끝 지점 길이 계산 
		MiddlePoint = Position + Length * (float)(Num - 1) * 0.5;
		EndPoint = Position + Length * (float)(Num - 1) + Length * 0.5;

		if (!ForTitle) {
			if (NumShelf > 2 && SDK::Random.Probability(50))
				XionAddActivated = true;

			// 선반 한 칸당 4개의 커피들을 랜덤으로 배치한다.
			// 마지막 칸은 3개만 배치한다.
			int GenTime = Num * 4 - 1;

			for (int i = 0; i < GenTime; ++i) {
				AvailableAddPeople = true;

				ItemStruct Coffee{};
				ItemStruct Other{};

				// 최소 4칸 간격으로 배치한다
				if (i - AddedIndex > 4 && NumShelf > 2) {
					// 각 커피 칸마다 10퍼센트의 확률로 시온 위치를 지정한다.
					// 시온 위치가 지정된 자리에는 사람이 배치될 수 없고 한 번 지정하면 다시 지정되지 않는다.
					if (!AddedXionPosition && XionAddActivated && SDK::Random.Probability(10)) {
						AvailableAddPeople = false;
						AddedXionPosition = true;

						// 시온 생성
						float Position = PositionValue - 0.75 + 0.5 * i + 0.35;
						SDK::Scene.AddObject(new Xion(Position, 0.0, false, Xion_Blocking), "xion", LAYER3);

						// 시온이 배치된 자리 별도 표시
						Coffee.IsXionFront = true;

						// 인덱스 기록
						AddedIndex = i;

						// 시온이 등장할 자리 기록
						XionIndex = i;
					}

					//각 커피 칸 마다 5퍼센트의 확률로 사람을 배치한다
					if (AvailableAddPeople && SDK::Random.Probability(10)) {
						SDK::Vector2 AddPosition = glm::vec2(PositionValue - 0.75 + 0.5 * i, 0.0);
						SDK::Scene.AddObject(new People(AddPosition), "people", LAYER3);

						// 사람이 배치된 자리의 커피는 별도 표시한다
						Coffee.IsPeopleFront = true;

						// 마지막으로 사람을 추가한 인덱스 기록
						AddedIndex = i;
					}
				}

				// 타입 결정
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

				// 위치 결정
				// 1이면 위칸, 0이면 아래칸
				// 커피와 다른 물건이 겹치지 않도록 배치한다
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

				// x 위치는 커피와 다른 물건이 같도록 한다
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
				// 카메라 위치가 중간 지점에 도달하면 다음 선반을 미리 생성한다
				if (!NextShelfGenerated && MiddlePoint <= SDK::Camera.Position.x) {
					NextShelfGenerated = true;
					SDK::Scene.AddObject(new Shelf(NumShelf + 1, EndPoint + Length * 2.0), "shelf", LAYER2);

					// 이드가 이동해야 할 다음 위치를 알린다
					ED->TellNextPosition(EndPoint + Length * 2.0 - 1.75);
				}
			}

			// 마지막 선반이 화면에서 보이지 않게 되면 스스로 삭제한다
			if (EndPoint < SDK::Camera.Position.x - SDK::ASP(1.0))
				SDK::Scene.DeleteObject(this);
		}
	}

	void RenderFunc() {
		// 선반 렌더링
		for (int i = StartShelfIndex; i < NumShelf; i++) {
			GLfloat ShelfPosition = Position + Length * i;

			// 화면보다 오른쪽에 있는 선반은 렌더링을 건너뛴다
			if (ShelfPosition > SDK::Camera.Position.x + SDK::ASP(1.0) + Length * 0.5)
				break;

			// 더 이상 화면에서 보이지 않을 선반은 렌더링을 더 이상 하지 않는다.
			else if (Position + Length * StartShelfIndex < SDK::Camera.Position.x - SDK::ASP(1.0) - Length * 0.5) {
				StartShelfIndex++;
				continue;
			}

			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, ShelfPosition, 0.0);
			SDK::Transform.Scale(SDK::MoveMatrix, Length, Length);

			// 맨 앞부분과 맨 뒷 부분을 제외한 나머지 부분은 같은 프레임으로 렌더링한다.
			if(i == 0) // 앞부분
				SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Shelf, 0);

			else if(i == NumShelf - 1) // 뒷부분
				SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Shelf, 2);

			else // 중간 부분
				SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Shelf, 1);
		}


		// 커피 렌더링
		size_t CoffeeVecSize = CoffeeVec.size();
		for (int i = StartCoffeeIndex; i < CoffeeVecSize; i++) {
			// 화면보다 오른쪽에 있는 커피는 렌더링을 건너뛴다.
			if (CoffeeVec[i].Position.x > SDK::Camera.Position.x + SDK::ASP(1.0) + 0.255)
				break;

			// 더 이상 보이지 않을 커피는 더 이상 렌더링 하지 않는다.
			else if (CoffeeVec[i].Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - 0.255) {
				StartCoffeeIndex++;
				continue;
			}

			Begin();
			SDK::Transform.Move(SDK::MoveMatrix, CoffeeVec[i].Position);
			SDK::Transform.Scale(SDK::MoveMatrix, 0.45, 0.45);

			// 파괴된 커피는 파괴된 스프라이트를 렌더링한다
			if(CoffeeVec[i].Destroyed)
				SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.DestroyedCoffee, CoffeeVec[i].Type);
			else {
				if(i == CurrentCoffeeIndex && !CoffeeVec[CurrentCoffeeIndex].IsPeopleFront && !CoffeeVec[CurrentCoffeeIndex].IsXionFront && !ForTitle)
					SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.CoffeeBack, CoffeeVec[i].Type);
				SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Coffee, CoffeeVec[i].Type);
			}
		}
	

		// 다른 물건 렌더링
		size_t OtherVecSize = OtherVec.size();
		for (int i = StartOtherIndex; i < OtherVecSize; i++) {
			// 화면보다 오른쪽에 있는 물건은 렌더링을 건너뛴다.
			if (OtherVec[i].Position.x > SDK::Camera.Position.x + SDK::ASP(1.0) + 0.255)
				break;

			// 더 이상 보이지 않을 물건은 더 이상 렌더링 하지 않는다.
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

	// 가장 앞에 있는 커피의 위 또는 아래의 위치 여부를 얻는다
	// 위에 있을 시 true,  아래에 있을 시 false
	ItemStruct GetFrontCoffee() {
		return CoffeeVec[CurrentCoffeeIndex];
	}

	ItemStruct GetNextCoffee() {
		return CoffeeVec[CurrentCoffeeIndex + 1];
	}

	int GetCurrentIndex() {
		return CurrentCoffeeIndex;
	}

	// 커피를 부술 수 있는 상태로 전환한다
	void EnableCoffeeHit() {
		CoffeeVec[CurrentCoffeeIndex].IsPeopleFront = false;
		CoffeeVec[CurrentCoffeeIndex].IsXionFront = false;
	}

	int GetCoffeeSize() { 
		return CoffeeVec.size();
	}

	// 가장 앞에 있는 커피를 부순다.
	void BreakCoffee() {
		// 점수 추가
		if(auto Score = SDK::Scene.Find("score_indicator"); Score)
			Score->AddScore(10);

		// 사운드 3가지 중 1가지 랜덤 선택
		int RandomNum = SDK::Random.Gen(RANDOM_TYPE_INT, 0, 2);

		if(CoffeeVec[CurrentCoffeeIndex].Type == Coffee_Box)
			SDK::SoundTool.Play(SDK::SOUND.BoxBreak[RandomNum], SndChannel);

		else if(CoffeeVec[CurrentCoffeeIndex].Type == Coffee_Bottle)
			SDK::SoundTool.Play(SDK::SOUND.Bottle[RandomNum], SndChannel);

		else if(CoffeeVec[CurrentCoffeeIndex].Type == Coffee_Can)
			SDK::SoundTool.Play(SDK::SOUND.CanBreak[RandomNum], SndChannel);

		// 파괴한 품목 수 업데이트
		SDK::GLOBAL.DestroyedItems[CoffeeVec[CurrentCoffeeIndex].Type]++;

		// 종이 커피 이외의 커피를 부수면 커피가 터져나오는 애니메이션 객체를 추가한다
		if(CoffeeVec[CurrentCoffeeIndex].Type != Coffee_Box)
			SDK::Scene.AddObject(new Explode(CoffeeVec[CurrentCoffeeIndex].Position, false), "explode", LAYER5);
		// 종이 커피라면 커피 스틱이 터져나오는 애니메이션 객체를 추가한다
		else
			SDK::Scene.AddObject(new Explode(CoffeeVec[CurrentCoffeeIndex].Position, true), "explode", LAYER5);

		//캔커피라면 찌그러진 캔을 추가한다
		if (CoffeeVec[CurrentCoffeeIndex].Type == Coffee_Can)
			SDK::Scene.AddObject(new DestroyedCan(CoffeeVec[CurrentCoffeeIndex].Position), "destroyed_can", LAYER5);

		// 커피는 파괴 상태가 되고 더 이상 이드와 상호작용하지 않는다.
		CoffeeVec[CurrentCoffeeIndex].Destroyed = true;

		// 참조 인덱스 증가
		CurrentCoffeeIndex++;

		// 커피가 모두 부숴지면 자신의 태그를 지워 다음 선반이 검색되도록 한다.
		if (CurrentCoffeeIndex == NumShelf * 4 - 1)
			ObjectTag = "";
	}
};