#include <iostream>
#include "error.h"

namespace jasonpascal
{
    void errorToken(const std::string& msg)
    {
        std::cerr <<"Token Error:" << msg << std::endl;
    }
}
