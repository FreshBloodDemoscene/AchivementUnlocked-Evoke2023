#version 440 core
out vec4 FragColor;

layout(location = 0) uniform vec2 uScreenResolution;

in vec2 vPos;

void main()
{
    float ratio = uScreenResolution.x / uScreenResolution.y;
    vec2 pos = vPos * vec2(ratio, 1.0);
    FragColor = vec4(1.0-step(0.5, distance(pos, vec2(0.0))), 0.0, 0.0, 1.0);
} 
