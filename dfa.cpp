#include "dfa.hpp";
#include <iostream>


std::vector<DfaState> DFA::GetStates() {
	return this->states;
}
std::vector<char> DFA::GetAlphabet() {
	return this->alphabet;
}

DFA::DFA() {
	
}

void DFA::AddNewState(std::string label, sf::Vector2f position, sf::Font& font) {

	// By default, if its the first state added, make it starting
	DfaState state(label, position, font);

	if (!states.size()) {
		state.SetIsAccepting(true);
	}

	this->states.push_back(state);
}

void SetAlphabet(std::vector<char> alphabet) {

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

bool DFA::AddNewTransition(int stateFrom, int stateTo, int id) {

	DfaState stateToObj = states[stateTo];

	// Problem: when adding a transition, make it so that it is not assigned a character somehow
	// One potential solution:
	// make it so that when you add a transition, it only adds a blank transition
	// then, if the user clicks on it and adds a character, only then add an entry into the
	// transition map and have it assigned to that particular transition

	StateTransition transition = states[stateFrom].AddStateTransition(states[stateFrom].GetStatePosition(),
		stateToObj.GetStatePosition(), states[stateFrom].GetStateCircle().getRadius(),
		stateToObj.GetStateCircle().getRadius(), stateTo, stateFrom, id);
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
			if (states[i].GetTransitionObjects()[k].GetMainArrow().CheckCollision(positionClicked)) {
				std::cout << "bomboclaat";
				return 0;
			}
		}
	}


	


	return -2; // didn't find anything
}
