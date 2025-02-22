#pragma once

// 이드 프레임
enum ED_StateEnum {
	ED_Idle,
	ED_HitHigh1,
	ED_HitHigh2,
	ED_HitLow1,
	ED_HitLow2,
	ED_KickPeople,
	ED_GameOver,
	ED_Title,
	ED_Happy
};

// 시온 프레임
enum XionStateEnum {
	Xion_Nervous,
	Xion_Cry1,
	Xion_Curious,
	Xion_Blocking,
	Xion_Cry2,
	Xion_Hurt,
	Xion_Happy
};

// 사람 프레임
enum PeopleTypeEnum {
	Listy,
	Mayo,
	Naia,
	Hildae,
	Silphir,
	Balong,
	Kidian,
	Melloon,
	Yumimi,
	Daepyo,
	EOE
};

// 커피 종류
enum CoffeeTypeEnum {
	Coffee_Box,
	Coffee_Bottle,
	Coffee_Can
};

// 파괴된 커피 종류
enum DestroyedTypeEnum {
	Item_BoxCoffee,
	Item_BottleCoffee,
	Item_CanCoffee,
	Item_People,
	Item_Xion,
	Item_HouseDoc
};

// 엔딩 분기
enum GameOverEndingEnum {
	GameOver_TimeOut,
	GameOver_HitXion,
	GameOver_Suppressed
};