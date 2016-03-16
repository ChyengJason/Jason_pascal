#include <fstream>
#include <iostream>
#include <string>
#include "token.h"
#include "dictionary.h"
#include "scanner.h"

using namespace jasonpascal;

int main()
{
  //  Scanner scanner("test.pas");
  Scanner scanner("scanner_test.pas");

    scanner.getNextToken();

    while (scanner.getToken().getTokenType() != TokenType::END_OF_FILE)
    {
        if (scanner.getErrorFlag())
        {
            return 1;
        }
        scanner.getToken().dump();//打印
        scanner.getNextToken();
    }

    return 0;
}
