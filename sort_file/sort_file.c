//
// Created by liushan on 17-9-2.
//

//
// Created by liushan on 17-8-8.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bubble_sort.h"
#include "../log_level.h"
#include "../get_run_para.h"
#include "sort_file.h"
#include "../write_log.h"

long long sort_file(const struct run_para *run_para_info, char * file_name, bool sort_desc,
                    const int res_len, long long * result, const struct log_level * log_write_level, FILE * f_log_file)
{
    const int line_len = 20;
    const int sample_len = 10000;
    long long sample_read[sample_len];
    long long line_count;
    char temp[line_len];
    int sample_len_index, index;
    long long rank_num;
    FILE * f_read;
    bool flag_start;
    char log_msg[2048];


    if ( (f_read = fopen(file_name, "r") ) == NULL)
    {
        sprintf(log_msg, "from sort_file open file %s failed\n", file_name);
        write_log(run_para_info->log_level, &(log_write_level->error), log_msg, f_log_file);
        exit(EXIT_FAILURE);
    }

    sample_len_index = 0;
    flag_start = true;
    line_count = 0;
    while (fgets(temp, line_len - 1, f_read) != NULL)
    {
        // 一次读取sample_len个数据，并放入sample_read数组
        if (sample_len_index < sample_len)
        {
            sample_read[sample_len_index] = atoll(temp);
            sample_len_index++;
        }
        else
        {
            // 此刻sample_len_index等于sample_len
            sort_file_sort_sample(result, sample_read, res_len, sample_len_index, flag_start, sort_desc);
            // 第一次存放到数组时, 关闭flag_start
            flag_start = false;
            sample_len_index = 0;
            sample_read[sample_len_index] = atoll(temp);
            sample_len_index++;
        }
        line_count++;
    }

    // 存在总行数小于res_len的情况，即总行数小于统计的结果
    // sample_len_index等于行数或者sample_len的余数, line_count等于行数
    // sample_len_index 作为样本长度的参数传入函数，
    // 如果总行数小于res_len, 则传参 line_count, 即取值排名line_count
    if (line_count >= res_len)
        rank_num = res_len;
    else
        rank_num = (int)line_count;

    sort_file_sort_sample(result, sample_read, rank_num, sample_len_index, flag_start, sort_desc);
    return rank_num;
}


void sort_file_sort_sample(long long *result, long long *sample_read, int res_len, int sample_len,
                           bool flag, bool sort_desc)
{
    int index = 0, seek = 0;
    int incr_num, res_len_compare;

    // 样本数小于排序的总数时，取最小值
    if (res_len > sample_len)
        incr_num = sample_len;
    else
        incr_num = res_len;
    // 读入的样本排序
    bubble_sort(sample_read, sample_len, incr_num, sort_desc);

    if (flag)
        seek = 0;
    else
        seek = res_len;

    for (index = 0; index < incr_num; index++)
    {
        result[seek] = sample_read[index];
        seek++;
    }
    // 新增的数，不一定是res_len的2倍
    res_len_compare = seek;
    // 第一次放入数组时，不需要排序，前半部分上面已经排序,后半部分不存在
    if (! flag)
        bubble_sort(result, res_len_compare, res_len, sort_desc);

}


