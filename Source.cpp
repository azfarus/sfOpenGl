#include "StandardFunctions.h"
#include"buffer_objects.h"
#include"Shapes.h"

using namespace std;
bool running;



int main() {



	


	
	sf::ContextSettings set = windowInit();
	sf::RenderWindow win(sf::VideoMode(1280, 960), "LOL",sf::Style::Close, set);
	running = win.isOpen();

	glewExperimental = GL_TRUE;
	glewInit();

	glm::mat4  trans = glm::mat4(1.0f);
	glm::mat4  id = glm::mat4(1.0f);
	glm::mat4 proj = glm::perspective<float>(glm::radians(75.0), 1280.0 / 960.0f, .25,40);
	glm::mat4 view =  glm::lookAt(
		glm::vec3(10.0f, 10.0f,10.0f ),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	
	

	 

	




	
	VAO a;
	a.bind();

	GLuint shaderProgram = shaderSet();

	
	VBO b;
	b.bind();

	
	EBO e;
	e.bind();
	

	GLint transformation = glGetUniformLocation(shaderProgram, "trans");
	GLint projectionU = glGetUniformLocation(shaderProgram, "proj");
	GLint viewU = glGetUniformLocation(shaderProgram, "view");
	GLint camera = glGetUniformLocation(shaderProgram, "camera");

	glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));



	attribute pos(shaderProgram, "position", 0),
		col(shaderProgram, "colour", 3),
		norm(shaderProgram, "normal", 6);

	pos.enable();
	col.enable();
	norm.enable();



	sphere s(3 , transformation), s2(2 , transformation);
	plane p(25, 25, glm::vec3(-12.5, -12.5, 0), transformation);
	

	


	float i = 0 , j = 0 , k = 0 , theta = 0;
	while (running)
	{
	

		
	
		glm::vec3 cam(100 * cos(theta), 100 * sin(theta), 100*(sin(theta)+cos(theta)));
		glUniform3fv(camera, 1, glm::value_ptr(cam));

		s.position( 0, 0, 0);
		s.draw();
		s2.position(10 * cos(theta), 10 * sin(theta), 0);
		theta += .001;
		s2.draw();
		p.draw();
		
		
		glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(transformation, 1, GL_FALSE, glm::value_ptr(trans));
		
		
		
		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
				return 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				
				trans = glm::rotate(trans, glm::radians(.5f), glm::vec3(0.0f, 0.0f, 1.0f));
				
				
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
	