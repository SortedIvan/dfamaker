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
void RotateRectangle(sf::ConvexShape& rect, float angle);

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
	
	sf::CircleShape triangle(40, 3);
	triangle.setFillColor(sf::Color::Yellow);
	triangle.setPosition(100, 100);


	sf::RectangleShape rectangle(sf::Vector2f(100, 5));
	rectangle.setPosition(300, 300);
	rectangle.setFillColor(sf::Color::White);


	//rectangle.getTransform().transformPoint()

	int selectedState = -1;
	int transitionCounter = 0;
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

					int tempSelected = dfa.GetSelectedStateIndex(mousePos);
					if (tempSelected != -2 && stateIsSelected && tempSelected != selectedState && selectedState != -1) { // if a different state was selected

						sf::Vector2f directionVector = dfa.GetStates()[selectedState].GetStatePosition() - dfa.GetStates()[tempSelected].GetStatePosition();
						float vectorLength = std::sqrt(directionVector.x * directionVector.x + directionVector.y * directionVector.y);
						directionVector.x = directionVector.x / vectorLength;
						directionVector.y = directionVector.y / vectorLength;

						std::cout << directionVector.x << std::endl;
						std::cout << directionVector.y;

						sf::Vector2f center = dfa.GetStates()[selectedState].GetStateCircle().getPosition()
							+ sf::Vector2f(dfa.GetStates()[selectedState].GetStateCircle().getRadius(),
							dfa.GetStates()[selectedState].GetStateCircle().getRadius());

						sf::Vector2f outgoingPoint = directionVector * dfa.GetStates()[selectedState].GetStateCircle().getRadius() + center;
						triangle.setPosition(outgoingPoint);

					}
					else if (tempSelected != -2 && stateIsSelected && tempSelected == selectedState) { // self-loop

					}
					else { // First time selecting state
						selectedState = tempSelected;
						stateIsSelected = true;
					}
				}
				else {
					dfa.AddNewState("", (sf::Vector2f)GetMousePosition(window), font);
					stateIsSelected = false;
					selectedState = -1;
				}
			}

		}

		// < ------- DO DRAWING LOGIC HERE ------------->
		// --------- clear the screen ----------
		window.clear(default_bg_color);

		// --------- draw on the screen ---------	
		dfa.DrawAllStates(window);
		window.draw(triangle);
		window.draw(rectangle);

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

void AddStateTransitionBetweenStates() {

}

void RotateRectangle(sf::ConvexShape& rect, float angle) {

	// Works by rotating the corners of the given shape by working
	// around the center of the shape. Thus it rotates itself around its center
	for (int i = 0; i < rect.getPointCount(); i++) {
		float pointX = rect.getPoint(i).x;
		float pointY = rect.getPoint(i).y;

		// Get the origin by substracting corners with center
		float xToOrigin = pointX - rect.getLocalBounds().height / 2;
		float yToOrigin = pointX - rect.getLocalBounds().width / 2;

		// apply rotation
		// now apply rotation
		float rotatedX = xToOrigin * std::cos(angle) - yToOrigin * std::sin(angle);
		float rotatedY = xToOrigin * std::sin(angle) + yToOrigin * std::cos(angle);

		rect.setPoint(i, sf::Vector2f(rotatedX, rotatedY));
	}




}