#include "SDK_UITool.h"

SDK::SDK_UITool SDK::UITool;

void SDK::SDK_UITool::ClampMatrixToCorner(int EdgePosition, SDK::Matrix4& Matrix, float Width, float Height, float GapX, float GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		Matrix = translate(Matrix, SDK::Vector3(SDK::WindowRect.LeftX + Width / 2.0 + GapX, SDK::WindowRect.LeftY + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_LEFT_UP:
		Matrix = translate(Matrix, SDK::Vector3(SDK::WindowRect.LeftX + Width / 2.0 + GapX, SDK::WindowRect.RightY - Height / 2.0 - GapY, 0.0));
		break;

	case EDGE_RIGHT_DOWN:
		Matrix = translate(Matrix, SDK::Vector3(SDK::WindowRect.RightX - Width / 2.0 - GapX, SDK::WindowRect.LeftY + Height / 2.0 + GapY, 0.0));
		break;

	case EDGE_RIGHT_UP:
		Matrix = translate(Matrix, SDK::Vector3(SDK::WindowRect.RightX - Width / 2.0 - GapX, SDK::WindowRect.RightY - Height / 2.0 - GapY, 0.0));
		break;
	}
}

void SDK::SDK_UITool::ClampMatrixToPosition(SDK::Matrix4& Matrix, float PositionX, float PositionY, float Width, float Height, float GapX, float GapY) {
	Matrix = translate(Matrix, SDK::Vector3(PositionX + Width / 2.0 + GapX, PositionY + Height / 2.0 + GapY, 0.0));
}

void SDK::SDK_UITool::ClampPositionToCorner(int EdgePosition, float& PositionX, float& PositionY, float Width, float Height, float GapX, float GapY) {
	switch (EdgePosition) {
	case EDGE_LEFT_DOWN:
		PositionX = SDK::WindowRect.LeftX + Width / 2.0 + GapX;
		PositionY = SDK::WindowRect.LeftY + Height / 2.0 + GapY;
		break;

	case EDGE_LEFT_UP:
		PositionX = SDK::WindowRect.LeftX + Width / 2.0 + GapX;
		PositionY = SDK::WindowRect.RightY - Height / 2.0 - GapY;
		break;

	case EDGE_RIGHT_DOWN:
		PositionX = SDK::WindowRect.RightX - Width / 2.0 - GapX;
		PositionY = SDK::WindowRect.LeftY + Height / 2.0 + GapY;
		break;

	case EDGE_RIGHT_UP:
		PositionX = SDK::WindowRect.RightX - Width / 2.0 - GapX;
		PositionY = SDK::WindowRect.RightY - Height / 2.0 - GapY;
		break;
	}
}

void SDK::SDK_UITool::ClampPositionToCoord(float& ThisPositionX, float& ThisPositionY, float OtherPositionX, float OtherPositionY, float Width, float Height, float GapX, float GapY) {
	ThisPositionX = OtherPositionX + Width / 2.0 + GapX;
	ThisPositionY = OtherPositionY + Height / 2.0 + GapY;
}