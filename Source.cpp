#include "ExpressionCalculator.h"
#include <iostream>

int main() {
    std::string Expression;
    while (true) {
        std::cout << "Input Expression: ";
        std::getline(std::cin, Expression);
        std::cout << "Result: " << std::fixed << ExpressionCalculator::CalculateExpressionString(Expression) << std::endl;
    }
}