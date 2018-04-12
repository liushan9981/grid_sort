//
// Created by liushan on 17-8-19.
//

#ifndef GRID_SORT_WRITE_LOG_H
#define GRID_SORT_WRITE_LOG_H
#define CUR_DATE_LEN 25
#define LOG_FORMAT "%-23s  [%s]  %s\n"
#endif //GRID_SORT_WRITE_LOG_H


#include "log_level.h"


void write_log(const char * log_write_min_level_name, const struct log_grade * log_level_para,
               const char * log_msg, FILE * f_log_file);
void write_log_get_cur_date(char *cur_date);

