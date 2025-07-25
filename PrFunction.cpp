#include "prfunction.h"
#include <stdlib.h>
#include "String.h"
//#include <iostream> // только для отладки


PrFunction::PrFunction()
{
}


PrFunction::~PrFunction()
{
    free(mData);
}


void PrFunction::calculate(const char *str)
{
    calculate(str, utils::strlen(str));
    mExist = true;
}


void PrFunction::calculate(const char *str, const int size)
{
    resize(size);
    if (size == 0)
        return;

    mData[0] = 0;

    for (int i = 1; i < size; ++i)
    {
        int j = mData[i-1];
        while ((j > 0) && !equal(str[i], str[j]))
            j = mData[j - 1];
        if (equal(str[i], str[j]))
            ++j;
        mData[i] = j;
    }
}


bool PrFunction::exist() const
{
    return mExist;
}


const int& PrFunction::operator[] (const int index) const
{
    return mData[index];
}

// void PrFunction::debugOutput() const
// {
//     std::cout << "prefix function" << std::endl;
//     for (int i = 0; i != mSize; ++i)
//     {
//         std::cout << "i " << i << " : " << mData[i] << std::endl;
//     }
// }


bool PrFunction::equal(const char c1, const char c2) const
{
    return c1 == c2 || c1 == '?' || c2 == '?';
}


void PrFunction::resize(const int size)
{
    free(mData);
    mData = (int*)malloc(size * sizeof(int));
    mSize = size;
}
