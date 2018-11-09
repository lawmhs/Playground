#include <SFML/Graphics.hpp>
#include"Gamestate"
#include"Gameobjs"
// to run:
// g++ -c main.cpp
// g++ main.o -o sfml-app -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
// ./sfml-app

// This is where our game starts
int main()
{
    // Make a window that is 1920 by 1080 pixels in size
    sf::RenderWindow window(sf::VideoMode(800, 600), "Application");
 
    while (window.isOpen())
    {// Game loop starts here
 
        //Handle input from the keyboard, mouse and controller
		
        // Process movement, AI and other logic here
        
        sf::Event event;
		while (window.pollEvent(event))
			{
				// Request for closing the window
				if (event.type == sf::Event::Closed) {
					window.close();
					}
				// The escape key was pressed
				if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
					window.close();
					}
			}
        
        // Rub out the last frame
        window.clear();
 
        // Draw the current scene here
 
 
 
        // Show the current scene
        window.display();
 
    }// Do all of the above over and over again
 
// User has quit- go back to the operating system
    return 0;
}
