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

void physics(sf::RenderWindow& win) {
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

	camera c1(shaderProgram);

	time_data prev_time, curr_time;
	//PseudoPrompting for g
	float grav;
	std::cout << "Enter preferred g: ";
	std::cin >> grav;
	//For converting to minus
	grav = 0 - grav;

	
	
    
	
	
	l1.position(100, 10, 0);

	s.position(0, 0, 0);


	

	curr_time = prev_time = std::chrono::steady_clock::now();
	

	
	

	float  theta = 0, fov = 75;
	bool view_flag = 0;
	while (running)
	{

		curr_time = std::chrono::steady_clock::now();
		us_time del = curr_time - prev_time;
		prev_time = curr_time;
		
	
		
		
		
		
		
		ball.update(del, grav);
		l.draw();
		l2.draw();
	l1.draw();
	

		
		
		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
				win.close();
				
				return;
				
				
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


		

		c1.update();


		win.display();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}


void LA(sf::RenderWindow& win)
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

	camera c1(shaderProgram);



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

					c1.move_position(true);
				}
				else if (winEvent.mouseWheel.delta < 0) {
					c1.move_position(false);
				}
			}
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				c1.reset();
			}


			glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));
		}

		
		
		


		c1.update(rot_matrix);

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

void featuremenu1(sf::RenderWindow& win)
{
	sf::RectangleShape background2(sf::Vector2f(750.0f, 900.0f));
	background2.setPosition(260.0f, 0.0f);
	sf::Texture menu_bg1;
	menu_bg1.loadFromFile("bounce_menu.png");
	background2.setTexture(&menu_bg1);

	Button g, up, down, exit;

	system("cls");
	g.create(550, 300, 150, 80, "g.png");
	up.create(550, 250, 150, 40, "upswitch.png");
	down.create(550, 390, 150, 40, "downswitch.png");
	exit.create(870, 850, 100, 40, "exit.png");
	int up_clk = 0, down_clk = 0;
	float g_val = 9.8;


	while (win.isOpen())
	{
		sf::Event eve;
		while (win.pollEvent(eve))
		{
			up_clk = up.onButton(win);
			if (up_clk)
			{
				up_clk = 0;
				std::cout << "gravitational acceleration(g) increased\n";
				//rotation.changeval(&theta, 0.00005);
			}
			down_clk = down.onButton(win);
			if (down_clk)
			{
				down_clk = 0;

				std::cout << "gravitational acceleration(g) decreased\n";
				//rotation.changeval(&theta, -0.00005);
			}


			if (eve.type == sf::Event::Closed)
			{
				win.close();
			}
		}
		win.clear();
		win.draw(background2);
		g.drawButton(win);
		g.drawtext(win, g_val);
		up.drawButton(win);
		exit.drawButton(win);
		down.drawButton(win);

		win.display();
	}
	
}

void featuremenu2(sf::RenderWindow& win)
{
	sf::RectangleShape background2(sf::Vector2f(750.0f, 900.0f));
	background2.setPosition(260.0f, 0.0f);
	sf::Texture menu_bg2;
	menu_bg2.loadFromFile("planet_menu.png");
	background2.setTexture(&menu_bg2);

	Button rotation, up, down, exit;

	system("cls");
	rotation.create(550, 600, 150, 80, "rotation.png");
	up.create(550, 550, 150, 40, "upswitch.png");
	down.create(550, 690, 150, 40, "downswitch.png");
	exit.create(870, 850, 100, 40, "exit.png");
	int up_clk = 0, down_clk = 0;

	sf::Text text;
	sf::Font font;
	font.loadFromFile("Montserrat.ttf");
	text.setFont(font);
	text.setString("The solar system consists of the sun and the objects that orbit around it, including planets, moons, \nasteroids, and comets. There are eight planets in the solar system: Mercury, Venus, Earth, \nMars, Jupiter, Saturn, Uranus, and Neptune. \nPluto was once considered a planet, but it has since been reclassified as a dwarf planet.");
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::White);
	text.setPosition(50, 180);

	sf::Text text2;
	text2.setFont(font);
	text2.setString("*PRESS R TO CHANGE THE VIEWING ANGLE*");
	text2.setCharacterSize(30);
	text2.setFillColor(sf::Color::White);
	text2.setPosition(270, 360);
	text2.setStyle(sf::Text::Bold);

	


	while (win.isOpen())
	{
		sf::Event eve;
		while (win.pollEvent(eve))
		{
			up_clk = up.onButton(win);
			if (up_clk)
			{
				up_clk = 0;
				std::cout << "Rotational speed increased\n";
				//g.changeval(&g, 0.1);
			}
			down_clk = down.onButton(win);
			if (down_clk)
			{
				down_clk = 0;
				
					std::cout << "Rotational speed decreased\n";
					//g.changeval(&g, -0.1);
			}


			if (eve.type == sf::Event::Closed)
			{
				win.close();
			}

		}
		win.clear();
		win.draw(background2);
		rotation.drawButton(win);
		up.drawButton(win);
		down.drawButton(win);
		exit.drawButton(win);
		win.draw(text);
		win.draw(text2);
		
		win.display();
	}
}

void featuremenu3(sf::RenderWindow& win)
{
	sf::RectangleShape background3(sf::Vector2f(750.0f, 900.0f));
	background3.setPosition(260.0f, 0.0f);
	sf::Texture menu_bg3;
	menu_bg3.loadFromFile("la_menu.png");
	background3.setTexture(&menu_bg3);

	sf::Text text;
	sf::Font font;
	font.loadFromFile("Montserrat.ttf");
	text.setFont(font);
	text.setString("The matrix in operation is:");
	text.setCharacterSize(25);
	text.setFillColor(sf::Color::White);
	text.setPosition(50, 180);

	sf::Text text2;
	text2.setFont(font);
	text2.setString("Instructions: \nr+x to rotate clockwise around x axis \nr+y to rotate clockwise around y axis \nr+z to rotate clockwise around z axis \nl+x to rotate counterclockwise around x axis \nl+y to rotate counterclockwise around y axis \nl+z to rotate counterclockwise around z axis");
	text2.setCharacterSize(20);
	text2.setFillColor(sf::Color::White);
	text2.setPosition(80, 575);

	sf::Text matt[3][3];
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			matt[i][j].setFont(font);
			matt[i][j].setCharacterSize(20);
			matt[i][j].setFillColor(sf::Color::White);
			matt[i][j].setPosition(100 + j * 50, 250 + i * 50);
		}
	}


	//Linker object er theke asha matrix take ekhane x er moddhe dhukaish
	glm::mat3 x(glm::vec3(1, 0, 1)
		, glm::vec3(1, 0, 1)
		, glm::vec3(1, 5, 0 ));

	Button new_mat, exit;

	system("cls");
	new_mat.create(80, 450, 150, 80, "new_mat.png");
	exit.create(870, 850, 100, 40, "exit.png");

	int mat_clk = 0;

	while (win.isOpen())
	{
		sf::Event eve;
		while (win.pollEvent(eve))
		{
			mat_clk = new_mat.onButton(win);
			if (mat_clk)
			{
				mat_clk = 0;
				for (int i = 0; i < 3; i++)
				{
					std::cout << "Enter the elements of row no." << i + 1 << " of a NEW 3x3 matrix : ";
					std::cin >> x[i][0] >> x[i][1] >> x[i][2];
				}
				
			}
			if (eve.type == sf::Event::Closed)
			{
				win.close();
			}

		}
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				std::stringstream ss;
				ss << x[i][j];
				std::string str = ss.str();
				matt[i][j].setString(str);
			}
		}
		win.clear();
		win.draw(background3);
		win.draw(text);
		win.draw(text2);
		exit.drawButton(win);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				win.draw(matt[i][j]);
			}
		}
		new_mat.drawButton(win);
		win.display();
	}
}


void featuremenu4(sf::RenderWindow& win)
{
	sf::RectangleShape background2(sf::Vector2f(750.0f, 900.0f));
	background2.setPosition(260.0f, 0.0f);
	sf::Texture menu_bg1;
	menu_bg1.loadFromFile("graph_menu.png");
	background2.setTexture(&menu_bg1);

	sf::Text text;
	sf::Font font;
	font.loadFromFile("Montserrat.ttf");
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setPosition(100, 300);

	int a = 1, b = 2, c = 3;

	int graph_type = 1;

	std::string str = "The equation of the graph is: ";

	if (graph_type == 1)
	{
		std::stringstream ss;
		ss << c;
		str += ss.str();

		str += "sinz = ";
		std::stringstream ss2;
		ss2 << a;
		str += ss2.str();
		str += "sinx + ";

		std::stringstream ss3;
		ss3 << b;
		str += ss3.str();
		str += "siny";
		
	}
	else if (graph_type == 2)
	{
		std::stringstream ss;
		ss << c;
		str += ss.str();

		str += "cosz = ";
		std::stringstream ss2;
		ss2 << a;
		str += ss2.str();
		str += "cosx + ";

		std::stringstream ss3;
		ss3 << b;
		str += ss3.str();
		str += "cosy";
	

	}
	else if(graph_type == 3)
	{
		std::stringstream ss;
		ss << c;
		str += ss.str();

		str += "z^3 = ";
		std::stringstream ss2;
		ss2 << a;
		str += ss2.str();
		str += "x^3 + ";

		std::stringstream ss3;
		ss3 << b;
		str += ss3.str();
		str += "y^3";
	
	}
	else if (graph_type == 4)
	{
		str = "x^2/";
		std::stringstream ss;
		ss << a;
		str += ss.str();

		str += "^2 + y^2/";
		std::stringstream ss2;
		ss2 << b;
		str += ss2.str();
		str += "^2 + z^2/";

		std::stringstream ss3;
		ss3 << c;
		str += ss3.str();
		str += " = 1";


	}

	text.setString(str);

	sf::Text text2;
	text2.setFont(font);
	text2.setString("Instructions: \nr+x to rotate clockwise around x axis \nr+y to rotate clockwise around y axis \nr+z to rotate clockwise around z axis \nl+x to rotate counterclockwise around x axis \nl+y to rotate counterclockwise around y axis \nl+z to rotate counterclockwise around z axis");
	text2.setCharacterSize(20);
	text2.setFillColor(sf::Color::White);
	text2.setPosition(100, 575);

	Button exit;
	exit.create(870, 850, 100, 40, "exit.png");



	while (win.isOpen())
	{
		sf::Event eve;
		while (win.pollEvent(eve))
		{
			if (eve.type == sf::Event::Closed)
			{
				win.close();
			}
		}
		win.clear();
		win.draw(background2);
		win.draw(text);
		win.draw(text2);
		exit.drawButton(win);
		win.display();
	}

}



void GraphPlotter(sf::RenderWindow& win)
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


	camera c1(shaderProgram);
	light_source ll(0.1, shaderProgram, "sun.jpg");
	ll.position(0, 0, 50);
	
	/*line l2(glm::vec3(0, 0, 0), glm::vec3(-50, 0, 0), glm::vec3(1, 0, 0), shaderProgram);*/
	graph gr(shaderProgram, glm::vec3(1, 1, 1), "moon.jpg");


	float i = 0, j = 0, k = 0, theta = 0, fov = 75, look_at = 40;
	bool view_flag = false;
	while (running)
	{
		int j = -25;
		int p = 0;
		line* l[50];
		for (int i = 50; i >= 0; i -= 5)
		{
			l[p] = new line(glm::vec3(-50, j, 0), glm::vec3(50, j, 0), glm::vec3(1, 1, 1), shaderProgram);
			l[p]->draw();
			p++;
			l[p] = new line(glm::vec3(j, -50, 0), glm::vec3(j, 50, 0), glm::vec3(1, 1, 1), shaderProgram);
			l[p]->draw();
			j += 5;
		}
		//l2.draw();
		ll.draw();
		gr.draw();


		sf::Event winEvent;
		while (win.pollEvent(winEvent)) {
			if (winEvent.type == sf::Event::Closed) {
				running = false;
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

					c1.move_position(true);
				}
				else if (winEvent.mouseWheel.delta < 0) {
					c1.move_position(false);
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				c1.reset();
			}



			glUniformMatrix4fv(projectionU, 1, GL_FALSE, glm::value_ptr(proj));
		}






		c1.update(rot_matrix);

		win.display();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}



}
