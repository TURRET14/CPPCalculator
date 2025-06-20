#include "ExpressionCalculator.h"
#include "CalculatorException.h"
#include <vector>
#include <exception>
#include <string>
#include <algorithm>

double ExpressionCalculator::CalculateExpression(std::vector<double> NumberList, std::vector<char> OperatorList) {
    if (NumberList.size() != OperatorList.size() + 1) {
        throw CalculatorException("Incorrect Expression!");
	}
    for (int Count = 0; Count < OperatorList.size();) {
        if (OperatorList[Count] == '*' || OperatorList[Count] == '/') {
            double FirstNumber = NumberList[Count];
            double SecondNumber = NumberList[Count + 1];
            double Result = 0;
            if (OperatorList[Count] == '*') {
                Result = FirstNumber * SecondNumber;
            }
            else if (OperatorList[Count] == '/') {
                if (SecondNumber == 0) {
                    throw CalculatorException("Division By Zero!");
                }
                else {
                    Result = FirstNumber / SecondNumber;
                }
            }
            NumberList.erase(NumberList.begin() + Count + 1);
            NumberList[Count] = Result;
            OperatorList.erase(OperatorList.begin() + Count);
        }
        else {
            Count = Count + 1;
        }
    }
    for (int Count = 0; Count < OperatorList.size();) {
        double FirstNumber = NumberList[0];
        double SecondNumber = NumberList[1];
        double Result = 0;
        if (OperatorList[Count] == '+') {
            Result = FirstNumber + SecondNumber;
        }
        else if (OperatorList[Count] == '-') {
            Result = FirstNumber - SecondNumber;
        }
        NumberList.erase(NumberList.begin() + 1);
        NumberList[0] = Result;
        OperatorList.erase(OperatorList.begin());
    }
    return NumberList[0];
}

double ExpressionCalculator::CalculateExpressionString(std::string Expression) {
    Expression.erase(std::remove(Expression.begin(), Expression.end(), ' '), Expression.end());
    if (Expression.empty()) {
        throw CalculatorException("Empty Expression!");
    }
    Expression.insert(0, "(");
    Expression.push_back(')');
    int LeftParCount = std::count(Expression.begin(), Expression.end(), '(');
    int RightParCount = std::count(Expression.begin(), Expression.end(), ')');
    if (LeftParCount != RightParCount) {
        throw CalculatorException("Incorrect Expression!");
    }

    for (int ParCount = 0; ParCount < LeftParCount; ParCount = ParCount + 1) {
        int LeftParPos = Expression.find_last_of('(');
        int RightParPos = Expression.find_first_of(')', LeftParPos);
        if (RightParPos == std::string::npos) {
            throw CalculatorException("Incorrect Expression!");
        }

        std::string CurrentExpression = Expression.substr(LeftParPos + 1, RightParPos - LeftParPos - 1);
        std::vector<double> NumberList;
        std::vector<char> OperatorList;
        std::string CurrentNumber;
        bool IsFloatingPoint = false;
        for (int Count = 0; Count < CurrentExpression.size(); Count = Count + 1) {
            if (Count == CurrentExpression.size() - 1) {
                if (isdigit(CurrentExpression[Count]) || (CurrentExpression[Count] == '.' && !CurrentNumber.empty() && CurrentNumber != "-")) {
                    CurrentNumber.push_back(CurrentExpression[Count]);
                    NumberList.push_back(std::stod(CurrentNumber));
                    CurrentNumber.clear();
                }
                else {
                    throw CalculatorException("Incorrect Expression!");
                }
            }
            else if (CurrentExpression[Count] == '+' || CurrentExpression[Count] == '-' || CurrentExpression[Count] == '*' || CurrentExpression[Count] == '/') {
                if (CurrentNumber.empty()) {
                    if (CurrentExpression[Count] == '+' || CurrentExpression[Count] == '-') {
                        CurrentNumber.push_back(CurrentExpression[Count]);
                    }
                    else {
                        throw CalculatorException("Incorrect Expression!");
                    }
                }
                else if (CurrentNumber == "-" || CurrentNumber == "+") {
                    throw CalculatorException("Incorrect Expression!");
                }
                else {
                    if (CurrentNumber == ".") {
                        throw CalculatorException("Incorrect Expression!");
                    }
                    else {
                        NumberList.push_back(std::stod(CurrentNumber));
                        CurrentNumber.clear();
                        IsFloatingPoint = false;
                        OperatorList.push_back(CurrentExpression[Count]);
                    }
                }
            }
            else if (isdigit(CurrentExpression[Count])) {
                CurrentNumber.push_back(CurrentExpression[Count]);
            }
            else if (CurrentExpression[Count] == '.') {
                if (IsFloatingPoint == false) {
                    CurrentNumber.push_back(CurrentExpression[Count]);
                    IsFloatingPoint = true;
                }
                else {
                    throw CalculatorException("Incorrect Expression!");
                }
            }
            else {
                throw CalculatorException("Incorrect Expression!");
            }
        }
        Expression.replace(LeftParPos, RightParPos - LeftParPos + 1, std::to_string(CalculateExpression(NumberList, OperatorList)));
    }
    return std::stod(Expression);
}