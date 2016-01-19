#pragma once

#include "common.h"

#define RECURSION 6

class Planet {
private:
	GLuint index;
	GLuint index_count;
	GLuint vao;
	GLuint pid;
	GLuint vbo_position;
	GLuint vbo_index;
	mat4   M;
	GLfloat radius;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	//add indices of the vertex of a triangle
	void add_indices(GLuint v1, GLuint v2, GLuint v3, std::vector<GLuint> *vect) {
		vect->push_back(v1);
		vect->push_back(v2);
		vect->push_back(v3);
	}

	// add vertex to vector after having fixed position to be on unit sphere,
	// return index
	GLuint add_vertex(GLfloat v1, GLfloat v2, GLfloat v3) {
		GLfloat length = sqrt(v1*v1 + v2*v2 + v3*v3);
		vertices.push_back(v1 / length);
		vertices.push_back(v2 / length);
		vertices.push_back(v3 / length);

		return index++;
	}

	// return index of point in the middle of points given by indices p1 and p2,
	// and add this new point to the list of vertices
	GLuint get_middle_point(int p1, int p2) {
		//get the corresponding vectors
		vec3 point1(vertices[p1 * 3], vertices[p1 * 3 + 1], vertices[p1 * 3 + 2]);
		vec3 point2(vertices[p2 * 3], vertices[p2 * 3 + 1], vertices[p2 * 3 + 2]);

		vec3 middle = (point1 + point2) / 2.0f;

		GLuint i = add_vertex(middle[0], middle[1], middle[2]);

		return i;
	}

	void generate_icosphere(int recursion) {
		index = 0;

		float t = (1.0f + sqrt(5.0f)) / 2.0f;
		add_vertex(-1, t, 0);
		add_vertex(1, t, 0);
		add_vertex(-1, -t, 0);
		add_vertex(1, -t, 0);

		add_vertex(0, -1, t);
		add_vertex(0, 1, t);
		add_vertex(0, -1, -t);
		add_vertex(0, 1, -t);

		add_vertex(t, 0, -1);
		add_vertex(t, 0, 1);
		add_vertex(-t, 0, -1);
		add_vertex(-t, 0, 1);

		add_indices(0, 11, 5, &indices);
		add_indices(0, 5, 1, &indices);
		add_indices(0, 1, 7, &indices);
		add_indices(0, 7, 10, &indices);
		add_indices(0, 10, 11, &indices);

		add_indices(1, 5, 9, &indices);
		add_indices(5, 11, 4, &indices);
		add_indices(11, 10, 2, &indices);
		add_indices(10, 7, 6, &indices);
		add_indices(7, 1, 8, &indices);

		add_indices(3, 9, 4, &indices);
		add_indices(3, 4, 2, &indices);
		add_indices(3, 2, 6, &indices);
		add_indices(3, 6, 8, &indices);
		add_indices(3, 8, 9, &indices);

		add_indices(4, 9, 5, &indices);
		add_indices(2, 4, 11, &indices);
		add_indices(6, 2, 10, &indices);
		add_indices(8, 6, 7, &indices);
		add_indices(9, 8, 1, &indices);

		//refine triangle
		for (int i = 0; i < recursion; ++i) {
			std::vector<GLuint> new_indicies;

			//for each triangle
			for (int j = 0; j < indices.size(); j += 3) {
				//get the corresponding indices of edges
				GLuint p1 = indices[j];
				GLuint p2 = indices[j + 1];
				GLuint p3 = indices[j + 2];

				//replace each triangle by 4 triangles
				GLuint i_ab = get_middle_point(p1, p2);
				GLuint i_bc = get_middle_point(p2, p3);
				GLuint i_ca = get_middle_point(p3, p1);

				add_indices(p1, i_ab, i_ca, &new_indicies);
				add_indices(p2, i_bc, i_ab, &new_indicies);
				add_indices(p3, i_ca, i_bc, &new_indicies);
				add_indices(i_ab, i_bc, i_ca, &new_indicies);
			}
			indices.clear();
			indices = new_indicies;
		}

		index_count = indices.size();
	}

public:
	void init(const float rad) {
		radius = rad;
		generate_icosphere(RECURSION);

		M = mat4::Identity();

		/// Compiling shader
		pid = opengp::load_shaders("planet_vshader.glsl", "planet_fshader.glsl", NULL, "planet_tcshader.glsl", "planet_teshader.glsl");
		printf("test\n");
		check_error_gl();
		if (!pid) exit(EXIT_FAILURE);
		glUseProgram(pid);
		
		/// Creating vbo and vao and bind everything to vao
		glGenVertexArrays(1, &vao);

		glBindVertexArray(vao);
			glGenBuffers(1, &vbo_position);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

			GLuint vpoint_id = glGetAttribLocation(pid, "position");
			glEnableVertexAttribArray(vpoint_id);
			glVertexAttribPointer(vpoint_id, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

			glGenBuffers(1, &vbo_index);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

			glPatchParameteri(GL_PATCH_VERTICES, 3);
		glBindVertexArray(0);

		check_error_gl();
	}

	void draw(	const mat4& tb_M, const mat4& V, const mat4& P, const vec3& eye, const vec3& eye_dir, 
				const float nearDist, const float farDist, const float hNear, const float wNear, const float hFar, const float wFar) {
		// Checking eye value
		//std::cout << "Camera Position : (" << eye.x() << " , " << eye.y() << " , " << eye.z() << std::endl;

		glUseProgram(pid);
		glBindVertexArray(vao);

		/// Passing MVP to shader
		//mat4 mvp = P * V * tb_M * M;
		//GLuint mvp_id = glGetUniformLocation(pid, "MVP");
		//glUniformMatrix4fv(mvp_id, 1, GL_FALSE, mvp.data());

		glUniform1f(glGetUniformLocation(pid, "radius"), radius);

		mat4 tmp_M = tb_M;
		glUniformMatrix4fv(glGetUniformLocation(pid, "M"), 1, GL_FALSE, tmp_M.data());

		mat4 vp = P * V;
		glUniformMatrix4fv(glGetUniformLocation(pid, "VP"), 1, GL_FALSE, vp.data());

		glUniform3fv(glGetUniformLocation(pid, "eye"), 1, eye.data());
		glUniform3fv(glGetUniformLocation(pid, "eye_dir"), 1, eye_dir.data());
		glUniform1f(glGetUniformLocation(pid, "near_dist"), nearDist);
		glUniform1f(glGetUniformLocation(pid, "far_dist"),farDist);
		glUniform1f(glGetUniformLocation(pid, "hNear"), hNear);
		glUniform1f(glGetUniformLocation(pid, "wNear"), wNear);
		glUniform1f(glGetUniformLocation(pid, "hFar"), hFar);
		glUniform1f(glGetUniformLocation(pid, "wFar"), wFar);

		//glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawElements(GL_PATCHES, index_count, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}
};