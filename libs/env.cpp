#include "env.hpp"

ENV::ENV()
    : errCode(0)
{

}

const wchar_t* ENV::operator[](const wchar_t* str)
{
    size_t pReturnValue;
    _wgetenv_s(&pReturnValue, NULL, 0, str); // get string length
    if (pReturnValue == 0)
        return L"Empty";
    wchar_t* buff = new wchar_t[pReturnValue];
    errCode = _wgetenv_s(&pReturnValue, buff, pReturnValue, str);
    if (errCode != 0)
    {
        delete[] buff;
        return L"Error";
    }
    return buff;
}

errno_t ENV::geterr()
{
    return errCode;
}