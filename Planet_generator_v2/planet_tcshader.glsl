#version 420 core

layout(vertices = 3) out;
in vec3 vPosition[];
out vec3 tcPosition[];

uniform vec3 eye;
uniform vec3 eye_dir;
uniform float near_dist;
uniform float far_dist;
uniform float hNear, wNear, hFar, wFar;
uniform float lod_factor;
uniform mat4 MVP;
uniform mat4 M;
uniform float radius;

float getAvgDist2(float dist0, float dist1) {
	return (dist0 + dist1) / 2.0;
}

float getAvgDist3(float dist0, float dist1, float dist2) {
	return (dist0 + dist1 + dist2) / 3.0;
}

float getTessLvl(float dist0, float dist1, float dist2 = 0) {
	float avgDist = 0;
	if(dist2 == 0) {
		avgDist = getAvgDist2(dist0, dist1);
	} else {
		avgDist = getAvgDist3(dist0, dist1, dist2);
	}
	

	if(avgDist <= 20) {
		return 100;
	}
	else if(avgDist <= 30) {
		return 64;
	}
	else if(avgDist <= 50) {
		return 20;
	}
    else if (avgDist <= 100) {
        return 10.0;
    }
    else {
        return 1.0;
    }
}

bool inFrustum(vec3 p) {

}

#define ID gl_InvocationID

void main() {
	tcPosition[ID] = vPosition[ID];

	float eyeToV0 = distance(eye, vPosition[0]);
	float eyeToV1 = distance(eye, vPosition[1]);
	float eyeToV2 = distance(eye, vPosition[2]);

	if(ID == 0) {
		gl_TessLevelOuter[0] = getTessLvl(eyeToV1, eyeToV2);
		gl_TessLevelOuter[1] = getTessLvl(eyeToV2, eyeToV0);
		gl_TessLevelOuter[2] = getTessLvl(eyeToV0, eyeToV1);
		gl_TessLevelInner[0] = getTessLvl(eyeToV0, eyeToV1, eyeToV2);
	}
}