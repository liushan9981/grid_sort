//
// Created by liushan on 17-9-9.
//

#ifndef GRID_SORT_LOG_LEVEL_H
#define GRID_SORT_LOG_LEVEL_H
// 日志名称和日志级别，级别为整数值，值从低到高表示严重级别从低到高
struct log_grade {
    char level_name[10];
    int grade;
};

// 定义日志级别，4个级别
struct log_level {
    struct log_grade info;
    struct log_grade notice;
    struct log_grade warn;
    struct log_grade error;
};

#endif //GRID_SORT_LOG_LEVEL_H



