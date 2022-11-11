#pragma once
#include"StandardFunctions.h"
#include<math.h>
#include<glm/gtc/random.hpp>


class base_shape {

protected:
	glm::mat4x4 pos, rot, scal;
	glm::vec3 center , color ,cam_pos , lookAt;
	
	std::vector<float> buffer;
	std::vector<GLuint> element;
	
	GLint transformation , rot_scale;
	GLint glpoint;

	texture tex;
	GLuint* element_pointer;
	float* buffer_pointer;


	
public:
	base_shape(GLint shader ,glm::vec3 color , std::string filepath = "default.png") : tex(filepath) {
		this->glpoint = glGetUniformLocation(shader, "glpointt");
		if (this->glpoint <= 0) {
			std::cout << "Pointerror\n";
		}
		else std::cout << "nothing\n";
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

		glUniform1i(this->glpoint, 0);
		glUniformMatrix4fv(rot_scale, 1, GL_FALSE, glm::value_ptr(rot * scal));
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element.size() * sizeof(GLuint), element_pointer, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, element.size(), GL_UNSIGNED_INT, 0);
		


	}

	glm::vec3 get_cam() {
		return glm::vec3(pos * rot * scal * glm::vec4(cam_pos, 1));
	}

	glm::vec3 get_lookAt() {
		return glm::vec3(pos * rot * scal * glm::vec4(lookAt, 1));
	}

	
};
class line : public base_shape {

public:
	line(glm::vec3 start , glm::vec3 end  , glm::vec3 color  , GLint shader):
	base_shape(shader , color) {
		pushVectors(this->buffer, start, color, start, glm::vec2(.5, .5));
		pushVectors(this->buffer, end, color, end, glm::vec2(.5, .5));
		buffer_pointer = &buffer[0];
	}
	
	void draw() {

		tex.bind();
		glm::mat4 trans = pos * rot * scal;

		glUniform1i(this->glpoint, 1);
		glUniformMatrix4fv(rot_scale, 1, GL_FALSE, glm::value_ptr(rot * scal));
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glDrawArrays(GL_LINES,0, buffer.size());



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
		lookAt = glm::vec3(-radius , radius, 0);
		genVertices();

	}
	sphere_shape(float radius, GLint shader , std::string f_path) :base_shape(shader ,glm::vec3(1,1,1), f_path) {
		this->radius = radius;
		lookAt = glm::vec3(-radius, radius, 0);
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
				if (i == stacks / 2 && j == sectors / 2) {
					cam_pos = glm::vec3(point.x *1.25 , point.y * 1.25 , point.z *1.25);
				}


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
		
		glUniform1i(this->glpoint, 0);
		
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

class random_pts : public base_shape
{
	glm::mat4 custom_trans;
public:
	random_pts(GLint shader , glm::vec3 clr , std::string f_path):base_shape(shader , clr , f_path) {
		custom_trans = glm::mat4(1.0f);
		genVertices();
	}
	void genVertices() {
		
		for (unsigned int i = 0; i < 500; i++) {
			glm::vec3 point = glm::sphericalRand(20.0);
			pushVectors(buffer , point, color, point, glm::vec2(.5, .5));
		}

		buffer_pointer = &buffer[0];
		std::cout << buffer.size()<<std::endl;
		
	}

	void ste_customTrans(glm::mat3 custom_transformation) {
		custom_trans = glm::mat4(custom_transformation);
	}

	void draw() {

		tex.bind();
		glm::mat4 trans = pos * rot * scal;
		glUniform1i(this->glpoint, 1);
		
		glUniformMatrix4fv(rot_scale, 1, GL_FALSE, glm::value_ptr(rot * scal));
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(custom_trans));
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glDrawArrays(GL_POINTS, 0 ,buffer.size());



	}
};

