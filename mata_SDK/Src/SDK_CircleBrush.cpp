#include "SDK_CircleBrush.h"
#include "SDK_Camera.h"
#include "SDK_Transform.h"
#include "SDK_SystemResource.h"

glm::mat4 CircleMatrix;

SDK::CircleBrush::CircleBrush(bool CamInheritanceFlag) {
	CamInheritanceCommand = CamInheritanceFlag;
}

void SDK::CircleBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void SDK::CircleBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void SDK::CircleBrush::SetColor(SDK::Color3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void SDK::CircleBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void SDK::CircleBrush::Draw(float X, float Y, float Diameter, float OpacityValue) {
	Transform.Identity(CircleMatrix);
	Opacity = 1.0f;

	Transform.Move(CircleMatrix, X, Y);
	Opacity = OpacityValue;
	Radius = Diameter * 0.5;

	Render();
}

void SDK::CircleBrush::Render() {
	if (!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(CircleMatrix));

	gluDisk(SDK::SYSRES.GLU_CIRCLE, 0.0, Radius, 80, 1);
}



SDK::LineCircleBrush::LineCircleBrush(bool CamInheritanceFlag, bool StaticWidthFlag) {
	CamInheritanceCommand = CamInheritanceFlag;
	StaticWidthCommand = StaticWidthFlag;
}

void SDK::LineCircleBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void SDK::LineCircleBrush::SetColor(float R, float G, float B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void SDK::LineCircleBrush::SetColor(SDK::Color3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void SDK::LineCircleBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (float)R;
	Color.g = (1.0f / 255.0f) * (float)G;
	Color.b = (1.0f / 255.0f) * (float)B;
}

void SDK::LineCircleBrush::Draw(float X, float Y, float Diameter, float Width, float OpacityValue) {
	Transform.Identity(CircleMatrix);
	Opacity = 1.0f;

	Transform.Move(CircleMatrix, X, Y);
	Opacity = OpacityValue;
	Radius = Diameter * 0.5;

	if(RenderType == RENDER_TYPE_DEFAULT && StaticWidthCommand)
		WidthValue = Width / Camera.Zoom;
	else if ((RenderType == RENDER_TYPE_DEFAULT && !StaticWidthCommand) || RenderType == RENDER_TYPE_STATIC)
		WidthValue = Width;

	Render();
}

void SDK::LineCircleBrush::Render() {
	if (!CamInheritanceCommand)
		Camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	Camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(CircleMatrix));

	gluDisk(SDK::SYSRES.GLU_LINE_CIRCLE, Radius - WidthValue * 0.5, Radius + WidthValue * 0.5, 80, 1);
}