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
		
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element.size() * sizeof(GLuint),element_pointer, GL_STATIC_DRAW);
		//glDrawArrays(GL_POINTS, 0, buffer.size());
		glDrawElements(GL_TRIANGLES,element.size() , GL_UNSIGNED_INT, 0);
	}

	void position(float x, float y, float z) {
		trans = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
	}

	void move(float x, float y, float z) {
		trans = glm::translate(trans, glm::vec3(x, y, z));
		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
	}

	
};
