#pragma once

#include "common.h"

class TrackBall {
public:
	TrackBall() : radius(1.0f) {}

	/// Called when the left mouse button is pressed
	void begin_drag(float x, float y) {
		anchor_pos = vec3(x, y, 0.0f);
		project_on_surface(anchor_pos);
	}

	/// Called while the cursor is moved around while the left mouse button is still pressed
	/// and return the rotation matrix
	mat4 drag(float x, float y) {
		vec3 curr_pos = vec3(x, y, 0.0f);
		project_on_surface(curr_pos);

		vec3 rot_axis = anchor_pos.cross(curr_pos).normalized();
		float angle = rotation_speed * acos(curr_pos.normalized().dot(anchor_pos.normalized()));

		mat4 eigen = Eigen::Affine3f(Eigen::AngleAxisf(angle * PI / 180, rot_axis)).matrix();
		mat4 rotation = mat4::Identity();
		rotation.block(0, 0, 3, 3) = eigen.block(0, 0, 3, 3);

		return rotation;
	}

private:
	float radius;
	float rotation_speed = 1.0f;
	vec3 anchor_pos;
	mat4 rotation;

	void project_on_surface(vec3& p) const {
		float norm = pow(p[0], 2) + pow(p[1], 2);
		float z(0.0f);

		if (norm > pow(radius, 2)) {
			// use hyperbolic sheet
			z = (pow(radius, 2) / 2.0f) / sqrt(norm);
		}
		else {
			// use normal shere
			z = sqrt(pow(radius, 2) - norm);
		}

		p[2] = z;
	}
};