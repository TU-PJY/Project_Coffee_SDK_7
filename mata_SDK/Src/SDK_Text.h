#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SDK_Header.h"
#include <windows.h>
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace SDK {
	class Text {
	private:
		SDK::Vector2             RenderPosition{};
		SDK::Vector2             CurrentRenderOffset{};
		float					 Rotation{};

		size_t                   TextWordCount{};
		float                    TextLength{};
		float                    TextRenderSize{};
		float                    TextLineGap{};
		float                    MiddleHeight{};

		std::vector<float>		 LineLengthBuffer{};
		std::wstring             CurrentText{};
		std::wstring             PrevText{};
		int                      CurrentLine{};

		float					 TextOpacity{ 1.0f };
		glm::vec3                TextColor{ glm::vec3(1.0, 1.0, 1.0) };

		bool                     ShadowRenderCommand{};
		float					 ShadowOpacity{};
		SDK::Vector2             ShadowOffset{};
		glm::vec3                ShadowColor{};

		glm::vec3                RenderColor{};
		float					 RenderOpacity{};

		HDC                      hDC{};
		HFONT                    Font{};
		unsigned int             FontBase{};
		std::unordered_map <wchar_t, GLYPHMETRICSFLOAT> TextGlyph{};
		std::unordered_set <wchar_t> GlyphCache{};
		std::vector<wchar_t>     TextVec{};
		int                      PrevSize{};

		int                      TextAlign{ ALIGN_DEFAULT };
		int                      RenderType{ RENDER_TYPE_STATIC };
		int                      HeightAlign{ HEIGHT_ALIGN_DEFAULT };
		bool                     FixMiddleCommand{};

	public:
		~Text();
		void Init(SDK::FontName FontName, int Type=FW_DONTCARE, int Italic = FALSE);
		void SetColor(float R, float G, float B);
		void SetColor(SDK::Color3& Color);
		void SetColorRGB(int R, int G, int B);
		void SetAlign(int AlignOpt);
		void SetLineGap(float Value);
		void EnableFixMiddle();
		void DisableFixMiddle();
		void SetHeightAlign(int Type);
		void EnableShadow();
		void DisableShadow();
		void SetShadow(float OffsetX, float OffsetY, float Opacity, SDK::Color3 Color = SDK::Color3(0.0, 0.0, 0.0));
		void Rotate(float RotationValue);
		void SetOpacity(float Value);
		void Render(SDK::Vector2& Position, float Size, const wchar_t* Fmt, ...);
		void Render(float X, float Y, float Size, const wchar_t* Fmt, ...);
		void RenderString(SDK::Vector2& Position, float Size, std::string Str);
		void RenderString(float X, float Y, float Size, std::string Str);
		void RenderWString(SDK::Vector2& Position, float Size, std::wstring Str);
		void RenderWString(float X, float Y, float Size, std::wstring Str);
		void SetRenderType(int Type);

	private:
		void InputText(std::vector<wchar_t>& Input, SDK::Vector2& Position, float Size);
		void ProcessText(wchar_t* Text, SDK::Vector2& Position, float Size);
		void ComputeTextLength(const wchar_t* Text);
		void ComputeGlyphCache(wchar_t* Text);
		void TransformText();
		void PrepareRender();
		void CreateNewGlyph(wchar_t& Char);
	};
}