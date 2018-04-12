//
// Created by liushan on 17-9-2.
//

#include <stdbool.h>


void bubble_sort(long long * all_num, int all_num_len, int rank_num, bool sort_desc)
{
    int index, seek;
    bool trans_flag;
    long long temp;

    for (index = 0; index < rank_num; index++)
        for (seek = index + 1; seek < all_num_len; seek++)
        {
            trans_flag = false;
            if ( (! sort_desc) && (all_num[index] > all_num[seek]) )
                trans_flag = true;
            else if ( (sort_desc) && (all_num[index] < all_num[seek]) )
                trans_flag = true;
            if (trans_flag)
            {
                temp = all_num[index];
                all_num[index] = all_num[seek];
                all_num[seek] = temp;
            }
        }

}