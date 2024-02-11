#include "dfa_state_transition.hpp"
# define M_PI  3.14159265358979323846  /* pi */
#include "SFML/Graphics.hpp"
#include <iostream>

StateTransition::StateTransition() {

}

void StateTransition::SetUpStateTransition(
	sf::Vector2f stateFrom, sf::Vector2f stateTo,
	float stateFromRadius, float stateToRadius, 
	int stateToValue, int transitionFromValue,
	sf::Font& font
) {
	
	if (stateFrom == stateTo) { // This means we are looking at a self-loop
		// TODO: Add logic to handle the same self loop being added twice
		// First, we need four points for all of the arrow spots

		float fixOffset = 8.f;

		sf::Vector2f pStart(stateFrom.x + stateFromRadius / 2, (stateFrom.y - stateFromRadius + fixOffset));
		sf::Vector2f firstInterim(stateFrom.x + stateFromRadius / 2, stateFrom.y - stateFromRadius * 2 + fixOffset);
		sf::Vector2f secondInterim(stateFrom.x - stateFromRadius / 2, stateFrom.y - stateFromRadius * 2 + fixOffset);
		sf::Vector2f pEnd(stateFrom.x - stateFromRadius / 2, stateFrom.y - stateFromRadius + fixOffset);

		transitionTo = transitionFromValue;
		transitionFrom = transitionFromValue;

		isSelfLoop = true;

		rhs.setLinePoints(pStart, firstInterim);
		top.setLinePoints(firstInterim, secondInterim);
		lhs.setLinePoints(secondInterim, pEnd);

		sf::Vector2f dirVector(0, 1);

		sf::Transform rotationTransform;
		rotationTransform.rotate(45.f);
		sf::Vector2f rotatedLeft = rotationTransform.transformPoint(-dirVector);
		rotationTransform = sf::Transform();
		rotationTransform.rotate(-45.f); 
		sf::Vector2f rotatedRight = rotationTransform.transformPoint(-dirVector);

		arrowTipOne.setLinePoints(pEnd, rotatedLeft * 15.f + pEnd);
		arrowTipTwo.setLinePoints(pEnd, rotatedRight * 15.f + pEnd);

		transitionLabel.setPosition(
			sf::Vector2f(
				stateFrom.x, 
				stateFrom.y - stateFromRadius * 2 - fixOffset - 12.f
			)
		);

		transitionLabel.setFont(font);
		transitionLabel.setCharacterSize(20);
		transitionLabel.setFillColor(sf::Color::White);
		
		return;
	}

	//sf::Vector2f stateFrom = stateFromObj.GetStatePosition();
	//sf::Vector2f stateTo = stateToObj.GetStatePosition();
	sf::Vector2f dirVector = stateTo - stateFrom;

	float vectorLength = std::roundf(std::sqrt(dirVector.x * dirVector.x + dirVector.y * dirVector.y));

	arrowLength = vectorLength;

	sf::Vector2f dirVectorNormalized =
		sf::Vector2f(
			dirVector.x / vectorLength,
			dirVector.y / vectorLength
		);
	// 1) Calculate the rotation between the states
	float rotationRadians = std::atan2(dirVectorNormalized.y, dirVectorNormalized.x);
	float rotationDegrees = rotationRadians * (180 / M_PI); // Normalize to degrees
	// 2) Calculate the distance between the states
	float distance = std::sqrt(std::pow((stateTo.x - stateFrom.x), 2)
		+ std::pow((stateTo.y - stateFrom.y), 2));

	this->distance = distance;


	// 3) Calculate the out/in point
	//sf::Vector2f outgoingPoint = (dirVectorNormalized * stateFromObj.GetStateCircle().getRadius()) + stateFrom;
	//sf::Vector2f toPoint = (-dirVectorNormalized * stateToObj.GetStateCircle().getRadius()) + stateTo;
	sf::Vector2f outgoingPoint = (dirVectorNormalized * stateFromRadius) + stateFrom;
	sf::Vector2f toPoint = (-dirVectorNormalized * stateToRadius) + stateTo;

	mainArrow.setLinePoints(outgoingPoint, toPoint);
	// 4) Rotate the arrow tip lines
	sf::Transform rotationTransform;
	rotationTransform.rotate(45.f);

	// Rotate the vector left (counter-clockwise)
	sf::Vector2f rotatedLeft = rotationTransform.transformPoint(-dirVectorNormalized);

	// Reset the rotation matrix
	rotationTransform = sf::Transform();

	// Rotate the vector right (clockwise)
	rotationTransform.rotate(-45.f); // negative angle for clockwise rotation
	sf::Vector2f rotatedRight = rotationTransform.transformPoint(-dirVectorNormalized);

	arrowTipOne.setLinePoints(toPoint, rotatedLeft * 15.f + toPoint);
	arrowTipTwo.setLinePoints(toPoint, rotatedRight * 15.f + toPoint);

	transitionTo = stateToValue;
	transitionFrom = transitionFromValue;

	// Set the transition label position:

	//std::cout << rotationDegrees;

	float offset = 0.f;

	// Leaving this as seperate checks for potential change in the future
	if (rotationDegrees >= 0 && rotationDegrees <= 45) {
		offset = 5.f;
	}
	else if (rotationDegrees >= -135 && rotationDegrees <= -90) {
		offset = 5.f;
	}


	transitionLabel.setPosition(
		sf::Vector2f(
			stateFrom.x + dirVector.x / 2 + offset,
			stateFrom.y + dirVector.y / 2
		)
	);

	transitionLabel.setFont(font);
	transitionLabel.setCharacterSize(20);
	transitionLabel.setFillColor(sf::Color::White);
}

void StateTransition::SetTransitionTwo(int stateTo) {
	transitionTo = stateTo;
}

int StateTransition::GetTransitionTo() {
	return transitionTo;
}

void StateTransition::Draw(sf::RenderWindow& window) {

	if (!isSelfLoop) {
		window.draw(mainArrow);
	}
	else {
		window.draw(rhs);
		window.draw(lhs);
		window.draw(top);
	}

	// Always draw tips
	window.draw(arrowTipOne);
	window.draw(arrowTipTwo);

	if (symbols.size() > 0) {
		window.draw(transitionLabel);
	}
}

sfLine StateTransition::GetMainArrow() {
	return mainArrow;
}

bool StateTransition::GetIsAssigned() {
	return isAssigned;
}
void StateTransition::SetIsAssigned(bool value) {
	this->isAssigned = value;
}

int StateTransition::GetTransitionFrom() {
	return transitionFrom;
}

int StateTransition::GetTransitionId() {
	return id;
}

void StateTransition::SetTransitionId(int id) {
	this->id = id;
}

void StateTransition::SetTransitionColor(sf::Color color) {

	if (!isSelfLoop) {
		mainArrow.SetArrowColor(color);

	}
	else {
		rhs.SetArrowColor(color);
		lhs.SetArrowColor(color);
		top.SetArrowColor(color);
	}

	arrowTipOne.SetArrowColor(color);
	arrowTipTwo.SetArrowColor(color);
}


void StateTransition::SetTransitionDistance(float distance) {
	this->distance = distance;
}

bool StateTransition::GetIsSelfLoop() {
	return isSelfLoop;
}

void StateTransition::SetIsSelfLoop(bool value) {
	isSelfLoop = value;
}

sfLine StateTransition::GetRhsArrow() {
	return rhs;
}

sfLine StateTransition::GetLhsArrow() {
	return lhs;
}

sfLine StateTransition::GetTopArrow() {
	return top;
}

std::vector<char> StateTransition::GetTransitionSymbols() {
	return symbols;
}

bool StateTransition::AddTransitionSymbol(char symbol) {
	if (symbol != '~') {

		for (int i = 0;i < symbols.size(); i++) {
			if (symbols[i] == symbol) {
				// symbol already exists
				return false;
			}
		}

		std::string symbolString;
		symbols.push_back(symbol);
		sf::Text sizeReference;

		bool largeEnough = false;
		if (symbols.size() > 1) {
			largeEnough = true;
		}

		float offset = 0.f;

		for (int i = 0; i < symbols.size(); i++) {
			if (largeEnough && i > 0) {
				symbolString.push_back(',');
			}
			symbolString.push_back(symbols[i]);
			offset -= 5.f;
		}
		transitionLabel.setString(symbolString);
		transitionLabel.setPosition(sf::Vector2f(
			transitionLabel.getPosition().x + offset,
			transitionLabel.getPosition().y
		));

		return true;
	}

	return false;
}

char StateTransition::RemoveSingleSymbol() {
	if (symbols.size() > 0) {

		char ref = symbols[symbols.size() - 1];
		float offset = 0.f;

		std::string currentLabel = transitionLabel.getString();
		
		if (symbols.size() > 1) {
			currentLabel.pop_back();
			currentLabel.pop_back();
			offset += 11.f;
		}

		transitionLabel.setString(currentLabel);
		transitionLabel.setPosition(sf::Vector2f(
			transitionLabel.getPosition().x + offset,
			transitionLabel.getPosition().y
		));

		symbols.pop_back();

		return ref;
	}
	return '~';
}