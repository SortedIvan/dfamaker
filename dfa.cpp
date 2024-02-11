#include "dfa.hpp";
#include <iostream>

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

bool DFA::CheckIfStateSelected(sf::Vector2f positionClicked) {
	for (int i = 0; i < states.size(); i++) {
		if (states[i].GetStateCircle().getGlobalBounds().contains(positionClicked)) {
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

	bool transitionIsAssigned = false;
	int stateFrom = 0;

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

bool DFA::DeleteTransition() {
	// Firstly, make sure the transition actually exists
	if (currentSelectedTrans.first != -1 && currentSelectedTrans.second != -1) {
		
		int to = states[currentSelectedTrans.first].GetStateTransition(currentSelectedTrans.second).GetTransitionTo();
		// using to, delete it from its incoming array
		states[to].DeleteIncomingTransition(currentSelectedTrans.first);

		// delete the transition here
		bool result = states[currentSelectedTrans.first].DeleteTransition(
			currentSelectedTrans.second
		);

		currentSelectedTrans.first = -1;
		currentSelectedTrans.second = -1;
		previousSelectedTrans.first = -1;
		previousSelectedTrans.second = -1;

		return result;
	}
	return false;
}

// Terrible solution, n^2 complexity but best that can be done with current implementation
bool DFA::DeleteState(int selectedState) {
	// Problem: Can't delete the state right away. 
	// have to check whether the state has incoming transitions from other states
	
	// Get all incoming transitions
	std::vector<int> incomingTransitions = states[selectedState].GetIncomingTransitions();

	// for all incoming transitions,
	for (int i = 0; i < incomingTransitions.size(); i++) {
		for (int k = 0; k < states[incomingTransitions[i]].GetTransitionObjects().size(); k++) {
			if (states[incomingTransitions[i]].GetTransitionObjects()[k].GetTransitionTo() == selectedState) {
				states[incomingTransitions[i]].DeleteTransition(k); // Remove the incoming transition
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
		states[0].SetAcceptingStateManually(true);
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

	return result.first;
}
bool DFA::RemoveSymbolFromTransition() {
	if (currentSelectedTrans.first != -1 && currentSelectedTrans.second != -1) {
		bool result = states[currentSelectedTrans.first].DeleteSingleTransitionSymbol(currentSelectedTrans.second);

		if (result) {
			if (states[currentSelectedTrans.first].GetTransitionObjects().size() > 0) {
				if (states[currentSelectedTrans.first].GetTransitionObjects()[currentSelectedTrans.second].GetTransitionSymbols().size() > 0) {
					DeSelectTransition();

					return result;
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