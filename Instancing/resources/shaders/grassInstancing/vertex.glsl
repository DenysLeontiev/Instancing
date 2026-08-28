#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

layout (location = 2) in vec2 aOffset;
layout (location = 3) in float aRotation;
layout (location = 4) in float aScale;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float uTime;

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

void main() 
{
    vec3 pos = aPos * aScale;

    float cosAngle = cos(aRotation);
    float sinAngle = sin(aRotation);

    vec3 rotatedPos;
    rotatedPos.x = pos.x * cosAngle + pos.z * sinAngle;
    rotatedPos.y = pos.y;
    rotatedPos.z = -pos.x * sinAngle + pos.z * cosAngle;

    float heightFactor = clamp(aPos.y / 1.5, 0.0, 1.0); 
    float bendFactor = heightFactor * heightFactor;

    float wave = sin(uTime * 2.0 + aOffset.x * 0.1 + aOffset.y * 0.1);
    float windStrength = 0.25 * aScale;

    vec3 windOffset = vec3(wave * windStrength * bendFactor, 0.0, wave * 0.5 * windStrength * bendFactor);

    vec3 worldPos = rotatedPos + windOffset + vec3(aOffset.x, 0.0, aOffset.y);

    gl_Position = projection * view * model * vec4(worldPos, 1.0);

    float rand = hash(aOffset);
    vec3 rootColor = mix(vec3(0.05, 0.25, 0.04), vec3(0.12, 0.32, 0.06), rand);
    vec3 tipColor  = mix(vec3(0.40, 0.85, 0.18), vec3(0.65, 0.90, 0.15), rand); 

    ourColor = mix(rootColor, tipColor, heightFactor);
}