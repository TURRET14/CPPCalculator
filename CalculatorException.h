#include <exception>

class CalculatorException : public std::exception
{
public:
	CalculatorException();
	CalculatorException(const char* Message);
};