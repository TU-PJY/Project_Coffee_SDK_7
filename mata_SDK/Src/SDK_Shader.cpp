#include "SDK_Shader.h"
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <fstream>

SDK::SDK_Shader SDK::Shader;

SDK::ShaderResource IMAGE_SHADER;
SDK::ShaderResource TEXT_SHADER;
SDK::ShaderResource SHAPE_SHADER;
SDK::ShaderResource MATRIX_COMPT_SHADER;

SDK::SSBO SSBO_MATRIX_INPUT, SSBO_MATRIX_OUTPUT;

SDK::ShaderLocation IMAGE_OPACITY_LOCATION, IMAGE_COLOR_LOCATION, IMAGE_MODEL_LOCATION;
SDK::ShaderLocation IMAGE_PROJECTION_LOCARION, IMAGE_VIEW_LOCATION, IMAGE_VIEW_POSITION_LOCATION;
SDK::ShaderLocation BLUR_STRENGTH_LOCATION, BLUR_STATE_LOCATION, BLUR_EXECUTION_LOCATION, TEXTURE_SIZE_LOCATION;

SDK::ShaderLocation TEXT_OPACITY_LOCATION, TEXT_COLOR_LOCATION, TEXT_MODEL_LOCATION;
SDK::ShaderLocation TEXT_PROJECTION_LOCATION, TEXT_VIEW_LOCATION, TEXT_VIEW_POSITION_LOCATION;

SDK::ShaderLocation SHAPE_OPACITY_LOCATION, SHAPE_COLOR_LOCATION, SHAPE_MODEL_LOCATION;
SDK::ShaderLocation SHAPE_PROJECTION_LOCATION, SHAPE_VIEW_LOCATION, SHAPE_VIEW_POSITION_LOCATION;


char* SDK::SDK_Shader::LoadShaderFile(std::string FileName) {
	std::ifstream ShaderFile(FileName, std::ios::in | std::ios::binary | std::ios::ate);
	if (!ShaderFile.is_open()) {
		std::cout << "Can not open shader file: " << FileName << std::endl;
		exit(EXIT_FAILURE);
	}

	std::streamsize Length = ShaderFile.tellg();
	ShaderFile.seekg(0, std::ios::beg);

	char* Buffer = new char[Length + 1];

	if (!ShaderFile.read(Buffer, Length)) {
		delete[] Buffer;
		ShaderFile.close();
		std::cout << "Can not read shader file: " << FileName << std::endl;
		exit(EXIT_FAILURE);
	}

	Buffer[Length] = '\0';

	ShaderFile.close();

	return Buffer;
}

void SDK::SDK_Shader::LoadVertexShader(std::string VertexShader) {
	VertexShaderSource = LoadShaderFile(VertexShader);
	VertexShaderBuffer = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderBuffer, 1, (const char**)&VertexShaderSource, 0);
	glCompileShader(VertexShaderBuffer);

	int Result{};
	char ErrorLog[512]{};

	glGetShaderiv(VertexShaderBuffer, GL_COMPILE_STATUS, &Result);

	if (!Result) {
		glGetShaderInfoLog(VertexShaderBuffer, 512, NULL, ErrorLog);
		std::cout << "ERROR: vertex shader error\n" << ErrorLog << std::endl;

		return;
	}
}

void SDK::SDK_Shader::LoadFragmentShader(std::string FragmentShader) {
	FragmentShaderSource = LoadShaderFile(FragmentShader);
	FragmentShaderBuffer = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderBuffer, 1, (const char**)&FragmentShaderSource, 0);
	glCompileShader(FragmentShaderBuffer);

	int Result{};
	char ErrorLog[512]{};

	glGetShaderiv(FragmentShaderBuffer, GL_COMPILE_STATUS, &Result);

	if (!Result) {
		glGetShaderInfoLog(FragmentShaderBuffer, 512, NULL, ErrorLog);
		std::cout << "ERROR: fragment shader error\n" << ErrorLog << std::endl;

		return;
	}
}

void SDK::SDK_Shader::LoadComputeShader(std::string ComputeShader) {
	ComputeShaderSource = LoadShaderFile(ComputeShader);
	CommputeShaderBuffer = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(CommputeShaderBuffer, 1, (const char**)&ComputeShaderSource, 0);
	glCompileShader(CommputeShaderBuffer);

	int Result{};
	char ErrorLog[512]{};

	glGetShaderiv(CommputeShaderBuffer, GL_COMPILE_STATUS, &Result);

	if (!Result) {
		glGetShaderInfoLog(CommputeShaderBuffer, 512, NULL, ErrorLog);
		std::cout << "ERROR: compute shader error\n" << ErrorLog << std::endl;

		return;
	}
}

void SDK::SDK_Shader::CreateShader(SDK::ShaderResource& Shader) {
	Shader = glCreateProgram();
	glAttachShader(Shader, VertexShaderBuffer);
	glAttachShader(Shader, FragmentShaderBuffer);
	glLinkProgram(Shader);
	glDeleteShader(VertexShaderBuffer);
	glDeleteShader(FragmentShaderBuffer);
}

void SDK::SDK_Shader::CreateComputeShader(SDK::ShaderResource& Shader) {
	Shader = glCreateProgram();
	glAttachShader(Shader, CommputeShaderBuffer);
	glLinkProgram(Shader);
	glDeleteShader(CommputeShaderBuffer);
}

void SDK::SDK_Shader::CreateShaderLocation() {
	// Image Shader
	IMAGE_OPACITY_LOCATION       = glGetUniformLocation(IMAGE_SHADER, "Opacity");
	IMAGE_COLOR_LOCATION         = glGetUniformLocation(IMAGE_SHADER, "Color");
	IMAGE_MODEL_LOCATION         = glGetUniformLocation(IMAGE_SHADER, "ModelMatrix");
	BLUR_STRENGTH_LOCATION       = glGetUniformLocation(IMAGE_SHADER, "BlurStrength");
	BLUR_STATE_LOCATION          = glGetUniformLocation(IMAGE_SHADER, "BlurState");
	BLUR_EXECUTION_LOCATION      = glGetUniformLocation(IMAGE_SHADER, "BlurExecution");
	TEXTURE_SIZE_LOCATION        = glGetUniformLocation(IMAGE_SHADER, "TextureSize");

	// Text Shader
	TEXT_OPACITY_LOCATION        = glGetUniformLocation(TEXT_SHADER, "Opacity");
	TEXT_COLOR_LOCATION          = glGetUniformLocation(TEXT_SHADER, "Color");
	TEXT_MODEL_LOCATION          = glGetUniformLocation(TEXT_SHADER, "ModelMatrix");

	// Shape Shader
	SHAPE_OPACITY_LOCATION       = glGetUniformLocation(SHAPE_SHADER, "Opacity");
	SHAPE_COLOR_LOCATION         = glGetUniformLocation(SHAPE_SHADER, "Color");
	SHAPE_MODEL_LOCATION         = glGetUniformLocation(SHAPE_SHADER, "ModelMatrix");

	// Image Camera
	IMAGE_PROJECTION_LOCARION    = glGetUniformLocation(IMAGE_SHADER, "ProjectionMatrix");
	IMAGE_VIEW_LOCATION          = glGetUniformLocation(IMAGE_SHADER, "ViewMatrix");
	IMAGE_VIEW_POSITION_LOCATION = glGetUniformLocation(IMAGE_SHADER, "ViewPosition");
	
	// Text Camera
	TEXT_PROJECTION_LOCATION     = glGetUniformLocation(TEXT_SHADER, "ProjectionMatrix");
	TEXT_VIEW_LOCATION           = glGetUniformLocation(TEXT_SHADER, "ViewMatrix");
	TEXT_VIEW_POSITION_LOCATION  = glGetUniformLocation(TEXT_SHADER, "ViewPosition");

	// Shape Camera
	SHAPE_PROJECTION_LOCATION    = glGetUniformLocation(SHAPE_SHADER, "ProjectionMatrix");
	SHAPE_VIEW_LOCATION          = glGetUniformLocation(SHAPE_SHADER, "ViewMatrix");
	SHAPE_VIEW_POSITION_LOCATION = glGetUniformLocation(SHAPE_SHADER, "ViewPosition");
}

void SDK::SDK_Shader::CreateSSBO() {
	glGenBuffers(1, &SSBO_MATRIX_INPUT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_MATRIX_INPUT);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * 5, nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO_MATRIX_INPUT);

	glGenBuffers(1, &SSBO_MATRIX_OUTPUT);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_MATRIX_OUTPUT);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO_MATRIX_OUTPUT);
}