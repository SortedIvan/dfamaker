#pragma once
#include <string>
#include <map>
#include "SFML/Graphics.hpp"

class DfaState {
	private:
		// ------- state related vars
		std::string label;
		std::map<char, int> transitions;
		bool isAccepting = false;
		bool isStarting;

		// ------- graphics related vars
		sf::Vector2f statePosition;
		sf::CircleShape stateCircle;
		//circle.setOrigin(circle.getRadius(), circle.getRadius());
	public:
		DfaState(std::string label, sf::Vector2f statePosition, sf::CircleShape stateCircle, bool isStarting);
		void SetIsAccepting(bool isAccepting);
		bool AddStateTransition(char transitionChar, int toState);
		void RemoveStateTransition(char transitionChar);
		void SetStateColor(sf::Color color);
	    void SetIsStarting(bool isStarting);
		void SetStateLabel(std::string label);
		void ChangeStateTransition(char oldTransitionChar, char newTransitionChar);
};