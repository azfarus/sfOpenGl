#include"StandardFunctions.h"
#include"vertexShader.h"
#include "buffer_objects.h"
#include "Shapes.h"
#include <thread>


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

void keplar(sf::RenderWindow& win, data_object& d) {

	
	

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
	GLint light = glGetUniformLocation(shaderProgram, "camera");









	
	glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));

	
	
	sphere_shape earth(3, glm::vec3(1, 1, 1),  shaderProgram,  "earth.jpg"),
				moon(1.5 , glm::vec3(1,1,1) , shaderProgram , "moon.jpg");
	light_source sun(2.5, shaderProgram , "sun.jpg");
	random_pts r(shaderProgram, glm::vec3(1, 1, 1), "moon.jpg" );
	//line l(glm::vec3(0, 0, 0), glm::vec3(50, -50, 50), glm::vec3(1, 1, 1), shaderProgram);


	camera c1(shaderProgram);



	/*Button exit;
	exit.create(600, 600, 400, 400, "exit.png");*/




	float  theta = 0, fov = 75;
	float inc;
	bool view_flag = 0;

	//Pseudo prompting
	std::cout << "Please Enter How Fast You want the rotation (Suggested value = 0.000005) : ";
	std::cin >> inc;





	while (running)
	{
		//theta += .000005;
		theta += inc;
		float earth_x, earth_y, earth_z,
			moon_x, moon_y, moon_z;

		earth_x = 25 * sin(theta);
		earth_y = 25 * cos(theta);
		earth_z = 0;


		moon_x = earth_x + 12 * sin(theta * 13.36);
		moon_y = earth_y + 12 * cos(theta * 13.36);
		moon_z = earth_z + 4.5 * sin(theta * 13.36);

		earth.position(earth_x, earth_y, earth_z);
		moon.position(moon_x, moon_y, moon_z);
		sun.position(0, 0, 0);
		float inc_neg = 0 - inc;
		earth.rotate(glm::degrees(inc * 365), glm::vec3(0, 0, 1));
		moon.rotate(glm::degrees(inc_neg * 13.36996), glm::vec3(0, 0, 1));

		sun.draw();
		earth.draw();
		moon.draw();
		r.draw();
		//l.draw();


		

		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
				win.close();
				
				
				return ;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				
				c1.rotate_camera_horizontal(-1);
				
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				c1.rotate_camera_horizontal(1);
				

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				c1.rotate_camera_vertical(1);
				

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				c1.rotate_camera_vertical(-1);
				

			}
			else if (winEvent.type == sf::Event::MouseWheelMoved) {
				if (winEvent.mouseWheel.delta < 0) {

					c1.move_position(true);
				}
				else if (winEvent.mouseWheel.delta > 0) {
					c1.move_position(false);
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				view_flag = !view_flag;
				c1 = camera(shaderProgram);
				while (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {

				}
			}

			
			glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));
		}

		if(view_flag)
		{
			c1.set_position(earth.get_cam());
			c1.set_lookat(earth.get_lookAt());
		}

		c1.update();

		
		/*if (exit.onButton(win))
		{
			menuscreen(win);
		}

		exit.drawButton(win);*/

		



		win.display();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

	}

	

}

void physics(sf::RenderWindow& win, data_object& d) {
	bool running = win.isOpen();

	Button exit;
	exit.create(870, 850, 100, 40, "exit.png");
	

	glm::mat4 proj = glm::perspective<float>(glm::radians(75.0), 1280.0 / 960.0f, .25, 400);
	

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

	GLint projectionU = glGetUniformLocation(shaderProgram, "proj");
	


    glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));


	sphere_shape s(50, shaderProgram, "sun.jpg");

	light_source l1(3, shaderProgram, "moon.jpg");

	sphere_obj ball(shaderProgram, glm::vec3(3, 0, 0), glm::vec3(0, 0, 15));

	line l(glm::vec3(-100, 0, 0), glm::vec3(100, 0, 0), glm::vec3(1, 0, 0), shaderProgram);
	line l2(glm::vec3(0, -100, 0), glm::vec3(0, 100, 0), glm::vec3(1, 1, 0), shaderProgram);

	camera2 c1(shaderProgram);

	time_data prev_time, curr_time;
	//PseudoPrompting for g
	float grav;
	std::cout << "Enter preferred g: ";
	std::cin >> grav;
	//For converting to minus
	grav = 0 - grav;

	obj_file plane(shaderProgram , "plane.obj" , glm::vec3(.5 , .5 , 0.5));
	
    
	
	
	l1.position(100, 10, 100);

	s.position(0, 0, 0);


	

	curr_time = prev_time = std::chrono::steady_clock::now();
	

	c1.set_position(glm::vec4(10, 10, 10, 1));
	
	plane.rotate(90, glm::vec3(1, 0, 0));
	plane.scale(100, 100, 100);
	while (running)
	{

		curr_time = std::chrono::steady_clock::now();
		us_time del = curr_time - prev_time;
		prev_time = curr_time;
		
	
		
		
		
		
		
		ball.update(del, grav);
		l1.draw();
		plane.draw(1);

		
		
		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
				win.close();

				return;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				c1.rotate_camera_horizontal(1);

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				c1.rotate_camera_horizontal(-1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				c1.rotate_camera_vertical(1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				c1.rotate_camera_vertical(-1);


			}
			else if (winEvent.type == sf::Event::MouseWheelMoved) {
				if (winEvent.mouseWheel.delta > 0) {

					c1.move_position(-1, 10);
				}
				else if (winEvent.mouseWheel.delta < 0) {
					c1.move_position(1, 10);
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

				c1.move_position(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				c1.move_position(1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				c1.move_position_side(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				c1.move_position_side(1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
				c1.move_position_up(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				c1.move_position_up(1, 2);
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				c1.reset();
			}


			glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));



		}

		c1.update(); /// pass rot matrix

		/*Button exit;
		exit.create(870, 850, 100, 40, "exit.png");
		if (exit.onButton(win))
		{
			menuscreen(win);
		}

		exit.drawButton(win);*/

		win.display();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}
}


void LA(sf::RenderWindow& win, data_object& d)
{

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
	GLint light = glGetUniformLocation(shaderProgram, "camera");









	
	glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));



	
	random_pts r(shaderProgram, glm::vec3(1, 1, 1), "moon.jpg");
	line l( glm::vec3(0, 0, 0), glm::vec3(50, 0, 0), glm::vec3(1, 0, 0), shaderProgram);
	line ly(glm::vec3(0, 0, 0), glm::vec3(0, 50, 0), glm::vec3(0, 1, 0), shaderProgram);
	line lz(glm::vec3(0, 0, 0), glm::vec3(0, 0, 50), glm::vec3(0, 0, 1), shaderProgram);

	glm::mat4x4 rot_matrix(1.0f);


	//Prompting for Input 
	glm::vec3 first, second, third;
	int r1, r2, r3;

	glm::vec3 row1, row2, row3;
	for (int i = 0; i < 3; i++)
	{
		std::cout << "Enter the elements of row no." << i + 1 << " of a 3x3 matrix : ";
		std::cin >> r1 >> r2 >> r3;
		if (i == 0)
		{
			row1.x = r1;
			row1.y = r2;
			row1.z = r3;
		}
		else if (i == 1)
		{
			row2.x = r1;
			row2.y = r2;
			row2.z = r3;
		}
		else
		{
			row3.x = r1;
			row3.y = r2;
			row3.z = r3;
		}
	}

	//Previous Hardcode below
	//glm::mat3 x(glm::vec3(1, 0, 1)
	//	, glm::vec3(1, 0, 1)
	//	, glm::vec3(1, 1, 0 ));

	glm::mat3 x(row1, row2, row3);


	r.ste_customTrans(x);
	r.set_spherical_rad(20);

	camera2 c1(shaderProgram);



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
				win.close();

				return;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				c1.rotate_camera_horizontal(1);

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				c1.rotate_camera_horizontal(-1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				c1.rotate_camera_vertical(1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				c1.rotate_camera_vertical(-1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(1, 0, 0));
					//obs.rotate(1, glm::vec3(1, 0, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(0, 1, 0));
					//obs.rotate(1, glm::vec3(0, 1, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(0, 0, 1));
					//obs.rotate(1, glm::vec3(0, 0, 1));

				}



			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(1, 0, 0));
					//obs.rotate(-1, glm::vec3(1, 0, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(0, 1, 0));
					//obs.rotate(-1, glm::vec3(0, 1, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(0, 0, 1));
					//obs.rotate(-1, glm::vec3(0, 0, 1));

				}



			}
			else if (winEvent.type == sf::Event::MouseWheelMoved) {
				if (winEvent.mouseWheel.delta > 0) {

					c1.move_position(-1, 10);
				}
				else if (winEvent.mouseWheel.delta < 0) {
					c1.move_position(1, 10);
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

				c1.move_position(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				c1.move_position(1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				c1.move_position_side(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				c1.move_position_side(1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
				c1.move_position_up(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				c1.move_position_up(1, 2);
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				c1.reset();
			}


			glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));



		}






		c1.update(); /// pass rot matrix

		/*Button exit;
		exit.create(870, 850, 100, 40, "exit.png");
		if (exit.onButton(win))
		{
			menuscreen(win);
		}

		exit.drawButton(win);*/

		win.display();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}
}





void print(glm::vec3 x) {
	std::cout << x.x << " " << x.y << " " << x.z << "\n";
}

int menuscreen(sf::RenderWindow &win)
{

	

	sf::RectangleShape background(sf::Vector2f(750.0f, 900.0f));
	background.setPosition(260.0f, 0.0f);
	sf::Texture nightsky;
	nightsky.loadFromFile("3d_xplore.png");
	background.setTexture(&nightsky);


	Button bounce, la, planet, graph, exit, return_to_menu;

	system("cls");
	bounce.create(xthresh, ythresh + 80 * 0, 200, 80, "bb.png");
	la.create(xthresh, ythresh + 80 * 1 + 10, 200, 80, "la.png");
	planet.create(xthresh, ythresh + 80 * 2 + 20, 200, 80, "planets.png");
	graph.create(xthresh, ythresh + 80 * 3 + 30, 200, 80, "graph.png");
	exit.create(870, 850, 100, 40, "exit.png");
	return_to_menu.create(870, 850, 100, 40, "exit.png");


	sf::ContextSettings set = windowInit();
	

	

	int bounce_clk = 0, graph_clk = 0, planet_clk = 0, la_clk = 0, exit_clk = 0, return_clk = 0;

	while (win.isOpen())
	{
		sf::Event eve;
		while (win.pollEvent(eve))
		{
			bounce_clk = bounce.onButton(win);
			if (bounce_clk)
			{
				bounce_clk = 0;

				win.close();
				
				return 1;

			}
			


			planet_clk = planet.onButton(win);
			if (planet_clk)
			{
				planet_clk = 0;

				win.close();
				return 2;
				
				
				
				
			}
			

			la_clk = la.onButton(win);
			if (la_clk)
			{
				la_clk = 0;

				win.close();
				return 3;
			}

			graph_clk = graph.onButton(win);
			if (graph_clk)
			{
				graph_clk = 0;

				win.close();
				return 4;
			}
			exit_clk = exit.onButton(win);
			if (exit_clk)
			{
				win.close();
				return 0;
			}

			
			
			if (eve.type == sf::Event::Closed)
			{
				win.close();
			}


		}
		win.clear();
		win.draw(background);
		bounce.drawButton(win);
		la.drawButton(win);
		planet.drawButton(win);
		graph.drawButton(win);
		exit.drawButton(win);
		
		
		//win.draw(text);
		win.display();
	}
}

void featuremenu1(sf::RenderWindow& win, data_object& d)
{
	sf::RectangleShape background2(sf::Vector2f(750.0f, 900.0f));
	background2.setPosition(260.0f, 0.0f);
	sf::Texture menu_bg1;
	menu_bg1.loadFromFile("bounce_menu.png");
	background2.setTexture(&menu_bg1);

	while (win.isOpen())
	{
		sf::Event eve;
		while(win.pollEvent(eve))
		{
			if (eve.type == sf::Event::MouseButtonPressed)
			{
				std::cout << "Hey\n";
			}
			if (eve.type == sf::Event::Closed)
			{
				win.close();
			}

		}
		win.clear();
		win.draw(background2);
		win.display();
	}
}

void featuremenu2(sf::RenderWindow& win, data_object& d)
{
	sf::RectangleShape background2(sf::Vector2f(750.0f, 900.0f));
	background2.setPosition(260.0f, 0.0f);
	sf::Texture menu_bg2;
	menu_bg2.loadFromFile("planet_menu.png");
	background2.setTexture(&menu_bg2);

	while (win.isOpen())
	{
		sf::Event eve;
		while (win.pollEvent(eve))
		{
			if (eve.type == sf::Event::MouseButtonPressed)
			{
				std::cout << "Hey\n";
			}
			if (eve.type == sf::Event::Closed)
			{
				win.close();
			}

		}
		win.clear();
		win.draw(background2);
		win.display();
	}
}

void featuremenu3(sf::RenderWindow& win, data_object& d)
{
	sf::RectangleShape background3(sf::Vector2f(750.0f, 900.0f));
	background3.setPosition(260.0f, 0.0f);
	sf::Texture menu_bg3;
	menu_bg3.loadFromFile("la_menu.png");
	background3.setTexture(&menu_bg3);

	while (win.isOpen())
	{
		sf::Event eve;
		while (win.pollEvent(eve))
		{
			if (eve.type == sf::Event::MouseButtonPressed)
			{
				std::cout << "Hey\n";
			}
			if (eve.type == sf::Event::Closed)
			{
				win.close();
			}

		}
		win.clear();
		win.draw(background3);
		win.display();
	}
}
void GraphPlotter(sf::RenderWindow& win, data_object& d)
{
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
	GLint light = glGetUniformLocation(shaderProgram, "camera");





	glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));


	glm::mat4x4 rot_matrix(1.0f);


	glm::mat3 x(glm::vec3(1, 0, 1)
		, glm::vec3(1, 0, 1)
		, glm::vec3(1, 1, 0));


	camera2 c1(shaderProgram);
	light_source ll(0.1, shaderProgram, "sun.jpg");
	ll.position(0, 0, 50);
	
	/*line l2(glm::vec3(0, 0, 0), glm::vec3(-50, 0, 0), glm::vec3(1, 0, 0), shaderProgram);*/
	graph gr(shaderProgram, glm::vec3(1, 1, 1), "moon.jpg");


	
	bool view_flag = false;


	line* l[100];
	line* m[100];
	float j = -50;
	int p = 0;
	for (int i =0; i < 100; i ++)
	{
		l[i] = new line(glm::vec3(-50, j, 0), glm::vec3(50, j, 0), glm::vec3(.5, .5, .5), shaderProgram);
		//l[p]->draw();
		
		m[i] = new line(glm::vec3(j, -50, 0), glm::vec3(j, 50, 0), glm::vec3(.5, .5, .5), shaderProgram);
		//l[p]->draw();
		j += 1;
	}

	p = 0;

	while (running)
	{

		for(int i = 0 ; i < 100 ; i++)
		{
			l[i]->draw();
			m[i]->draw();
		}
		
		
		//l2.draw();
		ll.draw();
		gr.draw();


		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
				win.close();

				return;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				c1.rotate_camera_horizontal(1);

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				c1.rotate_camera_horizontal(-1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				c1.rotate_camera_vertical(1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				c1.rotate_camera_vertical(-1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(1, 0, 0));
					//obs.rotate(1, glm::vec3(1, 0, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(0, 1, 0));
					//obs.rotate(1, glm::vec3(0, 1, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(0, 0, 1));
					//obs.rotate(1, glm::vec3(0, 0, 1));

				}



			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(1, 0, 0));
					//obs.rotate(-1, glm::vec3(1, 0, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(0, 1, 0));
					//obs.rotate(-1, glm::vec3(0, 1, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				{
					rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(0, 0, 1));
					//obs.rotate(-1, glm::vec3(0, 0, 1));

				}



			}
			else if (winEvent.type == sf::Event::MouseWheelMoved) {
				if (winEvent.mouseWheel.delta > 0) {

					c1.move_position(-1, 10);
				}
				else if (winEvent.mouseWheel.delta < 0) {
					c1.move_position(1, 10);
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

				c1.move_position(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				c1.move_position(1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				c1.move_position_side(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				c1.move_position_side(1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
				c1.move_position_up(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				c1.move_position_up(1, 2);
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				c1.reset();
			}


			glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));



		}






		c1.update(); /// pass rot matrix

		/*Button exit;
		exit.create(870, 850, 100, 40, "exit.png");
		if (exit.onButton(win))
		{
			menuscreen(win);
		}

		exit.drawButton(win);*/

		win.display();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}


}
void obj_load(sf::RenderWindow& win, data_object& d)
{

	bool running = win.isOpen();

	glm::mat4  trans = glm::mat4(1.0f);
	glm::mat4 proj = glm::perspective<float>(glm::radians(75.0), 1280.0 / 960.0f, .25, 400);


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
	GLint light = glGetUniformLocation(shaderProgram, "camera");










	glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));






	light_source sun(2, shaderProgram, "sun.jpg"), sun2(2, shaderProgram, "sun.jpg");

	char filename[128];
	std::cout << "Enter Filename: ";
	std::cin >> filename;
	obj_file obs(shaderProgram, filename);







	sun.position(0, 0, 100);
	sun2.position(0, 100, 0);
	camera2 c1(shaderProgram);

	c1.set_position(glm::vec4(10, 10, 10, 1));

	float i = 0, j = 0, k = 0, theta = 0, fov = 75, look_at = 40;
	bool view_flag = false, state = false;
	while (running)
	{



		sun2.draw();
		obs.draw();


		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
				win.close();

				return;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				c1.rotate_camera_horizontal(1);

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				c1.rotate_camera_horizontal(-1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				c1.rotate_camera_vertical(1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				c1.rotate_camera_vertical(-1);


			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				{
					//rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(1, 0, 0));
					obs.rotate(1, glm::vec3(1, 0, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
				{
					//rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(0, 1, 0));
					obs.rotate(1, glm::vec3(0, 1, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				{
					//rot_matrix = glm::rotate(rot_matrix, glm::radians(.6f), glm::vec3(0, 0, 1));
					obs.rotate(1, glm::vec3(0, 0, 1));

				}



			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
				{
					//rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(1, 0, 0));
					obs.rotate(-1, glm::vec3(1, 0, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
				{
					//rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(0, 1, 0));
					obs.rotate(-1, glm::vec3(0, 1, 0));

				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
				{
					//rot_matrix = glm::rotate(rot_matrix, glm::radians(-.6f), glm::vec3(0, 0, 1));
					obs.rotate(-1, glm::vec3(0, 0, 1));

				}



			}
			else if (winEvent.type == sf::Event::MouseWheelMoved) {
				if (winEvent.mouseWheel.delta > 0) {

					c1.move_position(-1, 10);
				}
				else if (winEvent.mouseWheel.delta < 0) {
					c1.move_position(1, 10);
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

				c1.move_position(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				c1.move_position(1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				c1.move_position_side(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				c1.move_position_side(1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
				c1.move_position_up(-1, 2);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
				c1.move_position_up(1, 2);
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				c1.reset();
			}


			glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));



		}






		c1.update(); /// pass rot matrix

		/*Button exit;
		exit.create(870, 850, 100, 40, "exit.png");
		if (exit.onButton(win))
		{
			menuscreen(win);
		}

		exit.drawButton(win);*/

		win.display();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}
}