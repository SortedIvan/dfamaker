#include "dfa_state.hpp"

const int DEFAULT_STATE_RADIUS = 40;
const int DEFAULT_STATE_SELECTED_RADIUS = 35;

const sf::Color DEFAULT_STATE_COLOR = sf::Color::White;

DfaState::DfaState(std::string label, sf::Vector2f statePosition, sf::Font& font) {
	this->label = label;
	this->statePosition = statePosition;
	this->stateCircle.setRadius(DEFAULT_STATE_RADIUS);
	this->stateCircle.setOrigin(sf::Vector2f(DEFAULT_STATE_RADIUS, DEFAULT_STATE_RADIUS));
	this->stateCircle.setPosition(statePosition);
	this->stateCircle.setFillColor(DEFAULT_STATE_COLOR);

	this->outlining.setPosition(statePosition);
	this->outlining.setRadius(DEFAULT_STATE_SELECTED_RADIUS);
	this->outlining.setOrigin(sf::Vector2f(DEFAULT_STATE_SELECTED_RADIUS, DEFAULT_STATE_SELECTED_RADIUS));
	this->outlining.setOutlineThickness(2.0f);
	this->outlining.setOutlineColor(sf::Color::Red);
	this->outlining.setFillColor(sf::Color::Transparent);

	this->textLabel.setFont(font);
	this->textLabel.setString(label);
	this->textLabel.setOrigin(sf::Vector2f(DEFAULT_STATE_RADIUS / 2 - 5, DEFAULT_STATE_RADIUS / 2)); // Center it in the circle
	this->textLabel.setPosition(statePosition);
	this->textLabel.setCharacterSize(20);
	this->textLabel.setFillColor(sf::Color::Black);

	this->stateCenter = statePosition;


}

DfaState::DfaState() {

}

void DfaState::Draw(sf::RenderWindow& window, bool isSelected) {
	window.draw(stateCircle); // draw the state first
	window.draw(textLabel); // draw the state label

	if (isSelected) {
		window.draw(outlining);
	}

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
	this->textLabel.setString(label);
}

void DfaState::ChangeStateTransition(char oldTransitionChar, char newTransitionChar) {
	if (oldTransitionChar == newTransitionChar) {
		return;
	}

	auto oldTransition = transitions.find(oldTransitionChar);

	if (oldTransition != transitions.end()) { // The transition exists
		StateTransition oldTransitionValue = transitions.at(oldTransitionChar);
		RemoveStateTransition(oldTransitionChar);
		//AddStateTransition(newTransitionChar, oldTransitionValue); TO DO
	}
}

std::string DfaState::GetStateLabel() {
	return this->label;
}

std::map<char, StateTransition> DfaState::GetStateTransitions() {
	return this->transitions;
}

bool DfaState::GetIsAccepting() {
	return this->isAccepting;
}

bool DfaState::GetIsStarting() {
	return this->isStarting;
}

sf::Vector2f DfaState::GetStatePosition() {
	return this->statePosition;
}

sf::CircleShape DfaState::GetStateCircle() {
	return this->stateCircle;
}

sf::Text& DfaState::GetTextLabelRef() {
	return textLabel;
}

sf::Vector2f DfaState::GetStateCenter() {
	return stateCenter;
}

void DfaState::DrawStateArrows(sf::RenderWindow& window) {
	for (int i = 0; i < transitions.size(); i++) {
		transitions.at(i).Draw(window);
	}
}

bool DfaState::AddStateTransition(sf::Vector2f stateFrom, sf::Vector2f stateTo, float stateFromRadius, float stateToRadius, int stateToValue) {
	//StateTransition transition(stateFromObj, stateToObj, toState);
	//
	//if (transitions.count(transitionChar)) {
	//	return false; // transition already exists with this char
	//}

	//transitions.insert({ transitionChar, transition });

	StateTransition transition;

	transition.SetUpStateTransition(stateFrom, stateTo, stateFromRadius, stateToRadius, stateToValue);

	return true;
}
