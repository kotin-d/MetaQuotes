#include "clogreader.h"
#include <windows.h>
#include "String.h" // это моя строка

//#include "UnitTest.h"


int main(int argc, char* argv[])
{
    // запуск тестов
    //UnitTest units;
    //units.FullTest();

    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwCount = 0;

    const char msg[] = "bad arguments";

    if (argc != 3)
    {
        WriteConsoleA(hStdout, &msg, sizeof(msg), &dwCount, NULL);
        return 1;
    }

    const char *filename = argv[1];
    const char *pattern = argv[2];

    // для отладки
    //const char* filename = "log.txt";
    //const char* pattern = "*";

    CLogReader logReader;

    if (logReader.Open(filename))
    {
        // исходя из логики задачи.  
        // буфер заполняется снаружи
        char buf[256] = "\0";
        int bufsize = 256;

        if (logReader.SetFilter(pattern))
        {
            while (logReader.GetNextLine(buf, bufsize))
            {
                WriteConsoleA(hStdout, &buf, utils::strlen(buf), &dwCount, NULL);
                WriteConsoleA(hStdout, "\n", 1, &dwCount, NULL);
            }
        }
        else
        {
            const char msg[] = "bad filter";
            WriteConsoleA(hStdout, &msg, sizeof(msg), &dwCount, NULL);
            return 2;

        }
        logReader.Close();
     }

    return 0;
}
