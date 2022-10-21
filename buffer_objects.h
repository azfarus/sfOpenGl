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
	void unbind() {
		glBindVertexArray(0);
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
		
	}
	void unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	void unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
};


class attribute {
	GLint i;
	const int totalvals;
	int offset;
	std::string name;
public:

	
	attribute(GLuint shaderprog  , std::string name ,int offset_val ) : totalvals(11) {
		i = glGetAttribLocation(shaderprog, name.c_str());
		if (i <0) {
			std::cout << "Failed to Locate Attribute.\n";
		}
		offset = offset_val;
	}

	void enable(int read = 3) {
		glVertexAttribPointer(i, read, GL_FLOAT, GL_FALSE, totalvals * sizeof(float), (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(i);
		return;
	}


};

class texture {
	GLuint textureID;
	std::string filepath;
public:
	texture(std::string filepath) {
		this->filepath = filepath;
		textureID = loadTexture(filepath);
	}
	void bind() {
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	void unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

