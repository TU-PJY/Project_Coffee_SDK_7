#version 460 core

layout(location = 0) in vec3 InputPosition;
layout(location = 2) in vec2 VTextureCoord;

out vec3     FragPosition;
out vec2     TextureCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main() {
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(InputPosition, 1.0);
    FragPosition = vec3(ModelMatrix * vec4(InputPosition, 1.0));
    TextureCoord = VTextureCoord;
}