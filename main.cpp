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
void HandleTransitionSymbolInput(sf::Event& e, DFA& dfa);
bool DeleteTransition(sf::Event& e, DFA& dfa);
void HandleInputStringTextEditing(std::string& inputString, sf::Event& e, sf::Text& inputStringHolder);
void DrawAllTextBoxEntriesAndHighlights(std::vector<sf::Text>& textBoxEntries,std::vector<sf::RectangleShape>& hightlights,
	sf::RenderWindow& window);
void HandleInputStringValidation(std::vector<sf::Text>& textBoxEntries, sf::Text& inputStringHolder,
	std::string& inputString, int& currentTextBoxEntry, bool& stringAcceptedState,
	DFA& dfa, bool& stateIsSelected, bool& transitionIsSelected,sf::Font& font, sf::RenderWindow& window, std::vector<sf::RectangleShape>& highlights);


int main() {

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Test");
	sf::Event e;

	int transitionIdCounter = 0;

	DFA dfa;
	// Define DFA logic here (to be moved onto somewhere else than main.cpp

	sf::Font font;
	TryLoadFont(font, "./testfont.ttf");
	

	//<------------Begin-textbox-logic------------------------------->
	sf::RectangleShape textBox(sf::Vector2f(300,window.getSize().y - 50.f));
	sf::RectangleShape textBoxSecondary(sf::Vector2f(300, 50.f));
	sf::Text textBoxDescr;
	sf::Text inputStringHolder; // used to show visually the input strings
	std::vector<sf::Text> textBoxEntries; // shows the accepted/rejected strings
	std::vector<sf::RectangleShape> textBoxHighlights; // the highlight over each string, should be parallel to above vector
	int currentTextEntry = 0;

	float textBoxXOffset = 50.f;
	float textBoxYOffset = 35.f;
	float stringEntriesBound = textBoxYOffset + 30.f;


	textBoxDescr.setString("String history:");
	textBoxDescr.setCharacterSize(20.f);
	textBoxDescr.setPosition(window.getSize().x - textBoxXOffset - textBox.getSize().x + 10.f
		, textBoxYOffset + 10.f);
	textBoxDescr.setFont(font);

	inputStringHolder.setCharacterSize(20.f);
	inputStringHolder.setPosition(window.getSize().x - textBoxXOffset - textBox.getSize().x + 15.f
		, window.getSize().y - 55.f);
	inputStringHolder.setFont(font);

	textBox.setPosition(window.getSize().x - textBoxXOffset - textBox.getSize().x
		, textBoxYOffset);
	textBox.setFillColor(sf::Color::Transparent);
	textBox.setOutlineThickness(10.f);
	textBox.setOutlineColor(sf::Color::White);

	textBoxSecondary.setPosition(window.getSize().x - textBoxXOffset - textBox.getSize().x
		,window.getSize().y - 65.f);
	textBoxSecondary.setFillColor(sf::Color::Transparent);
	textBoxSecondary.setOutlineThickness(5.f);
	textBoxSecondary.setOutlineColor(sf::Color::White);

	// <------------End-textbox-graphics------------------------------>


	sfLine line(sf::Vector2f(100, 100), sf::Vector2f(200, 200));

	sfLine arrowLineOne(sf::Vector2f(100, 100), sf::Vector2f(200, 200));
	sfLine arrowLineTwo(sf::Vector2f(100, 100), sf::Vector2f(200, 200));

	int selectedState = -1;
	int transitionCounter = 0;
	bool stateIsSelected = false;
	bool shiftHeldDown = false;
	bool transitionIsSelected = false;
	bool stringAcceptedState = false;
	bool textboxState = false;

	std::string inputString;

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

			if (e.type == sf::Event::KeyReleased) {
				if (e.key.code == sf::Keyboard::Delete) {
					if (transitionIsSelected) {
						// First, check if user is trying to delete the transition
						if (DeleteTransition(e, dfa)) { 
							transitionIsSelected = false;
							dfa.DeSelectTransition();
							continue;
						}
					}

					if (stateIsSelected) { // Check to see if user wants to delete a state
						dfa.DeleteState(selectedState);
						stateIsSelected = false;
						continue;
					}

				}

				if (e.key.code == sf::Keyboard::Tab) {
					if (stateIsSelected) {
						dfa.ChangeStateAccepting(selectedState);
					}
				}

				if (e.key.code == sf::Keyboard::Enter) {
					HandleInputStringValidation(textBoxEntries, inputStringHolder, inputString,
						currentTextEntry, stringAcceptedState, dfa, stateIsSelected,
						transitionIsSelected, font, window, textBoxHighlights);
				}

			}

			if (e.type == sf::Event::TextEntered)
			{
				if (textboxState) {
					HandleInputStringTextEditing(inputString,e, inputStringHolder);
					continue;
				}


				// First, check for prevalent conditions
				if (e.text.unicode == '\b') {
					if (transitionIsSelected) {

						dfa.RemoveSymbolFromTransition();
						transitionIsSelected = false;

						continue;
					}
				}
				if (stateIsSelected) {
					HandleStateLabelInput(e, dfa, selectedState);
					continue;
				}
				if (transitionIsSelected) {
					HandleTransitionSymbolInput(e, dfa);
					// De-select transition after
					dfa.DeSelectTransition();
					continue;
				}

			}

			// On Mouse Click Release
			if (e.type == sf::Event::MouseButtonReleased) {
				sf::Vector2f mousePos = (sf::Vector2f)GetMousePosition(window);

				if (stringAcceptedState) { // first clear the accepted state
					dfa.SetAllStatesDefaultColor();
					stringAcceptedState = false;
				}

				// Check whether the user clicked in the text box

				if (textBox.getGlobalBounds().contains(mousePos)) {
					transitionIsSelected = true;
					stateIsSelected = false;
					selectedState = -1;
					dfa.DeSelectState();
					dfa.DeSelectTransition();

					textboxState = true;

					continue;
				}


				if (dfa.SelectStateTransition(mousePos) != -2) { // We have clicked on a transition
					transitionIsSelected = true;
					stateIsSelected = false;
					textboxState = false;
					selectedState = -1;
					dfa.DeSelectState();

					continue;
				}
				else {
					transitionIsSelected = false;
					textboxState = false;
				}

				if (dfa.CheckIfStateSelected(mousePos)) {
					int tempSelected = dfa.GetSelectedStateIndex(mousePos);
					// && tempSelected != selectedState - put this in the below if statement for diff state logic
					if (stateIsSelected && selectedState != -1) { // if a different state was selected

						if (shiftHeldDown) { // Add a new transition

							bool result = dfa.AddNewTransition(
								selectedState, tempSelected,
								transitionIdCounter, font);

							if (result) {
								transitionIdCounter++;
							}
							selectedState = tempSelected;

						}
						else {
							selectedState = tempSelected;

						}

						if (selectedState >= 0) {
							dfa.SetSelectedState(selectedState);
						}

						textboxState = false; // Exit textbox
					}
					else if (tempSelected != -2 && stateIsSelected && tempSelected == selectedState) { // self-loop
						if (shiftHeldDown) {

						}
					}
					else { // First time selecting state
						selectedState = tempSelected;
						stateIsSelected = true;
						textboxState = false; // Exit textbox
						dfa.DeSelectTransition();

						if (selectedState >= 0) {
							dfa.SetSelectedState(selectedState);
						}

					}
				}
				else {
					dfa.AddNewState("", (sf::Vector2f)GetMousePosition(window), font);
					stateIsSelected = false;
					selectedState = -1;
					dfa.DeSelectTransition();
					textboxState = false; // Exit textbox
				}
			}

		}

		//<----------- End of event logic, anything that takes place before drawing ------------>

		//std::cout << selectedState << std::endl;

		// < ------- DO DRAWING LOGIC HERE ------------->
		// --------- clear the screen ----------
		window.clear(default_bg_color);


		// --------- draw on the screen ---------	
		dfa.DrawAllStates(window);
		window.draw(textBox);
		window.draw(textBoxSecondary);
		window.draw(textBoxDescr);
		window.draw(inputStringHolder);
		DrawAllTextBoxEntriesAndHighlights(textBoxEntries, textBoxHighlights, window);


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

void HandleTransitionSymbolInput(sf::Event& e, DFA& dfa) {
	if (e.text.unicode != '\b' && e.text.unicode != '\r' &&
		e.key.code != sf::Keyboard::Left && e.key.code != sf::Keyboard::Right && e.text.unicode != 36
		&& e.key.code != sf::Keyboard::Escape && e.key.code != '~' && e.key.code != sf::Keyboard::Tab)
	{
		dfa.SetTransitionSymbol(e.key.code);
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

	return rect;


}

bool DeleteTransition(sf::Event& e, DFA& dfa) {
	dfa.DeleteTransition();
	return true;
}

void HandleInputStringTextEditing(std::string& inputString, sf::Event& e, sf::Text& inputStringHolder) {
	if (e.text.unicode != '\b' && e.text.unicode != '\r' &&
		e.key.code != sf::Keyboard::Left && e.key.code != sf::Keyboard::Right && e.text.unicode != 36
		&& e.key.code != sf::Keyboard::Escape && e.key.code != '~' && e.key.code != sf::Keyboard::Tab)
	{
		inputString.push_back(e.text.unicode);
	}
	else if (e.text.unicode == '\b'){
		if (inputString.size() > 0) {
			inputString.pop_back();
		}
	}
	inputStringHolder.setString(inputString);
}

void DrawAllTextBoxEntriesAndHighlights(std::vector<sf::Text>& textBoxEntries, std::vector<sf::RectangleShape>& hightlights,
	sf::RenderWindow& window) {
	for (int i = 0; i < textBoxEntries.size(); i++) { // should be parallel
		window.draw(textBoxEntries[i]);
		window.draw(hightlights[i]);
	}
}

void HandleInputStringValidation(std::vector<sf::Text>& textBoxEntries, sf::Text& inputStringHolder,
	std::string& inputString, int& currentTextBoxEntry, bool& stringAcceptedState,
	DFA& dfa, bool& stateIsSelected, bool& transitionIsSelected, sf::Font& font, sf::RenderWindow& window, std::vector<sf::RectangleShape>& highlights) {
	// 1) Both inputString and the text are not necessary, but easier to distinct both

	// Explicit check, this is also done in the function below,
	// but has to be replaced
	if (dfa.GetStates().size() == 0) {
		return;
	}

	// These variables should be passed by reference/value but easier to define here
	float textBoxXOffset = 50.f;
	float textBoxYOffset = 35.f;
	float textBoxSize = 300.f;
	sf::Color semiTransparentColorRed(255, 0, 0, 128);
	sf::Color semiTransparentColorGreen(0, 255, 0, 128);

	if (inputString.size() <= 0) {
		return; // nothing to validate
	}

	sf::Text stringEntry;
	stringEntry.setString(inputString);
	stringEntry.setCharacterSize(20.f);
	
	// This makes sure that the positioning between strings is spaced
	float textPositionHeightValue = (35.f * (textBoxEntries.size() + 2));

	stringEntry.setPosition(window.getSize().x - textBoxXOffset - textBoxSize + 10.f
		, textBoxYOffset + textPositionHeightValue);
	stringEntry.setFont(font);

	sf::RectangleShape highlight;
	highlight.setSize(sf::Vector2f(stringEntry.getGlobalBounds().width, stringEntry.getGlobalBounds().height + 9.f));
	highlight.setPosition(stringEntry.getPosition());

	// clean the holder variables
	inputStringHolder.setString("");
	bool result = dfa.CheckIfStringAccepted(inputString);
	
	inputString.clear();

	// !TODO!, this should be a pair of boolean values, as this can break
	// check CheckIfStringAccepted returns
	if (result) {
		stringAcceptedState = true;

		highlight.setFillColor(semiTransparentColorGreen);

		if (stateIsSelected) {
			dfa.DeSelectState();
		}
		if (transitionIsSelected) {
			dfa.DeSelectTransition();
		}
		
	}
	else {
		stringAcceptedState = true;
		highlight.setFillColor(semiTransparentColorRed);

		if (stateIsSelected) {
			dfa.DeSelectState();
		}
		if (transitionIsSelected) {
			dfa.DeSelectTransition();
		}

	}
	textBoxEntries.push_back(stringEntry);
	highlights.push_back(highlight);

}