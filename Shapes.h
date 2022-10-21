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
	
	glm::mat4  trans ;
public:
	sphere(float rad   , GLint transf) : pi(3.14159) , stacks(200) , sectors(200) {
		radius = rad;
		genVertices();
		trans = glm::mat4(1.0f);
		transformation = transf;
	}
	void genVertices() {
		if (buffer.size() > 0) return;
		glm::vec3 color ;
		color.x = 1;
		color.y = 1;
		color.z = 1;
		
		for (float i = 0; i < stacks ; i++) {
			for (float j = 0; j < sectors; j++) {
				glm::vec3 point;

				float phi = (pi / 2) - pi * (i / (float)stacks);
				float theta = 2 * pi * (j / (float)sectors);
				point.x = radius * cos(phi) * cos(theta);
				point.y = radius * cos(phi) * sin(theta);
				point.z = radius * sin(phi);
				pushVectors(buffer, point, color, glm::normalize(point));
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
	}

	void draw() {
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element.size() * sizeof(GLuint),element_pointer, GL_STATIC_DRAW);
		//glDrawArrays(GL_POINTS, 0, buffer.size());
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
		pushVectors(buffer, a, color, glm::vec3(0, 0, 1));
		pushVectors(buffer, b, color, glm::vec3(0, 0, 1));
		pushVectors(buffer, c, color, glm::vec3(0, 0, 1));
		pushVectors(buffer, d, color, glm::vec3(0, 0, 1));

		

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
//Cube

class Cube {
private:
	std::vector<float> buffer;
	std::vector<GLuint> element;
	GLuint* element_pointer;
	GLint transformation;
	float* buffer_pointer;
	float side;

	glm::mat4  trans;
public:
	//Cube constructor
	Cube(float s, GLint transf)
	{
		side = s;
		trans = glm::mat4(1.0f);
		transformation = transf;
		genVertices();
	}

	void genVertices()
	{
		if (buffer.size() > 0) return;
		glm::vec3 color;
		color.x = .5;
		color.y = .4;
		color.z = 1;

		//push vectors part
		glm::vec3
			//Front 4
			a(-1.0f, -1.0f, 1.0f),
			b(1.0f, -1.0f, 1.0f),
			c(1.0, 1.0, 1.0),
			d(-1.0, 1.0, 1.0),
			//Back 4
			e(-1.0f, -1.0f, -1.0f),
			f(1.0f, -1.0f, -1.0f),
			g(1.0, 1.0, -1.0),
			h(-1.0, 1.0, -1.0);

		//Pushing
		pushVectors(buffer, a, color, glm::vec3(1, 0, 0));
		pushVectors(buffer, b, color, glm::vec3(0, 1, 0));
		pushVectors(buffer, c, color, glm::vec3(0, 0, 1));
		pushVectors(buffer, d, color, glm::vec3(1, 1, 1));
		pushVectors(buffer, e, color, glm::vec3(1, 0, 0));
		pushVectors(buffer, f, color, glm::vec3(0, 1, 0));
		pushVectors(buffer, g, color, glm::vec3(0, 0, 1));
		pushVectors(buffer, h, color, glm::vec3(1, 1, 1));

		buffer_pointer = &buffer[0];
		populateElement();
	}

	void populateElement()
	{
		//populating step
		element.push_back(0);
		element.push_back(1);
		element.push_back(2);
		element.push_back(2);
		element.push_back(3);
		element.push_back(0);
		element.push_back(1);
		element.push_back(5);
		element.push_back(6);
		element.push_back(6);
		element.push_back(2);
		element.push_back(1);
		element.push_back(7);
		element.push_back(6);
		element.push_back(5);
		element.push_back(5);
		element.push_back(4);
		element.push_back(7);
		element.push_back(4);
		element.push_back(0);
		element.push_back(3);
		element.push_back(3);
		element.push_back(7);
		element.push_back(4);
		element.push_back(4);
		element.push_back(5);
		element.push_back(1);
		element.push_back(1);
		element.push_back(0);
		element.push_back(4);
		element.push_back(3);
		element.push_back(2);
		element.push_back(6);
		element.push_back(6);
		element.push_back(7);
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
	//Default
	void position(float x, float y, float z) {
		trans = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));

	}

	void move(float x, float y, float z) {
		trans = glm::translate(trans, glm::vec3(x, y, z));

	}
};