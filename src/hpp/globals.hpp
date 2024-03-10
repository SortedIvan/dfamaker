#pragma once
/*
	@author = Ivan
	This file serves as an import to all other files that need the below constants
*/

/*
	Math conventions
*/
# define M_PI  3.14159265358979323846

/*
	Screen conventions
*/
const static float SCREEN_X_SIZE = 1200.f;
const static float SCREEN_Y_SIZE = 800.f;
const static float SCREEN_X_RATIO = SCREEN_X_SIZE / 1920.f;  // Default ratio based on your initial screen size
const static float SCREEN_Y_RATIO = SCREEN_Y_SIZE / 1080.f;   // Default ratio based on your initial screen size
const static float DEFAULT_STATE_RADIUS_RATIO = 40.f / 1920.f;  // Default ratio based on your initial screen size
const static float DEFAULT_STATE_SELECTED_RADIUS_RATIO = 25.f / 1920.f;  // Default ratio based on your initial screen size
const static float DEFAULT_STATE_ACCEPTING_RADIUS_RATIO = 35.f / 1920.f;  // Default ratio based on your initial screen size
const static float STARTING_STATE_ARROW_LEN_RATIO = 25.f / 1920.f;  // Default ratio based on your initial screen size
const static int TARGET_FPS = 315;

/*
	Editor overlay constants
*/
const static sf::Color STRING_ACCEPTED = sf::Color::Green;
const static sf::Color STRING_DECLINED = sf::Color::Red;
const static sf::Color DEFAULT_BG_COLOR(0x00, 0x01, 0x33);
const static sf::Color PLACEMENT_INDICATOR_OUTLINE(0x73, 0x93, 0xB3);
const static sf::Color DEFAULT_STATE_COLOR(255, 255, 255, 50);
const static float TEXT_HIGHLIGHTER_CHANGE_SPEED = 0.7f;
const static float TYPING_EXITED_CD = 1.f;

/*
	Character naming conventions
*/
const static char TAB = '\t';
const static char BACKSPACE = '\b';

