#include <string>
#include <vector>

class ExpressionCalculator {
public:
    static double CalculateExpression(std::vector<double> NumberList, std::vector<char> OperatorList);
    static double CalculateExpressionString(std::string Expression);
};
