#include "dfa_state.hpp"

DfaState::DfaState(std::string label, sf::Vector2f statePosition, sf::CircleShape stateCircle, bool isStarting) {
	this->label = label;
	this->statePosition = statePosition;
	this->stateCircle = stateCircle;
	this->isStarting = isStarting;
}

bool DfaState::AddStateTransition(char transitionChar, int toState) {
	std::pair<char, int> transition;
	transition.first = transitionChar;
	transition.second = toState;

	if (transitions.count(transitionChar)) { // transition already exists with this char
		return false;
	}

	this->transitions.insert(transition);
	return true;
}

void DfaState::SetIsAccepting(bool isAccepting) {
	this->isAccepting = isAccepting;
}

void DfaState::RemoveStateTransition(char transitionChar) {
	transitions.erase(transitionChar);
}

void DfaState::SetStateColor(sf::Color color) {
	this->stateCircle.setFillColor(color);
}

void DfaState::SetIsStarting(bool isStarting) {
	this->isStarting = isStarting;
}

void DfaState::SetStateLabel(std::string label) {
	this->label = label;
}

void DfaState::ChangeStateTransition(char oldTransitionChar, char newTransitionChar) {
	if (oldTransitionChar == newTransitionChar) {
		return;
	}

	auto oldTransition = transitions.find(oldTransitionChar);

	if (oldTransition != transitions.end()) { // The transition exists
		int oldTransitionValue = transitions.at(oldTransitionChar);
		RemoveStateTransition(oldTransitionChar);
		AddStateTransition(newTransitionChar, oldTransitionValue);
	}
}