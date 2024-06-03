#include "MathExpression.h"

MathExpression::MathExpression()
	: expression{ "" }, expressionOpz{ "" } {}

MathExpression::MathExpression(std::string expression)
	: expression{ expression }, expressionOpz{ "" } {}

MathExpression::MathExpression(const char* expression)
	: MathExpression(std::string(expression)) {}

std::string& MathExpression::Expression()
{
	return expression;
}

std::string MathExpression::ExpressionOpz() const
{
	return expressionOpz;
}

double& MathExpression::operator[](std::string varibale)
{
	if (variables.find(varibale) == variables.end())
		variables.emplace(varibale, 0.0);

	return variables[varibale];
}

double& MathExpression::operator[](const char* varibale)
{
	return this->operator[](std::string(varibale));
}

int MathExpression::BracketsCheck()
{
	int position{};
	const std::string bracketsOpen = "([{";
	const std::string bracketsClose = ")]}";
	const std::string brackets = bracketsOpen + bracketsClose;

	std::stack<char> bracketsStack;

	while (position < expression.length())
	{
		// non brackets
		if (brackets.find(expression[position]) == std::string::npos)
		{
			position++;
			continue;
		}

		// opens bracket
		if (bracketsOpen.find(expression[position]) != std::string::npos)
		{
			bracketsStack.push(expression[position]);
			position++;
			continue;
		}

		// closes bracket
		if (bracketsStack.empty())
			return position;

		int typeClose = bracketsClose.find(expression[position]);
		int typeOpen = bracketsOpen.find(bracketsStack.top());

		if (typeClose != typeOpen)
			return position;
		bracketsStack.pop();

		position++;
	}

	return ((bracketsStack.empty()) ? -1 : position);
}

void MathExpression::CreateOpz()
{
	int position{};

	const std::string bracketsOpen = "([{";
	const std::string bracketsClose = ")]}";
	const std::string operationsAdditive = "+-";
	const std::string operationsMultiplex = "*/";

	const std::string whiteChar = " \t\n";

	std::stack<char> operationsStack;
	
	bool isUnar{ true };
	bool isLastOperator{ false };

	while (position < expression.length())
	{

		// ignore spaces
		if (whiteChar.find(expression[position]) != std::string::npos)
		{
			position++;
			continue;
		}

		// unar minus
		if (expression[position] == '-' && (isUnar || isLastOperator))
		{
			expressionOpz += '~';
			isUnar = false;
			isLastOperator = false;

			position++;
			continue;
		}

		// numbers
		if (std::isdigit(expression[position]) || expression[position] == '.')
		{
			std::string number = "";
			while (position < expression.length() &&
				(std::isdigit(expression[position]) ||
					expression[position] == '.' ||
					std::tolower(expression[position]) == 'e'))
			{
				if (std::tolower(expression[position]) == 'e' &&
					expression[position + 1] == '-')
				{
					number.push_back(expression[position]);
					number.push_back(expression[position + 1]);
					position += 2;
				}
				number.push_back(expression[position]);
				position++;
			}
			number.push_back('#');
			expressionOpz.append(number);
			
			isLastOperator = false;
			isUnar = false;

			//position++;
			continue;
		}

		// variables
		if (std::isalpha(expression[position]))
		{
			std::string variable = "";
			while (position < expression.length() &&
				(std::isalnum(expression[position]) ||
					expression[position] == '_'))
				variable.push_back(expression[position++]);
			
			double value = variables[variable];
			if (value < 0)
				expressionOpz.push_back('~');
			expressionOpz.append(std::to_string(abs(value)) + "#");

			isUnar = false;
			isLastOperator = false;

			//position++;
			continue;
		}

		// opening brackets
		if (bracketsOpen.find(expression[position]) != std::string::npos)
		{
			operationsStack.push(expression[position]);
			isLastOperator = false;
			isUnar = true;

			position++;
			continue;
		}
			
		// closing brackets
		if (bracketsClose.find(expression[position]) != std::string::npos)
		{
			while (!operationsStack.empty()
				&& bracketsOpen.find(operationsStack.top()) == std::string::npos)
			{
				expressionOpz.push_back(operationsStack.top());
				operationsStack.pop();
			}
			operationsStack.pop();

			isLastOperator = false;
			isUnar = false;

			position++;
			continue;
		}

		// multiplex operations * and /
		if (operationsMultiplex.find(expression[position]) != std::string::npos)
		{
			while (!operationsStack.empty()
				&& operationsMultiplex.find(operationsStack.top()) != std::string::npos)
			{
				expressionOpz.push_back(operationsStack.top());
				operationsStack.pop();
			}
			operationsStack.push(expression[position]);

			isLastOperator = true;
			isUnar = false;

			position++;
			continue;
		}

		// additive operation + and -
		if (operationsAdditive.find(expression[position]) != std::string::npos)
		{
			while (!operationsStack.empty()
				&& bracketsOpen.find(operationsStack.top()) == std::string::npos)
			{
				expressionOpz.push_back(operationsStack.top());
				operationsStack.pop();
			}
			operationsStack.push(expression[position]);
			isLastOperator = true;
			isUnar = false;

			position++;
			continue;
		}
	}

	while (!operationsStack.empty())
	{
		expressionOpz.push_back(operationsStack.top());
		operationsStack.pop();
	}
}

double MathExpression::CalculateOpz()
{
	std::stack<double> numbersStack;
	int position{};
	const std::string operators = "-+*/";

	while (position < expressionOpz.length())
	{

		// numbers
		if (std::isdigit(expressionOpz[position]) 
			|| expressionOpz[position] == '.'
			|| expressionOpz[position] == '~')
		{
			int tilda{};
			
			while (expressionOpz[position] == '~')
			{
				position++;
				tilda++;
			}
				

			std::string number = "";
			while (position < expressionOpz.length() && expressionOpz[position] != '#')
				number.push_back(expressionOpz[position++]);
			
			double value = std::stod(number);
			if (tilda % 2) value = -value;

			numbersStack.push(value);
			position++;
			continue;
		}

		// operators
		if (operators.find(expressionOpz[position]) != std::string::npos)
		{
			double result{};
			
			double operandRight{ numbersStack.top() };
			numbersStack.pop();

			double operandLeft{ numbersStack.top() };
			numbersStack.pop();

			switch (expressionOpz[position])
			{
			case '+': result = operandLeft + operandRight; break;
			case '-': result = operandLeft - operandRight; break;
			case '*': result = operandLeft * operandRight; break;
			case '/': result = operandLeft / operandRight; break;
			default:
				break;
			}
			numbersStack.push(result);
		}
		position++;
	}
	return numbersStack.top();
}

double MathExpression::Calculate()
{
	CreateOpz();
	return CalculateOpz();
}





