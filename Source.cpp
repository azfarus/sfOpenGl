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
	menu_start:
	sf::RenderWindow win(sf::VideoMode(1280, 960), "win");
	
	int x = menuscreen(win);
	//data_object d;
	if(x == 1)
	{
		data_object d;
		//sf::RenderWindow mwin(sf::VideoMode(1280, 960), "Mwin", sf::Style::Close);
		//mwin.setActive(false);
		
		std::thread thread1(&featuremenu1, std::ref(d));;
		
		
		sf::RenderWindow fwin(sf::VideoMode(1280, 960), "Features", sf::Style::Close, set);

		
		glewExperimental = GL_TRUE;
		glewInit();
		physics(fwin, d);

		thread1.join();
		
		if(thread1.joinable())
		{
			thread1.join();
			
			cout << "gherq\n";
			  
			
		}
		thread1.~thread();
		goto menu_start;
		
	}
	else if(x == 2)
	{
		data_object d;
		/*sf::RenderWindow mwin(sf::VideoMode(1280, 960), "Xplore", sf::Style::Close);
		mwin.setActive(false);*/
		std::thread thread2(&featuremenu2,  std::ref(d));


		sf::RenderWindow fwin(sf::VideoMode(1280, 960), "Features", sf::Style::Close, set);


		glewExperimental = GL_TRUE;
		glewInit();
		keplar(fwin, d);

		thread2.join();
		goto menu_start;
	}
	else if(x == 3)
	{
		data_object d;
		//sf::RenderWindow mwin(sf::VideoMode(1280, 960), "Xplore", sf::Style::Close);
		//mwin.setActive(false);
		std::thread thread2(&featuremenu3,  std::ref(d));

		sf::RenderWindow fwin(sf::VideoMode(1280, 960), "Features", sf::Style::Close, set);


		glewExperimental = GL_TRUE;
		glewInit();
		LA(fwin, d);
		thread2.join();
		goto menu_start;
	}
	else if (x == 4)
	{
		data_object d;
		//for graph
		
		/*sf::RenderWindow mwin(sf::VideoMode(1280, 960), "Xplore", sf::Style::Close);
		mwin.setActive(false);*/
		std::thread thread4(&featuremenu4,  std::ref(d));

		sf::RenderWindow fwin(sf::VideoMode(1280, 960), "Features", sf::Style::Close, set);


		glewExperimental = GL_TRUE;
		glewInit();
		GraphPlotter(fwin, d);

		thread4.join();
		goto menu_start;
	}
	else if (x == 5)
	{
		data_object d;
		//for graph

		/*sf::RenderWindow mwin(sf::VideoMode(1280, 960), "Xplore", sf::Style::Close);
		mwin.setActive(false);*/
		std::thread thread4(&featuremenu5, std::ref(d));

		sf::RenderWindow fwin(sf::VideoMode(1280, 960), "Features", sf::Style::Close, set);


		glewExperimental = GL_TRUE;
		glewInit();
		obj_load(fwin, d);

		thread4.join();
		goto menu_start;
	}

	

	
	//physics(win);
	//keplar(win);
	//LA(win);
	

}
	