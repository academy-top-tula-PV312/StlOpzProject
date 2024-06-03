#include <iostream>
#include "MathExpression.h"

int main()
{

    MathExpression math("20 + cos(5)");
    math["a"] = 10;
    math["b"] = -30;
    math["c"] = -20;
    
    math.CreateOpz();
    std::cout << math.ExpressionOpz() << "\n";

    std::cout << "Result = " << math.CalculateOpz() << "\n";



}
