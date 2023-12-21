#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "dfa.hpp"

sf::Vector2i GetMousePosition(sf::RenderWindow& window);
void DrawStateOnScreen(sf::RenderWindow& window);
void TryLoadFont(sf::Font& font, std::string path);
void HandleStateLabelInput(sf::Event& e, DFA& dfa, int selectedState);
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

	sf::Font font;
	TryLoadFont(font, "./testfont.ttf");

	int selectedState = 0;
	bool stateIsSelected = false;


	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}

			if (e.type == sf::Event::TextEntered)
			{
				if (stateIsSelected) {
					HandleStateLabelInput(e, dfa, selectedState);
				}
			}

			// On Mouse Click Release
			if (e.type == sf::Event::MouseButtonReleased) {
				sf::Vector2f mousePos = (sf::Vector2f)GetMousePosition(window);

				if (dfa.CheckIfStateSelected(mousePos)) {
					selectedState = dfa.GetSelectedStateIndex(mousePos);
					stateIsSelected = true;
				}
				else {
					dfa.AddNewState("test", (sf::Vector2f)GetMousePosition(window), font);
					stateIsSelected = false;
				}
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


void HandleStateLabelInput(sf::Event& e, DFA& dfa, int selectedState) {
	if (e.text.unicode != '\b' && e.text.unicode != '\r' &&
		e.key.code != sf::Keyboard::Left && e.key.code != sf::Keyboard::Right && e.text.unicode != 36
		&& e.key.code != sf::Keyboard::Escape)
	{
		dfa.ChangeStateLabelText(e, selectedState);
	}
	else if (e.text.unicode == '\b') {
		dfa.DeleteStateLabelCharacter(selectedState);
	}
}
