#pragma once
#include "line.hpp"

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
		char symbol = '~';
		float distance;
		sf::Text transitionLabel;

	public:
		StateTransition();
		void SetTransitionTwo(int stateTo);
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
		void SetTransitionSymbol(char symbol);
		char GetTransitionSymbol();
		void SetTransitionDistance(float distance);
		bool GetIsSelfLoop();
		void SetIsSelfLoop(bool value);
		std::vector<char> GetTransitionSymbols();
		void AddTransitionSymbol(char symbol);
		

};