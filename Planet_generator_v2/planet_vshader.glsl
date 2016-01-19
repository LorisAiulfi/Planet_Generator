#version 420 core

in vec3 position;
out vec3 vPosition;

uniform mat4 M;
uniform float radius;

void main() {
	vec4 pos4 =  vec4(radius * position, 1.0);
	vPosition = (M * pos4).xyz/pos4.w;
}