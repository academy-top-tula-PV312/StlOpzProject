#include <iostream>
#include "MathExpression.h"

int main()
{

    MathExpression math("a + 20 * b");
    math["a"] = 10;
    math["b"] = 30;
    math["a"] = 50;
    
    math.CreateOpz();
    std::cout << math.ExpressionOpz() << "\n";

    std::cout << "Result = " << math.CalculateOpz() << "\n";



}
