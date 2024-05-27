#include <iostream>
#include "MathExpression.h"

int main()
{
    MathExpression math("");
    int result;
    if ((result = math.BracketsCheck()) == -1)
        std::cout << "no errors\n";
    else
        std::cout << "error position: " << result << "\n";
}
