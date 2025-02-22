#version 460 core

layout(std430, binding = 0) readonly buffer MatrixInput {
    mat4 Matrix1;
    mat4 Matrix2;
    mat4 Matrix3;
    mat4 Matrix4;
    mat4 Matrix5;
};

layout(std430, binding = 1) buffer MatrixOutput {
    mat4 Result;
};

layout(local_size_x = 4, local_size_y = 4, local_size_z = 1) in;

void main() {
    uint Row = gl_GlobalInvocationID.y;
    uint Col = gl_GlobalInvocationID.x;
    
    mat4 Combined = Matrix1 * Matrix2;
    Combined = Combined * Matrix3;
    Combined = Combined * Matrix4;
    Combined = Combined * Matrix5;

    Result[Row][Col] = Combined[Row][Col];
}