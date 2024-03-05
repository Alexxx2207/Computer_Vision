#include "MathFunctions.hpp"

int MathFunctions::clamp(int input, const int lowerBound, const int upperBound) {
	if (input < lowerBound) {
		input = lowerBound;
	}
	else if (input > upperBound) {
		input = upperBound;
	}

	return input;
}