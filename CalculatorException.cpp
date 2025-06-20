#include "CalculatorException.h"

CalculatorException::CalculatorException() : std::exception() {};
CalculatorException::CalculatorException(const char* Message) : std::exception(Message) {};