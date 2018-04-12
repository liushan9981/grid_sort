//
// Created by liushan on 17-9-2.
//

#include <stdio.h>
#include <string.h>

void get_last_char(char * buffer, char * temp, int num)
{
    int my_index, index;
    size_t mysize;

    mysize = strlen(buffer);
    mysize--;

    for (my_index = 0, index = (num - 2); my_index < (num - 1); my_index++)
    {
        temp[my_index] = buffer[(mysize - index)];
        index--;
    }
    temp[my_index] = '\0';
}