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

		int previousSelectedTransition = -2;
		int currentSelectedTransition = -2;
		std::pair<int, int> previousSelectedTrans;
		std::pair<int, int> currentSelectedTrans;
		int startingStateIndex = 0;

	public:
		DFA();
		// getters
		std::vector<DfaState> GetStates();
		std::vector<char> GetAlphabet();
		int GetSelectedStateIndex(sf::Vector2f positionClicked);
		
		void AddNewState(std::string label, sf::Vector2f position, sf::Font& font, int id);
		void SetAlphabet(std::vector<char> alphabet);
		void DrawAllStates(sf::RenderWindow& window);
		bool CheckIfStateSelected(sf::Vector2f position);
		void ChangeStateLabelText(sf::Event& event, int selectedState);
		void DeleteStateLabelCharacter(int selectedState);
		bool AddNewTransition(int stateFrom, int stateTo, int id, sf::Font& font);
		void SetSelectedState(int selectedState);
		int SelectStateTransition(sf::Vector2f positionClicked);
		void SetTransitionSymbol(char symbol);
		void DeSelectState();
		void DeSelectTransition();
		bool DeleteSelectedTransition();
		bool DeleteState(int selectedState);
		void ChangeStateAccepting(int selectedState);
		bool CheckIfStringAccepted(std::string input);
		void SetStartingState(int index);
		std::pair<bool, DfaState> CheckStringAcceptingRecurs(DfaState currentState, std::string input, size_t position);
		bool RemoveSymbolFromTransition();
		bool SetAllStatesDefaultColor();
		bool DeleteTransition(int state, int transIndex);
		bool DeleteTransitionById(int stateFrom, int stateTo);
		int CheckStateTransitionCollision(sf::Vector2f position);
		std::tuple<bool, int, std::string> CheckIfDfa();
		int GetSelectedStateId(sf::Vector2f positionClicked);
		int FindStateIndexById(int id);


};