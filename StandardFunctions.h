#pragma once
#define GLEW_STATIC
#define us_time std::chrono::duration<double , std::ratio<1 , 1000000>>
#define time_data std::chrono::time_point<std::chrono::steady_clock >

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<SFML/OpenGL.hpp>
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<iostream>
#include "stb_image.h"
#include<chrono>
#include<Windows.h>
#include<vector>
#include<mutex>
#include<sstream>

#define xthresh 530
#define ythresh 200


class Button {
public:
	int buttpos_x, buttpos_y;
	int buttwidth, buttheight;
	sf::RectangleShape butt;
	sf::Texture n, f, p, jama;
	int val;
	/*sf::SoundBuffer buf;
	sf::Sound click;*/


	void create(int x, int y, int w, int h, std::string Nn)
	{
		butt.setSize(sf::Vector2f(w, h));

		//butt.setOrigin(sf::Vector2f(w / 2.0, h / 2.0));
		butt.setPosition(sf::Vector2f(x, y));
		buttpos_x = x; buttpos_y = y;
		buttwidth = w; buttheight = h;
		n.loadFromFile(Nn);
		/*f.loadFromFile(Ff);
		p.loadFromFile(Pp);
		buf.loadFromFile("mouse2.wav");
		click.setBuffer(buf);*/
		butt.setTexture(&n);

	}
	int onButton(sf::RenderWindow& window)
	{
		sf::Vector2i mousepos(sf::Mouse::getPosition(window));
		int mouse_x = mousepos.x;
		int mouse_y = mousepos.y;

		if ((buttpos_x < mouse_x && buttpos_x + buttwidth > mouse_x) && (buttpos_y < mouse_y && buttpos_y + buttheight > mouse_y))
		{
			//butt.setTexture(&f);
			//

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{

				//butt.setTexture(&p);
				//strcpy(userInput, set);
				window.draw(butt);
				window.display();
				//click.play();
				Sleep(200);
				//window.close();
				std::cout << "Button clicked\n";


				return 1;
			}
			
				
			
		}
		/*else
		{
			butt.setTexture(&n);
			return 0;
		}*/
		return 0;


	}
	void changeval(float& theta, float inc)
	{
		theta += inc;
	}

	void drawtext(sf::RenderWindow& window, float& g)
	{
		sf::Text text;
		sf::Font font;
		font.loadFromFile("Montserrat.ttf");
		text.setFont(font);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::Black);
		text.setPosition(buttpos_x + buttwidth / 2 + 10, buttpos_y + buttheight / 2 - 16);
		text.setStyle(sf::Text::Bold);


		std::stringstream ss;
		ss << g;
		std::string str = ss.str();

		// Set the text string to the variable value
		text.setString(str);

		// Draw the text
		window.draw(text);



	}




	void drawButton(sf::RenderWindow& window)
	{
		window.draw(butt);
	}
};

class data_object
{
	//common
	std::mutex m;
	
	
	// bouncing balls
	float g;

	//LA
	glm::mat3 la_matrix;

	// keplar
	float theta;

	// graph
	std::string math_expr_data;

	
public:
	bool service;
	bool exit;
	data_object()
	{
		theta = 5e-6;
		g = 9.8;
		service = false;
		exit = false;
		la_matrix = glm::mat3(1.0f);
	}

	void set_service(bool x)
	{
		m.lock();
			service = x;
		m.unlock();
	}

	void set_math_expr(std::string val)
	{
		m.lock();
		math_expr_data = val;
		m.unlock();
	}

	std::string get_math_expr()
	{
		return math_expr_data;
	}

	void set_matrix(glm::mat3 x)
	{
		m.lock();
		la_matrix = x;
		m.unlock();
	}

	glm::mat3 get_matrix()
	{
		return la_matrix;
	}

	float get_g()
	{
		m.lock();
		float x = g;
		m.unlock();
		return x;
	}

	void set_g(float val)
	{
		m.lock();
		g = val;
		m.unlock();
	}


	void set_exit(bool val)
	{
		m.lock();
		exit = val;
		m.unlock();
	}

	float get_theta()
	{
		return theta;
	}
	void set_theta(float val)
	{
		m.lock();
		theta = val;
		m.unlock();
		
	}
};

void pushVectors(std::vector<float>& v, glm::vec3 a, glm::vec3 b, glm::vec3 c , glm::vec2 d);
void pushvals(std::vector<float>& v, float init_x, float fin_x, float init_y, float fin_y, float sharpness);
GLuint shaderSet();
sf::ContextSettings windowInit();
GLuint loadTexture(std::string filepath , int flip = 0);
void keplar(sf::RenderWindow& win, data_object& d);
void LA(sf::RenderWindow& win, data_object& d);
void print(glm::vec3 x);
void physics(sf::RenderWindow& win, data_object& d);
int menuscreen(sf::RenderWindow& win);
void featuremenu1( data_object& d);
void featuremenu2( data_object& d);
void featuremenu3( data_object& d);
void featuremenu4( data_object& d);
void featuremenu5( data_object& d);
void GraphPlotter(sf::RenderWindow& win, data_object& d);
void obj_load(sf::RenderWindow& win, data_object& d);