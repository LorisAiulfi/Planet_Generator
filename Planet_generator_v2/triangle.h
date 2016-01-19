#pragma once

#include "common.h"

class Triangle {

protected:
	GLfloat vertices[9] {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	GLuint vao;
	GLuint pid;
	GLuint vbo;
	mat4   M;

public:
	void init() {
		/// Creating vbo and vao and bind everything to vao
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		
		glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		/// Compiling shaders
		pid = opengp::load_shaders("triangle_vshader.glsl", "triangle_fshader.glsl");
		if (!pid) exit(EXIT_FAILURE);
		glUseProgram(pid);

		/// The model matrix is by default the identity matrix
		M = mat4::Identity();
	}

	void draw(const mat4& tb_M, const mat4& V, const mat4& P) {
		glUseProgram(pid);
		glBindVertexArray(vao);

		/// Passing MVP to vshader
		mat4 mvp = P * V * tb_M * M;
		GLuint mvp_id = glGetUniformLocation(pid, "MVP");
		glUniformMatrix4fv(mvp_id, 1, GL_FALSE, mvp.data());

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);
	}
};