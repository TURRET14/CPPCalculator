#include <string>
#include <vector>

class ExpressionCalculator {
public:
    static long long CalculateExpression(std::vector<std::string> NumberList, std::vector<char> OperatorList);
    static long long CalculateExpressionString(std::string Expression);
};
