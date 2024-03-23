/**
 * @file main.cpp
 * @brief Description of the contents of the file.
 *
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 *
 * Copyright (C) 2024 Ivan Ovcharov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *
 * If you have questions or need further information, you can contact me through my GitHub:
 *
 * Ivan Ovcharov
 * GitHub: https://github.com/SortedIvan
 */

#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include <iostream>
#include "../hpp/dfa.hpp"
#include "../hpp/line.hpp"
#include "../hpp/scalehandler.hpp"
#include "../hpp/filesystem.hpp"
#include "../hpp/button.hpp"

/*
	 Function definitions 
*/
sf::Vector2f GetMousePosition(sf::RenderWindow& window);
void TryLoadFont(sf::Font& font, std::string path);
void HandleStateLabelInput(sf::Event& e, DFA& dfa, int selectedState);
void RotateRectangle(sf::ConvexShape& rect, float angle);
float dot_product(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
void HandleTransitionSymbolInput(sf::Event& e, DFA& dfa);
bool DeleteTransition(sf::Event& e, DFA& dfa);
void HandleInputStringTextEditing(std::string& inputString, sf::Event& e, sf::Text& inputStringHolder, bool& isTyping);
void DrawAllTextBoxEntriesAndHighlights(std::vector<sf::Text>& textBoxEntries,std::vector<sf::RectangleShape>& hightlights,
	sf::RenderWindow& window);
void HandleInputStringValidation(std::vector<sf::Text>& textBoxEntries, sf::Text& inputStringHolder,
	std::string& inputString, int& currentTextBoxEntry, bool& stringAcceptedState,
	DFA& dfa, bool& stateIsSelected, bool& transitionIsSelected,sf::Font& font,
	sf::RenderWindow& window, std::vector<sf::RectangleShape>& highlights, bool& errorMode);
void UpdateAlphabetDisplay(DFA& dfa, sf::Text& alphabetHolder);
void HandleMouseHover(DFA& dfa, bool& mouseOverState, bool& mouseOverTransition, bool& mouseOverButton,
	int& hoveredOverStateId, int& highlightedState, sf::RenderWindow& window,
	bool stateMovingMode, Button& saveFileBtn, Button& loadFileBtn);
bool ChangeTransitionDirection(DFA& dfa, sf::Event& e);
void SwitchAutomaticStateLabelling(bool& automaticStateLabeling, int& automaticStateLabelCounter);
void TryLoadImage(sf::Image& image, std::string path);
void HandleTextBoxTypingHighlighter(sf::Text& inputStringHolder, sf::RectangleShape& typingIndicator);
void HandleButtonPresses(Button& saveFileButton, Button& loadFileButton, bool& saved);
void LoadDfaFromFile(DfaFile& file, bool& automaticStateLabels, int& automaticStateLabelCount, int& stateCounter,
	int& transitionCounter, DFA& dfa);

int main() {

	/*
		Titlebar && Close prevent the screen from being resized by default
		The application is not yet optimized for resizing
	*/
	sf::RenderWindow window(
		sf::VideoMode(SCREEN_X_SIZE, SCREEN_Y_SIZE),
		"Dfa Generator",
		sf::Style::Titlebar | sf::Style::Close);

	window.setFramerateLimit(TARGET_FPS);

	sf::Event e;

	/*
		Utility classes
	*/ 
	ScaleHandler scaleHandler;
	FileSystem fileSystem;

	DfaFile file = fileSystem.LoadFile("filestructure.json");

	sf::Font font;
	sf::Image icon;

	/*
		Load the logo and main font for all text
	*/
	TryLoadFont(font, "testfont.ttf");
	TryLoadImage(icon, "dfaicon.png");

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	
	//<------------Begin-textbox-logic------------------------------->
	sf::RectangleShape textBox(sf::Vector2f(300 + window.getSize().x / 1200, window.getSize().y - 50.f));
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
	textBox.setOutlineThickness(5.f);
	textBox.setOutlineColor(sf::Color::White);

	textBoxSecondary.setPosition(window.getSize().x - textBoxXOffset - textBox.getSize().x
		,window.getSize().y - 65.f);
	textBoxSecondary.setFillColor(sf::Color::Transparent);
	textBoxSecondary.setOutlineThickness(5.f);
	textBoxSecondary.setOutlineColor(sf::Color::White);

	sf::RectangleShape highlightIndicator(sf::Vector2f(inputStringHolder.getCharacterSize() /2, inputStringHolder.getCharacterSize()));
	highlightIndicator.setFillColor(sf::Color::White);
	highlightIndicator.setPosition(sf::Vector2f(inputStringHolder.getPosition().x, inputStringHolder.getPosition().y + inputStringHolder.getCharacterSize() / 4));

	// <------------End-textbox-graphics------------------------------>

	// <------------Alphabet-graphics--------------------------------->
	sf::Text alphabetHolder;
	alphabetHolder.setString("Alphabet:");
	alphabetHolder.setCharacterSize(20.f);
	alphabetHolder.setPosition(
		sf::Vector2f(50.f, window.getSize().y - 50.f)
	);
	alphabetHolder.setFont(font);
	// <------------End-alphabet-graphics----------------------------->

	// <------------Overlay-Graphics---------------------------->
	sf::CircleShape statePlacementIndicator;
	statePlacementIndicator.setRadius(scaleHandler.DEFAULT_STATE_RADIUS);
	statePlacementIndicator.setOrigin(sf::Vector2f(scaleHandler.DEFAULT_STATE_RADIUS, scaleHandler.DEFAULT_STATE_RADIUS));
	statePlacementIndicator.setFillColor(DEFAULT_STATE_COLOR);

	sf::RectangleShape errorIndicator(sf::Vector2f(350, 100));
	errorIndicator.setPosition(35, 35);
	errorIndicator.setFillColor(sf::Color::Transparent);
	errorIndicator.setOutlineThickness(4.f);
	errorIndicator.setOutlineColor(sf::Color::White);

	sf::Text errorMessageLabel;
	errorMessageLabel.setString("Error indicator:");
	errorMessageLabel.setCharacterSize(13.f);
	errorMessageLabel.setPosition(40.f, 40.f);
	errorMessageLabel.setFont(font);

	sf::Text errorMessage;
	errorMessage.setString("");
	errorMessage.setCharacterSize(13.f);
	errorMessage.setPosition(sf::Vector2f(sf::Vector2f(40.f, 55.f)));
	errorMessage.setFont(font);

	sf::RectangleShape automaticStateLabelsCheckBox(sf::Vector2f(25, 25));
	sf::RectangleShape automaticStateLabelsCheckBoxChecked(sf::Vector2f(20, 20));

	sf::Text automaticStateLabelsText;
	automaticStateLabelsText.setString("Auto state label");
	automaticStateLabelsText.setCharacterSize(13.f);
	automaticStateLabelsText.setFont(font);

	automaticStateLabelsCheckBox.setFillColor(sf::Color::Transparent);
	automaticStateLabelsCheckBox.setOutlineThickness(4.f);
	automaticStateLabelsCheckBox.setOutlineColor(sf::Color::White);
	automaticStateLabelsCheckBox.setPosition(sf::Vector2f(errorIndicator.getPosition().x + errorIndicator.getSize().x + 35.f, errorIndicator.getPosition().y));

	automaticStateLabelsCheckBoxChecked.setFillColor(sf::Color::White);
	automaticStateLabelsCheckBoxChecked.setPosition(sf::Vector2f(errorIndicator.getPosition().x + errorIndicator.getSize().x + 37.5f, errorIndicator.getPosition().y + 2.5f));
	automaticStateLabelsText.setPosition(sf::Vector2f(errorIndicator.getPosition().x + errorIndicator.getSize().x + 75.f, errorIndicator.getPosition().y));

	/*
		Button declarations
		save & file, size is calculated with respect to the checkbox
	*/
	Button saveFileBtn(sf::Vector2f(
		automaticStateLabelsText.getPosition().x + 150.f,
		automaticStateLabelsCheckBox.getPosition().y),
		sf::Vector2f(75.f, 40.f), font, "Save");

	Button loadFileBtn(sf::Vector2f(
		automaticStateLabelsText.getPosition().x + 250.f,
		automaticStateLabelsCheckBox.getPosition().y),
		sf::Vector2f(75.f, 40.f), font, "Load");

	// <---------------End-Overlay-Graphics----------------------->
	DFA dfa;
	int transitionIdCounter = 0;
	int selectedState = -1;
	int highlightedState = -1;
	int transitionCounter = 0; //<=	These can overflow, but unrealistic in practice
	int stateCounter = 0;	   //<=|
	bool stateIsSelected = false;
	bool shiftHeldDown = false;
	bool transitionIsSelected = false;
	bool stringAcceptedState = false;
	bool textboxState = false;
	bool mouseOverlayMode = true;
	bool mouseOverState = false;
	bool mouseOverTransition = false;
	bool mouseOverTextbox = false;
	bool mouseOverButton = false;
	bool errorMode = false;
	bool stateMovingMode = false;
	int hoveredOverStateId = -1;
	bool automaticStateLabelGeneration = true;
	int automaticStateLabelCount = 0;
	sf::Clock cursorTimer;
	sf::Clock typingTimer;
	bool cursorIndicator = true;
	bool isTyping = false;
	bool canPlace = false;
	bool saved = false;
	bool userLoadingFileMode = false;

	std::string inputString;

	// ---- test -----
	while (window.isOpen()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !userLoadingFileMode)
		{
			shiftHeldDown = true;
		}
		else {
			shiftHeldDown = false;
		}
		
		//	<------------Mouse-Overlay-Handling----------------------->
		sf::Vector2i mousePosition = (sf::Vector2i)GetMousePosition(window);
		if (mousePosition.x >= 0 && mousePosition.x < window.getSize().x &&
			mousePosition.y >= 0 && mousePosition.y < window.getSize().y && !userLoadingFileMode) {
			// Mouse is on the screen
			canPlace = true;
		}

		if (textBox.getGlobalBounds().contains((sf::Vector2f)mousePosition) ||
			textBoxSecondary.getGlobalBounds().contains((sf::Vector2f)mousePosition)) {
			mouseOverTextbox = true;
		}
		else {
			mouseOverTextbox = false;
		}

		canPlace = !mouseOverButton && !mouseOverTextbox && !mouseOverState && !mouseOverTransition && !userLoadingFileMode
			&& !errorIndicator.getGlobalBounds().contains((sf::Vector2f)mousePosition)
			&& !automaticStateLabelsCheckBox.getGlobalBounds().contains((sf::Vector2f)mousePosition);

		if (mouseOverlayMode) { 
			// update the overlay position
			if (canPlace) {
				statePlacementIndicator.setPosition((sf::Vector2f)mousePosition);
			}
		}

		if (!userLoadingFileMode) {
			HandleMouseHover(dfa, mouseOverState, mouseOverTransition,
				mouseOverButton, hoveredOverStateId, highlightedState,
				window, stateMovingMode, saveFileBtn, loadFileBtn);
		}

		// <------------End-Mouse-Overlay-Handling----------------------->

		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}

			if (saveFileBtn.GetIsPressed()) {
				if (saved) {
					continue;
				}
			}

			if (loadFileBtn.GetIsPressed() && !userLoadingFileMode) {
				if (!saved) {
					 // prompt user to save here
				}

				userLoadingFileMode = true;
			}


			if (e.type == sf::Event::KeyReleased) {
				if (e.type == sf::Event::Resized) {
					scaleHandler.UpdateConstants(window.getSize());
				}

				if (e.key.code == sf::Keyboard::Delete) {

					if (!userLoadingFileMode) {
						if (transitionIsSelected) {
							// First, check if user is trying to delete the transition
							if (DeleteTransition(e, dfa)) {
								transitionIsSelected = false;
								dfa.DeSelectTransition();
								UpdateAlphabetDisplay(dfa, alphabetHolder);
								continue;
							}
						}

						if (stateIsSelected) { // Check to see if user wants to delete a state
							dfa.DeleteState(selectedState);
							UpdateAlphabetDisplay(dfa, alphabetHolder);
							stateIsSelected = false;
							continue;
						}
					}
				}

				// Check if user is trying to change the state direction
				if (transitionIsSelected  && !userLoadingFileMode) {
					bool changed = ChangeTransitionDirection(dfa, e); 

					if (changed) {
						saved = false;
					}
				}

				if (e.key.code == sf::Keyboard::Tab) {
					if (stateIsSelected && !userLoadingFileMode) {
						dfa.ChangeStateAccepting(selectedState);
						saved = false;
						continue;
					}
				}

				if (e.key.code == sf::Keyboard::Enter) {

					if (!userLoadingFileMode) {
						std::tuple<bool, int, std::string> isDfa = dfa.CheckIfDfa();

						if (!std::get<0>(isDfa)) {
							errorMessage.setString(std::get<2>(isDfa));
							errorMode = true;
						}
						else {
							HandleInputStringValidation(textBoxEntries, inputStringHolder, inputString,
								currentTextEntry, stringAcceptedState, dfa, stateIsSelected,
								transitionIsSelected, font, window, textBoxHighlights, errorMode);
							errorMode = false;
							errorMessage.setString("");
							saved = false;
						}

						// Reset the typing highlight indicator to the default position
						highlightIndicator.setPosition(
							sf::Vector2f(
								inputStringHolder.getPosition().x,
								inputStringHolder.getPosition().y + inputStringHolder.getCharacterSize() / 4
							));

						dfa.DeSelectState();
						dfa.DeSelectTransition();
					}
				}
			}

			if (e.type == sf::Event::TextEntered) {
				if (textboxState && !userLoadingFileMode) {
					HandleInputStringTextEditing(inputString,e, inputStringHolder, isTyping);
					HandleTextBoxTypingHighlighter(inputStringHolder, highlightIndicator);
					saved = false;
					continue;
				}


				// First, check for prevalent conditions
				if (e.text.unicode == BACKSPACE) {
					if (transitionIsSelected && !userLoadingFileMode) {
						dfa.RemoveSymbolFromTransition();
						UpdateAlphabetDisplay(dfa, alphabetHolder);
						transitionIsSelected = false;
						saved = false;
						continue;
					}
				}

				if (stateIsSelected && e.text.unicode != TAB && !userLoadingFileMode) {
					HandleStateLabelInput(e, dfa, selectedState);
					saved = false;
					continue;
				}
				if (transitionIsSelected && !userLoadingFileMode) {
					HandleTransitionSymbolInput(e, dfa);
					UpdateAlphabetDisplay(dfa, alphabetHolder);
					saved = false;
					continue;
				}

			}

			// On Mouse Click Release
			if (e.type == sf::Event::MouseButtonReleased) {
				
				sf::Vector2f mousePos = (sf::Vector2f)GetMousePosition(window);

				// Check if load/save button was clicked
				
				if (!userLoadingFileMode) {
					if (stateMovingMode) {
						stateMovingMode = false;
					}

					if (stringAcceptedState) { // first clear the accepted state
						dfa.SetAllStatesDefaultColor();
						stringAcceptedState = false;
					}

					// Check whether the user clicked in the text box
					if (textBox.getGlobalBounds().contains(mousePos)) {
						transitionIsSelected = false;
						stateIsSelected = false;
						selectedState = -1;
						dfa.DeSelectState();
						dfa.DeSelectTransition();
						textboxState = true;
						continue;
					}

					if (automaticStateLabelsCheckBox.getGlobalBounds().contains((sf::Vector2f)mousePos)) {
						SwitchAutomaticStateLabelling(automaticStateLabelGeneration, automaticStateLabelCount);
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

					int tempSelected = dfa.GetSelectedStateId(mousePos);
					if (tempSelected != -2) {
						// if a different state was selected
						if (stateIsSelected && selectedState != -1) {

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

							saved = false;
							textboxState = false; // Exit textbox
						}
						else { // First time selecting state
							selectedState = tempSelected;
							stateIsSelected = true;
							textboxState = false;
							dfa.DeSelectTransition();
							if (selectedState >= 0) {
								dfa.SetSelectedState(selectedState);
							}
						}
					}
					else {
						if (canPlace) {
							if (automaticStateLabelGeneration) {
								std::cout << GetMousePosition(window).x << " " << GetMousePosition(window).y << std::endl;
								dfa.AddNewState("q" + std::to_string(automaticStateLabelCount), mousePos, font, stateCounter);
								automaticStateLabelCount += 1;
								dfa.StateToString(stateCounter);
							}
							else {
								dfa.AddNewState("", mousePos, font, stateCounter);
							}

							stateCounter++;
							stateIsSelected = false;
							selectedState = -1;
							dfa.DeSelectTransition();
							textboxState = false; // Exit textbox
							saved = false;
						}
					}
				}
			}

		}

		saveFileBtn.Update(GetMousePosition(window));
		loadFileBtn.Update(GetMousePosition(window));

		//<----------- End of event logic, anything that takes place before drawing ------------>
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			// Handle movement of state

			if (!userLoadingFileMode) {
				if (hoveredOverStateId != -1 && !shiftHeldDown && !stateMovingMode) {
					selectedState = hoveredOverStateId;
					stateIsSelected = true;
					textboxState = false;
					dfa.DeSelectTransition();

					if (selectedState >= 0) {
						dfa.SetSelectedState(selectedState);
					}
					saved = false;
				}

				if (stateIsSelected && mouseOverState
					&& hoveredOverStateId != -1 && hoveredOverStateId == selectedState && !shiftHeldDown) {

					stateMovingMode = true;

					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					dfa.MoveStatePosition((sf::Vector2f)mousePosition, selectedState);
					saved = false;
				}
			}
		}

		// Blinking highlighter
		if (cursorTimer.getElapsedTime().asSeconds() > TEXT_HIGHLIGHTER_CHANGE_SPEED) {
			cursorIndicator = !cursorIndicator;
			cursorTimer.restart();
		}

		// Exiting typing
		if (isTyping) {
			if (typingTimer.getElapsedTime().asSeconds() > TYPING_EXITED_CD) {
				isTyping = false;
				typingTimer.restart();
			}
		}

		// --------- clear the screen ----------
		window.clear(DEFAULT_BG_COLOR);

		// --------- draw on the screen ---------
		if (isTyping && mouseOverTextbox) {
			window.draw(highlightIndicator);
		}
		if (mouseOverTextbox && !isTyping) {
			if (cursorIndicator) {
				window.draw(highlightIndicator);
			}
		}

		window.draw(textBox);
		window.draw(textBoxSecondary);
		window.draw(textBoxDescr);
		window.draw(inputStringHolder);
		dfa.DrawAllStates(window);
		DrawAllTextBoxEntriesAndHighlights(textBoxEntries, textBoxHighlights, window);
		window.draw(alphabetHolder);

		saveFileBtn.Render(window);
		loadFileBtn.Render(window);

		window.draw(automaticStateLabelsCheckBox);

		if (automaticStateLabelGeneration) {
			window.draw(automaticStateLabelsCheckBoxChecked);
		}

		window.draw(automaticStateLabelsText);
		window.draw(errorMessageLabel);
		window.draw(errorIndicator);

		if (errorMode) {
			window.draw(errorMessage);
		}
;

		if (mouseOverlayMode && canPlace && !mouseOverState && !mouseOverTransition && !mouseOverButton) {
			window.draw(statePlacementIndicator);
		}

		// --------- display on the screen --------
		window.display();

	}
}

void TryLoadFont(sf::Font& font, std::string path)
{
	if (!font.loadFromFile(path)) {
		std::cout << "Error loading the font file" << std::endl;
		system("pause");
	}
}

void TryLoadImage(sf::Image& image, std::string path) {
	if (!image.loadFromFile(path)) {
		std::cout << "Error loading the image file at path: " + path << std::endl;
		system("pause");
	}
}

void HandleStateLabelInput(sf::Event& e, DFA& dfa, int selectedState) {
	if (e.text.unicode != '\b' && e.text.unicode != '\r' &&
		e.key.code != sf::Keyboard::Left && e.key.code != sf::Keyboard::Right && e.text.unicode != 36
		&& e.key.code != sf::Keyboard::Escape && e.key.code != sf::Keyboard::Tab)
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

void SwitchAutomaticStateLabelling(bool& automaticStateLabeling, int& automaticStateLabelCounter) {
	automaticStateLabeling = !automaticStateLabeling;
	automaticStateLabelCounter = 0;
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

bool DeleteTransition(sf::Event& e, DFA& dfa) {
	dfa.DeleteSelectedTransition();
	return true;
}

void HandleInputStringTextEditing(std::string& inputString, sf::Event& e, sf::Text& inputStringHolder, bool& isTyping) {
	if (e.type == sf::Event::TextEntered && e.text.unicode != '\b' && e.text.unicode != '\r' &&
		e.key.code != sf::Keyboard::Left && e.key.code != sf::Keyboard::Right && e.text.unicode != 36
		&& e.key.code != sf::Keyboard::Escape && e.key.code != '~' && e.text.unicode != '\t')
	{
		inputString.push_back(e.text.unicode);
	}
	else if (e.text.unicode == '\b'){
		if (inputString.size() > 0) {
			inputString.pop_back();
		}
	}
	inputStringHolder.setString(inputString);
	isTyping = true;
}

void HandleTextBoxTypingHighlighter(sf::Text& inputStringHolder, sf::RectangleShape& typingIndicator) {
	if (inputStringHolder.getString().getSize() > 0) {
		sf::Vector2f position = inputStringHolder.findCharacterPos(inputStringHolder.getString().getSize() - 1);
		typingIndicator.setPosition(sf::Vector2f(position.x + typingIndicator.getSize().x, position.y + inputStringHolder.getCharacterSize() / 4));
	}
	else {
		typingIndicator.setPosition(sf::Vector2f(inputStringHolder.getPosition().x, inputStringHolder.getPosition().y + inputStringHolder.getCharacterSize() / 4));
	}
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
	DFA& dfa, bool& stateIsSelected, bool& transitionIsSelected, sf::Font& font
	, sf::RenderWindow& window, std::vector<sf::RectangleShape>& highlights, bool& errorMode) {
	// Both inputString and the text are not necessary, but easier to distinguish them like this

	// First, run a sanity check and see if it satiesfies DFA conditions
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
		errorMode = false;

		highlight.setFillColor(semiTransparentColorGreen);
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

void UpdateAlphabetDisplay(DFA& dfa, sf::Text& alphabetHolder) {
	std::string alphabetString = "Alphabet: ";

	for (int i = 0; i < dfa.GetAlphabet().size(); i++) {
		alphabetString.push_back(dfa.GetAlphabet()[i]);

		if (i != dfa.GetAlphabet().size() - 1) { // not the last element
			alphabetString.push_back(',');
		}
	}
	alphabetHolder.setString(alphabetString);
}

void HandleMouseHover(DFA& dfa, bool& mouseOverState, bool& mouseOverTransition,bool& mouseOverButton, 
	int& hoveredOverStateId, int& highlightedState, sf::RenderWindow& window, bool stateMovingMode,
	Button& saveFileBtn, Button& loadFileBtn) {
	
	if (stateMovingMode) {
		return;
	}

	if (saveFileBtn.IsMouseOverButton() || loadFileBtn.IsMouseOverButton()) {
		mouseOverButton = true;
	}
	else {
		mouseOverButton = false;
	}

	sf::Vector2i mousePosition = (sf::Vector2i)GetMousePosition(window);
	mouseOverState = false;  // Set to false before checking states
	mouseOverTransition = false;
	hoveredOverStateId = -1;

	for (int i = 0; i < dfa.GetStates().size(); i++) {
		if (dfa.GetStates()[i].GetStateCircle().getGlobalBounds().contains((sf::Vector2f)mousePosition)) {
			mouseOverState = true;  // Set to true if mouse is over any state
			hoveredOverStateId = dfa.GetStates()[i].GetStateId();
			break;  // No need to check further once we find a state under the mouse
		}
		if (dfa.CheckStateTransitionCollision((sf::Vector2f)mousePosition) == 0) {
			mouseOverTransition = true;
			break;
		}
	}
}

bool ChangeTransitionDirection(DFA& dfa, sf::Event& e) {
	switch (e.key.code) {
		case sf::Keyboard::Right:
			dfa.ChangeStateTransitionDirection(RIGHT);
			return true;
		case sf::Keyboard::Left:
			dfa.ChangeStateTransitionDirection(LEFT);
			return true;
		case sf::Keyboard::Up:
			dfa.ChangeStateTransitionDirection(TOP);
			return true;
		case sf::Keyboard::Down:
			dfa.ChangeStateTransitionDirection(BOTTOM);
			return true;
	}
	return false;
}

sf::Vector2f GetMousePosition(sf::RenderWindow& window) {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
	return window.mapPixelToCoords(mousePosition);
}

void HandleButtonPresses(Button& saveFileButton, Button& loadFileButton, bool& saved) {
	if (saveFileButton.GetIsPressed()) {
		if (saved) {
			return;
		}
	}
}

void LoadDfaFromFile(DfaFile& file, bool& automaticStateLabels, int& automaticStateLabelCount, int& stateCounter,
	int& transitionCounter, DFA& dfa, sf::Font& font) {
	
	automaticStateLabels = file.GetAutomaticStateLabels();
	automaticStateLabelCount = file.GetAutomaticStateLabelCount();
	stateCounter = file.GetStateCounter();
	transitionCounter = file.GetTransitionCounter();
	
	for (int i = 0; i < file.GetStates().size(); i++) {
		
		dfa.AddNewState(file.GetStates()[i].label, file.GetStates()[i].statePosition, font, file.GetStates()[i].stateId);

		if (file.GetStates()[i].isAccepting) {
			dfa.ChangeStateAccepting(file.GetStates()[i].stateId);
		}

		for (int k = 0; k < file.GetStates()[i].transitionObjects.size(); k++) {
			dfa.AddNewTransition(
				file.GetStates()[i].transitionObjects[k].transitionFrom,
				file.GetStates()[i].transitionObjects[k].transitionTo, 
				file.GetStates()[i].transitionObjects[k].id, 
				font);
		}

		//dfa.AddNewState()
	}

}
