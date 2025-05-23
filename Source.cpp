#include "Header.h"
#include <iostream>
#include <vector>
#include <exception>
#include <string>
#include <algorithm>


long long ExpressionCalculator::CalculateExpression(std::vector<std::string> NumberList, std::vector<char> OperatorList) {
    for (int Count = 0; Count < OperatorList.size();) {
        if (OperatorList[Count] == '*' || OperatorList[Count] == '/') {
            long long FirstNumber = std::stoll(NumberList[Count]);
            long long SecondNumber = std::stoll(NumberList[Count + 1]);
            long long Result = 0;
            if (OperatorList[Count] == '*') {
                Result = FirstNumber * SecondNumber;
            }
            else if (OperatorList[Count] == '/') {
                if (SecondNumber == 0) {
                    throw std::exception();
                }
                else {
                    Result = FirstNumber / SecondNumber;
                }
            }
            NumberList.erase(NumberList.begin() + Count + 1);
            NumberList[Count] = std::to_string(Result);
            OperatorList.erase(OperatorList.begin() + Count);
        }
        else {
            Count = Count + 1;
        }
    }
    for (int Count = 0; Count < OperatorList.size();) {
        long long FirstNumber = std::stoll(NumberList[0]);
        long long SecondNumber = std::stoll(NumberList[1]);
        long long Result = 0;
        if (OperatorList[Count] == '+') {
            Result = FirstNumber + SecondNumber;
        }
        else if (OperatorList[Count] == '-') {
            Result = FirstNumber - SecondNumber;
        }
        NumberList.erase(NumberList.begin() + 1);
        NumberList[0] = std::to_string(Result);
        OperatorList.erase(OperatorList.begin());
    }
    return std::stoll(NumberList[0]);
}

long long ExpressionCalculator::CalculateExpressionString(std::string Expression) {
    Expression.erase(std::remove(Expression.begin(), Expression.end(), ' '), Expression.end());

    int LastNumber = 0;
    for (int Count = 0; Count < Expression.size(); Count = Count + 1) {
        if (isdigit(Expression[Count])) {
            LastNumber = Count;
        }
        else if (Expression[Count] != '+' && Expression[Count] != '-' && Expression[Count] != '*' && Expression[Count] != '/') {
            throw std::exception();
        }
    }

    Expression = Expression.substr(0, LastNumber + 1);
    std::vector<std::string> NumberList;
    std::vector<char> OperatorList;
    std::string CurrentNumber;
    for (int Count = 0; Count < Expression.size(); Count = Count + 1) {
        if (Count == Expression.size() - 1) {
            if (isdigit(Expression[Count])) {
                CurrentNumber.push_back(Expression[Count]);
                NumberList.push_back(CurrentNumber);
                CurrentNumber.clear();
            }
            else {
                throw std::exception();
            }
        }
        else if (Expression[Count] == '+' || Expression[Count] == '-' || Expression[Count] == '*' || Expression[Count] == '/') {
            if (CurrentNumber.empty()) {
                if (Expression[Count] == '+' || Expression[Count] == '-') {
                    if (Expression[Count] == '-') {
                        CurrentNumber.push_back(Expression[Count]);
                    }
                }
                else {
                    throw std::exception();
                }
            }
            else if (CurrentNumber == "-") {
                throw std::exception();
            }
            else {
                NumberList.push_back(CurrentNumber);
                CurrentNumber.clear();
                OperatorList.push_back(Expression[Count]);
            }
        }
        else if (isdigit(Expression[Count])) {
            CurrentNumber.push_back(Expression[Count]);
        }
        else {
            throw std::exception();
        }
    }
    return CalculateExpression(NumberList, OperatorList);
}


int main() {
    std::string Expression;
    std::cout << "Input Expression: ";
    std::getline(std::cin, Expression);
    std::cout << "Result: " << ExpressionCalculator::CalculateExpressionString(Expression);
}