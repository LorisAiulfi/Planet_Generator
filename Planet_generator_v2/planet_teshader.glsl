#version 420 core

layout(triangles, equal_spacing, ccw) in;
in vec3 tcPosition[];
out vec3 tePosition;

uniform mat4 VP;

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
	return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main() {
	tePosition = interpolate3D(tcPosition[0], tcPosition[1], tcPosition[2]);
	gl_Position = VP * vec4(tePosition, 1);
}