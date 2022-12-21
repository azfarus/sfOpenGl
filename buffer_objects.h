#pragma once
#include"StandardFunctions.h"
#include <mutex>

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

	void reset() {
		*this = camera(shaderprogram);
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
		
		if(fw) x +=  1.0f;
		else x -= 1.0f;
		float divisor = x2 - x1;
		if (abs(divisor) < .001) divisor = .01;
		y = ((x - x1) * (y2 - y1)) / (divisor) + y1;
		z = ((x - x1) * (z2 - z1)) / (divisor) + z1;

		this->position = glm::vec3(x, y, z);
		this->look_at = position - look_at_finder;
		return;


	}

	void rotate_camera_horizontal(float dir = 1 ,float  sens = 2)
	{
		float amount = dir * sens;

		cam_horizontal = glm::rotate(glm::mat4x4(1.0f), glm::radians(amount), glm::vec3(cam_vertical)) * cam_horizontal ;

		this->look_at_finder = glm::rotate(glm::mat4x4(1.0f), glm::radians(amount), glm::vec3(cam_vertical)) * glm::vec4(this->look_at_finder ,1);

		
		this->look_at = position - look_at_finder;
	}

	void rotate_camera_vertical(float dir = 1, float  sens = 2)
	{
		float amount = dir * sens;

		cam_vertical = glm::rotate(glm::mat4x4(1.0f), glm::radians(amount), glm::vec3(cam_horizontal)) * cam_vertical;

		this->look_at_finder = glm::rotate(glm::mat4x4(1.0f), glm::radians(amount), glm::vec3(cam_horizontal)) * glm::vec4(this->look_at_finder, 1);
		this->look_at = position - look_at_finder;
	}

	void update(glm::mat4x4 model_rotate = glm::mat4x4(1.0f))
	{
		glm::mat4x4 view = glm::lookAt(this->position, this->look_at, this->axis);
		
		glUniformMatrix4fv(this->viewU, 1, GL_FALSE, glm::value_ptr(view*model_rotate));
	}
};



class camera2
{
	GLuint shaderprogram, viewU;
	glm::vec4 position;
	glm::vec4 look_at; 
	glm::vec3 axis;

	glm::vec4 cam_up, cam_horz, cam_front;
	
public:
	camera2(GLuint shader)
	{
		this->shaderprogram = shader;

		cam_up = glm::vec4(0, 0, 1, 1);
		cam_horz = glm::vec4(0, 1, 0, 1);
		cam_front = glm::vec4(-1, 0, 0, 1);

		position = glm::vec4(0, 0, 0 , 1);
		look_at = position + cam_front;
		axis = glm::vec3(0, 0, 1);
		
		this->viewU = glGetUniformLocation(this->shaderprogram, "view");

	}

	void reset() {
		*this = camera2(shaderprogram);
	}

	void set_position(glm::vec4 pos)
	{
		this->position = pos;
	}
	
	void set_axis(glm::vec3 axis)
	{
		this->axis = axis;
	}

	void move_position(int fw = 1 , float sens = 300)
	{
		
		glm::vec3 incr((fw*cam_front.x) / sens, (fw*cam_front.y) / sens, (fw*cam_front.z) / sens);
		position = glm::vec4((glm::vec3(position) + incr) , position.w);
		//position =  glm::translate(glm::mat4x4(1.0f), incr) * position ;
		

	}
	void move_position_side(int fw = 1, float sens = 300)
	{

		glm::vec3 incr((fw * cam_horz.x) / sens, (fw * cam_horz.y) / sens, (fw * cam_horz.z) / sens);
		position = glm::vec4((glm::vec3(position) + incr), position.w);
		//position =  glm::translate(glm::mat4x4(1.0f), incr) * position ;


	}
	void move_position_up(int fw = 1, float sens = 300)
	{

		glm::vec3 incr((fw * cam_up.x) / sens, (fw * cam_up.y) / sens, (fw * cam_up.z) / sens);
		position = glm::vec4((glm::vec3(position) + incr), position.w);
		//position =  glm::translate(glm::mat4x4(1.0f), incr) * position ;


	}

	void rotate_camera_horizontal(int dir = 1, float  sens = .6f)
	{
		cam_front =  glm::rotate(glm::mat4x4(1.0f), glm::radians(sens * dir), glm::vec3(cam_up)) * cam_front   ;
		cam_horz = glm::rotate(glm::mat4x4(1.0f), glm::radians(sens * dir), glm::vec3(cam_up)) * cam_horz;
	}

	void rotate_camera_vertical(int dir = 1, float  sens = .6)
	{

		cam_front = glm::rotate(glm::mat4x4(1.0f), glm::radians(sens * dir), glm::vec3(cam_horz)) * cam_front;
		cam_up = glm::rotate(glm::mat4x4(1.0f), glm::radians(sens * dir), glm::vec3(cam_horz)) * cam_up;
		
	}

	void update(glm::mat4x4 r = glm::mat4x4(1.0f))
	{
		look_at = position + cam_front;

		glm::mat4x4 view = glm::lookAt(glm::vec3(this->position), glm::vec3(this->look_at), this->axis);

		glUniformMatrix4fv(this->viewU, 1, GL_FALSE, glm::value_ptr(view * r));
	}
};




