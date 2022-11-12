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

class camera
{
	GLuint shaderprogram , viewU;
	glm::vec3 position;
	glm::vec3 look_at , look_at_finder; // position = look_at + look_at_finder
	glm::vec3 axis;
	

	glm::vec4 cam_vertical;
	glm::vec4 cam_horizontal;
public:
	camera(GLuint shader )
	{
		this->shaderprogram = shader;
		
		position = glm::vec3(10, 10, 10);
		look_at = glm::vec3(5, 5, 5);
		axis = glm::vec3(0, 0, 1);
		look_at_finder = position - look_at;
		cam_vertical = glm::rotate(glm::mat4x4(1.0f), glm::radians(90.0f), glm::vec3(-1, 1, 0)) * glm::vec4(position,1);
		cam_horizontal = glm::vec4(-1, 1, 0, 1);
		this->viewU = glGetUniformLocation(this->shaderprogram, "view");

	}

	void set_position(glm::vec3 pos)
	{
		this->position = pos;
	}
	void set_lookat(glm::vec3 lookat)
	{
		this->look_at = lookat;
	}
	void set_axis(glm::vec3 axis)
	{
		this->axis = axis;
	}

	void move_position(bool fw = 1 )
	{
		float x1,x2,
		      y1,y2,
		      z1,z2;
		x1 = this->look_at.x;
		y1 = this->look_at.y;
		z1 = this->look_at.z;

		x2 = this->position.x;
		y2 = this->position.y;
		z2 = this->position.z;

		float x = x2, y = y2, z = z2;
		float del = (x2 - x1) / abs(x2 - x1);
		if(fw) x += del * .2f;
		else x -= del*1.0f;
		y = ((x - x1) * (y2 - y1)) / (x2 - x1) + y1;
		z = ((x - x1) * (z2 - z1)) / (x2 - x1) + z1;

		this->position = glm::vec3(x, y, z);
		return;


	}

	void rotate_camera_horizontal(float dir = 1 ,float  sens = .5)
	{
		float amount = dir * sens;

		cam_horizontal = glm::rotate(glm::mat4x4(1.0f), glm::radians(amount), glm::vec3(cam_vertical)) * cam_horizontal ;

		this->look_at_finder = glm::rotate(glm::mat4x4(1.0f), glm::radians(amount), glm::vec3(cam_vertical)) * glm::vec4(this->look_at_finder ,1);

		
		this->look_at = position - look_at_finder;
	}

	void rotate_camera_vertical(float dir = 1, float  sens = .5)
	{
		float amount = dir * sens;

		cam_vertical = glm::rotate(glm::mat4x4(1.0f), glm::radians(amount), glm::vec3(cam_horizontal)) * cam_vertical;

		this->look_at_finder = glm::rotate(glm::mat4x4(1.0f), glm::radians(amount), glm::vec3(cam_horizontal)) * glm::vec4(this->look_at_finder, 1);
		this->look_at = position - look_at_finder;
	}

	void update()
	{
		glm::mat4x4 view = glm::lookAt(this->position, this->look_at, this->axis);
									
		glUniformMatrix4fv(this->viewU, 1, GL_FALSE, glm::value_ptr(view));
	}
};

