#version 460 compatibility

layout(location = 0) out vec4 FragColor;
uniform sampler2D OutTexture;

in vec3           FragPosition;
in vec2           TextureCoord;

uniform vec3      ViewPosition;
uniform float     Opacity;
uniform vec3      Color;

uniform bool      BlurState;
uniform int       BlurExecution;
uniform float     BlurStrength; 
uniform vec2      TextureSize;

vec4 ComputeBlur() {
    vec4 ReturnColor = vec4(0.0);
    float TotalWeight = 0.0;
    float Sigma = BlurStrength;
    float TwoSigmaSq = 2.0 * Sigma * Sigma;

    for (int y = -BlurExecution; y <= BlurExecution; ++y) {
        for (int x = -BlurExecution; x <= BlurExecution; ++x) {
            vec2 Offset = vec2(float(x), float(y)) * TextureSize;
            float DistanceSq = float(x * x + y * y);
            float Weight = exp(-DistanceSq / TwoSigmaSq);
            vec2 SampleCoord = clamp(TextureCoord + Offset, vec2(0.0), vec2(1.0));
            vec4 SampleColor = texture(OutTexture, SampleCoord);
            ReturnColor += SampleColor * Weight;
            TotalWeight += Weight;
        }
    }

    return ReturnColor / TotalWeight;
}

void main() {
    vec4 FinalResult = vec4(0.0);

     if (BlurState)
        FinalResult = ComputeBlur();
    else
        FinalResult = texture(OutTexture, TextureCoord);

    FragColor = vec4(FinalResult.rgb + Color.rgb, FinalResult.a * Opacity);
}