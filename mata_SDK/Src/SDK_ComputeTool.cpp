#include "SDK_ComputeTool.h"

SDK::SDK_ComputeTool SDK::ComputeTool;

void SDK::SDK_ComputeTool::ComputeMatrix(SDK::Matrix4& Dest, SDK::Matrix4& Matrix1, SDK::Matrix4& Matrix2, SDK::Matrix4& Matrix3, SDK::Matrix4& Matrix4, SDK::Matrix4& Matrix5) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_MATRIX_INPUT);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SDK::Matrix4), glm::value_ptr(Matrix1));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(SDK::Matrix4), sizeof(SDK::Matrix4), glm::value_ptr(Matrix2));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(SDK::Matrix4) * 2, sizeof(SDK::Matrix4), glm::value_ptr(Matrix3));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(SDK::Matrix4) * 3, sizeof(SDK::Matrix4), glm::value_ptr(Matrix4));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(SDK::Matrix4) * 4, sizeof(SDK::Matrix4), glm::value_ptr(Matrix5));

	glUseProgram(MATRIX_COMPT_SHADER);
	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_MATRIX_OUTPUT);
	void* Result = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(SDK::Matrix4), GL_MAP_READ_BIT);
	memcpy(glm::value_ptr(Dest), Result, sizeof(SDK::Matrix4));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}