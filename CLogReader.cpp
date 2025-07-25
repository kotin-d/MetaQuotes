#include "clogreader.h"
#include "String.h"
#include <stdio.h>
#include <stdlib.h>
//#include <iostream> // только для отладки


CLogReader::CLogReader()
{
}


CLogReader::~CLogReader()
{
    if (mFilterItems)
        delete[] mFilterItems;
}


bool CLogReader::Open(const char* filename)
{
    mFile = fopen(filename, "r");
    return mFile;
}


void CLogReader::Close()
{
    fclose(mFile);
}


bool CLogReader::SetFilter(const char *filter)
{
    if (filter == nullptr || *filter == '\0')
        return false;

    if (mFilterItems)
    {
        delete[] mFilterItems;
        mFilterItemCount = 0;
    }

    const char *str = filter;
    bool inGroup = false;

    while (*str != '\0')
    {
        if (*str == '*')
        {
            ++mFilterItemCount;
            inGroup = false;
        }
        else
        {
            if (inGroup == false)
                ++mFilterItemCount;
            inGroup = true;
        }
        ++str;
    }

    if (mFilterItemCount == 0)
    {
        mFilterItems = nullptr;
        return false;
    }
    mFilterItems = new FilterItem[mFilterItemCount];

    str = filter;
    int index = 0;
    inGroup = false;

    while (*str != '\0')
    {
        if (*str == '*')
        {
            if (inGroup) {
                mFilterItems[index].append('\0');
                mFilterItems[index].calculate(TrivialMatchCount);
                ++index;
            }
            inGroup = false;
            mFilterItems[index++].setStar();
        }
        else
        {
            inGroup = true;
            mFilterItems[index].append(*str);
        }
        ++str;
    }

    if (inGroup) {
        mFilterItems[index].append('\0');
        mFilterItems[index].calculate(TrivialMatchCount);
    }

    //for (size_t i = 0; i != mFilterItemCount; ++i) {
    //    if (mFilterItems[i].isStar())
    //        std::cout << "*" << std::endl;
    //    else
    //        std::cout << mFilterItems[i].filter().buffer() << std::endl;
    //}

    return true;
}


bool CLogReader::GetNextLine(char *buf, const int bufsize)
{
    String str;
    while (str.lineFromFile(mFile)) {
        if (check(str)) {
            if (str.size() >= bufsize) {
                utils::memncpy(buf, str.buffer(), bufsize - 1);
                buf[bufsize - 1] = '\0';
            }
            else {
                utils::strncpy(buf, str.buffer(), str.size());
            }
            return true;
        }
    }
    return false;
}


bool CLogReader::check(const String& str) const
{
    if (mFilterItems == nullptr)
        return true;

    int offset = 0;
    bool freeFind = false;

    for (size_t index = 0; index != mFilterItemCount; ++index)
    {
        const auto& filter = mFilterItems[index];

        if (filter.isStar())
        {
            freeFind = true;
        }
        else
        {
            if (freeFind)
            {
                // На первый взгляд тут рекурсия, и нужно проходить последовательно по всем хвостам,  у которых проеверять хвосты.
                // Но из-за того что нам нужен только check, и без разницы как до него дошли, то достаточно проверить последний фильтр в конце
                if (index == mFilterItemCount - 1)
                {
                    return isContain(filter.filter(), str, str.size() - filter.size());
                }
                else
                {
                    const int result = match(filter, str.buffer() + offset);
                    if (result == -1)
                        return false;

                    offset += result + filter.size() - 1;
                    freeFind = false;
                }
            }
            else
            {
                if (isContain(filter.filter(), str, offset))
                {
                    offset += filter.filter().size() - 1;
                }
                else
                {
                    return false;
                }
            }
        }
    }

    if (freeFind)
        return true;
    else
        return offset == str.size() - 1;
}


int CLogReader::match(const FilterItem& pattern, const char* str) const
{
    if (pattern.isStar())
        return -1;

    if (pattern.isTrivial())
        return trivialMatch(pattern.filter().buffer(), str);
    else
        return kmpMatch(pattern.filter().buffer(), str, pattern.prFunction());
}


int CLogReader::trivialMatch(const char *pattern, const char *str) const
{
    const char *origin = str;
    while (*origin != '\0')
    {
        const char *origin_find = origin;
        const char *pattern_find = pattern;

        while (*pattern_find != '\0')
        {
            if (*origin_find == '\0')
                return -1;

            if (*origin_find == *pattern_find || *pattern_find == '?')
            {
                ++origin_find;
                ++pattern_find;
                continue;
            }
            else
            {
                break;
            }
        }
        if (*pattern_find == '\0')
            return origin - str;

        ++origin;
    }
    return -1;
}


bool CLogReader::isContain(const String& sub, const String& origin, size_t offset) const
{
    if (sub.size() > origin.size() - offset)
        return false;

    for (size_t index = 0; index != sub.size() - 1; ++index)
    {
        if (sub[index] != origin[index + offset] && sub[index] != '?')
            return false;
    }
    return true;
}


int CLogReader::kmpMatch(const char* pattern, const char* origin, const PrFunction& prFunction) const
{
    const int originLen = utils::strlen(origin);
    const int patternLen = utils::strlen(pattern);

    for (int i = 0, j = 0; i < originLen; ++i)
    {
        while (j > 0 && (pattern[j] != origin[i] && pattern[j] != '?'))
            j = prFunction[j - 1];
        if (pattern[j] == origin[i] || pattern[j] == '?')
            j++;
        if (j == patternLen)
            return i - j + 1;
    }

    return -1;
}
