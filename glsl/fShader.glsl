#version 430
in vec2 st;
out vec4 color;
layout (binding=0) uniform sampler2D samp;
void main(void) {
    color = texture(samp, st);
}