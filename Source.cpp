#include "StandardFunctions.h"
#include"buffer_objects.h"
#include"Shapes.h"

using namespace std;
bool running;



int main() {



	


	
	sf::ContextSettings set = windowInit();
	sf::RenderWindow win(sf::VideoMode(1280, 960), "LOL",sf::Style::Close, set);


	glewExperimental = GL_TRUE;
	glewInit();


	//keplar(win);
	LA(win);
	

}
	