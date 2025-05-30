#include "Operation.h"
Operation::Operation(char Operator, long long** First, long long** Second) {
	this->Operator = Operator;
	this->First = First;
	this->Second = Second;
}