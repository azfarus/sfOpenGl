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

void pushvals(vector<float>& v) {
   // z = sin(x) + sin(y)

	for (float x = -10; x < 10; x += .2) {
		
			v.push_back(x);
			v.push_back(0);
			v.push_back(0);
			v.push_back(1);
			v.push_back(0);
			v.push_back(0);
		
	}

	for (float x = -10; x < 10; x += .2) {

		v.push_back(0);
		v.push_back(x);
		v.push_back(0);
		v.push_back(0);
		v.push_back(1);
		v.push_back(0);

	}

	for (float x = -10; x < 10; x += .2) {

		v.push_back(0);
		v.push_back(0);
		v.push_back(x);
		v.push_back(0);
		v.push_back(0);
		v.push_back(1);

	}

	for (float x = -8.999; x < 9; x += .02) {
		for (float y = -3.999; y < 4; y += .02) {
			v.push_back(x);
			v.push_back(y);
			v.push_back(sin(x) + sin(y));
			v.push_back(1);
			v.push_back(1);
			v.push_back(1);	
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
	glm::mat4 proj = glm::perspective<float>(glm::radians(65.0), 1280.0 / 960.0f, .25, 40);
	glm::mat4 view =  glm::lookAt(
		glm::vec3(10.0f, 10.0f,10.0f ),
		glm::vec3(0.4f, 0.0f, -0.5f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	
	

	 

	float triangle[] = {
		// front
	    0, - 0.525731 , 0.850651,  0,0,1,
  0.850651  ,0,  0.525731,	1,0,1,
  0.850651  ,0, - 0.525731,	0,0,1,
 - 0.850651 , 0, - 0.525731,	0,1,1,
 - 0.850651,  0,  0.525731,	0,1,1,
 - 0.525731  ,0.850651  ,0,	1,0,1,
  0.525731  ,0.850651  ,0,	0,0,1,
  0.525731 ,- 0.850651  ,0,	1,0,1,
 - 0.525731 ,- 0.850651,  0,	0,0,1,
  0, - 0.525731 ,- 0.850651,	1,0,1,
  0 , 0.525731, - 0.850651,	0,1,1,
  0  ,0.525731  ,0.850651,	1,0,1
		
	};

	GLuint elements[] = {
		  2 , 3,  7,
  2  ,8 , 3,
  4 , 5,  6,
  5, 4 , 9,
  7 , 6 , 12,
  6  ,7 , 11,
  10 , 11 , 3,
  11 , 10,  4,
  8 , 9 , 10,
  9 , 8 , 1,
  12 , 1,  2,
  1,  12,  5,
  7 , 3  ,11,
  2 , 7 , 12,
  4 , 6  ,11,
  6 , 5,  12,
  3 , 8 , 10
,  8 , 2 , 1,
  4 , 10,  9,
  5 , 9 , 1
		
	};

	for (int i = 0; i < sizeof(elements) / sizeof(GLuint); i++) {
		elements[i]--;
	}

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
	GLint transformation = glGetUniformLocation(shaderProgram, "trans");
	GLint projectionU = glGetUniformLocation(shaderProgram, "proj");
	GLint viewU = glGetUniformLocation(shaderProgram, "view");

	glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));


	


	
		

	

	cout << glGetError() << endl;
	float i = 0 , j = 0 , k = 0;
	while (running)
	{
		//win.clear();

	
		//Sleep(500);

		
		glBufferData(GL_ARRAY_BUFFER, graph.size(), points, GL_STATIC_DRAW);

		
		
		
		

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
		

		
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(posAttrib);

		glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(colorAttrib);

		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));

		glDrawArrays(GL_POINTS, 0, graph.size());
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
	