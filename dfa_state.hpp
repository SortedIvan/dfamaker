#pragma once
#include <string>
#include <map>
#include "SFML/Graphics.hpp"
#include <vector>
#include "line.hpp"
#include "dfa_state_transition.hpp"

class DfaState {
	private:
		// ------- state related vars
		std::string label;
		std::vector<StateTransition> transitionObjects;
		std::map<char, int> transitions;
		bool isAccepting = false;
		bool isStarting;

		// ------- graphics related vars
		sf::Vector2f statePosition;
		sf::Vector2f stateCenter;
		sf::CircleShape stateCircle;
		sf::CircleShape outlining;
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
		bool GetIsAccepting();
		bool GetIsStarting();
		sf::Vector2f GetStatePosition();
		sf::CircleShape GetStateCircle();
		sf::Vector2f GetStateCenter();
		std::vector<StateTransition> GetTransitionObjects();

		void ChangeStateTransition(char oldTransitionChar, char newTransitionChar);
		//bool AddStateTransition(char transitionChar, int toState);
		void RemoveStateTransition(char transitionChar);

		StateTransition AddStateTransition(sf::Vector2f stateFrom, sf::Vector2f stateTo, float stateFromRadius, 
			float stateToRadius, int stateToValue, int stateFromValue, int id, sf::Font& font);
		bool AddStateTransitionSymbol(char transitionChar, int selectedStateTransition);
	
		sf::Text& GetTextLabelRef();

		void Draw(sf::RenderWindow& window, bool is_selected);
		void DrawStateArrows(sf::RenderWindow& window);
		void ChangeTransitionColor(int transitionIndex, sf::Color color);
		void SetTransitionSymbol(int transitionIndex, char symbol);



};