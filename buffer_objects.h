#pragma once
#include"StandardFunctions.h"

class VAO {
	GLuint i;
public:
	VAO() {
		glGenVertexArrays(1, &i);
	}
	void bind() {
		glBindVertexArray(i);
	}
};

class VBO {
	GLuint i;
public:
	VBO() {
		glGenBuffers(1, &i);
	}
	void bind() {
		glBindBuffer(GL_ARRAY_BUFFER , i);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_PROGRAM_POINT_SIZE);
	}
};

class EBO {
	GLuint i;
public:
	EBO() {
		glGenBuffers(1, &i);
	}
	void bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i);
		
	}
};


class attribute {
	GLint i;
	const int totalvals;
	int offset;
	std::string name;
public:

	
	attribute(GLuint shaderprog  , std::string name ,int offset_val ) : totalvals(9) {
		i = glGetAttribLocation(shaderprog, name.c_str());
		if (i <0) {
			std::cout << "Failed to Locate Attribute.\n";
		}
		offset = offset_val;
	}

	void enable() {
		glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, totalvals * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		return;
	}


};

