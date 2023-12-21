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
		void AddNewState(std::string label, sf::Vector2f position, sf::Font& font);
		void SetAlphabet(std::vector<char> alphabet);

		void DrawAllStates(sf::RenderWindow& window);

		int GetSelectedStateIndex(sf::Vector2f positionClicked);
		bool CheckIfStateSelected(sf::Vector2f positionClicked);

		void ChangeStateLabelText(sf::Event& event, int selectedState);
		void DeleteStateLabelCharacter(int selectedState);
};