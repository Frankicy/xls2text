/* xls2text.cpp */
extern "C" {
#include "xls_wrapper.h"
}
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input.xls> <output.txt>\n";
        return 1;
    }

    XlsText *txt = xls_to_text(argv[1]);
    if (!txt)
    {
        std::cerr << "Failed to read XLS\n";
        return 2;
    }

    std::ofstream ofs(argv[2]);
    for (int r = 0; r < txt->rows; ++r)
    {
        for (int c = 0; c < txt->cols; ++c)
        {
            const char *cell = txt->data[r * txt->cols + c];
            ofs << (cell ? cell : "");
            if (c + 1 != txt->cols) ofs << '\t';
        }
        ofs << '\n';
    }

    xls_text_free(txt);
    return 0;
}