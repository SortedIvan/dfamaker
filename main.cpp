#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "dfa.hpp"
#include "line.hpp"

# define M_PI  3.14159265358979323846  /* pi */

sf::Vector2i GetMousePosition(sf::RenderWindow& window);
void TryLoadFont(sf::Font& font, std::string path);
void HandleStateLabelInput(sf::Event& e, DFA& dfa, int selectedState);
static const sf::Color default_bg_color(0x00, 0x01, 0x33);
void RotateRectangle(sf::ConvexShape& rect, float angle);
float dot_product(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
sf::VertexArray Test(sf::RenderWindow& window, sf::Vector2f from, sf::Vector2f to);
// Method for spawning arrows
// 1) Click on the shape
// 2) Hold ctrl
// 3) Click on other shapes (including possibly itself)

// class DFA -> holds DFA_STATES, Alphabet, etc
// class DFA_STATE -> holds everything related to the states

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 800), "Test");
	sf::Event e;

	int transitionIdCounter = 0;

	DFA dfa;
	// Define DFA logic here (to be moved onto somewhere else than main.cpp

	sf::Font font;
	TryLoadFont(font, "./testfont.ttf");
	
	sf::CircleShape triangle(40, 3);
	triangle.setFillColor(sf::Color::Yellow);
	triangle.setPosition(100, 100);


	sf::RectangleShape rectangle(sf::Vector2f(5, 5));
	rectangle.setPosition(300, 300);
	rectangle.setFillColor(sf::Color::Black);

	sfLine line(sf::Vector2f(100, 100), sf::Vector2f(200, 200));

	sfLine arrowLineOne(sf::Vector2f(100, 100), sf::Vector2f(200, 200));
	sfLine arrowLineTwo(sf::Vector2f(100, 100), sf::Vector2f(200, 200));

	int selectedState = -1;
	int transitionCounter = 0;
	bool stateIsSelected = false;
	bool shiftHeldDown = false;

	// ---- test -----

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

				if (dfa.SelectStateTransition(mousePos) != -2) { // We have clicked on a transition
					continue;
				}

				if (dfa.CheckIfStateSelected(mousePos)) {
					int tempSelected = dfa.GetSelectedStateIndex(mousePos);

					if (stateIsSelected && tempSelected != selectedState && selectedState != -1) { // if a different state was selected

						if (shiftHeldDown) { // Add a new transition

							dfa.AddNewTransition(selectedState, tempSelected, transitionIdCounter);
							transitionIdCounter++;
							

							selectedState = tempSelected;
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

		//std::cout << selectedState << std::endl;

		if (selectedState >= 0) {
			dfa.SetSelectedState(selectedState);
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



//TO DO: Switch from curr way of collision checking to this
sf::VertexArray Test(sf::RenderWindow& window, sf::Vector2f from, sf::Vector2f to) {
	
	float width = 6.f;

	sf::Vector2f dirVector = to - from;

	sf::Transform transform;

	float distance = std::sqrt(std::pow((to.x - from.x), 2) + std::pow((to.y - from.y), 2));

	//acos(a dot b / len(a) . len(b))

	float adj = to.x - from.x;
	float opp = to.y - from.y;
	float hypp = std::sqrt(opp * opp + adj * adj);
	//float angle = std::acos(adj / hypp) * (180 / M_PI);

	float angle = std::atan2(opp, adj) * (180 / M_PI);
	std::cout << angle << std::endl;

	sf::Vector2f point1(from.x + width, from.y);
	sf::Vector2f point2(from.x - width, from.y);
	sf::Vector2f point3(from.x - width, from.y + distance);
	sf::Vector2f point4(from.x + width, from.y + distance);

	transform.rotate(angle - 90, from);

	point1 = transform.transformPoint(point1);
	point2 = transform.transformPoint(point2);
	point3 = transform.transformPoint(point3);
	point4 = transform.transformPoint(point4);

	sf::VertexArray rect(sf::Quads, 4);
	rect[0].position = point1;
	rect[1].position = point2;
	rect[2].position = point3;
	rect[3].position = point4;

	rect[0].texCoords = point1;
	rect[1].texCoords = point2;
	rect[2].texCoords = point3;
	rect[3].texCoords = point4;

	rect[0].color = sf::Color::Red;
	rect[1].color = sf::Color::Yellow;
	rect[2].color = sf::Color::Yellow;
	rect[3].color = sf::Color::Red;

	std::cout << "bomboclaat";
	return rect;


}
