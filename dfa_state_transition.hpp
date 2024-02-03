#pragma once
#include "line.hpp"

class StateTransition {
	private: 
		sfLine mainArrow;
		sfLine arrowTipOne;
		sfLine arrowTipTwo;
		float arrowLength;
		int transitionTo;
		int transitionFrom;
		bool isAssigned;
		int id = 0; // Unsafe, can overflow but I don't care
		char symbol = '~';

	public:
		StateTransition();
		void SetTransitionTwo(int stateTo);
		int GetTransitionTo();
		int GetTransitionFrom();
		void Draw(sf::RenderWindow& window);
		void SetUpStateTransition(sf::Vector2f stateFrom, sf::Vector2f stateTo, float stateFromRadius, 
			float stateToRadius, int stateToValue, int transitionFromValue);
		sfLine GetMainArrow();
		bool GetIsAssigned();
		void SetIsAssigned(bool value);
		int GetTransitionId();
		void SetTransitionId(int id);
		void SetTransitionColor(sf::Color color);
		void SetTransitionSymbol(char symbol);
		char GetTransitionSymbol();

		
};