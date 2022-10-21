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

	
	

	 

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);




	VAO a; 
	
	a.bind();
	

GLuint shaderProgram = shaderSet();

	
	VBO b;
	b.bind();
	EBO e;
	e.bind();

	attribute pos(shaderProgram, "position", 0),
		col(shaderProgram, "colour", 3),
		norm(shaderProgram, "normal", 6),
		uv(shaderProgram, "uv", 9);
	
	
	 
	
	

	GLint transformation = glGetUniformLocation(shaderProgram, "trans");
	GLint projectionU = glGetUniformLocation(shaderProgram, "proj");
	GLint viewU = glGetUniformLocation(shaderProgram, "view");
	GLint light = glGetUniformLocation(shaderProgram, "camera");

	glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));
	
	sphere_shape s(3,glm::vec3(1,1,1), transformation , "C:/Users/samaz/Pictures/jupiter.jpg");
	//sphere_shape s2(1, glm::vec3(1, 1, 0) ,transformation);
	light_source s3(3, transformation, light, "C:/Users/samaz/Pictures/sun.jpg");
	
	pos.enable();
	col.enable();
	norm.enable();
	uv.enable(2);

	



	
	//plane p(25, 25, glm::vec3(-12.5, -12.5, 0), transformation);
	


	


	float i = 0 , j = 0 , k = 0 , theta = 0;
	while (running)
	{
	

		
	
		

		s.position( 0, 0, 0);
		
		s.draw();
		s3.position(10 * cos(theta), 10 * sin(theta), 0);
		theta += .001;
		s3.scale(1.003, 1, 1);
		s3.rotate(.1, glm::vec3(0, 0, 1));
		s3.draw();
		//p.draw();
		
		
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
	