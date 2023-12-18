#include "dfa.hpp";


std::vector<DfaState> DFA::GetStates() {
	return this->states;
}
std::vector<char> DFA::GetAlphabet() {
	return this->alphabet;
}

DFA::DFA() {
	
}

void DFA::AddNewState(std::string label, sf::Vector2f position) {

	// By default, if its the first state added, make it starting
	DfaState state(label, position);

	if (!states.size()) {
		state.SetIsAccepting(true);
	}

	this->states.push_back(state);
}
void SetAlphabet(std::vector<char> alphabet) {

}

void DFA::DrawAllStates(sf::RenderWindow& window) {
	for (int i = 0; i < states.size(); i++) {
		window.draw(states[i].GetStateCircle());
	}
}