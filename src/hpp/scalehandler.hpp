#ifndef SCREEN_HANDLER_HPP
#define SCREEN_HANDLER_HPP

#include <SFML/Graphics.hpp>
#include "globals.hpp"

class ScaleHandler {
public:
    static void UpdateConstants(const sf::Vector2u& windowSize);

    static int SCREEN_X_SIZE;
    static int SCREEN_Y_SIZE;
    static int DEFAULT_STATE_RADIUS;
    static int DEFAULT_STATE_SELECTED_RADIUS;
    static int DEFAULT_STATE_ACCEPTING_RADIUS;
    static int STARTING_STATE_ARROW_LEN;
};

#endif // SCREEN_HANDLER_HPP