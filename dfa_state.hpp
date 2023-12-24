#pragma once
#include <string>
#include <map>
#include "SFML/Graphics.hpp"
#include <vector>

class DfaState {
	private:
		// ------- state related vars
		std::string label;
		std::map<char, int> transitions;
		std::vector<sf::RectangleShape> transitionLines;
		std::vector<sf::CircleShape> transitionArrows; // sf::CircleShape triangle(20, 3);
		bool isAccepting = false;
		bool isStarting;

		// ------- graphics related vars
		sf::Vector2f statePosition;
		sf::CircleShape stateCircle;
		sf::Text textLabel;

	public:
		DfaState(std::string label, sf::Vector2f statePosition, sf::Font& font);
		DfaState();
		// Setters
		void SetIsAccepting(bool isAccepting);
		void SetStateColor(sf::Color color);
	    void SetIsStarting(bool isStarting);
		void SetStateLabel(std::string label);

		// Getters
		std::string GetStateLabel();
		std::map<char, int> GetStateTransitions();
		bool GetIsAccepting();
		bool GetIsStarting();
		sf::Vector2f GetStatePosition();
		sf::CircleShape GetStateCircle();

		void ChangeStateTransition(char oldTransitionChar, char newTransitionChar);
		bool AddStateTransition(char transitionChar, int toState);
		void RemoveStateTransition(char transitionChar);

		sf::Text& GetTextLabelRef();

		void Draw(sf::RenderWindow& window);
};