#include "ExpressionCalculator.h"
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
    if (Expression.empty()) {
        throw std::exception();
    }
    Expression.insert(0, "(");
    Expression.push_back(')');
    int LeftParCount = std::count(Expression.begin(), Expression.end(), '(');
    int RightParCount = std::count(Expression.begin(), Expression.end(), ')');
    if (LeftParCount != RightParCount) {
        throw std::exception();
    }

    for (int Count = 0; Count < LeftParCount; Count = Count + 1) {
        int LeftParPos = Expression.find_last_of('(');
        int RightParPos = Expression.find_first_of(')', LeftParPos);
        if (RightParPos == std::string::npos) {
            throw std::exception();
        }
        
        std::string CurrentExpression = Expression.substr(LeftParPos + 1, RightParPos - LeftParPos - 1);
        std::vector<std::string> NumberList;
        std::vector<char> OperatorList;
        std::string CurrentNumber;
        for (int Count = 0; Count < CurrentExpression.size(); Count = Count + 1) {
            if (Count == CurrentExpression.size() - 1) {
                if (isdigit(CurrentExpression[Count])) {
                    CurrentNumber.push_back(CurrentExpression[Count]);
                    NumberList.push_back(CurrentNumber);
                    CurrentNumber.clear();
                }
                else {
                    throw std::exception();
                }
            }
            else if (CurrentExpression[Count] == '+' || CurrentExpression[Count] == '-' || CurrentExpression[Count] == '*' || CurrentExpression[Count] == '/') {
                if (CurrentNumber.empty()) {
                    if (CurrentExpression[Count] == '+' || CurrentExpression[Count] == '-') {
                        if (CurrentExpression[Count] == '-') {
                            CurrentNumber.push_back(CurrentExpression[Count]);
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
                    OperatorList.push_back(CurrentExpression[Count]);
                }
            }
            else if (isdigit(CurrentExpression[Count])) {
                CurrentNumber.push_back(CurrentExpression[Count]);
            }
            else {
                throw std::exception();
            }
        }
        Expression.replace(LeftParPos, RightParPos - LeftParPos + 1, std::to_string(CalculateExpression(NumberList, OperatorList)));
    }
    return std::stoll(Expression);
}