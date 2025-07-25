#pragma once

#include "String.h"
#include "PrFunction.h"

// вспомогательные класс, для хранения куска фильтра с его префикс функцией
class FilterItem
{
public:
    FilterItem();
    ~FilterItem();

    void calculate(int trivialLimit);
    void setStar();

    bool isTrivial() const;
    bool isStar() const;

    void append(const char symbol);
    size_t size() const;

    // этот класс, внутринй для CLogReader.  Недоступен из других мест,  в реальности требует запретов на включение
    // так как предполагаем что он внутрение (это не реализовано),  поэтому так смело ведем себя со ссылками
    const String& filter() const;
    const PrFunction& prFunction() const;

private:
    String mItem;
    PrFunction mPrFunction;

    bool mIsStar = false;
};
