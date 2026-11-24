#pragma once
#include <conio.h>

#include "Config.h"

[[nodiscard]] inline int ReadKey()
{
    const int key = _getch();
    if (key == 0 || key == 224)
    {
        return kKeyExtended + _getch();
    }
    return key;
}
