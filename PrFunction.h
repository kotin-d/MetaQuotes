#pragma once


// Вспомогательный класс,  для расчета префикс-функции.
// предполагается,  что он используется только в CLogReader,  и никто другой
// его использовать не будет.
class PrFunction final
{
public:
    PrFunction();
    ~PrFunction();

    // рассчитать префикс функцию
    void calculate(const char *str);

    // проверка,что префикс-функция расчитана
    bool exist() const;

    // нужен только константный метод.  Изменять через
    // через индексатор нельзя,  только через calculate
    const int& operator[] (const int index) const;

    // вывод значений на консоль, нужен только для отладки
 //   void debugOutput() const;

private:
    void calculate(const char *str, const int size);

    void resize(const int s);
    bool equal(const char c1, const char c2) const;

private:
    int* mData = nullptr;
    int mSize = 0;

    bool mExist = false;
};
