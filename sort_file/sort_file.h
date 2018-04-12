//
// Created by liushan on 17-9-2.
//

#ifndef GRID_SORT_SORT_FILE_H
#define GRID_SORT_SORT_FILE_H

#include "../log_level.h"
#endif //GRID_SORT_SORT_FILE_H
#include <stdbool.h>

long long sort_file(const struct run_para *run_para_info, char * file_name, bool sort_desc,
                    const int res_len, long long * result, const struct log_level * log_write_level, FILE * f_log_file);
void sort_file_sort_sample(long long *result, long long *sample_read, int res_len,
                           int sample_len, bool flag, bool sort_desc);