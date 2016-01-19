#include "common.h"
#include "controller_callbacks.h"
#include "trackball.h"

/// Objects that will be in the scene
#include "planet.h"

#define NEAR_PLANE 0.0000001f
#define FAR_PLANE 100000000000.0f
#define FOVY 80.0f

#define PLANET_RADIUS 1000

TrackBall trackball;
Controller controller;
Planet planet;

/// Camera Parameters
vec3 eye, eye_dir, up;
GLfloat hNear;
GLfloat wNear;
GLfloat hFar;
GLfloat wFar;

/// mvp parameters
mat4 P; // projection
mat4 V; // view
mat4 trackball_matrix;

/// ********************** Controller Callbacks *************************
void mouse_pos_callback(GLFWwindow* window, double posX, double posY) {
	controller.mouse_pos_callback(window, posX, posY);
}

void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
	controller.mouse_wheel_callback(window, xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int modifier) {
	controller.mouse_button_callback(window, button, action, modifier);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int modifier) {
	controller.keyboard_callback(window, key, scancode, action, modifier);
}
/// ********************************************************************

void update_view() {
	vec3 center = eye + eye_dir;
	V = Eigen::lookAt(eye, center, up);
}

void init() {
	glViewport(0, 0, WIDTH, HEIGHT);

	/// Setup Projection matrix
	float ratio = WIDTH / (float)HEIGHT;
	P = Eigen::perspective(FOVY, ratio, NEAR_PLANE, FAR_PLANE);
	
	/// Setup View matrix
	eye = vec3(0, 0, PLANET_RADIUS + 10.0f);
	eye_dir = vec3(0, 0, -1.0f);
	up = vec3(0, 1, 0);
	update_view();

	/// Setup View Frustum
	hNear = 2.0 * tan(FOVY / 2.0) * NEAR_PLANE;
	wNear = hNear * ratio;
	hFar = 2.0 * tan(FOVY / 2.0) * FAR_PLANE;
	wFar = hFar * ratio;

	/// Default trackball position
	trackball_matrix = mat4::Identity();

	controller.init(&trackball, &trackball_matrix, &eye_dir, &eye, &up);

	planet.init(PLANET_RADIUS);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
}

void display() {
	controller.update_actions();
	update_view();

	glClearColor(0.2, 0.4, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	planet.draw(trackball_matrix, V, P, eye, eye_dir, NEAR_PLANE, FAR_PLANE, hNear, wNear, hFar, wFar);
}

int main(int, char**) {
	// Create Window and setup controls callbacks
	glfwInitWindowSize(WIDTH, HEIGHT);
	glfwMakeWindow("Planet Generator");
	glfwDisplayFunc(display);
	glfwSetMouseButtonCallback(getWindow(), mouse_button_callback);
	glfwSetCursorPosCallback(getWindow(), mouse_pos_callback);
	glfwSetScrollCallback(getWindow(), mouse_wheel_callback);
	glfwSetKeyCallback(getWindow(), keyboard_callback);

	init();

	glfwSwapInterval(0);
	glfwMainLoop();

	return EXIT_SUCCESS;
}