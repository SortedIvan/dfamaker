#pragma once
#include "line.hpp"

enum TransitionDirection {
	TOP = 1,
	BOTTOM = 2,
	LEFT = 3,
	RIGHT = 4
};

class StateTransition {
	private: 
		// For regular state transitions
		sfLine mainArrow;

		// For self loops
		sfLine rhs;
		sfLine top;
		sfLine lhs;

		// Both self-loops and regular transitions have arrow tips
		sfLine arrowTipOne;
		sfLine arrowTipTwo;

		float arrowLength;
		int transitionTo;
		int transitionFrom;
		bool isAssigned;
		bool isSelfLoop;
		int id = 0; // Unsafe, can overflow but I don't care

		std::vector<char> symbols;
		float distance;
		sf::Text transitionLabel;

		// Variables to adjust direction and do future calculations
		sf::Vector2f stateToPosition;
		float stateFromRadius;
		float stateToRadius;
		TransitionDirection direction = TOP; // default to top, only for self loops

	public:
		StateTransition();
		void SetTransitionTo(int stateTo);
		int GetTransitionTo();
		int GetTransitionFrom();
		void Draw(sf::RenderWindow& window);
		void SetUpStateTransition(sf::Vector2f stateFrom, sf::Vector2f stateTo, float stateFromRadius, 
			float stateToRadius, int stateToValue, int transitionFromValue, sf::Font& font);
		sfLine GetMainArrow();
		sfLine GetRhsArrow();
		sfLine GetLhsArrow();
		sfLine GetTopArrow();

		bool GetIsAssigned();
		void SetIsAssigned(bool value);
		int GetTransitionId();
		void SetTransitionId(int id);
		void SetTransitionColor(sf::Color color);
		void SetTransitionDistance(float distance);
		bool GetIsSelfLoop();
		void SetIsSelfLoop(bool value);
		std::vector<char> GetTransitionSymbols();
		bool AddTransitionSymbol(char symbol);
		char RemoveSingleSymbol();
		bool CheckSymbolExists(char symbol);
		void ChangeTransitionDirection(TransitionDirection direction, sf::Vector2f stateFrom);
		void SetStateToPosition(sf::Vector2f newStateToPosition);
		sf::Vector2f GetStateToPosition();
		void MoveStateTransitionRegular(sf::Vector2f stateFrom, sf::Vector2f stateTo, float stateFromRadius, float stateToRadius);
		void MoveStateTransitionSelfLoop(sf::Vector2f stateFrom);

};