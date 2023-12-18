#pragma once
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include "dfa_state.hpp"

class DFA {
	private:
		std::vector<DfaState> states;
		std::vector<char> alphabet;
	public:
		DFA();
		// getters
		std::vector<DfaState> GetStates();
		std::vector<char> GetAlphabet();
		
		// setters
		void AddNewState(std::string label, sf::Vector2f position);
		void SetAlphabet(std::vector<char> alphabet);

		void DrawAllStates(sf::RenderWindow& window);
};