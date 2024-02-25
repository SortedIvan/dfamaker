#pragma once
#include <string>
#include <map>
#include "SFML/Graphics.hpp"
#include <vector>
#include "line.hpp"
#include "dfa_state_transition.hpp"


struct startingOutline {
	sfLine mainArrow;
	sfLine arrowTipOne;
	sfLine arrowTipTwo;
	float arrowLength;
};

class DfaState {
	private:
		// ------- state related vars
		std::string label;
		int stateId;
		std::vector<StateTransition> transitionObjects;
		std::map<char, int> transitions;
		std::vector<int> incomingTransitions;
		std::vector<int> outgoingTransitions;
		bool isAccepting = false;
		bool isStarting;
		startingOutline startingOutline;
		// ------- graphics related vars
		sf::Vector2f statePosition;
		sf::Vector2f stateCenter;
		sf::CircleShape stateCircle;
		sf::CircleShape outlining;
		sf::CircleShape acceptingOutline;
		sf::Text textLabel;

	public:
		DfaState(std::string label, sf::Vector2f statePosition, sf::Font& font);
		DfaState();
		// Setters
		void SetIsAccepting();
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
		StateTransition GetStateTransition(int index);

		//bool AddStateTransition(char transitionChar, int toState);
		void RemoveStateTransition(char transitionChar);

		StateTransition AddStateTransition(sf::Vector2f stateFrom, sf::Vector2f stateTo, float stateFromRadius, 
			float stateToRadius, int stateToValue, int stateFromValue, int id, sf::Font& font);
	
		sf::Text& GetTextLabelRef();

		void Draw(sf::RenderWindow& window, bool is_selected);
		void DrawStateArrows(sf::RenderWindow& window);
		void ChangeTransitionColor(int transitionIndex, sf::Color color);
		void SetTransitionSymbol(int transitionIndex, char symbol);
		bool DeleteTransition(int transitionIndex);
		bool DeleteTransitionTo(int toState);
		bool AddIncomingTransition(int from);
		bool AddOutgoingTransition(int to);
		std::vector<int> GetIncomingTransitions();
		std::vector<int> GetOutgoingTransitions();
		bool DeleteOutgoingTransition(int value);
		bool DeleteIncomingTransition(int value);
		void SetAcceptingStateManually(bool truthValue);
		int GetTransitionTo(char symbol);
		std::pair<bool, int> DeleteSingleTransitionSymbol(int transitionIndex);
		bool CheckTransitionExists(int from, int to);
		bool CheckTransitionExists(char symbol);
		void SetDefaultColor();
		void SetStringAcceptedColor();
		void SetStringDeclinedColor();
		std::map<char, int> GetTransitions();
		StateTransition GetTransition(int index);
		void SetTransitionTo(int transitionIndex, int value);
		void SetStateId(int id);
		int GetStateId();
		std::pair<int, StateTransition> GetTransitionByStateTo(int stateTo);
		void ChangeStateTransitionDirection(int selectedTransition, TransitionDirection direction);
		void MoveStatePosition(sf::Vector2f newStatePosition);
};