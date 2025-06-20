#include "ExpressionCalculator.h"
#include "CalculatorException.h"
#include "Operation.h"
#include <vector>
#include <exception>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>

double ExpressionCalculator::CalculateExpression(std::vector<double> NumberList, std::vector<char> OperatorList) {
    std::vector<Operation> OperationList;
    std::queue<Operation> OperationQueue;
    std::stack<std::pair<double*, double**>> Pointers;
    
    try {
        if (OperatorList.size() != NumberList.size() - 1) {
            throw CalculatorException("Incorrect Expression!");
        }
        else if (NumberList.size() == 1) {
            return NumberList[0];
        }

        OperationList.push_back(Operation(OperatorList[0], new double* (new double(NumberList[0])), new double* (new double(NumberList[1]))));

        Pointers.push({ OperationList.back().First[0], OperationList.back().First });
        Pointers.push({ OperationList.back().Second[0], OperationList.back().Second });

        for (int Count = 1; Count < OperatorList.size(); Count = Count + 1) {
            OperationList.push_back(Operation(OperatorList[Count], OperationList[Count - 1].Second, new double* (new double(NumberList[Count + 1]))));
            Pointers.push({ OperationList.back().Second[0], OperationList.back().Second });
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


        double TotalResult = 0;
        while (!OperationQueue.empty()) {
            double FirstNumber = **OperationQueue.front().First;
            double SecondNumber = **OperationQueue.front().Second;
            double Result = 0;
            if (OperationQueue.front().Operator == '*') {
                Result = FirstNumber * SecondNumber;
                *(OperationQueue.front().Second) = *(OperationQueue.front().First);
                **OperationQueue.front().Second = Result;
            }
            else if (OperationQueue.front().Operator == '/') {
                if (SecondNumber == 0) {
                    throw CalculatorException("Division By Zero!");
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
            OperationQueue.pop();
        }

        while (!Pointers.empty()) {
            std::pair<double*, double**> PointerPair = Pointers.top();
            delete PointerPair.first;
            PointerPair.first = nullptr;
            delete PointerPair.second;
            PointerPair.second = nullptr;
            Pointers.pop();
        }
        return TotalResult;
    }
    catch (CalculatorException Exception) {
        while (!Pointers.empty()) {
            std::pair<double*, double**> PointerPair = Pointers.top();
            delete PointerPair.first;
            PointerPair.first = nullptr;
            delete PointerPair.second;
            PointerPair.second = nullptr;
            Pointers.pop();
        }
        throw Exception;
    }
    catch (std::exception Exception) {
        while (!Pointers.empty()) {
            std::pair<double*, double**> PointerPair = Pointers.top();
            delete PointerPair.first;
            PointerPair.first = nullptr;
            delete PointerPair.second;
            PointerPair.second = nullptr;
            Pointers.pop();
        }
        throw Exception;
    }
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