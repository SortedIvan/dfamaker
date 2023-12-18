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
		DfaState(std::string label, sf::Vector2f statePosition);

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
};