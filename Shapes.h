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
		cam_pos = center;
		lookAt = glm::vec3(0, 0, 0);
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
		center = pos * glm::vec4(0, 0, 0, 1);
	}
	void scale(float x , float y , float z) {
		scal = glm::scale(glm::mat4x4(1.0f), glm::vec3(x, y, z));
	}

	void draw() {
		
		tex.bind();
		glm::mat4 trans = pos *rot *scal ;

		glUniform1i(this->glpoint, 0);
		glUniformMatrix4fv(rot_scale, 1, GL_FALSE, glm::value_ptr(rot ));
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
		genVertices();
	}

	
	void genVertices() {
		buffer.clear();
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

class sphere_obj : public sphere_shape{
protected:
	glm::vec3 velocity;
	glm::vec3 space_coord;
	float g;
public:
	sphere_obj(GLint shader ,glm::vec3 vel , glm::vec3 pos) : sphere_shape(3 , shader , "moon.jpg") {
		velocity = vel;
		space_coord = pos;
		g = -9.806F;
	}

	void update(us_time del, float grav) {
		g = grav;
		double delta = del.count() * 1e-6;
		velocity += glm::vec3(0, 0, g * delta );
		
		if( center.z <  radius)
		{
			std::cout << radius << "  ";
			print(center);
			velocity = glm::reflect(velocity, glm::vec3(0, 0, 1));
		}
		space_coord += glm::vec3(velocity.x * delta , velocity.y * delta , velocity.z * delta );
		position(space_coord.x, space_coord.y, space_coord.z);
		draw();
		return;

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
		buffer.clear();
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
	float spherical_rad;
public:
	random_pts(GLint shader , glm::vec3 clr , std::string f_path ):base_shape(shader , clr , f_path) {
		custom_trans = glm::mat4(1.0f);
		spherical_rad = 300;
		genVertices();
	}

	void set_spherical_rad(float rad)
	{
		this->spherical_rad = rad;
		genVertices();
	}
	void genVertices() {
		
		buffer.clear();
		for (unsigned int i = 0; i < 500; i++) {
			glm::vec3 point = glm::sphericalRand(spherical_rad);
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

		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(custom_trans));
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
		glDrawArrays(GL_POINTS, 0 ,buffer.size());



	}
};


//////////---------------------------Graph Part-----------------------------////////////////////
class graph : public base_shape {
private:
	int type;
	float a, b, c;
public:
		//Constructor

	graph(GLint shader, glm::vec3 color, std::string filepath = "default.png") : base_shape(shader, color, filepath) {
		std::cout << "Enter Type: ";
		std::cin >> type;
		genVertices(type);
	}
	//Setter Function
	void setParameters()
	{
		std::cout << "Enter the values of a,b,c for the equations: ";
		std::cin >> a >> b >> c;
	}
	//Getter
	float getA()
	{
		return a;
	}
	float getB()
	{
		return b;
	}
	float getC()
	{
		return c;
	}

	void pushSin(std::vector<float>& v, float init_x, float fin_x, float init_y, float fin_y, float sharpness) {
		// z = x + y 
		v.clear();


		const float factor = (abs(fin_x - init_x) / sharpness);
		for (float x = init_x; x < fin_x; x += factor) {
			for (float y = init_y; y < fin_y; y += factor) {

				glm::vec3 vertex[4];
				float arrx[] = { factor , 0 , factor , 0 };
				float arry[] = { factor , factor , 0 , 0 };
				float m, n;
				for (int i = 0; i < 4; i++) {


					m = x + arrx[i];
					n = y + arry[i];
					vertex[i] = glm::vec3(m, n, asin(sin(m) + sin(n)));
				}
				glm::vec3 vex01, vex02, vex31, vex32, cp0, cp3, color;
				color = glm::vec3(1, 1, 1);
				vex01 = vertex[1] - vertex[0];
				vex02 = vertex[2] - vertex[0];
				vex31 = vertex[1] - vertex[3];
				vex32 = vertex[2] - vertex[3];

				cp0 = glm::cross(vex31, vex32);
				cp3 = glm::cross(vex02, vex01);
				cp0 = glm::normalize(cp0);
				cp3 = glm::normalize(cp3);
				glm::vec2 uv(0, 0);
				pushVectors(v, vertex[0], color, -cp0, uv);
				pushVectors(v, vertex[1], color, -cp0, uv);
				pushVectors(v, vertex[2], color, -cp0, uv);

				pushVectors(v, vertex[3], color, -cp3, uv);
				pushVectors(v, vertex[1], color, -cp3, uv);
				pushVectors(v, vertex[2], color, -cp3, uv);



			}
		}

	}
	void pushCos(std::vector<float>& v, float init_x, float fin_x, float init_y, float fin_y, float sharpness) {
		// z = x + y cos variant
		v.clear();


		const float factor = (abs(fin_x - init_x) / sharpness);
		for (float x = init_x; x < fin_x; x += factor) {
			for (float y = init_y; y < fin_y; y += factor) {

				glm::vec3 vertex[4];
				float arrx[] = { factor , 0 , factor , 0 };
				float arry[] = { factor , factor , 0 , 0 };
				float m, n;
				for (int i = 0; i < 4; i++) {


					m = x + arrx[i];
					n = y + arry[i];
					vertex[i] = glm::vec3(m, n, acos(cos(m) + cos(n)));
				}
				glm::vec3 vex01, vex02, vex31, vex32, cp0, cp3, color;
				color = glm::vec3(1, 1, 1);
				vex01 = vertex[1] - vertex[0];
				vex02 = vertex[2] - vertex[0];
				vex31 = vertex[1] - vertex[3];
				vex32 = vertex[2] - vertex[3];

				cp0 = glm::cross(vex31, vex32);
				cp3 = glm::cross(vex02, vex01);
				cp0 = glm::normalize(cp0);
				cp3 = glm::normalize(cp3);
				glm::vec2 uv(0, 0);
				pushVectors(v, vertex[0], color, -cp0, uv);
				pushVectors(v, vertex[1], color, -cp0, uv);
				pushVectors(v, vertex[2], color, -cp0, uv);

				pushVectors(v, vertex[3], color, -cp3, uv);
				pushVectors(v, vertex[1], color, -cp3, uv);
				pushVectors(v, vertex[2], color, -cp3, uv);



			}
		}

	}
	void pushCubic(std::vector<float>& v, float init_x, float fin_x, float init_y, float fin_y, float sharpness) {
		// c*z = b*x*x*x + a*y*y*y
		v.clear();


		const float factor = (abs(fin_x - init_x) / sharpness);
		for (float x = init_x; x < fin_x; x += factor) {
			for (float y = init_y; y < fin_y; y += factor) {

				glm::vec3 vertex[4];
				float arrx[] = { factor , 0 , factor , 0 };
				float arry[] = { factor , factor , 0 , 0 };
				float m, n;
				for (int i = 0; i < 4; i++) {


					m = x + arrx[i];
					n = y + arry[i];
					vertex[i] = glm::vec3(m, n, (m*m*m)+(n*n*n));
				}
				glm::vec3 vex01, vex02, vex31, vex32, cp0, cp3, color;
				color = glm::vec3(1, 1, 1);
				vex01 = vertex[1] - vertex[0];
				vex02 = vertex[2] - vertex[0];
				vex31 = vertex[1] - vertex[3];
				vex32 = vertex[2] - vertex[3];

				cp0 = glm::cross(vex31, vex32);
				cp3 = glm::cross(vex02, vex01);
				cp0 = glm::normalize(cp0);
				cp3 = glm::normalize(cp3);
				glm::vec2 uv(0, 0);
				pushVectors(v, vertex[0], color, -cp0, uv);
				pushVectors(v, vertex[1], color, -cp0, uv);
				pushVectors(v, vertex[2], color, -cp0, uv);

				pushVectors(v, vertex[3], color, -cp3, uv);
				pushVectors(v, vertex[1], color, -cp3, uv);
				pushVectors(v, vertex[2], color, -cp3, uv);



			}
		}

	}
	void pushEll(std::vector<float>& v, float init_x, float fin_x, float init_y, float fin_y, float sharpness) {
		// z = Ellipse
		v.clear();

		float sqA = a * a;
		float sqB = b * b;
		float sqC = c * c;

		const float factor = (abs(fin_x - init_x) / sharpness);
		for (float x = init_x; x < fin_x; x += factor) {
			for (float y = init_y; y < fin_y; y += factor) {

				glm::vec3 vertex[4];
				float arrx[] = { factor , 0 , factor , 0 };
				float arry[] = { factor , factor , 0 , 0 };
				float m, n;
				float sqm, sqn;
				float mn;
				for (int i = 0; i < 4; i++) {


					m = x + arrx[i];
					n = y + arry[i];
					sqm = m * m;
					sqm /= sqA;
					
					sqn = n * n;
					sqn /= sqB;

					mn = (1 - sqm - sqn) * sqC;;
					vertex[i] = glm::vec3(m, n, sqrt(mn)); //x^2/a^2+y^2/b^2+z^2/c^2 = 1
				}
				glm::vec3 vex01, vex02, vex31, vex32, cp0, cp3, color;
				color = glm::vec3(1, 1, 1);
				vex01 = vertex[1] - vertex[0];
				vex02 = vertex[2] - vertex[0];
				vex31 = vertex[1] - vertex[3];
				vex32 = vertex[2] - vertex[3];

				cp0 = glm::cross(vex31, vex32);
				cp3 = glm::cross(vex02, vex01);
				cp0 = glm::normalize(cp0);
				cp3 = glm::normalize(cp3);
				glm::vec2 uv(0, 0);
				pushVectors(v, vertex[0], color, -cp0, uv);
				pushVectors(v, vertex[1], color, -cp0, uv);
				pushVectors(v, vertex[2], color, -cp0, uv);

				pushVectors(v, vertex[3], color, -cp3, uv);
				pushVectors(v, vertex[1], color, -cp3, uv);
				pushVectors(v, vertex[2], color, -cp3, uv);



			}
		}

	}
	void genVertices(int t) {
		buffer.clear();
		if (buffer.size() > 0) return;

		switch (t)
		{
			case 1:
				pushSin(buffer, -10, 10, -10, 10, 100);//sinx
				buffer_pointer = &buffer[0];
				break;
			case 2:
				pushCos(buffer, -10, 10, -10, 10, 100);//cosx
				buffer_pointer = &buffer[0];
				break;
			case 3:
				pushCubic(buffer, -10, 10, -10, 10, 100);//Cubic
				buffer_pointer = &buffer[0];
				break;
			case 4:
				pushEll(buffer, -10, 10, -10, 10, 100);//Ellipse
				buffer_pointer = &buffer[0];
		}
		
		
	}


	void draw()
	{
		

			tex.bind();
			glm::mat4 trans = pos * rot * scal;
			glUniform1i(this->glpoint, 2);

			glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
			glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer_pointer, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, buffer.size());



		
	}
};