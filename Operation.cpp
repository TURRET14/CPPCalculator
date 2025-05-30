#include "Operation.h"
Operation::Operation(char OperatorArg, double** FirstArg, double** SecondArg) {
	Operator = OperatorArg;
	First = FirstArg;
	Second = SecondArg;
}