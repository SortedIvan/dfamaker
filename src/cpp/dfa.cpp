/**
 * @file dfa.cpp
 * @brief Description of the contents of the file.
 *
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 *
 * Copyright (C) [year] Ivan Ovcharov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * ---
 *
 * If you have questions or need further information, you can contact me through my GitHub:
 *
 * Ivan Ovcharov
 * GitHub: https://github.com/SortedIvan
 */


#include "../hpp/dfa.hpp";
#include <iostream>
#include <set>

void RemoveSingleCharacterFromAlphabet(std::vector<DfaState>& states,
	std::pair<int, int>& currentSelectedTrans, char removedChar, bool& symbolExistsAfterRemoval,
	std::vector<char>& alphabet);

std::vector<DfaState> DFA::GetStates() {
	return this->states;
}
std::vector<char> DFA::GetAlphabet() {
	return this->alphabet;
}

DFA::DFA() {
	currentSelectedTrans.first = -1;
	currentSelectedTrans.second = -1;
	previousSelectedTrans.first = -1;
	previousSelectedTrans.second = -1;
}

void DFA::AddNewState(std::string label, sf::Vector2f position, sf::Font& font, int id) {

	// By default, if its the first state added, make it starting
	DfaState state(label, position, font);

	state.SetStateId(id);

	if (states.size() == 0) {
		state.SetIsStarting(true);
		startingStateIndex = 0;
	}

	this->states.push_back(state);
}

void DFA::SetAlphabet(std::vector<char> alphabet) {

}

// If the state is marked as selected, draw its outlining
// otherwise => draw it normally
void DFA::DrawAllStates(sf::RenderWindow& window) {
	for (int i = 0; i < states.size(); i++) {

		if (selectedState == states[i].GetStateId()) {
			states[i].Draw(window, true);
			states[i].DrawStateArrows(window);
		}
		else {
			states[i].Draw(window, false);
			states[i].DrawStateArrows(window);
		}
	}
}

bool DFA::CheckIfStateSelected(sf::Vector2f position) {
	for (int i = 0; i < states.size(); i++) {
		if (states[i].GetStateCircle().getGlobalBounds().contains(position)) {
			return true;
		}
	}
	return false;
}

int DFA::GetSelectedStateIndex(sf::Vector2f positionClicked) {
	for (int i = 0; i < states.size(); i++) {
		if (states[i].GetStateCircle().getGlobalBounds().contains(positionClicked)) {
			return i;
		}
	}
	return -2;
}

int DFA::GetSelectedStateId(sf::Vector2f positionClicked) {
	for (int i = 0; i < states.size(); i++) {
		if (states[i].GetStateCircle().getGlobalBounds().contains(positionClicked)) {
			return states[i].GetStateId();
		}
	}
	return -2;
}

void DFA::ChangeStateLabelText(sf::Event& event, int stateId) {

	int index = FindStateIndexById(stateId);

	std::string currentLabel = states[index].GetStateLabel();
	states[index].SetStateLabel(currentLabel += event.text.unicode);
}

void DFA::DeleteStateLabelCharacter(int stateId) {

	int index = FindStateIndexById(stateId);

	if (states[index].GetStateLabel().size() != 0) {
		std::string currentLabel = states[index].GetStateLabel();
		currentLabel.resize(currentLabel.size() - 1);
		states[index].SetStateLabel(currentLabel);
	}
}

bool DFA::AddNewTransition(int stateFrom, int stateTo, int id, sf::Font& font) {
	int stateFromIndex = FindStateIndexById(stateFrom);
	int stateToIndex = FindStateIndexById(stateTo);

	DfaState stateToObj = states[stateToIndex];

	if (states[stateFromIndex].CheckTransitionExists(stateFrom, stateTo)) {
		// transition already exists
		return false;
	}


	StateTransition transition = states[stateFromIndex].AddStateTransition(states[stateFromIndex].GetStatePosition(),
		stateToObj.GetStatePosition(), states[stateFromIndex].GetStateCircle().getRadius(),
		stateToObj.GetStateCircle().getRadius(), stateTo, stateFrom, id, font);


	// if stateFrom == stateTo, only add to one array - for example incoming for consistency
	if (stateFrom == stateTo) {
		states[stateToIndex].AddIncomingTransition(stateFrom);
		return true;
	}

	states[stateToIndex].AddIncomingTransition(stateFrom);
	states[stateFromIndex].AddOutgoingTransition(stateTo);

	return true;
}

void DFA::SetSelectedState(int selectedState) {
	this->selectedState = selectedState;
}

// this code is cheap and fucking sucks
// it has to cycle through all states to find the transition that was clicked
int DFA::SelectStateTransition(sf::Vector2f positionClicked) {
	for (int i = 0; i < states.size(); i++) {
		for (int k = 0; k < states[i].GetTransitionObjects().size(); k++) {
			
			if (states[i].GetTransitionObjects()[k].GetMainArrow().CheckCollision(positionClicked)
				|| states[i].GetTransitionObjects()[k].GetRhsArrow().CheckCollision(positionClicked)
				|| states[i].GetTransitionObjects()[k].GetLhsArrow().CheckCollision(positionClicked)
				|| states[i].GetTransitionObjects()[k].GetTopArrow().CheckCollision(positionClicked)) {
				
				std::pair<int, int> trans = std::make_pair(states[i].GetStateId(), k);

				if (currentSelectedTrans.first == trans.first && currentSelectedTrans.second == trans.second) {
					// The same state was clicked;
					std::cout << "The same state was clicked;";

					return 0;
				}
				
				try {
					// First time selecting a state
					if (previousSelectedTrans.first == -1 && previousSelectedTrans.second == -1) {
						previousSelectedTrans = trans;
						currentSelectedTrans = trans;
						states[i].ChangeTransitionColor(k, sf::Color::Yellow);
					}
					else {
						previousSelectedTrans = currentSelectedTrans;

						int previousSelectedIndex = FindStateIndexById(previousSelectedTrans.first);

						states[previousSelectedIndex]
							.ChangeTransitionColor(previousSelectedTrans.second, sf::Color::White);
						currentSelectedTrans = trans;
						states[i].ChangeTransitionColor(k, sf::Color::Yellow);
					}
				}
				catch (const std::exception& e) {
					std::cout << e.what(); // information from error printed
				}

				return 0;
			}
		}
	}
	return -2; // didn't find anything
}

void DFA::SetTransitionSymbol(char symbol) {
	if (currentSelectedTrans.first != -1 && currentSelectedTrans.second != -1) {
		
		int stateOfTransition = FindStateIndexById(currentSelectedTrans.first);

		for (int i = 0; i < states[stateOfTransition].GetTransitionObjects().size(); i++) {

			if (currentSelectedTrans.second == i) {
				continue; // skip the same transition
			}

			if (states[stateOfTransition]
				.GetTransitionObjects()[i]
				.CheckSymbolExists(symbol)) 
			{
				// symbol already exists on this transition
				return;
			}
		}

		bool inAlphabet = false;

		for (int i = 0; i < alphabet.size(); i++) {
			if (alphabet[i] == symbol) {
				inAlphabet = true;
				break;
			}
		}

		if (!inAlphabet) { // skip adding to if already in alphabet
			alphabet.push_back(symbol);
		}

		states[stateOfTransition].SetTransitionSymbol(currentSelectedTrans.second, symbol);
	}
}

void DFA::DeSelectState() {
	selectedState = -1;
}

void DFA::DeSelectTransition() {
	if (currentSelectedTrans.first != -1 && currentSelectedTrans.second != -1) {

		int stateIndex = FindStateIndexById(currentSelectedTrans.first);

		states[stateIndex].ChangeTransitionColor(currentSelectedTrans.second,sf::Color::White);
		currentSelectedTrans.first = -1;
		currentSelectedTrans.second = -1;
		previousSelectedTrans.first = -1;
		previousSelectedTrans.second = -1;
	}
}

bool DFA::DeleteSelectedTransition() {
	// Firstly, make sure the transition actually exists
	if (currentSelectedTrans.first != -1 && currentSelectedTrans.second != -1) {
		int selectedStateIndex = FindStateIndexById(currentSelectedTrans.first);
		int to = states[selectedStateIndex].GetStateTransition(currentSelectedTrans.second).GetTransitionTo();
		return DeleteTransitionById(currentSelectedTrans.first, to);
	}
	return false;
}

bool DFA::DeleteState(int selectedState) {
	try {
		int selectedStateIndex = FindStateIndexById(selectedState);

		std::vector<int> incomingTransitions = states[selectedStateIndex].GetIncomingTransitions();
		std::vector<int> outgoingTransitions = states[selectedStateIndex].GetOutgoingTransitions();

		// Strip the state of all transitions and finally, delete the state itself

		for (int i = 0; i < incomingTransitions.size(); i++) {
			DeleteTransitionById(incomingTransitions[i], selectedState);
		}

		// The only problem is here:
		// for self loops, the incoming and outgoing are the same.

		for (int i = 0; i < outgoingTransitions.size(); i++) {
			DeleteTransitionById(selectedState, outgoingTransitions[i]);
		}

		states.erase(states.begin() + selectedStateIndex);

		if (states.size()) {
			// if not last state, we make another state accepting
			states[0].SetIsStarting(true);
			states[0].ConfigureStartingOutline();
		}

		this->selectedState = -1;
		return true;
	}
	catch (const std::exception& e) {
		std::cout << e.what();
		return false;
	}
}

void DFA::ChangeStateAccepting(int selectedState) {
	int selectedStateIndex = FindStateIndexById(selectedState);
	states[selectedStateIndex].SetIsAccepting();
}

std::pair<bool, DfaState> DFA::CheckStringAcceptingRecurs(DfaState currentState, std::string input, size_t position) {
	// Base case: reached the end of the input string
	if (position == input.length()) {
		return std::make_pair(currentState.GetIsAccepting(), currentState);
	}

	// Get the current character from the input string
	char currentChar = input[position];

	// Check if there is a transition for the current character
	int nextStateId = currentState.GetTransitionTo(currentChar);

	if (nextStateId == -1) {
		// No transition for the current character, string not accepted
		return std::make_pair(false, currentState);
	}

	int nextStateIndex = FindStateIndexById(nextStateId);

	DfaState nextState = states[nextStateIndex];

	return CheckStringAcceptingRecurs(nextState, input, position + 1);
}

bool DFA::CheckIfStringAccepted(std::string input) {
	// Check if there are any states in the DFA
	if (states.empty()) {
		return false;
	}

	// Get the starting state
	DfaState startingState = states[startingStateIndex];

	// Call the recursive helper function
	auto result = CheckStringAcceptingRecurs(startingState, input, 0);

	// Temporary solution: using the state's position as a unique id
	for (int i = 0; i < states.size(); i++) {
		if (states[i].GetStatePosition() == result.second.GetStatePosition()) {
			// Found the final state, make its color change depending on the result
			if (result.first) { // string was accepted
				states[i].SetStringAcceptedColor();
			}
			else {
				states[i].SetStringDeclinedColor();
			}
		}
	}

	return result.first;
}
bool DFA::RemoveSymbolFromTransition() {
	if (currentSelectedTrans.first != -1 && currentSelectedTrans.second != -1) {

		int selectedStateIndex = FindStateIndexById(currentSelectedTrans.first);

		int symbolsSize = states[selectedStateIndex].GetTransitionObjects()[currentSelectedTrans.second].GetTransitionSymbols().size();

		if (symbolsSize == 1) { // we are about to delete the transiton
			return DeleteSelectedTransition();
		}

		char deletedCharRef = states[selectedStateIndex].GetTransitionObjects()[currentSelectedTrans.second].GetTransitionSymbols()[symbolsSize - 1];

		// deletedCharRef is to know what symbol to remove from alphabet (if needed)
		// if no other transition contains the symbol, we remove it

		std::pair<bool,int> result = states[selectedStateIndex].DeleteSingleTransitionSymbol(currentSelectedTrans.second);
		bool symbolExistsAfterRemoval = false;

		if (result.first) {
			if (result.second == 0) { // symbol deleted
				if (states[selectedStateIndex].GetTransitionObjects().size() > 0) {
					if (states[selectedStateIndex].GetTransitionObjects()[currentSelectedTrans.second].GetTransitionSymbols().size() > 0) {

						RemoveSingleCharacterFromAlphabet(states, currentSelectedTrans, deletedCharRef,
							symbolExistsAfterRemoval, alphabet);

						DeSelectTransition();
						return result.first;
					}
				}
			}
		}

		currentSelectedTrans.first = -1;
		currentSelectedTrans.second = -1;
		previousSelectedTrans.first = -1;
		previousSelectedTrans.second = -1;


		return result.first;
	}
	return false;
}

bool DFA::SetAllStatesDefaultColor() {

	if (states.size() < 0) {
		return false;
	}

	for (int i = 0; i < states.size(); i++) {
		states[i].SetDefaultColor();
	}
}

void RemoveSingleCharacterFromAlphabet(std::vector<DfaState>& states,
	std::pair<int,int>& currentSelectedTrans, char removedChar, bool& symbolExistsAfterRemoval,
	std::vector<char>& alphabet) {

	symbolExistsAfterRemoval = false;

	for (int i = 0; i < states.size(); i++) {
		if (states[i].CheckTransitionExists(removedChar)) {
			symbolExistsAfterRemoval = true;
			break;
		}
	}

	if (!symbolExistsAfterRemoval) {
		for (int i = 0; i < alphabet.size(); i++) {
			if (alphabet[i] == removedChar) {
				alphabet.erase(alphabet.begin() + i);
				break;
			}
		}
	}
}

bool DFA::DeleteTransition(int state, int transIndex) {

	int stateIndex = FindStateIndexById(state);

	// Firstly, make sure the transition actually exists
	int to = states[stateIndex].GetStateTransition(transIndex).GetTransitionTo();

	int toIndex = FindStateIndexById(to);

	// using to, delete it from its incoming array
	states[toIndex].DeleteIncomingTransition(state);

	std::vector<char> transitionSymbolsDeleted = states[stateIndex]
		.GetTransition(transIndex).GetTransitionSymbols();

	// delete the transition here
	bool result = states[stateIndex].DeleteTransition(transIndex);

	// After deletion, check whether the symbols are still contained in some transition
	for (int i = 0; i < transitionSymbolsDeleted.size(); ++i) {
		for (int k = 0; k < states.size(); ++k) {
			if (states[k].CheckTransitionExists(transitionSymbolsDeleted[i])) {
				// In-place solution for symbols that need removal:
				// Pop out the symbols that do exist
				transitionSymbolsDeleted.erase(transitionSymbolsDeleted.begin() + i);
				// Since its in place, we reset the counter 
				i = -1;
				break;
			}

		}
	}

	for (int i = 0; i < transitionSymbolsDeleted.size(); i++) {
		for (int k = 0; k < alphabet.size(); k++) {
			if (alphabet[k] == transitionSymbolsDeleted[i]) {
				alphabet.erase(alphabet.begin() + k);
				break;
			}
		}
	}

	currentSelectedTrans.first = -1;
	currentSelectedTrans.second = -1;
	previousSelectedTrans.first = -1;
	previousSelectedTrans.second = -1;

	return result;
}

bool DFA::DeleteTransitionById(int stateFrom, int stateTo) {

	int stateFromIndex = FindStateIndexById(stateFrom);
	int stateToIndex = FindStateIndexById(stateTo);

	states[stateToIndex].DeleteIncomingTransition(stateFrom);
	states[stateFromIndex].DeleteOutgoingTransition(stateTo);

	std::pair<int, StateTransition> transition = states[stateFromIndex]
		.GetTransitionByStateTo(stateTo);

	std::vector<char> transitionSymbolsDeleted = transition.second.GetTransitionSymbols();

	bool result = states[stateFromIndex].DeleteTransition(transition.first);

	// After deletion, check whether the symbols are still contained in some transition
	for (int i = 0; i < transitionSymbolsDeleted.size(); ++i) {
		for (int k = 0; k < states.size(); ++k) {
			if (states[k].CheckTransitionExists(transitionSymbolsDeleted[i])) {
				// In-place solution for symbols that need removal:
				// Pop out the symbols that do exist
				transitionSymbolsDeleted.erase(transitionSymbolsDeleted.begin() + i);
				// Since its in place, we reset the counter 
				i = -1;
				break;
			}

		}
	}

	for (int i = 0; i < transitionSymbolsDeleted.size(); i++) {
		for (int k = 0; k < alphabet.size(); k++) {
			if (alphabet[k] == transitionSymbolsDeleted[i]) {
				alphabet.erase(alphabet.begin() + k);
				break;
			}
		}
	}

	currentSelectedTrans.first = -1;
	currentSelectedTrans.second = -1;
	previousSelectedTrans.first = -1;
	previousSelectedTrans.second = -1;

	return result;

}


int DFA::CheckStateTransitionCollision(sf::Vector2f position) {
	for (int i = 0; i < states.size(); i++) {
		for (int k = 0; k < states[i].GetTransitionObjects().size(); k++) {

			if (states[i].GetTransitionObjects()[k].GetMainArrow().CheckCollision(position)
				|| states[i].GetTransitionObjects()[k].GetRhsArrow().CheckCollision(position)
				|| states[i].GetTransitionObjects()[k].GetLhsArrow().CheckCollision(position)
				|| states[i].GetTransitionObjects()[k].GetTopArrow().CheckCollision(position)) {
				return 0;
			}
		}
	}
	return -2; // didn't find anything
}

std::tuple<bool, int, std::string> DFA::CheckIfDfa() {

	/*
		There are a couple of conditions that need to be checked
		1) Does every state have a label
		2) Is every symbol from the alphabet contained within every state
		3) Is there only one transition with a given symbol per state
	*/ 

	/*
		structure:
		bool: yes, no for dfa
		int: if no, which state
		string: if no, what problem {
			0: No state label
			1: Missing symbols from alphabet
			2: More than one transition for the same symbol
			2 is handled implicitly, the rest must be checked
		}
	*/
	std::tuple<bool, int, std::string> result;
	std::get<1>(result) = -1;
	std::get<2>(result) = "";

	// 1)
	for (int i = 0; i < states.size(); ++i) {
		if (states[i].GetStateLabel() == "") {
			std::get<0>(result) = false;
			std::get<1>(result) = i;
			std::get<2>(result) = "Missing state label";
			return result;
		}
	}

	// 2)
	for (int i = 0; i < states.size(); i++) {
		for (int k = 0; k < alphabet.size(); k++) {
			if (!states[i].CheckTransitionExists(alphabet[k])) {
				std::get<0>(result) = false;
				std::get<1>(result) = i;

				std::string errorMessage = "State: " + states[i].GetStateLabel() + " missing transition: " + alphabet[k];

				std::get<2>(result) = errorMessage;
				return result;
			}
		}
	}
	std::get<0>(result) = true;
	return result;
}

int DFA::FindStateIndexById(int id) {
	for (int i = 0; i < states.size(); i++) {
		if (states[i].GetStateId() == id) {
			return i;
		}
	}
}

void DFA::ChangeStateTransitionDirection(TransitionDirection direction) {
	if (currentSelectedTrans.first == -1 || currentSelectedTrans.second == -1) {
		return;
	}

	int stateIndex = FindStateIndexById(currentSelectedTrans.first);
	states[stateIndex].ChangeStateTransitionDirection(currentSelectedTrans.second, direction);
}

// TODO: Finish move state position
void DFA::MoveStatePosition(sf::Vector2f mousePosition, int selectedState) {
	int selectedStateIndex = FindStateIndexById(selectedState);
	states[selectedStateIndex].MoveStatePosition(mousePosition);
	
	for (int i = 0; i < states[selectedStateIndex].GetIncomingTransitions().size(); i++) {

		// self-loop
		if (states[selectedStateIndex].GetIncomingTransitions()[i] == selectedState) {
			states[selectedStateIndex].MoveStateTransitionPositionSelfLoop(
				states[selectedStateIndex].GetStateTransitionByTransitionTo(selectedState)
			);
		}
		else {
			int indexFrom = FindStateIndexById(states[selectedStateIndex].GetIncomingTransitions()[i]);
			states[indexFrom].MoveStateTransitionPositionRegular(states[selectedStateIndex].GetStateCenter(),
				states[indexFrom].GetStateTransitionByTransitionTo(selectedState));
		}
	}

	for (int i = 0; i < states[selectedStateIndex].GetOutgoingTransitions().size(); i++) {
		int indexTo = FindStateIndexById(states[selectedStateIndex].GetOutgoingTransitions()[i]);

		states[selectedStateIndex].MoveStateTransitionPositionRegular(states[indexTo].GetStateCenter(),
			states[selectedStateIndex].GetStateTransitionByTransitionTo(states[selectedStateIndex]
				.GetOutgoingTransitions()[i]));
	}	
}
