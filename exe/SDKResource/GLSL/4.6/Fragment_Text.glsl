#version 460 compatibility

layout(location = 0) out vec4 FragColor;

in vec3       FragPosition;
in vec2       TextureCoord;

uniform vec3  Color;
uniform vec3  ViewPosition;

uniform float Opacity;

void main() {
    FragColor = vec4(Color.rgb, Opacity);
}