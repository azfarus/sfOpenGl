#pragma once
#include"StandardFunctions.h"

class sphere {
private: 
	std::vector<float> buffer;
	float* buffer_pointer;
	float radius;
	const float pi;
	int stacks, sectors;
	sf::Vector3f center;
public:
	sphere(float rad = 4 ) : pi(3.14159) , stacks(200) , sectors(200) {
		radius = rad;
		center = sf::Vector3f(0, 0, 0);
		genVertices();

	}
	void genVertices() {
		glm::vec3 color ;
		color.x = 1;
		color.y = 1;
		color.z = 1;
		
		for (float i = 0; i < stacks; i++) {
			for (float j = 0; j < sectors; j++) {
				glm::vec3 point;

				float phi = (pi / 2) - pi * (i / (float)stacks);
				float theta = 2 * pi * (j / (float)sectors);
				point.x = radius * cos(phi) * cos(theta);
				point.y = radius * cos(phi) * sin(theta);
				point.z = radius * sin(phi);
				pushVectors(buffer, point, color, point);
				//std::cout << point.x <<" "<<point.y<<" "<<point.z<<"\n";
				
			}
		}

		buffer_pointer = &buffer[0];
	}

	void draw() {
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glDrawArrays(GL_POINTS, 0, buffer.size());
	}
};
