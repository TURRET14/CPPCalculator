#include <memory>

class Operation
{
public:
	Operation(char Operator, std::shared_ptr<std::shared_ptr<double>> First, std::shared_ptr<std::shared_ptr<double>> Second);
	char Operator;
	std::shared_ptr<std::shared_ptr<double>> First;
	std::shared_ptr<std::shared_ptr<double>> Second;
};