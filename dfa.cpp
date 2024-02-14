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

		states[currentSelectedTrans.first].SetTransitionSymbol(currentSelectedTrans.second, symbol);
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
		
		std::cout << alphabet.size();
		
		currentSelectedTrans.first = -1;
		currentSelectedTrans.second = -1;
		previousSelectedTrans.first = -1;
		previousSelectedTrans.second = -1;

		return result;
	}
	return false;
}

// O(n^2) complexity but best that can be done with current implementation
bool DFA::DeleteState(int selectedState) {

	// Get all incoming transitions
	std::vector<int> incomingTransitions = states[selectedState].GetIncomingTransitions();
	std::vector<int> outgoingTransitions;

	for (int i = 0; i < states[selectedState].GetTransitionObjects().size(); i++) {
		outgoingTransitions.push_back(
			states[selectedState].GetTransitionObjects()[i].GetTransitionTo());
	}

	// Remove all the state from all of the states it had an outgoing transition to
	for (int i = 0; i < outgoingTransitions.size(); i++) {
		states[outgoingTransitions[i]].DeleteIncomingTransition(selectedState);
	}

	//-- This has to be re-done
	// Currently, both a set and a vector are used as 
	// There might be multiple transitions with the same symbol
	// Thus trading memory for performance
	std::set<char> symbolsToCheckIfRemoval;
	std::vector<char> hasToBeDeleted;
	// Check if there are any symbols that need to be removed 
	for (int i = 0; i < states[selectedState].GetTransitionObjects().size(); i++) {
		for (int k = 0; k < states[selectedState].GetTransitionObjects()[i].GetTransitionSymbols().size(); k++) {
			symbolsToCheckIfRemoval.insert(states[selectedState].GetTransitionObjects()[i].GetTransitionSymbols()[k]);
		}
	}

	for (auto& element : symbolsToCheckIfRemoval)
	{
		for (int k = 0; k < states.size(); ++k) {
			if (states[k].CheckTransitionExists(element)) {
				hasToBeDeleted.push_back(element);
				break;
			}

		}
	}

	for (auto& element : symbolsToCheckIfRemoval)
	{
		for (int k = 0; k < alphabet.size(); k++) {
			if (alphabet[k] == element) {
				alphabet.erase(alphabet.begin() + k);
				break;
			}
		}
	}
	// -- 


	// for all incoming transitions,
	for (int i = 0; i < incomingTransitions.size(); i++) {
		for (int k = 0; k < states[incomingTransitions[i]].GetTransitionObjects().size(); k++) {
			if (states[incomingTransitions[i]].GetTransitionObjects()[k].GetTransitionTo() == selectedState) {
				//states[incomingTransitions[i]].DeleteTransition(k); // Remove the incoming transition
				DeleteTransition(incomingTransitions[i], k);
			}
		}
	}



	// Finally, delete the state itself and de-select
	// First, check if the state was accepting

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
