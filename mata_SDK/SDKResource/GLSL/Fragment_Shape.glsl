#version 330 core

in vec3       FragPosition;
in vec2       TextureCoord;

out vec4      FragColor;

uniform vec3  Color;
uniform vec3  ViewPosition;

uniform float Opacity;

void main() {
    FragColor = vec4(Color.rgb, Opacity);
}