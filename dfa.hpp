#pragma once
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include "dfa_state.hpp"

class DFA {
	private:
		std::vector<DfaState> states;
		std::vector<char> alphabet;
		int selectedState = -2;
	public:
		DFA();
		// getters
		std::vector<DfaState> GetStates();
		std::vector<char> GetAlphabet();
		int GetSelectedStateIndex(sf::Vector2f positionClicked);
		
		void AddNewState(std::string label, sf::Vector2f position, sf::Font& font);
		void SetAlphabet(std::vector<char> alphabet);
		void DrawAllStates(sf::RenderWindow& window);
		bool CheckIfStateSelected(sf::Vector2f positionClicked);
		void ChangeStateLabelText(sf::Event& event, int selectedState);
		void DeleteStateLabelCharacter(int selectedState);
		bool AddNewTransition(int stateA, int stateB);
		void SetSelectedState(int selectedState);

};