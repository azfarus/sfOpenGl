#pragma once
#include"StandardFunctions.h"




class plane {
private:
	std::vector<float> buffer;
	std::vector<GLuint> element;
	GLuint* element_pointer;
	GLint transformation;
	float* buffer_pointer;
	float w, h , z;
	glm::vec3 origin;
	const float pi;
	

	glm::mat4  trans;
public:
	plane(float width, float height , glm::vec3 origin , GLint transf) : pi(3.14159) {
		this->origin = origin;
		w = width ;
		h = height;
		z = origin.z;
		trans = glm::mat4(1.0f);
		transformation = transf;
		genVertices();
	}
	void genVertices() {
		if (buffer.size() > 0) return;
		glm::vec3 color;
		color.x = 0;
		color.y = 1;
		color.z = .8;
		glm::vec3 a(origin.x, origin.y, origin.z),
				b(origin.x+w, origin.y, origin.z),
				c(origin.x, origin.y+h, origin.z),
				d(origin.x+w, origin.y+h, origin.z);
		pushVectors(buffer, a, color, glm::vec3(0, 0, 1) , glm::vec2(0,0));
		pushVectors(buffer, b, color, glm::vec3(0, 0, 1), glm::vec2(1, 0));
		pushVectors(buffer, c, color, glm::vec3(0, 0, 1), glm::vec2(0, 1));
		pushVectors(buffer, d, color, glm::vec3(0, 0, 1), glm::vec2(1, 1));

		

		buffer_pointer = &buffer[0];
		populateElement();
	}

	void populateElement() {

		element.push_back(0);
		element.push_back(1);
		element.push_back(2);
		element.push_back(1);
		element.push_back(2);
		element.push_back(3);

		element_pointer = &element[0];
	}

	void draw() {
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element.size() * sizeof(GLuint), element_pointer, GL_STATIC_DRAW);
		//glDrawArrays(GL_POINTS, 0, buffer.size());
		glDrawElements(GL_TRIANGLES, element.size(), GL_UNSIGNED_INT, 0);
	}

	void position(float x, float y, float z) {
		trans = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
		
	}

	void move(float x, float y, float z) {
		trans = glm::translate(trans, glm::vec3(x, y, z));
		
	}


};

class base_shape {

protected:
	glm::mat4x4 pos, rot, scal;
	glm::vec3 center , color;
	
	std::vector<float> buffer;
	std::vector<GLuint> element;
	
	GLint transformation , rot_scale;
	texture tex;
	GLuint* element_pointer;
	float* buffer_pointer;
	
public:
	base_shape(GLint shader ,glm::vec3 color , std::string filepath = "C:/Users/samaz/Pictures/default.jpg") : tex(filepath) {
		this->transformation= glGetUniformLocation(shader, "trans");;
		this->rot_scale = glGetUniformLocation(shader, "rot_scale");
		pos = rot = scal = glm::mat4x4(1.0f);
		element_pointer = NULL;
		buffer_pointer = NULL;
		center = glm::vec3(0, 0, 0);
		this->color = color;
	}

	void rotate(float angle, glm::vec3 axis) {
		rot = glm::rotate(rot, glm::radians(angle), axis);
	}

	void position(float x, float y, float z) {
		
		pos = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
		center = pos * glm::vec4(0,0,0, 1);
	}

	void move(float x, float y, float z) {
		pos = glm::translate(pos, glm::vec3(x, y, z));
	}
	void scale(float x , float y , float z) {
		scal = glm::scale(glm::mat4x4(1.0f), glm::vec3(x, y, z));
	}

	void draw() {
		
		tex.bind();
		glm::mat4 trans = pos *rot *scal ;
		glUniformMatrix4fv(rot_scale, 1, GL_FALSE, glm::value_ptr(rot * scal));
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element.size() * sizeof(GLuint), element_pointer, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, element.size(), GL_UNSIGNED_INT, 0);
		


	}

	
};

class sphere_shape :public base_shape {
protected:
	float radius;
	int stacks = 50, sectors = 50;
	const float pi = 3.141592;
public:
	
	sphere_shape(float radius, glm::vec3 clr , GLint shader, std::string f_path = "earth.jpg") :base_shape( shader ,clr, f_path) {
		this->radius = radius;
		genVertices();

	}
	sphere_shape(float radius, GLint shader , std::string f_path) :base_shape(shader ,glm::vec3(1,1,1), f_path) {
		this->radius = radius;
	}

	
	void genVertices() {
		if (buffer.size() > 0) return;
		long double uv_increment = 1.0 / (long double)sectors;
		for (float i = 0; i < stacks; i++) {
			for (float j = 0; j < sectors; j++) {
				glm::vec3 point;
				glm::vec2 uv(j * uv_increment, i * uv_increment);
				float phi = (pi / 2) - pi * (i / (float)stacks);
				float theta = 2 * pi * (j / (float)sectors);
				point.x = radius * cos(phi) * cos(theta);
				point.y = radius * cos(phi) * sin(theta);
				point.z = radius * sin(phi);
				pushVectors(buffer, point, color, glm::normalize(point), uv);
				


			}
		}

		buffer_pointer = &buffer[0];

		populateElement();
	}

	void populateElement() {

		if (element.size() > 0) return;
		for (int j = 0; j < stacks + 1; j++) {
			for (int i = 0; i < sectors + 1; i++) {
				//1st triangle
				int n = j * stacks;
				element.push_back(n + i);
				element.push_back((i + 1) % sectors + n);
				element.push_back(n + i + sectors);
				//2nd triangle
				element.push_back(i + sectors + n);
				element.push_back(((i + 1) % sectors) + sectors + n);
				element.push_back((i + 1) % sectors + n);

			}
		}
	
		element_pointer = &element[0];
		
	}
};


class light_source: public sphere_shape {
	GLint lightSrc;
public:
	
	light_source(float rad , GLint shader,std::string filepath = "sun.jpg") :sphere_shape(rad, shader ,filepath) {
		radius = rad;
		this->lightSrc = glGetUniformLocation(shader, "camera");
		genVertices();
	}
	void genVertices() {
		if (buffer.size() > 0) return;
		long double uv_increment = 1.0 / (long double)sectors;
		for (float i = 0; i < stacks; i++) {
			for (float j = 0; j < sectors; j++) {
				glm::vec3 point;
				glm::vec2 uv((j+1) * uv_increment, (i+1) * uv_increment);
				float phi = (pi / 2) - pi * (i / (float)stacks);
				float theta = 2 * pi * (j / (float)sectors);
				point.x = radius * cos(phi) * cos(theta);
				point.y = radius * cos(phi) * sin(theta);
				point.z = radius * sin(phi);
				pushVectors(buffer, point, color, glm::normalize(-point), uv);
				

			}
		}

		buffer_pointer = &buffer[0];

		populateElement();
	}

	void draw() {
		//center = pos * glm::vec4(center, 1);
		
		
		glUniform3fv(lightSrc, 1, glm::value_ptr(center));
		tex.bind();
		
		
		
		glm::mat4 trans = pos * rot * scal;
		glUniformMatrix4fv(rot_scale, 1, GL_FALSE, glm::value_ptr(rot * scal));
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element.size() * sizeof(GLuint), element_pointer, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, element.size(), GL_UNSIGNED_INT, 0);



	}
	glm::vec3 get_center() {
		return center;
	}
};

