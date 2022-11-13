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

void pushVectors(std::vector<float>& v, glm::vec3 a, glm::vec3 b, glm::vec3 c , glm::vec2 d);
void pushvals(std::vector<float>& v, float init_x, float fin_x, float init_y, float fin_y, float sharpness);
GLuint shaderSet();
sf::ContextSettings windowInit();
GLuint loadTexture(std::string filepath , int flip = 0);
void keplar(sf::RenderWindow& win);
void LA(sf::RenderWindow& win);
void print(glm::vec3 x);
void physics(sf::RenderWindow& win);
