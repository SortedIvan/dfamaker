/**
 * @file scalehandler.cpp
 * @brief Description of the contents of the file.
 *
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 *
 * Copyright (C) [year] Ivan Ovcharov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 * ---
 *
 * If you have questions or need further information, you can contact me through my GitHub:
 *
 * Ivan Ovcharov
 * GitHub: https://github.com/SortedIvan
 */

#include "../hpp/scalehandler.hpp"
#include "../hpp/globals.hpp" // Include your global constants if needed

// Initialize the static members
int ScaleHandler::SCREEN_X_SIZE = 1200;
int ScaleHandler::SCREEN_Y_SIZE = 800;
int ScaleHandler::DEFAULT_STATE_RADIUS = 40;
int ScaleHandler::DEFAULT_STATE_SELECTED_RADIUS = 25;
int ScaleHandler::DEFAULT_STATE_ACCEPTING_RADIUS = 35;
int ScaleHandler::STARTING_STATE_ARROW_LEN = 25;

void ScaleHandler::UpdateConstants(const sf::Vector2u& windowSize) 
{
    SCREEN_X_SIZE = static_cast<int>(SCREEN_X_RATIO * windowSize.x);
    SCREEN_Y_SIZE = static_cast<int>(SCREEN_Y_RATIO * windowSize.y);
    DEFAULT_STATE_RADIUS = static_cast<int>(DEFAULT_STATE_RADIUS_RATIO * windowSize.x);
    DEFAULT_STATE_SELECTED_RADIUS = static_cast<int>(DEFAULT_STATE_SELECTED_RADIUS_RATIO * windowSize.x);
    DEFAULT_STATE_ACCEPTING_RADIUS = static_cast<int>(DEFAULT_STATE_ACCEPTING_RADIUS_RATIO * windowSize.x);
    STARTING_STATE_ARROW_LEN = static_cast<int>(STARTING_STATE_ARROW_LEN_RATIO * windowSize.x);
}