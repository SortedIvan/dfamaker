#include "dfa_state.hpp"
#include <iostream>

const int DEFAULT_STATE_RADIUS = 40;
const int DEFAULT_STATE_SELECTED_RADIUS = 25;
const int DEFAULT_STATE_ACCEPTING_RADIUS = 35;
const int STARTING_STATE_ARROW_LEN = 25.f;

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

	this->acceptingOutline.setPosition(statePosition);
	this->acceptingOutline.setRadius(DEFAULT_STATE_ACCEPTING_RADIUS);
	this->acceptingOutline.setOrigin(sf::Vector2f(DEFAULT_STATE_ACCEPTING_RADIUS, DEFAULT_STATE_ACCEPTING_RADIUS));
	this->acceptingOutline.setOutlineThickness(2.0f);
	this->acceptingOutline.setOutlineColor(sf::Color::Black);
	this->acceptingOutline.setFillColor(sf::Color::Transparent);

	this->textLabel.setFont(font);
	this->textLabel.setString(label);
	this->textLabel.setOrigin(sf::Vector2f(DEFAULT_STATE_RADIUS / 2 - 5, DEFAULT_STATE_RADIUS / 2)); // Center it in the circle
	this->textLabel.setPosition(statePosition);
	this->textLabel.setCharacterSize(20);
	this->textLabel.setFillColor(sf::Color::Black);

	this->stateCenter = statePosition;

	// <---- starting state graphics logic --->
	sf::Vector2f dirVector(1, 0);

	sf::Vector2f startingStatePointRef(statePosition.x - DEFAULT_STATE_RADIUS, statePosition.y);

	this->startingOutline.mainArrow.setLinePoints(
		sf::Vector2f(statePosition.x - DEFAULT_STATE_RADIUS - STARTING_STATE_ARROW_LEN, statePosition.y),
		startingStatePointRef);

	sf::Transform rotationTransform;
	rotationTransform.rotate(45.f);

	// Rotate the vector left (counter-clockwise)
	sf::Vector2f rotatedLeft = rotationTransform.transformPoint(-dirVector);

	// Reset the rotation matrix
	rotationTransform = sf::Transform();

	// Rotate the vector right (clockwise)
	rotationTransform.rotate(-45.f); // negative angle for clockwise rotation
	sf::Vector2f rotatedRight = rotationTransform.transformPoint(-dirVector);

	this->startingOutline.arrowTipOne.setLinePoints(startingStatePointRef, rotatedLeft * 15.f + startingStatePointRef);
	this->startingOutline.arrowTipTwo.setLinePoints(startingStatePointRef, rotatedRight * 15.f + startingStatePointRef);
}

DfaState::DfaState() {

}

void DfaState::Draw(sf::RenderWindow& window, bool isSelected) {
	window.draw(stateCircle); // draw the state first
	window.draw(textLabel); // draw the state label

	if (isSelected) {
		window.draw(outlining);
	}

	if (isAccepting) {
		window.draw(acceptingOutline);
	}

	if (isStarting) {
		window.draw(startingOutline.arrowTipOne);
		window.draw(startingOutline.arrowTipTwo);
		window.draw(startingOutline.mainArrow);
	}
}

void DfaState::SetIsAccepting() {
	isAccepting = !isAccepting;
}

void DfaState::SetAcceptingStateManually(bool truthValue) {
	isAccepting = truthValue;
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


std::string DfaState::GetStateLabel() {
	return this->label;
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
	for (int i = 0; i < transitionObjects.size(); i++) {
		transitionObjects.at(i).Draw(window);
	}
}

StateTransition DfaState::AddStateTransition(sf::Vector2f stateFrom, sf::Vector2f stateTo,
	float stateFromRadius, float stateToRadius, int stateToValue, int stateFromValue, int id, sf::Font& font) {
	StateTransition transition;
	transition.SetUpStateTransition(stateFrom, stateTo, stateFromRadius, stateToRadius, stateToValue, stateFromValue, font);
	transition.SetIsAssigned(false);
	transition.SetTransitionId(id);
	transitionObjects.push_back(transition);

	return transition;
}

std::vector<StateTransition> DfaState::GetTransitionObjects() {
	return transitionObjects;
}

StateTransition DfaState::GetStateTransition(int index) {
	return transitionObjects[index];
}

void DfaState::ChangeTransitionColor(int transitionIndex, sf::Color color) {
	transitionObjects[transitionIndex].SetTransitionColor(color);
}

void DfaState::SetTransitionSymbol(int transitionIndex, char symbol) {

	if (transitionObjects[transitionIndex].GetTransitionSymbol())

	transitionObjects[transitionIndex].SetTransitionSymbol(symbol);
	int to = transitionObjects[transitionIndex].GetTransitionTo();

	if (transitions.find(symbol) != transitions.end()) {
		// the same symbol was added, skip adding it
	}

	transitions.insert({ symbol, to }); // first time adding the symbol

}

bool DfaState::DeleteTransition(int transitionIndex) {
	try {
		StateTransition ref = transitionObjects[transitionIndex];
		char symbol = ref.GetTransitionSymbol();
		transitionObjects.erase(transitionObjects.begin() + transitionIndex);

		// we also have to remove the actual transition (if there existed one)

		if (symbol == '~') { // special assigned character
			// the transition did not have a symbol assigned to it, continue;
			return true;
		}

		if (transitions.find(symbol) == transitions.end()) {
			// such a transition does not exist
			return true;
		}

		transitions.erase(symbol);

		return true;

	}
	catch (const std::exception& e) {
		return false;
	}
}

bool DfaState::AddIncomingTransition(int from) {
	incomingTransitions.push_back(from);
	return true;
}

std::vector<int> DfaState::GetIncomingTransitions() {
	return incomingTransitions;
}

bool DfaState::DeleteIncomingTransition(int value) {
	
	for (int i = 0; i < incomingTransitions.size(); i++) {
		if (incomingTransitions[i] == value) {
			incomingTransitions.erase(incomingTransitions.begin() + i);
			break;
		}
	}

	return true;
}

int DfaState::GetTransitionTo(char symbol) {
	if (transitions.find(symbol) == transitions.end()) {
		return -1;
	}
	else {
		return transitions[symbol];
	}
}