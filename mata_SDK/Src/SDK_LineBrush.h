#pragma once
#include "SDK_Header.h"

namespace SDK {
	class LineBrush {
	private:
		float   Rotation{};
		float   Length{};

		SDK::Color3 Color{};
		float   Opacity{};

		int       RenderType{ RENDER_TYPE_DEFAULT };
		int       LineType{ LINE_TYPE_RECT };

		bool      StaticWidthCommand{};

	public:
		LineBrush(bool StaticWidthFlag = false);
		void SetRenderType(int Opt);
		void SetColor(float R, float G, float B);
		void SetColor(SDK::Color3& ColorValue);
		void SetColorRGB(int R, int G, int B);
		void SetLineType(int LineTypeOpt);
		void Draw(float X1, float Y1, float X2, float Y2, float Width, float OpacityValue = 1.0);
		void DrawLineX(float X1, float X2, float Y, float Width, float OpacityValue = 1.0);
		void DrawLineY(float Y1, float Y2, float X, float Width, float OpacityValue = 1.0);

	private:
		void Render();
		void DrawCircle(float X1, float Y1, float X2, float Y2, float Width);
		void RenderCircle(float Width);
	};
}