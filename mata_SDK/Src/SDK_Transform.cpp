#include "SDK_Transform.h"
#include "SDK_ObjectValue.h"

SDK::SDK_Transform SDK::Transform;

void SDK::SDK_Transform::Identity(SDK::Matrix4& Matrix) {
	Matrix = SDK::Matrix4(1.0f);
}

void SDK::SDK_Transform::Move(SDK::Matrix4& Matrix, float X, float Y) {
	Matrix = translate(Matrix, SDK::Vector3(X, Y, 0.0));
}

void SDK::SDK_Transform::Move(SDK::Matrix4& Matrix, SDK::Vector2& Position) {
	Matrix = translate(Matrix, SDK::Vector3(Position.x, Position.y, 0.0));
}

void SDK::SDK_Transform::Rotate(SDK::Matrix4& Matrix, float Degree) {
	Matrix = rotate(Matrix, glm::radians(-Degree), SDK::Vector3(0.0, 0.0, 1.0));
}

void SDK::SDK_Transform::RotateRadians(SDK::Matrix4& Matrix, float Radians) {
	Matrix = rotate(Matrix, -Radians, SDK::Vector3(0.0, 0.0, 1.0));
}

void SDK::SDK_Transform::RotateV(SDK::Matrix4& Matrix, float Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), SDK::Vector3(1.0, 0.0, 0.0));
}

void SDK::SDK_Transform::RotateH(SDK::Matrix4& Matrix, float Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), SDK::Vector3(0.0, 1.0, 0.0));
}

void SDK::SDK_Transform::Scale(SDK::Matrix4& Matrix, float X, float Y) {
	Matrix = scale(Matrix, SDK::Vector3(X, Y, 1.0));
}

void SDK::SDK_Transform::Scale(SDK::Matrix4& Matrix, SDK::Vector2& Size) {
	Matrix = scale(Matrix, SDK::Vector3(Size.x, Size.y, 1.0));
}

void SDK::SDK_Transform::Tilt(SDK::Matrix4& Matrix, float X, float Y) {
	Matrix[1][0] += X;
	Matrix[0][1] += Y;
}

void SDK::SDK_Transform::Tilt(SDK::Matrix4& Matrix, SDK::Vector2& Value) {
	Matrix[1][0] += Value.x;
	Matrix[0][1] += Value.y;
}

void SDK::SDK_Transform::ImageScale(SDK::Matrix4& Matrix, float Width, float Height) {
	if (Width > Height)
		Matrix = glm::scale(Matrix, SDK::Vector3(1.0, Height / Width, 1.0));
	else if (Width < Height)
		Matrix = glm::scale(Matrix, SDK::Vector3(Width / Height, 1.0, 1.0));
}

void SDK::SDK_Transform::Flip(int FlipFlag) {
	switch (FlipFlag) {
	case FLIP_TYPE_NONE:
		Identity(FlipMatrix);
		break;

	case FLIP_TYPE_H:
		RotateH(FlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_V:
		RotateV(FlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_HV:
		RotateH(FlipMatrix, 180.0f);
		RotateV(FlipMatrix, 180.0f);
		break;
	}
}

void SDK::SDK_Transform::UnitFlip(int FlipFlag) {
	switch (FlipFlag) {
	case FLIP_TYPE_NONE:
		Identity(UnitFlipMatrix);
		break;

	case FLIP_TYPE_H:
		RotateH(UnitFlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_V:
		RotateV(UnitFlipMatrix, 180.0f);
		break;

	case FLIP_TYPE_HV:
		RotateH(UnitFlipMatrix, 180.0f);
		RotateV(UnitFlipMatrix, 180.0f);
		break;
	}
}

bool SDK::SDK_Transform::CheckIdentity(SDK::Matrix4& Matrix) {
	if (std::memcmp(&Matrix, &SDK::IdentityMatrix, sizeof(SDK::Matrix4)) != 0)
		return false;
	
	return true;
}