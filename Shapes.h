#pragma once
#include"StandardFunctions.h"

class sphere {
private: 
	std::vector<float> buffer;
	std::vector<GLuint> element;
	GLuint* element_pointer;
	GLint transformation;
	float* buffer_pointer;
	float radius;
	const float pi;
	int stacks, sectors;
	VBO b;
	EBO e;
	glm::mat4  trans ;
public:
	sphere(float rad   , GLint transf) : pi(3.14159) , stacks(100) , sectors(100) {
		radius = rad;
		b.bind();
		e.bind();
		trans = glm::mat4(1.0f);
		transformation = transf;
		genVertices();
	}
	void genVertices() {
		if (buffer.size() > 0) return;
		glm::vec3 color ;
		color.x = 1;
		color.y = 1;
		color.z = 1;
		long double uv_increment = 1.0 /(long double)sectors;
		for (float i = 0; i < stacks ; i++) {
			for (float j = 0; j < sectors; j++) {
				glm::vec3 point;
				glm::vec2 uv(j * uv_increment, i * uv_increment);
				float phi = (pi / 2) - pi * (i / (float)stacks);
				float theta = 2 * pi * (j / (float)sectors);
				point.x = radius * cos(phi) * cos(theta);
				point.y = radius * cos(phi) * sin(theta);
				point.z = radius * sin(phi);
				pushVectors(buffer, point, color, glm::normalize(point) , uv);
				//std::cout << point.x <<" "<<point.y<<" "<<point.z<<"\n";
				
				
			}
		}
		
		buffer_pointer = &buffer[0];
		
		populateElement();
	}

	void populateElement() {
		
		if(element.size() > 0) return;
		for (int j = 0; j < stacks+1; j++) {
			for (int i = 0; i < sectors+1; i++) {
				//1st triangle
				int n = j * stacks;
				element.push_back(n+i);
				element.push_back((i+1)%sectors+n);
				element.push_back(n+i+sectors);
				//2nd triangle
				element.push_back(i + sectors + n);
				element.push_back((( i + 1) % sectors)+sectors+n);
				element.push_back((i + 1) % sectors + n);

			}
		}
		
		element_pointer = &element[0];
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element.size() * sizeof(GLuint), element_pointer, GL_STATIC_DRAW);
		
	
	}

	void draw() {
		b.bind(); e.bind();
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		
		glDrawElements(GL_TRIANGLES,element.size() , GL_UNSIGNED_INT, 0);
		
		
	}

	void position(float x, float y, float z) {
		trans = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
		
	}

	void move(float x, float y, float z) {
		trans = glm::translate(trans, glm::vec3(x, y, z));
		
	}

	
};


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
	int stacks, sectors;

	glm::mat4  trans;
public:
	plane(float width, float height , glm::vec3 origin , GLint transf) : pi(3.14159), stacks(200), sectors(200) {
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
