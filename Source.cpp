#define GLEW_STATIC

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/OpenGL.hpp>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include"vertexShader.h"
#include"fragmentShader.h"
#include<Windows.h>
#include<vector>

using namespace std;
bool running;

void pushVectors(vector<float>& v, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	v.push_back(a.x);
	v.push_back(a.y);
	v.push_back(a.z);
	v.push_back(b.x);
	v.push_back(b.y);
	v.push_back(b.z);
	v.push_back(c.x);
	v.push_back(c.y);
	v.push_back(c.z);

}

void pushvals(vector<float>& v) {
   // z = sin(x) + sin(y)
	v.clear();

	/*for (float i = 10; i > -10; i -= .2) {
		v.push_back(i);
		v.push_back(0);
		v.push_back(0);
		v.push_back(1);
		v.push_back(0);
		v.push_back(0);
	}

	for (float i = 10; i > -10; i -= .2) {
		v.push_back(0);
		v.push_back(i);
		v.push_back(0);
		v.push_back(0);
		v.push_back(1);
		v.push_back(0);
	}

	for (float i = 10; i > -10; i -= .2) {
		v.push_back(0);
		v.push_back(0);
		v.push_back(i);
		v.push_back(0);
		v.push_back(0);
		v.push_back(1);
	}*/
	const float factor = (20.0 / 500);
	for (float x = -10; x < 10; x +=factor ) {
		for (float y =-10 ; y <10 ; y +=factor ) {
			
			glm::vec3 vertex[4];
			float arrx[] = { factor , 0 , factor , 0 };
			float arry[] = { factor , factor , 0 , 0 };
			float m, n;
			for (int i = 0; i < 4; i++) {

				
				m = x + arrx[i];
				n = y + arry[i];
				vertex[i] = glm::vec3(m, n, asin(sin(m)+sin(n)));
			}
			glm::vec3 vex01, vex02, vex31, vex32 , cp0 , cp3 ,color;
			color = glm::vec3(1, 1, 1);
			vex01 = vertex[1] - vertex[0];
			vex02 = vertex[2] - vertex[0];
			vex31 = vertex[1] - vertex[3];
			vex32 = vertex[2] - vertex[3];

			cp0 = glm::cross(vex31, vex32);
			cp3 = glm::cross(vex02, vex01);
			cp0 = glm::normalize(cp0);
			cp3 = glm::normalize(cp3);

			pushVectors(v, vertex[0], color, cp0);
			pushVectors(v, vertex[1], color, cp0);
			pushVectors(v, vertex[2], color, cp0);

			pushVectors(v, vertex[3], color, cp3);
			pushVectors(v, vertex[1], color, cp3);
			pushVectors(v, vertex[2], color, cp3);


			
		}
	}

	
	
}

int main() {



	


	sf::ContextSettings set;
	set.majorVersion = 3;
	set.minorVersion = 2;
	set.antialiasingLevel = 8;
	set.attributeFlags = sf::ContextSettings::Core;
	set.depthBits = 24;
	set.stencilBits = 16;

	sf::RenderWindow win(sf::VideoMode(1280, 960), "LOL",sf::Style::Close, set);
	running = win.isOpen();

	glewExperimental = GL_TRUE;
	glewInit();

	glm::mat4  trans = glm::mat4(1.0f);
	glm::mat4  id = glm::mat4(1.0f);
	glm::mat4 proj = glm::perspective<float>(glm::radians(25.0), 1280.0 / 960.0f, .25,40);
	glm::mat4 view =  glm::lookAt(
		glm::vec3(10.0f, 10.0f,10.0f ),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	
	

	 

	



	float* points;
	vector<float> graph;
	pushvals(graph);

	points = &graph[0];
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vertexShader =glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	
	GLuint vb;
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	GLint colorAttrib = glGetAttribLocation(shaderProgram, "colour");
	GLint normalAttrib = glGetAttribLocation(shaderProgram, "normal");
	GLint transformation = glGetUniformLocation(shaderProgram, "trans");
	GLint projectionU = glGetUniformLocation(shaderProgram, "proj");
	GLint viewU = glGetUniformLocation(shaderProgram, "view");

	glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));

	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(colorAttrib);

	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(normalAttrib);


	


	
		


	float i = 0 , j = 0 , k = 0;
	while (running)
	{
	

		
		glBufferData(GL_ARRAY_BUFFER, graph.size()*sizeof(float), points, GL_STATIC_DRAW);
		
		

		
		

		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));

		glDrawArrays(GL_TRIANGLES, 0, graph.size());
		//glDrawElements(GL_TRIANGLE_STRIP, sizeof(elements)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		
		
		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
				return 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				
				trans = glm::rotate(trans, glm::radians(.5f), glm::vec3(0.0f, 0.0f, 1.0f));
				//trans = glm::rotate(trans, glm::radians(.5f), glm::vec3(0.0f, 1.0f, 0.0f));
				
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				i += .1;
				view = glm::lookAt(
					glm::vec3(10.0f, 10.0f, 10.0f),
					glm::vec3(i, 0.0f, k),
					glm::vec3(0.0f, 0.0f, 1.0f)
				);
				
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				i -= .1;
				view = glm::lookAt(
					glm::vec3(10.0f, 10.0f, 10.0f),
					glm::vec3(i, 0.0f, k),
					glm::vec3(0.0f, 0.0f, 1.0f)
				);
				
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				k += .1;
				view = glm::lookAt(
					glm::vec3(10.0f, 10.0f, 10.0f),
					glm::vec3(i, 0.0f, k),
					glm::vec3(0.0f, 0.0f, 1.0f)
				);
				
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				k -= .1;
				view = glm::lookAt(
					glm::vec3(10.0f, 10.0f, 10.0f),
					glm::vec3(i, 0.0f, k),
					glm::vec3(0.0f, 0.0f, 1.0f)
				);

			}

		}
		win.display();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}
	