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

	public:
		StateTransition();
		void SetTransitionTwo(int stateTo);
		int GetTransitionTo();
		void Draw(sf::RenderWindow& window);
		void SetUpStateTransition(sf::Vector2f stateFrom, sf::Vector2f stateTo, float stateFromRadius, 
			float stateToRadius, int stateToValue, int transitionFromValue);
};