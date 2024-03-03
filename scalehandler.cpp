#include "scalehandler.hpp"
#include "globals.hpp" // Include your global constants if needed

// Initialize the static members
int ScaleHandler::SCREEN_X_SIZE = 1200;
int ScaleHandler::SCREEN_Y_SIZE = 800;
int ScaleHandler::DEFAULT_STATE_RADIUS = 40;
int ScaleHandler::DEFAULT_STATE_SELECTED_RADIUS = 25;
int ScaleHandler::DEFAULT_STATE_ACCEPTING_RADIUS = 35;
int ScaleHandler::STARTING_STATE_ARROW_LEN = 25;

void ScaleHandler::UpdateConstants(const sf::Vector2u& windowSize) {
    SCREEN_X_SIZE = static_cast<int>(SCREEN_X_RATIO * windowSize.x);
    SCREEN_Y_SIZE = static_cast<int>(SCREEN_Y_RATIO * windowSize.y);
    DEFAULT_STATE_RADIUS = static_cast<int>(DEFAULT_STATE_RADIUS_RATIO * windowSize.x);
    DEFAULT_STATE_SELECTED_RADIUS = static_cast<int>(DEFAULT_STATE_SELECTED_RADIUS_RATIO * windowSize.x);
    DEFAULT_STATE_ACCEPTING_RADIUS = static_cast<int>(DEFAULT_STATE_ACCEPTING_RADIUS_RATIO * windowSize.x);
    STARTING_STATE_ARROW_LEN = static_cast<int>(STARTING_STATE_ARROW_LEN_RATIO * windowSize.x);
}