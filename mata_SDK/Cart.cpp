#include "Cart.h"

Cart::Cart(bool Fell, SDK::Vector2 PositionValue) {
	FellDown = Fell;
	Position = PositionValue;

	// �Ѿ��� ������ ��� 1�� �������� �������Ѵ�
	if (Fell)
		Frame = 1;
}

void Cart::UpdateFunc(float FrameTime) {
	// ȭ�� ���� ��� ��� ����
	if (Position.x < SDK::Camera.Position.x - SDK::ASP(1.0) - 0.75)
		SDK::Scene.DeleteObject(this);
}

void Cart::RenderFunc() {
	if (Position.x > SDK::Camera.Position.x + SDK::ASP(1.0) + 0.75)
		return;

	Begin();
	SDK::Transform.Move(SDK::MoveMatrix, Position);
	SDK::Transform.Scale(SDK::MoveMatrix, 1.5, 1.5);
	SDK::ImageTool.RenderStaticSpriteSheet(SDK::IMAGE.Cart, Frame);
}