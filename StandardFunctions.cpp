#include"StandardFunctions.h"
#include"vertexShader.h"
#include "buffer_objects.h"
#include "Shapes.h"


void pushVectors(std::vector<float>& v, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec2 d) {
	v.push_back(a.x);
	v.push_back(a.y);//position
	v.push_back(a.z);
	v.push_back(b.x);
	v.push_back(b.y);//color
	v.push_back(b.z);
	v.push_back(c.x);
	v.push_back(c.y);//normal
	v.push_back(c.z);
	v.push_back(d.x);
	v.push_back(d.y);

}

void pushvals(std::vector<float>& v, float init_x, float fin_x, float init_y, float fin_y, float sharpness) {
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
			pushVectors(v, vertex[0], color, cp0 ,uv );
			pushVectors(v, vertex[1], color, cp0,uv);
			pushVectors(v, vertex[2], color, cp0, uv);

			pushVectors(v, vertex[3], color, cp3, uv);
			pushVectors(v, vertex[1], color, cp3, uv);
			pushVectors(v, vertex[2], color, cp3, uv);



		}
	}



}

GLuint shaderSet() {						//Compile and bind the shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
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

	return shaderProgram;
}

sf::ContextSettings windowInit() {
	sf::ContextSettings set;
	set.majorVersion = 3;
	set.minorVersion = 2;
	set.antialiasingLevel = 8;
	set.attributeFlags = sf::ContextSettings::Core;
	set.depthBits = 24;
	set.stencilBits = 16;

	return set;
}

GLuint loadTexture(std::string filepath , int flip ) {
	stbi_set_flip_vertically_on_load(flip);
	int w, h, channels;
	unsigned char * data = stbi_load(filepath.c_str(), &w, &h, &channels, 0);
	GLuint texture = 0;
	if (data) {
		std::cout << "Loaded " << filepath << std::endl;
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else{
			std::cout << "Failed to load " << filepath << std::endl;
		
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	stbi_image_free(data);
	return texture;

	
	
}

void keplar(sf::RenderWindow& win) {

	bool running = win.isOpen();

	glm::mat4  trans = glm::mat4(1.0f);
	glm::mat4 proj = glm::perspective<float>(glm::radians(75.0), 1280.0 / 960.0f, .25, 400);
	glm::mat4 view = glm::lookAt(
		glm::vec3(10.0f, 10.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);




	VAO a;
	a.bind();
	VBO b;
	b.bind();
	EBO e;
	e.bind();
	GLuint shaderProgram = shaderSet();

	attribute pos(shaderProgram, "position", 0),
		col(shaderProgram, "colour", 3),
		norm(shaderProgram, "normal", 6),
		uv(shaderProgram, "uv", 9);

	pos.enable();
	col.enable();
	norm.enable();
	uv.enable(2);


	GLint transformation = glGetUniformLocation(shaderProgram, "trans");
	GLint projectionU = glGetUniformLocation(shaderProgram, "proj");
	GLint viewU = glGetUniformLocation(shaderProgram, "view");
	GLint light = glGetUniformLocation(shaderProgram, "camera");









	glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));

	
	
	sphere_shape earth(3, glm::vec3(1, 1, 1),  shaderProgram,  "earth.jpg"),
				moon(1.5 , glm::vec3(1,1,1) , shaderProgram , "moon.jpg");
	light_source sun(2.5, shaderProgram , "sun.jpg");
	random_pts r(shaderProgram, glm::vec3(1, 1, 1), "moon.jpg");
	//line l(glm::vec3(0, 0, 0), glm::vec3(50, -50, 50), glm::vec3(1, 1, 1), shaderProgram);











	float i = 0, j = 0, k = 0, theta = 0, fov = 75 , look_at = 10;
	bool view_flag= 0;
	while (running)
	{
		theta += .000005;
		float earth_x, earth_y, earth_z,
		moon_x, moon_y, moon_z;

		earth_x = 25 * sin(theta);
		earth_y = 25 * cos(theta);
		earth_z = 0;

		
		moon_x = earth_x + 12 * sin(theta*13.36);
		moon_y = earth_y + 12 * cos(theta * 13.36);
		moon_z = earth_z + 4.5*sin(theta * 13.36);

		earth.position(earth_x, earth_y, earth_z);
		moon.position(moon_x, moon_y, moon_z);
		sun.position(0, 0, 0);
		earth.rotate(glm::degrees(.000005 * 365) , glm::vec3(0,0,1));
		moon.rotate(glm::degrees(-.000005 * 13.36996), glm::vec3(0, 0, 1));

		sun.draw();
		earth.draw();
		moon.draw();
		r.draw();
		//l.draw();


		

		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
				return ;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				i += .5;
				
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				i -= .5;
				

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				k += .5;
				

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				k -= .5;
				

			}
			else if (winEvent.type == sf::Event::MouseWheelMoved) {
				if (winEvent.mouseWheel.delta > 0) {

					look_at--;
				}
				else if (winEvent.mouseWheel.delta < 0) {
					look_at++;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				view_flag = !view_flag;
				while (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {

				}
			}

			
			glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));
		}

		if (view_flag) {
			view = glm::lookAt(
				earth.get_cam(),
				earth.get_lookAt(),
				glm::vec3(0.0f, 0.0f, 1.0f)
			);
			glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));
		}
		else {
			view = glm::lookAt(
				glm::vec3(look_at,look_at , look_at),
				glm::vec3(i , 0 , k),
				glm::vec3(0.0f, 0.0f, 1.0f)
			);
			glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));
		}
		



		win.display();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}


void LA(sf::RenderWindow& win) {

	bool running = win.isOpen();

	glm::mat4  trans = glm::mat4(1.0f);
	glm::mat4 proj = glm::perspective<float>(glm::radians(75.0), 1280.0 / 960.0f, .25, 400);
	glm::mat4 view = glm::lookAt(
		glm::vec3(10.0f, 10.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);




	VAO a;
	a.bind();
	VBO b;
	b.bind();
	EBO e;
	e.bind();
	GLuint shaderProgram = shaderSet();

	attribute pos(shaderProgram, "position", 0),
		col(shaderProgram, "colour", 3),
		norm(shaderProgram, "normal", 6),
		uv(shaderProgram, "uv", 9);

	pos.enable();
	col.enable();
	norm.enable();
	uv.enable(2);


	GLint transformation = glGetUniformLocation(shaderProgram, "trans");
	GLint projectionU = glGetUniformLocation(shaderProgram, "proj");
	GLint viewU = glGetUniformLocation(shaderProgram, "view");
	GLint light = glGetUniformLocation(shaderProgram, "camera");









	glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));



	
	random_pts r(shaderProgram, glm::vec3(1, 1, 1), "moon.jpg");
	line l(glm::vec3(0, 0, 0), glm::vec3(50, 0, 0), glm::vec3(1, 0, 0), shaderProgram);
	line ly(glm::vec3(0, 0, 0), glm::vec3(0, 50, 0), glm::vec3(0, 1, 0), shaderProgram);
	line lz(glm::vec3(0, 0, 0), glm::vec3(0, 0, 50), glm::vec3(0, 0, 1), shaderProgram);

	glm::mat4x4 rot_matrix(1.0f);


	glm::mat3 x(glm::vec3(1, 0, 1)
		, glm::vec3(1, 0, 1)
		, glm::vec3(1, 1, 0 ));


	r.ste_customTrans(x);





	float i = 0, j = 0, k = 0, theta = 0, fov = 75, look_at = 40;
	bool view_flag = false;
	while (running)
	{
		
		r.draw();
		l.draw();
		ly.draw();
		lz.draw();




		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
				return;
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				i += .5f;

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				i -= .5f;


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				k += .5f;


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				k -= .5f;


			}
			else if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				{
					rot_matrix = glm::rotate( rot_matrix, glm::radians(.6f), glm::vec3(1, 0, 0));


				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(0, 1, 0));


				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(0, 0, 1));


				}


				
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(1, 0, 0));


				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(0, 1, 0));


				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(0, 0, 1));


				}



			}
			else if (winEvent.type == sf::Event::MouseWheelMoved) {
				if (winEvent.mouseWheel.delta > 0) {

					look_at--;
				}
				else if (winEvent.mouseWheel.delta < 0) {
					look_at++;
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				view_flag = !view_flag;
				while (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {

				}
			}


			glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));
		}

		
		view = glm::lookAt(
			glm::vec3(look_at, look_at, look_at),
			glm::vec3(i, 0, k),
			glm::vec3(0.0f, 0.0f, 1.0f));
		view = view * rot_matrix;
			glUniformMatrix4fv(viewU, 1, GL_FALSE, glm::value_ptr(view));
		




		win.display();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

