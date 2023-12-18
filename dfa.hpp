#pragma once
#include <string>
#include <vector>
#include "SFML/Graphics.hpp"
#include "dfa_state.hpp"

class DFA {
	private:
		std::vector<DfaState> states;
		std::vector<char> alphabet;
};