//
// Created by liushan on 17-8-19.
//

#include <stdio.h>


unsigned long long get_file_line_num(char * file_name)
{
    unsigned long long count = 0;
    FILE * f;
    char ch;

    f = fopen(file_name, "r");
    rewind(f);

    while ( (ch = getc(f) ) != EOF)
    {
        if (ch == '\n')
            count++;
    }
    fclose(f);

    return count;
}
