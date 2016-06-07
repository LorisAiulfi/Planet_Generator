#version 420 core

layout(vertices = 3) out;
in vec3 vPosition[];
out vec3 tcPosition[];
out flat float tessLvl[];

uniform vec3 up;
uniform vec3 eye;
uniform vec3 eye_dir;
uniform float near_dist;
uniform float far_dist;
uniform float hNear, wNear, hFar, wFar;
uniform float lod_factor;
uniform mat4 MVP;
uniform mat4 M;
uniform float radius;

#define ID gl_InvocationID

// Planes names enum
const uint TOP = 0;
const uint BOTTOM = 1;
const uint LEFT = 2;
const uint RIGHT = 3;
const uint NEAR = 4;
const uint FAR = 5;

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
        return 5;
    }
    else {
        return 1.0;
    }
}

//float screenSpaceTessLvl() {
//	return 0;
//}

bool inFrustum(vec3 p) {
	vec3 right = normalize(cross(eye_dir, up));

	// Compute the 4 planes of the view frustum
	// near and far planes' normals are respectively eye_dir and -eye_dir
	// store them in an array
	vec3[6] planes;

	vec3 fc = eye + eye_dir * far_dist;
	vec3 nc = eye + eye_dir * near_dist;

	vec3 a = (nc + right * wNear/2.0) - eye;
	a = normalize(a);

	//vec3 normalRight = cross(up, a);
	planes[RIGHT] = cross(up, a);

	a = (nc - right * wNear/2.0) - eye;
	a = normalize(a);

	//vec3 normalLeft = cross(a, up);
	planes[LEFT] =  cross(a, up);

	a = (nc + up * hNear/2.0) - eye;
	a = normalize(a);

	//vec3 normalTop = cross(right, a);
	planes[TOP] = cross(right, a);

	a = (nc - up * hNear/2.0) - eye;
	a = normalize(a);

	//vec3 normalBot = cross(a, right);
	planes[BOTTOM] = cross(a, right);

	for(int i = 0; i < 6; ++i) {
		if(distance(planes[i], p) < 0)
			return false;
	}

	return true;
}

void main() {
	tcPosition[ID] = vPosition[ID];

	float eyeToV0 = distance(eye, vPosition[0]);
	float eyeToV1 = distance(eye, vPosition[1]);
	float eyeToV2 = distance(eye, vPosition[2]);

	float lvlCenter = getTessLvl(eyeToV0, eyeToV1, eyeToV2);
	tessLvl[ID] = lvlCenter;

	if(ID == 0) {
		if(inFrustum(vPosition[0]) && inFrustum(vPosition[1]) && inFrustum(vPosition[2])) {
			
			float lvl1 = getTessLvl(eyeToV1, eyeToV2);
			float lvl2 = getTessLvl(eyeToV2, eyeToV0);
			float lvl3 = getTessLvl(eyeToV0, eyeToV1);
			float lvl4 = lvlCenter;

			gl_TessLevelOuter[0] = lvl1;
			gl_TessLevelOuter[1] = lvl2;
			gl_TessLevelOuter[2] = lvl3;
			gl_TessLevelInner[0] = lvl4;
		} else {
			gl_TessLevelOuter[0] = 0;
			gl_TessLevelOuter[1] = 0;
			gl_TessLevelOuter[2] = 0;
			gl_TessLevelInner[0] = 0;
		}
	}
}