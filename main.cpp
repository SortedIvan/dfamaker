#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "dfa.hpp"

# define M_PI  3.14159265358979323846  /* pi */

sf::Vector2i GetMousePosition(sf::RenderWindow& window);
void TryLoadFont(sf::Font& font, std::string path);
void HandleStateLabelInput(sf::Event& e, DFA& dfa, int selectedState);
static const sf::Color default_bg_color(0x00, 0x01, 0x33);
void RotateRectangle(sf::ConvexShape& rect, float angle);
float dot_product(const sf::Vector2f& lhs, const sf::Vector2f& rhs);

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
	bool shiftHeldDown = false;

	while (window.isOpen()) {


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		{
			shiftHeldDown = true;
		}
		else {
			shiftHeldDown = false;
		}

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

					if (stateIsSelected && tempSelected != selectedState && selectedState != -1) { // if a different state was selected

						if (shiftHeldDown) {

							std::vector<DfaState> states = dfa.GetStates();

							sf::Vector2f stateFrom = states[selectedState].GetStatePosition();
							sf::Vector2f stateTo = states[tempSelected].GetStatePosition();
							sf::Vector2f dirVector = stateTo - stateFrom;

							float vectorLength = std::roundf(std::sqrt(dirVector.x * dirVector.x + dirVector.y * dirVector.y));

							sf::Vector2f dirVectorNormalized =
								sf::Vector2f(
									dirVector.x / vectorLength,
									dirVector.y / vectorLength
								);
							// 1) Calculate the rotation between the states
							float rotationRadians = std::atan2(dirVectorNormalized.y, dirVectorNormalized.x);
							float rotationDegrees = rotationRadians * (180 / M_PI); // Normalize to degrees
							std::cout << rotationDegrees << std::endl;
							rectangle.setRotation(rotationDegrees);
							// 2) Calculate the distance between the states

							float distance = std::sqrt(std::pow((stateTo.x - stateFrom.x), 2)
								+ std::pow((stateTo.y - stateFrom.y), 2));

							std::cout << distance;
						}
						else {
							selectedState = tempSelected;
						}
					}
					else if (tempSelected != -2 && stateIsSelected && tempSelected == selectedState) { // self-loop
						if (shiftHeldDown) {

						}
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

		//<----------- End of event logic, anything that takes place before drawing ------------>

		std::cout << selectedState << std::endl;

		if (selectedState >= 0) {
			dfa.SetSelectedState(selectedState);
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

float dot_product(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
{
	return lhs.x * rhs.x + lhs.y + rhs.y;
}