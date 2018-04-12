//
// Created by liushan on 17-8-19.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "write_log.h"


void write_log(const char * log_write_min_level_name, const struct log_grade * log_level_para,
               const char * log_msg, FILE * f_log_file)
{
    extern struct log_level log_write_level;
    char cur_date[CUR_DATE_LEN]; // 时间字符串
    int log_write_min_level_grade; //记录日志的最小级别

    // 获取最小记录日志名称对应的级别
    if (strcmp(log_write_min_level_name, log_write_level.info.level_name) == 0)
        log_write_min_level_grade = log_write_level.info.grade;
    else if (strcmp(log_write_min_level_name, log_write_level.notice.level_name) == 0)
        log_write_min_level_grade = log_write_level.notice.grade;
    else if (strcmp(log_write_min_level_name, log_write_level.warn.level_name) == 0)
        log_write_min_level_grade = log_write_level.warn.grade;
    else if (strcmp(log_write_min_level_name, log_write_level.error.level_name) == 0)
        log_write_min_level_grade = log_write_level.error.grade;
    else
    {
        printf("error! log_level_para \"%s\" is not legal\n", log_write_min_level_name);
        exit(EXIT_FAILURE);
    }

    // 记录日志级别大于等于最小日志记录级别时，则记录日志
    if ((log_level_para->grade) >= log_write_min_level_grade)
    {
        write_log_get_cur_date(cur_date); //获取当前时间
        // 时间字符串，保留23位，写死的
        fprintf(f_log_file, LOG_FORMAT, cur_date, log_level_para->level_name, log_msg);
        fflush(f_log_file);
    }

}


void write_log_get_cur_date(char *cur_date)
{
    char * wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    time_t timep;
    struct tm * p;

    time(&timep);
    p = localtime(&timep);
    sprintf(cur_date, "%d/%d/%d %s %d:%d:%d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday,
            wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);

}





