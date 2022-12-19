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

#define xthresh 530
#define ythresh 200


class Button {
public:
	int buttpos_x, buttpos_y;
	int buttwidth, buttheight;
	sf::RectangleShape butt;
	sf::Texture n, f, p, jama;
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
			////std::cout << "Button hovering\n";

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{

				//butt.setTexture(&p);
				//strcpy(userInput, set);
				window.draw(butt);
				window.display();
				//click.play();
				Sleep(200);
				//window.close();



				return 1;
			}
			else
			{
				return 0;
			}
		}
		/*else
		{
			butt.setTexture(&n);
			return 0;
		}*/


	}

	/*void setTexture(std::string filename)
	{

		if (!jama.loadFromFile(filename))
		{
			std::cout << "Failed to load texture\n";
		}
		butt.setTexture(&jama);
		return;

	}*/
	void drawButton(sf::RenderWindow& window)
	{
		window.draw(butt);
	}
};

class data_object
{
	int x;
	std::mutex m;
public:
	data_object()
	{
		x = 0;

	}
	int get_x()
	{
		return x;
	}
	int set_x(int val)
	{
		m.lock();
		x = val;
		m.unlock();
	}
};

void pushVectors(std::vector<float>& v, glm::vec3 a, glm::vec3 b, glm::vec3 c , glm::vec2 d);
void pushvals(std::vector<float>& v, float init_x, float fin_x, float init_y, float fin_y, float sharpness);
GLuint shaderSet();
sf::ContextSettings windowInit();
GLuint loadTexture(std::string filepath , int flip = 0);
void keplar(sf::RenderWindow& win);
void LA(sf::RenderWindow& win);
void print(glm::vec3 x);
void physics(sf::RenderWindow& win , data_object& d);
int menuscreen(sf::RenderWindow& win );
void featuremenu1(sf::RenderWindow& win , data_object &d);
void featuremenu2(sf::RenderWindow& win);
void featuremenu3(sf::RenderWindow& win);

