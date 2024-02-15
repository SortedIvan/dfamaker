#include "dfa.hpp";
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

void DFA::AddNewState(std::string label, sf::Vector2f position, sf::Font& font) {

	// By default, if its the first state added, make it starting
	DfaState state(label, position, font);

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

		if (selectedState == i) {
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

void DFA::ChangeStateLabelText(sf::Event& event, int selectedState) {
	std::string currentLabel = states[selectedState].GetStateLabel();
	states[selectedState].SetStateLabel(currentLabel += event.text.unicode);
}

void DFA::DeleteStateLabelCharacter(int selectedState) {
	if (states[selectedState].GetStateLabel().size() != 0) {
		std::string currentLabel = states[selectedState].GetStateLabel();
		currentLabel.resize(currentLabel.size() - 1);
		states[selectedState].SetStateLabel(currentLabel);
	}
}

bool DFA::AddNewTransition(int stateFrom, int stateTo, int id, sf::Font& font) {

	DfaState stateToObj = states[stateTo];

	if (states[stateFrom].CheckTransitionExists(stateFrom, stateTo)) {
		// transition already exists
		return false;
	}


	StateTransition transition = states[stateFrom].AddStateTransition(states[stateFrom].GetStatePosition(),
		stateToObj.GetStatePosition(), states[stateFrom].GetStateCircle().getRadius(),
		stateToObj.GetStateCircle().getRadius(), stateTo, stateFrom, id, font);

	states[stateTo].AddIncomingTransition(stateFrom);

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
				
				std::pair<int, int> trans = std::make_pair(i, k);

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
						states[previousSelectedTrans.first]
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
		
		int stateOfTransition = currentSelectedTrans.first;

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
		states[currentSelectedTrans.first].ChangeTransitionColor(currentSelectedTrans.second,sf::Color::White);
		currentSelectedTrans.first = -1;
		currentSelectedTrans.second = -1;
		previousSelectedTrans.first = -1;
		previousSelectedTrans.second = -1;
	}
}

bool DFA::DeleteSelectedTransition() {
	// Firstly, make sure the transition actually exists
	if (currentSelectedTrans.first != -1 && currentSelectedTrans.second != -1) {
		
		int to = states[currentSelectedTrans.first].GetStateTransition(currentSelectedTrans.second).GetTransitionTo();
		// using to, delete it from its incoming array
		states[to].DeleteIncomingTransition(currentSelectedTrans.first);

		std::vector<char> transitionSymbolsDeleted = states[currentSelectedTrans.first]
			.GetTransition(currentSelectedTrans.second).GetTransitionSymbols();
		
		// delete the transition here
		bool result = states[currentSelectedTrans.first].DeleteTransition(
			currentSelectedTrans.second
		);

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
	return false;
}

// O(n^3) complexity but best that can be done with current implementation
// Problem: when deleted, GetStateTo() for all other states gets messed up
bool DFA::DeleteState(int selectedState) {

	if (states.size() == 1) { // if this is the last state
		states.erase(states.begin() + selectedState);
		this->selectedState = -1;
		return true;
	}

	// Get all the symbols that reside in all transitions of this state
	std::set<char> allSymbolsAffected;
	std::set<char> toKeep;

	for (int i = 0; i < states[selectedState].GetTransitionObjects().size(); i++) {
		for (int k = 0; k < states[selectedState].GetTransitionObjects()[i].GetTransitionSymbols().size(); k++) {
			allSymbolsAffected.insert(
				states[selectedState].GetTransitionObjects()[i].GetTransitionSymbols()[k]
			);
		}
	}

	for (int i = 0; i < states.size(); i++) {
		for (int k = 0; k < states[i].GetTransitionObjects().size(); k++) {
			int to = states[i].GetTransitionObjects()[k].GetTransitionTo();
			std::cout << to << std::endl;
		}
	}

	std::cout << "-----------" << std::endl;
	// We have to update the stateTo variable in StateTransition
	for (int i = 0; i < states.size(); i++) {
		for (int k = 0; k < states[i].GetTransitionObjects().size(); k++) {

			if (states[i].GetTransitionObjects()[k].GetTransitionTo() == 0) {
				continue;
			}

			int to = states[i].GetTransitionObjects()[k].GetTransitionTo() - 1;


			//std::cout << to << std::endl;

			states[i].SetTransitionTo(k, to);
		}
	}
	std::cout << "-----------" << std::endl;
	for (int i = 0; i < states.size(); i++) {
		for (int k = 0; k < states[i].GetTransitionObjects().size(); k++) {
			int to = states[i].GetTransitionObjects()[k].GetTransitionTo();
			std::cout << to << std::endl;
		}
	}


	// Furthermore, we need to also get the symbols from the affected incoming transitions
	// Delete all outgoing and incoming transitions
	// Outgoing transition deletion is automatically handled by deleting the state object
	std::vector<int> incomingTransitions = states[selectedState].GetIncomingTransitions();
	
	for (int i = 0; i < incomingTransitions.size(); i++) {
		for (int k = 0; k < states[incomingTransitions[i]].GetTransitionObjects().size(); k++) {
			for (int j = 0; j < states[incomingTransitions[i]].GetTransitionObjects()[k].GetTransitionSymbols().size(); j++) {
				allSymbolsAffected.insert(states[incomingTransitions[i]].GetTransitionObjects()[k].GetTransitionSymbols()[j]);
			}
		}
	}

	// We need to go to all states contained in incoming transitions and delete their transition to selectedState
	for (int i = 0; i < incomingTransitions.size(); i++) {
		states[incomingTransitions[i]].DeleteTransitionTo(selectedState);
	}

	// If no other states contain the symbols, remove them from the alphabet
	for (auto& element : allSymbolsAffected)
	{

		for (int i = 0; i < states.size(); i++) {

			if (i == selectedState) {
				continue;
			}

			if (states[i].CheckTransitionExists(element)) {
				
				bool toCorrectTransition = false;

				// We need to check whether the transition here is to the state we are deleting
				for (int k = 0; k < states[i].GetTransitionObjects().size(); k++) {
					if (states[i].GetTransitionObjects()[k].GetTransitionTo() == selectedState) {
						continue; // skip the bad transition
					}

					if (states[i].GetTransitionObjects()[k].CheckSymbolExists(element)) {
						toCorrectTransition = true;
					}
				}
				
				if (toCorrectTransition) {
					toKeep.insert(element);
				}

				break;
			}
		}
	}

	for (auto& element : allSymbolsAffected)
	{
		if (toKeep.find(element) != toKeep.end()) {
			continue;
		}

		for (int k = 0; k < alphabet.size(); k++) {
			if (alphabet[k] == element) {
				alphabet.erase(alphabet.begin() + k);
				break;
			}
		}
	}


	// Remove this state from all other states's incoming transitions list
	for (int i = 0; i < states.size(); i++) {
		// skip the state we are removing
		if (i == selectedState) {
			continue;
		}
		for (int k = 0; k < states[i].GetIncomingTransitions().size(); k++) {

			// Otherwise, check the transitions
			if (states[i].GetIncomingTransitions()[k] == selectedState) {
				states[i].DeleteIncomingTransition(selectedState);
			}

		}
	}

	// Finally, delete the actual state itself
	bool wasStarting = states[selectedState].GetIsStarting();

	states.erase(states.begin() + selectedState);
	this->selectedState = -1;

	// We also need to make the next available state (if there is one) starting
	if (states.size() > 0 && wasStarting) {
		states[0].SetIsStarting(true);
		return true;
	}

}

void DFA::ChangeStateAccepting(int selectedState) {
	states[selectedState].SetIsAccepting();
}

std::pair<bool, DfaState> DFA::CheckStringAcceptingRecurs(DfaState currentState, std::string input, size_t position) {
	// Base case: reached the end of the input string
	if (position == input.length()) {
		return std::make_pair(currentState.GetIsAccepting(), currentState);
	}

	// Get the current character from the input string
	char currentChar = input[position];

	// Check if there is a transition for the current character
	int nextStateIndex = currentState.GetTransitionTo(currentChar);

	if (nextStateIndex == -1) {
		// No transition for the current character, string not accepted
		return std::make_pair(false, currentState);
	}

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
		int symbolsSize = states[currentSelectedTrans.first].GetTransitionObjects()[currentSelectedTrans.second].GetTransitionSymbols().size();

		if (symbolsSize == 1) { // we are about to delete the transiton
			return DeleteSelectedTransition();
		}

		char deletedCharRef = states[currentSelectedTrans.first].GetTransitionObjects()[currentSelectedTrans.second].GetTransitionSymbols()[symbolsSize - 1];

		// deletedCharRef is to know what symbol to remove from alphabet (if needed)
		// if no other transition contains the symbol, we remove it

		std::pair<bool,int> result = states[currentSelectedTrans.first].DeleteSingleTransitionSymbol(currentSelectedTrans.second);
		bool symbolExistsAfterRemoval = false;

		std::cout << result.first;
		std::cout << result.second;

		if (result.first) {
			if (result.second == 0) { // symbol deleted
				if (states[currentSelectedTrans.first].GetTransitionObjects().size() > 0) {
					if (states[currentSelectedTrans.first].GetTransitionObjects()[currentSelectedTrans.second].GetTransitionSymbols().size() > 0) {

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
	// Firstly, make sure the transition actually exists
	int to = states[state].GetStateTransition(transIndex).GetTransitionTo();
	// using to, delete it from its incoming array
	states[to].DeleteIncomingTransition(state);

	std::vector<char> transitionSymbolsDeleted = states[state]
		.GetTransition(transIndex).GetTransitionSymbols();

	// delete the transition here
	bool result = states[state].DeleteTransition(transIndex);

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