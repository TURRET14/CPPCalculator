#include <exception>

class CalculatorException : std::exception
{
public:
	CalculatorException();
	CalculatorException(const char* Message);
};

