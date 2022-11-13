#include "StandardFunctions.h"
#include"buffer_objects.h"
#include"Shapes.h"
#include<thread>

using namespace std;
bool running;

void func(int x) {
	while(true) std::cout << x;
}



int main() {



	


	
	sf::ContextSettings set = windowInit();
	sf::RenderWindow win(sf::VideoMode(1280, 960), "LOL",sf::Style::Close, set);


	glewExperimental = GL_TRUE;
	glewInit();

	physics(win);
	//keplar(win);
	//LA(win);
	

}
	