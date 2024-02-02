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

bool DFA::AddNewTransition(int stateFrom, int stateTo) {

	DfaState stateToObj = states[stateTo];

	// Problem: when adding a transition, make it so that it is not assigned a character somehow
	// One potential solution:
	// make it so that when you add a transition, it only adds a blank transition
	// then, if the user clicks on it and adds a character, only then add an entry into the
	// transition map and have it assigned to that particular transition

	states[stateFrom].AddStateTransition(states[stateFrom].GetStatePosition(),
		stateToObj.GetStatePosition(), states[stateFrom].GetStateCircle().getRadius(),
		stateToObj.GetStateCircle().getRadius(), stateTo);



	return true;
}

void DFA::SetSelectedState(int selectedState) {
	this->selectedState = selectedState;
}