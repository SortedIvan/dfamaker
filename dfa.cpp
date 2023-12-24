#include "dfa.hpp";


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

void DFA::DrawAllStates(sf::RenderWindow& window) {
	for (int i = 0; i < states.size(); i++) {
		states[i].Draw(window);
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

bool DFA::AddNewTransition(int stateA, int stateB) {
	sf::Vector2f fromPosition = states[stateA].GetStatePosition();
	sf::Vector2f toPosition = states[stateA].GetStatePosition();

	sf::Vector2f directionVector = fromPosition - toPosition;

	return false;
}