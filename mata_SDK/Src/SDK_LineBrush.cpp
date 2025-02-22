#include "SDK_LineBrush.h"
#include "SDK_Transform.h"
#include "SDK_Math.h"
#include "SDK_ImageTool.h"
#include "SDK_Shader.h"
#include "SDK_Camera.h"
#include "SDK_SystemResource.h"
#include "SDK_ComputeTool.h"

glm::mat4 LineMatrix;

SDK::LineBrush::LineBrush(bool StaticWidthFlag) {
	StaticWidthCommand = StaticWidthFlag;
}

void SDK::LineBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void SDK::LineBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void SDK::LineBrush::SetColor(SDK::Color3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void SDK::LineBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void SDK::LineBrush::SetLineType(int LineTypeOpt) {
	if (LineTypeOpt > LINE_TYPE_ROUND)
		return;

	LineType = LineTypeOpt;
}

void SDK::LineBrush::Draw(float X1, float Y1, float X2, float Y2, float Width, float OpacityValue) {
	Transform.Identity(LineMatrix);
	Opacity = OpacityValue;

	Length = Math.ComputeDistance(X1, Y1, X2, Y2);
	Rotation = Math.ComputeRadians(X1, Y1, X2, Y2);

	Transform.Move(LineMatrix, X1, Y1);
	Transform.RotateRadians(LineMatrix, Rotation);
	Transform.Move(LineMatrix, Length / 2.0, 0.0);

	float DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Width / Camera.Zoom;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Width;

	if (LineType == LINE_TYPE_RECT)
		Transform.Scale(LineMatrix, Length + DrawWidth, DrawWidth);
	else if (LineType == LINE_TYPE_ROUND) 
		Transform.Scale(LineMatrix, Length, DrawWidth);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X1, Y1, X2, Y2, DrawWidth);
}

void SDK::LineBrush::DrawLineX(float X1, float X2, float Y, float Width, float OpacityValue) {
	Transform.Identity(LineMatrix);
	Opacity = OpacityValue;

	float DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Width / Camera.Zoom;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Width;

	Transform.Move(LineMatrix, (X1 + X2) / 2.0, Y);
	if (LineType == LINE_TYPE_RECT)
		Transform.Scale(LineMatrix, fabs(X1 - X2) + DrawWidth, DrawWidth);
	else if (LineType == LINE_TYPE_ROUND) 
		Transform.Scale(LineMatrix, fabs(X1 - X2), DrawWidth);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X1, Y, X2, Y, DrawWidth);
}

void SDK::LineBrush::DrawLineY(float Y1, float Y2, float X, float Width, float OpacityValue) {
	Transform.Identity(LineMatrix);
	Opacity = OpacityValue;

	float DrawWidth{};
	if (RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		DrawWidth = Width / Camera.Zoom;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		DrawWidth = Width;

	Transform.Move(LineMatrix, X, (Y1 + Y2) / 2.0);
	if (LineType == LINE_TYPE_RECT)
		Transform.Scale(LineMatrix, DrawWidth, fabs(Y1 - Y2) + DrawWidth);
	else if (LineType == LINE_TYPE_ROUND)
		Transform.Scale(LineMatrix, fabs(Y1 - Y2), DrawWidth);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X, Y1, X, Y2, DrawWidth);
}

void SDK::LineBrush::Render() {
	Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(LineMatrix));

	SDK::ImageTool.RenderRaw();
}

void SDK::LineBrush::DrawCircle(float X1, float Y1, float X2, float Y2, float Width) {
	Transform.Identity(LineMatrix);
	Transform.Move(LineMatrix, X1, Y1);
	RenderCircle(Width);

	Transform.Identity(LineMatrix);
	Transform.Move(LineMatrix, X2, Y2);
	RenderCircle(Width);
}

void SDK::LineBrush::RenderCircle(float Width) {
	Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(LineMatrix));

	gluDisk(SDK::SYSRES.GLU_CIRCLE, 0.0, Width * 0.5, 80, 1);
}
