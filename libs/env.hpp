#ifndef _ENV_H_
#define _ENV_H_

#include <cstring>
#include <cstdlib>

class ENV
{
private:
    errno_t errCode;

public:
    ENV();

    const wchar_t* operator[](const wchar_t* str);
    errno_t geterr();
};

#endif