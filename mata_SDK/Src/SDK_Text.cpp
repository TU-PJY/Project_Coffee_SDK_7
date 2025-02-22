#include "SDK_Text.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SDK_ComputeTool.h"
#include "SDK_StringTool.h"

glm::mat4 TextMatrix;

void SDK::Text::Init(SDK::FontName FontName, int Type, int Italic) {
	hDC = wglGetCurrentDC();
	FontBase = glGenLists(65536);

	Font = CreateFont(
		-1, 0, 0, 0, Type, Italic, FALSE, FALSE, DEFAULT_CHARSET, OUT_RASTER_PRECIS, CLIP_DEFAULT_PRECIS,
		NONANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, FontName
	);

	LineLengthBuffer.reserve(20);
}

void SDK::Text::SetRenderType(int Type) {
	RenderType = Type;
}

void SDK::Text::SetAlign(int AlignOpt) {
	TextAlign = AlignOpt;
}

void SDK::Text::SetLineGap(float Value) {
	TextLineGap = Value;
}

void SDK::Text::EnableFixMiddle() {
	FixMiddleCommand = true;
}

void SDK::Text::DisableFixMiddle() {
	FixMiddleCommand = false;
}

void SDK::Text::SetHeightAlign(int Type) {
	HeightAlign = Type;
}

void SDK::Text::EnableShadow() {
	ShadowRenderCommand = true;
}

void SDK::Text::DisableShadow() {
	ShadowRenderCommand = false;
}

void SDK::Text::SetShadow(float OffsetX, float OffsetY, float Opacity, SDK::Color3 Color) {
	ShadowOffset.x = OffsetX;
	ShadowOffset.y = OffsetY;
	ShadowOpacity = Opacity;
	ShadowColor = Color;
}

void SDK::Text::SetColor(float R, float G, float B) {
	TextColor.r = R;
	TextColor.g = G;
	TextColor.b = B;
}

void SDK::Text::SetColor(SDK::Color3& Color) {
	TextColor.r = Color.r;
	TextColor.g = Color.g;
	TextColor.b = Color.b;
}

void SDK::Text::SetColorRGB(int R, int G, int B) {
	TextColor.r = (1.0f / 255.0f) * (float)R;
	TextColor.g = (1.0f / 255.0f) * (float)G;
	TextColor.b = (1.0f / 255.0f) * (float)B;
}

void SDK::Text::Rotate(float RotationValue) {
	Rotation = RotationValue;
}

void SDK::Text::SetOpacity(float Value) {
	TextOpacity = Value;
}

void SDK::Text::Render(SDK::Vector2& Position, float Size, const wchar_t* Fmt, ...) {
	if (Fmt == NULL)
		return;

	TextVec.clear();

	va_list Args{};
	va_start(Args, Fmt);

	int CurrentSize = vswprintf(nullptr, 0, Fmt, Args) + 1;
	if (PrevSize < CurrentSize) {
		TextVec.resize(CurrentSize);
		PrevSize = CurrentSize;
	}

	vswprintf(TextVec.data(), CurrentSize, Fmt, Args);
	va_end(Args);

	InputText(TextVec, Position, Size);
}

void SDK::Text::Render(float X, float Y, float Size, const wchar_t* Fmt, ...) {
	if (Fmt == NULL)
		return;

	TextVec.clear();

	va_list Args{};
	va_start(Args, Fmt);

	int CurrentSize = vswprintf(nullptr, 0, Fmt, Args) + 1;
	if (PrevSize < CurrentSize) {
		TextVec.resize(CurrentSize);
		PrevSize = CurrentSize;
	}

	vswprintf(TextVec.data(), CurrentSize, Fmt, Args);
	va_end(Args);

	InputText(TextVec, SDK::Vector2(X, Y), Size);
}

void SDK::Text::RenderString(SDK::Vector2& Position, float Size, std::string Str) {
	Render(Position.x, Position.y, Size, SDK::StringTool.Wstring(Str).c_str());
}

void SDK::Text::RenderString(float X, float Y, float Size, std::string Str) {
	Render(X, Y, Size, SDK::StringTool.Wstring(Str).c_str());
}

void SDK::Text::RenderWString(SDK::Vector2& Position, float Size, std::wstring WStr) {
	Render(Position.x, Position.y, Size, WStr.c_str());
}

void SDK::Text::RenderWString(float X, float Y, float Size, std::wstring WStr) {
	Render(X, Y, Size, WStr.c_str());
}

////////////////// private
void SDK::Text::InputText(std::vector<wchar_t>& Input, SDK::Vector2& Position, float Size) {
	CurrentText = std::wstring(Input.data());

	if (ShadowRenderCommand) {
		RenderColor = ShadowColor;
		RenderOpacity = TextOpacity * ShadowOpacity;
		ProcessText((wchar_t*)CurrentText.c_str(), SDK::Vector2(Position.x + ShadowOffset.x * Size, Position.y + ShadowOffset.y * Size), Size);
	}

	RenderColor = TextColor;
	RenderOpacity = TextOpacity;
	ProcessText((wchar_t*)CurrentText.c_str(), Position, Size);
}

void SDK::Text::ProcessText(wchar_t* Text, SDK::Vector2& Position, float Size) {
	CurrentLine = 0;
	TextRenderSize = Size;
	RenderPosition = Position;
	CurrentRenderOffset = SDK::Vector2(0.0, 0.0);

	if (CurrentText.compare(PrevText) != 0) {
		TextWordCount = wcslen(Text);
		ComputeGlyphCache(Text);
		PrevText = CurrentText;
	}

	ComputeTextLength(Text);

	switch (HeightAlign) {
	case HEIGHT_ALIGN_MIDDLE:
		RenderPosition.y -= TextRenderSize * 0.5;
		break;

	case HEIGHT_ALIGN_UNDER:
		RenderPosition.y -= TextRenderSize;
		break;
	}

	for (int i = 0; i < TextWordCount; ++i) {
		if (Text[i] == L'\n') {
			CurrentRenderOffset.x = 0.0;
			CurrentRenderOffset.y -= (TextLineGap + TextRenderSize);

			if (TextAlign != ALIGN_DEFAULT) {
				++CurrentLine;
				TextLength = LineLengthBuffer[CurrentLine];
			}
			continue;
		}

		TransformText();
		Camera.SetCamera(RenderType);
		PrepareRender();

		glPushAttrib(GL_LIST_BIT);
		glListBase(FontBase);
		glCallLists(1, GL_UNSIGNED_SHORT, &Text[i]);
		glPopAttrib();

		if (Text[i] < 65536)
			CurrentRenderOffset.x += TextGlyph[Text[i]].gmfCellIncX * (TextRenderSize / 1.0f);
	}
}

void SDK::Text::ComputeGlyphCache(wchar_t* Text) {
	for (int i = 0; i < TextWordCount; ++i) {
		if (GlyphCache.count(Text[i]) == 0) {
			CreateNewGlyph(Text[i]);
			GlyphCache.insert(Text[i]); 
		}
	}
}

void SDK::Text::CreateNewGlyph(wchar_t& Char) {
	if (Char >= 65536)
		return;

	HFONT OldFont = (HFONT)SelectObject(hDC, Font);
	GLYPHMETRICSFLOAT Glyph;
	wglUseFontOutlinesW(hDC, Char, 1, FontBase + Char, 0.0f, 0.0f, WGL_FONT_POLYGONS, &Glyph);
	TextGlyph[Char] = Glyph;
	SelectObject(hDC, OldFont);
}

void SDK::Text::ComputeTextLength(const wchar_t* Text) {
	LineLengthBuffer.clear();
	float CurrentLineLength{};

	for (int i = 0; i < wcslen(Text); ++i) {
		if (Text[i] == L'\n') {
			LineLengthBuffer.emplace_back(CurrentLineLength);
			CurrentLineLength = 0.0f;
			continue;
		}

		unsigned int CharIndex = Text[i];
		if (CharIndex < 65536)
			CurrentLineLength += TextGlyph[CharIndex].gmfCellIncX * (TextRenderSize / 1.0f);
	}

	if (CurrentLineLength > 0.0f)
		LineLengthBuffer.emplace_back(CurrentLineLength);


	TextLength = LineLengthBuffer.front();

	MiddleHeight = 0.0;
	if (FixMiddleCommand) {
		size_t LineNum = LineLengthBuffer.size();

		if (LineNum > 1) {
			for (int i = 0; i < LineNum; ++i)
				MiddleHeight += (TextLineGap + TextRenderSize);
			MiddleHeight /= 2.0;
		}
	}
}

void SDK::Text::TransformText() {
	SDK::Transform.Identity(TextMatrix);
	SDK::Transform.Move(TextMatrix, RenderPosition.x, RenderPosition.y + MiddleHeight);

	switch (TextAlign) {
	case ALIGN_DEFAULT:
		if(Rotation != 0.0)
			SDK::Transform.Rotate(TextMatrix, Rotation);
		SDK::Transform.Move(TextMatrix, CurrentRenderOffset);
		break;

	case ALIGN_MIDDLE:
		if (Rotation != 0.0)
			SDK::Transform.Rotate(TextMatrix, Rotation);
		SDK::Transform.Move(TextMatrix, -TextLength * 0.5, 0.0);
		SDK::Transform.Move(TextMatrix, CurrentRenderOffset);
		break;

	case ALIGN_LEFT:
		if (Rotation != 0.0)
			SDK::Transform.Rotate(TextMatrix, Rotation);
		SDK::Transform.Move(TextMatrix, -TextLength, 0.0);
		SDK::Transform.Move(TextMatrix, CurrentRenderOffset);
		break;
	}

	SDK::Transform.Scale(TextMatrix, TextRenderSize, TextRenderSize);
}

void SDK::Text::PrepareRender() {
	glUseProgram(TEXT_SHADER);
	Camera.PrepareRender(SHADER_TYPE_TEXT);

	glUniform1f(TEXT_OPACITY_LOCATION, RenderOpacity);
	glUniform3f(TEXT_COLOR_LOCATION, RenderColor.r, RenderColor.g, RenderColor.b);
	glUniformMatrix4fv(TEXT_MODEL_LOCATION, 1, GL_FALSE, value_ptr(TextMatrix));
}

SDK::Text::~Text() {
	HFONT OldFont = (HFONT)SelectObject(hDC, Font);
	SelectObject(hDC, OldFont);
	DeleteObject(Font);
	glDeleteLists(FontBase, 65536);
	DeleteDC(hDC);
}