#pragma once

#include "common.h"
#include "trackball.h"

#define SCROLL_INC 10
#define MOVE_SPEED 0.01
#define ROTATION_SPEED 0.0005
#define FPS_VIEW_SPEED 1
#define NB_KEYS 300

class Controller {

private:
	TrackBall* trackball;
	mat4* tb_matrix;
	vec3* view_dir;
	vec3* eye;
	vec3* up;
	bool key_map[NB_KEYS];
	vec2 prev_mouse; // Position in opengl coordinates (not screen coord)

	void move(vec3 dir) {
		mat4 transform = Eigen::Affine3f(Eigen::Translation3f(dir * MOVE_SPEED)).matrix();
		vec4 eye4 = vec4(eye->x(), eye->y(), eye->z(), 1.0f);
		eye4 = transform * eye4;
		*eye = vec3(eye4.x(), eye4.y(), eye4.z());
	}

	void move_forward() {
		move(*view_dir);
	}

	void move_backward() {
		move(-*view_dir);
	}

	void move_up() {
		move(*up);
	}

	void move_down() {
		move(-*up);
	}

	void move_right() {
		vec3 right = view_dir->cross(*up);
		move(right);
	}

	void move_left() {
		vec3 left = up->cross(*view_dir);
		move(left);
	}

	void rotate_cw() {
		mat4 transform = Eigen::Affine3f(Eigen::AngleAxisf(M_PI * ROTATION_SPEED, *view_dir)).matrix();
		vec4 up4 = vec4(up->x(), up->y(), up->z(), 1.0f);
		up4 = transform * up4;
		*up = vec3(up4.x(), up4.y(), up4.z());
	}

	void rotate_ccw() {
		mat4 transform = Eigen::Affine3f(Eigen::AngleAxisf(M_PI * -ROTATION_SPEED, *view_dir)).matrix();
		vec4 up4 = vec4(up->x(), up->y(), up->z(), 1.0f);
		up4 = transform * up4;
		*up = vec3(up4.x(), up4.y(), up4.z());
	}

	void rotate_h(float rate) {
		mat4 transform = Eigen::Affine3f(Eigen::AngleAxisf(-rate * M_PI * FPS_VIEW_SPEED, *up)).matrix();
		vec4 view_dir4 = vec4(view_dir->x(), view_dir->y(), view_dir->z(), 1.0f);
		view_dir4 = transform * view_dir4;
		*view_dir = vec3(view_dir4.x(), view_dir4.y(), view_dir4.z()).normalized();
	}

	void rotate_v(float rate) {
		// Rotate view_dir
		vec3 axis = view_dir->cross(*up);
		mat4 transform = Eigen::Affine3f(Eigen::AngleAxisf(rate * M_PI * FPS_VIEW_SPEED, axis)).matrix();
		vec4 view_dir4 = vec4(view_dir->x(), view_dir->y(), view_dir->z(), 1.0f);
		view_dir4 = transform * view_dir4;
		*view_dir = vec3(view_dir4.x(), view_dir4.y(), view_dir4.z()).normalized();

		//  Rotate up accordingly
		vec4 up4 = vec4(up->x(), up->y(), up->z(), 1.0f);
		up4 = transform * up4;
		*up = vec3(up4.x(), up4.y(), up4.z()).normalized();
	}

	void rotate_view(vec2 dir) {
		rotate_h(dir.x());
		rotate_v(dir.y());
	}

	/*
		Execute the action associated with the given key
	*/
	void perform_action(int key) {
		switch (key) {
		case 'W':
			move_forward();
			break;
		case 'S':
			move_backward();
			break;
		case 'A':
			move_left();
			break;
		case 'D':
			move_right();
			break;
		case 'R':
			move_up();
			break;
		case 'F':
			move_down();
			break;
		case 'Q':
			rotate_ccw();
			break;
		case 'E':
			rotate_cw();
			break;
		default:
			std::cout << key << std::endl;
		}
		
	}


public:
	void init(TrackBall* tb, mat4* tb_mat, vec3* view_d, vec3* eye_pos, vec3* p_up) {
		trackball = tb;
		tb_matrix = tb_mat;
		view_dir = view_d;
		eye = eye_pos;
		up = p_up;
		prev_mouse = vec2(0, 0);
	}

	vec2 screen_to_opengl_coord(double x, double y, double w, double h) {
		return vec2(2.0f * x / w - 1.0f, 1.0f - 2.0f * y / h);
	}

	void mouse_pos_callback(GLFWwindow* window, double posX, double posY) {
		vec2 p = screen_to_opengl_coord(posX, posY, WIDTH, HEIGHT);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			*tb_matrix *= trackball->drag(p[0], p[1]);
		}

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			rotate_view(p - prev_mouse);
		}

		prev_mouse = p;
	}

	void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
		double zoom = -SCROLL_INC;
		if (yoffset > 0) {
			zoom = SCROLL_INC;
		}

		vec3 translate = *view_dir * zoom;
		mat4 transform = Eigen::Affine3f(Eigen::Translation3f(translate)).matrix();
		vec4 eye4 = vec4(eye->x(), eye->y(), eye->z(), 1.0f);
		eye4 = transform * eye4;

		*eye = vec3(eye4.x(), eye4.y(), eye4.z());
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int modifier) {
		double x, y;

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			glfwGetCursorPos(window, &x, &y);
			vec2 p = screen_to_opengl_coord(x, y, WIDTH, HEIGHT);
			trackball->begin_drag(p[0], p[1]);
		}
	}

	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int modifier) {
		if (action == GLFW_RELEASE) key_map[key] = false;
		else if (action == GLFW_PRESS) key_map[key] = true;
	}

	/*
	Lookup the status of all keys and perform the corresponding actions
	if the key is pressed
	*/
	void update_actions() {
		for (int i(0); i < sizeof(key_map); ++i) {
			if (key_map[i]) perform_action(i);
		}
	}
};

