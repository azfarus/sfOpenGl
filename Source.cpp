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


using namespace std;
bool running;

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
	
	

	 

	float triangle[] = {
	 0.5f,  0.5f, 0 , 1.0f, 0.0f, 0.0f, // Top-left
	-0.5f,  0.5f, 0 , 0.0f, 1.0f, 0.0f, // Top-right
	 0.5f, -0.5f, 0 , 0.0f, 0.0f, 1.0f, // Bottom-right
	-0.5f, -0.5f, 0 , 1.0f, 1.0f, 1.0f,
	  1.0f, 1.0f, 0 , 1.0f, 1.0f, 1.0f
		
	};

	GLuint elements[] = {
		0,1,2,
		2,3,1,
		2,4,0
	};
	
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

	

	

	

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	GLint colorAttrib = glGetAttribLocation(shaderProgram, "colour");
	GLint transformation = glGetUniformLocation(shaderProgram, "trans");



	
		

	

	cout << glGetError() << endl;
	float i = 0;
	while (running)
	{
		win.clear();

	
		//Sleep(500);

		GLuint vb;
		glGenBuffers(1, &vb);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);


		
		
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
		

		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(posAttrib);

		glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(colorAttrib);

		glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		//glDrawArrays(GL_TRIANGLES, 0, 4 );
		glDrawElements(GL_TRIANGLES, sizeof(elements)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
				return 0;
			}
			if (winEvent.MouseMoved) {
				i += .0001;
				trans = glm::rotate(id, glm::radians(i), glm::vec3(0.0f, 0.0f, 1.0f));
				if (i > 360) i = 0;
			}
		}
		win.display();
	}

}
	