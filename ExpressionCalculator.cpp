#include "ExpressionCalculator.h"
#include "Operation.h"
#include <vector>
#include <exception>
#include <string>
#include <algorithm>
#include <queue>

long long ExpressionCalculator::CalculateExpression(std::vector<std::string> NumberList, std::vector<char> OperatorList) {
    if (OperatorList.size() != NumberList.size() - 1) {
        throw std::exception();
    }
    else if (NumberList.size() == 1) {
        return std::stoll(NumberList[0]);
    }

    std::vector<Operation> OperationList;
    std::queue<Operation> OperationQueue;


    OperationList.push_back(Operation(OperatorList[0], new long long*(new long long(std::stoll(NumberList[0]))), new long long* (new long long(std::stoll(NumberList[0])))));
    for (int Count = 1; Count < OperatorList.size(); Count = Count + 1) {
        OperationList.push_back(Operation(OperatorList[Count], OperationList[Count - 1].Second, new long long* (new long long(std::stoll(NumberList[Count + 1])))));
    }

    for (int Count = 0; Count < OperationList.size(); Count = Count + 1) {
        if (OperationList[Count].Operator == '*' || OperationList[Count].Operator == '/') {
            OperationQueue.push(OperationList[Count]);
        }
    }

    for (int Count = 0; Count < OperationList.size(); Count = Count + 1) {
        if (OperationList[Count].Operator == '+' || OperationList[Count].Operator == '-') {
            OperationQueue.push(OperationList[Count]);
        }
    }


    long long TotalResult = 0;
    while(!OperationQueue.empty()) {
            long long FirstNumber = **OperationQueue.front().First;
            long long SecondNumber = **OperationQueue.front().Second;
            long long Result = 0;
            if (OperationQueue.front().Operator == '*') {
                Result = FirstNumber * SecondNumber;

                *(OperationQueue.front().Second) = *(OperationQueue.front().First);
                **OperationQueue.front().Second = Result;
            }
            else if (OperationQueue.front().Operator == '/') {
                if (SecondNumber == 0) {
                    throw std::exception();
                }
                else {
                    Result = FirstNumber / SecondNumber;

                    *(OperationQueue.front().Second) = *(OperationQueue.front().First);
                    **OperationQueue.front().Second = Result;
                }
            }
            else if (OperationQueue.front().Operator == '+') {
                Result = FirstNumber + SecondNumber;

                *(OperationQueue.front().First) = *(OperationQueue.front().Second);
                **OperationQueue.front().Second = Result;
            }
            else if (OperationQueue.front().Operator == '-') {
                Result = FirstNumber - SecondNumber;

                *(OperationQueue.front().First) = *(OperationQueue.front().Second);
                **OperationQueue.front().Second = Result;
            }
            TotalResult = Result;
            
            /*delete *(OperationQueue.front().First);
            *(OperationQueue.front().First) = nullptr;
            delete OperationQueue.front().First;
            OperationQueue.front().First = nullptr;

            delete *(OperationQueue.front().Second);
            *(OperationQueue.front().Second) = nullptr;
            delete OperationQueue.front().Second;
            OperationQueue.front().Second= nullptr;*/

            OperationQueue.pop();
        }
    return TotalResult;
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