#include "dfa_state_transition.hpp"
# define M_PI  3.14159265358979323846  /* pi */
#include "SFML/Graphics.hpp"


StateTransition::StateTransition() {

}

void StateTransition::SetUpStateTransition(sf::Vector2f stateFrom, sf::Vector2f stateTo, float stateFromRadius, float stateToRadius, int stateToValue, int transitionFromValue) {
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
}
void StateTransition::SetTransitionTwo(int stateTo) {
	transitionTo = stateTo;
}

int StateTransition::GetTransitionTo() {
	return transitionTo;
}

void StateTransition::Draw(sf::RenderWindow& window) {
	window.draw(mainArrow);
	window.draw(arrowTipOne);
	window.draw(arrowTipTwo);
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
	mainArrow.SetArrowColor(color);
}


void StateTransition::SetTransitionSymbol(char symbol) {
	this->symbol = symbol;
}

char StateTransition::GetTransitionSymbol() {
	return symbol;
}