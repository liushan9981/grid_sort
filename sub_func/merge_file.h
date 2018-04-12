//
// Created by liushan on 17-9-2.
//

#ifndef GRID_SORT_MERGE_FILE_H
#define GRID_SORT_MERGE_FILE_H

#include "../log_level.h"
#endif //GRID_SORT_MERGE_FILE_H

void merge_file(const struct run_para *run_para_info, char * file_pre, char * file_res, int num,
                const struct log_level * log_write_level, FILE * f_log_file);