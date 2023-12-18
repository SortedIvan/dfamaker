#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "dfa.hpp"

sf::Vector2i GetMousePosition(sf::RenderWindow& window);
void DrawStateOnScreen(sf::RenderWindow& window);

static const sf::Color default_bg_color(0x00, 0x01, 0x33);

// Method for spawning arrows
// 1) Click on the shape
// 2) Hold ctrl
// 3) Click on other shapes (including possibly itself)

// class DFA -> holds DFA_STATES, Alphabet, etc
// class DFA_STATE -> holds everything related to the states


int main() {

	sf::RenderWindow window(sf::VideoMode(800, 800), "Test");
	sf::Event e;

	DFA dfa;
	// Define DFA logic here (to be moved onto somewhere else than main.cpp

		
	sf::CircleShape shape(50.f);
	shape.setPosition(100, 100);

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}

			// On Mouse Click Release
			if (e.type == sf::Event::MouseButtonReleased) {
				dfa.AddNewState("test", (sf::Vector2f)GetMousePosition(window));
			}

		}

		// < ------- DO DRAWING LOGIC HERE ------------->
		// --------- clear the screen ----------
		window.clear(default_bg_color);

		// --------- draw on the screen ---------	
		dfa.DrawAllStates(window);


		// --------- display on the screen --------
		window.display();

	}
}

void TryLoadFont(sf::Font& font, std::string path)
{
	if (!font.loadFromFile(path))
	{
		std::cout << "Error loading the font file" << std::endl;
		system("pause");
	}
}

sf::Vector2i GetMousePosition(sf::RenderWindow& window) {
	return sf::Mouse::getPosition(window);
}

void DrawStateOnScreen(sf::RenderWindow& window) {
	sf::CircleShape circle;
	circle.setRadius(5);
	circle.setFillColor(sf::Color::White);
	circle.setPosition((sf::Vector2f)GetMousePosition(window));
	window.draw(circle);
}


