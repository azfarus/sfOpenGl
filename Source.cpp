#include "StandardFunctions.h"
#include"buffer_objects.h"
#include"Shapes.h"
#include<thread>

using namespace std;
bool running;

void func(int x) {
	while (x < 10)
		x++;
		std::cout << x;
		std::cout << "This from thread\n";
}



int main() {

	sf::ContextSettings set = windowInit();
	sf::RenderWindow win(sf::VideoMode(1280, 960), "Xplore", sf::Style::Close);
	int x = menuscreen(win);
	if(x == 1)
	{

		sf::RenderWindow mwin(sf::VideoMode(1280, 960), "Xplore", sf::Style::Close);
		mwin.setActive(false);
		std::thread thread1(&featuremenu1, std::ref(mwin));
		
		

		sf::RenderWindow fwin(sf::VideoMode(1280, 960), "Features", sf::Style::Close, set);


		glewExperimental = GL_TRUE;
		glewInit();
		physics(fwin);

		

		
	}
	else if(x == 2)
	{

		sf::RenderWindow mwin(sf::VideoMode(1280, 960), "Xplore", sf::Style::Close);
		mwin.setActive(false);
		std::thread thread2(&featuremenu2, std::ref(mwin));


		sf::RenderWindow fwin(sf::VideoMode(1280, 960), "Features", sf::Style::Close, set);


		glewExperimental = GL_TRUE;
		glewInit();
		keplar(fwin);
	}
	else if(x == 3)
	{

		sf::RenderWindow mwin(sf::VideoMode(1280, 960), "Xplore", sf::Style::Close);
		mwin.setActive(false);
		std::thread thread2(&featuremenu2, std::ref(mwin));

		sf::RenderWindow fwin(sf::VideoMode(1280, 960), "Features", sf::Style::Close, set);


		glewExperimental = GL_TRUE;
		glewInit();
		LA(fwin);
	}
	else if (x == 4)
	{
		//for graph
	}
		

	
	//physics(win);
	//keplar(win);
	//LA(win);
	

}
	