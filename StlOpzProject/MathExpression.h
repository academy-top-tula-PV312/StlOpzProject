#pragma once

#include <iostream>
#include <string>
#include <map>
#include <stack>


class MathExpression
{
	std::string expression;
	std::string expressionOpz;

	std::map<std::string, double> variables;

public:
	MathExpression();
	MathExpression(std::string expression);
	MathExpression(const char* expression);

	std::string& Expression();
	std::string ExpressionOpz() const;

	double& operator[](std::string varibale);
	double& operator[](const char* varibale);

	int BracketsCheck();
	void CreateOpz();
	double CalculateOpz();

	double Calculate();
};

