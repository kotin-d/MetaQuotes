#include "filteritem.h"


FilterItem::FilterItem()
{
}

FilterItem::~FilterItem()
{
}

void FilterItem::calculate(int trivialLimit) 
{
    if (mItem.size() >= trivialLimit)
        mPrFunction.calculate(mItem.buffer());
}

void FilterItem::setStar()
{
    mIsStar = true;
}

bool FilterItem::isTrivial() const
{
    return !mPrFunction.exist();
}

bool FilterItem::isStar() const
{
    return mIsStar;
}

void FilterItem::append(const char symbol)
{
    mItem.append(symbol);
}


size_t FilterItem::size() const
{
    return mItem.size();
}

const String& FilterItem::filter() const
{
    return mItem;
}

const PrFunction& FilterItem::prFunction() const
{
    return mPrFunction;
}
