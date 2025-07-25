#pragma once

#include <stdio.h>
#include "prfunction.h"
#include "filteritem.h"
#include "String.h"

class CLogReader
{
    friend class UnitTest;

public:
    CLogReader();
    ~CLogReader();

    bool Open(const char* filename); // открытие файла, false - ошибка
    void Close();                    // закрытие файла

    bool SetFilter(const char *filter); // установка фильтра строк, false - ошибка
    bool GetNextLine(char *buf, const int bufsize); // запрос очередной найденной строки,
                                                    // buf - буфер, bufsize - максимальная длина
                                                    // false - конец файла или ошибка

private:
    bool check(const String& str) const;
    int match(const FilterItem& pattern, const char* str) const;
    int trivialMatch(const char *pattern, const char *origin) const;
    int kmpMatch(const char *pattern, const char *origin, const PrFunction& prFunction) const;

    bool isContain(const String& sub, const String& origin, size_t offset) const;

private:
    FilterItem *mFilterItems = nullptr;
    int mFilterItemCount = 0;
    FILE* mFile = nullptr;

    // Граница перехода между тривиальным и кмп-решением. До этой границы тривиальное решение быстрее
    static constexpr int TrivialMatchCount = 2;
};
