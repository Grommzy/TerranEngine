#version 430 core

layout(location = 0) in vec2  aPos;
layout(location = 1) in vec2  aUV;
layout(location = 2) in vec4  aColor;
layout(location = 3) in float aDepth;

uniform mat4 uTransform;

out vec2 vUV;
out vec4 vColor;

void main()
{
    vUV    = aUV;
    vColor = aColor;
    gl_Position = uTransform * vec4(aPos, aDepth, 1.0);
}
