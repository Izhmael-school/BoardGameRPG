#pragma once
#include <vector>

enum InputType {
	Push,
	Down,
	Up
};

struct XInputControllerInput {
	std::vector<InputType> input;

	XInputControllerInput() {
		
	}
};

class InputData {

};

