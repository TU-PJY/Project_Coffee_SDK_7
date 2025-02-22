#pragma once
#include "SDK_Header.h"

namespace SDK {
	class LineRectBrush {
	private:
		SDK::Color3 Color{};
		float   Length{};
		float   Opacity{};
		int       RenderType{ RENDER_TYPE_DEFAULT };

		bool      CamInheritanceCommand{};
		bool      StaticWidthCommand{};

	public:
		LineRectBrush(bool CameraInheritanceFlag = false, bool StaticWidthFlag = false);
		void SetColor(float R, float G, float B);
		void SetColor(SDK::Color3& ColorValue);
		void SetColorRGB(int R, int G, int B);
		void SetRenderType(int Opt);
		void Draw(float X, float Y, float SizeX, float SizeY, float Width, float RotationValue = 0.0f, float OpacityValue = 1.0f);

	private:
		void DrawLine(float X, float Y, float OffsetX, float OffsetY, float Width, float Height, float RotationValue);
		void Render();
	};


	class RectBrush {
	private:
		SDK::Color3 Color{};
		float   Length{};
		float   Opacity{};
		int       RenderType{ RENDER_TYPE_DEFAULT };

		bool      CamInheritanceCommand{};

	public:
		RectBrush(bool Flag = false);
		void SetColor(float R, float G, float B);
		void SetColor(SDK::Color3& ColorValue);
		void SetColorRGB(int R, int G, int B);
		void SetRenderType(int Opt);
		void Draw(float X, float Y, float SizeX, float SizeY, float RotationValue = 0.0f, float OpacityValue = 1.0f);

	private:
		void Render();
	};
}