//
// Created by liushan on 17-8-19.
//

#ifndef GRID_SORT_READ_CONF_H
#define GRID_SORT_READ_CONF_H

#define CONF_ERR -1
#define INPUT_PARA_LEN 4
#define CONF_FILE_PARA_MARK "-c"
#define RUN_PARA_LEN_IP_ADDR 16
#define RUN_PARA_LEN_PATH 100
#define RUN_PARA_LEN_LOG_KEY 20
#define RUN_PARA_LEN_LOG_LEVEL 25
#define RUN_PARA_LEN_ROLE 10
#define RUN_PARA_LEN_NODE_LIST 200
#define INPUT_FORMAT "%s%s%100s"
#include <stdbool.h>

struct run_para {
    char master[RUN_PARA_LEN_IP_ADDR];
    int  master_port;
    int  node_port;
    char log_path[RUN_PARA_LEN_PATH];
    char log_level[RUN_PARA_LEN_LOG_LEVEL];
    char role[RUN_PARA_LEN_ROLE];
    char node_list[RUN_PARA_LEN_NODE_LIST];
    char temp_dir[RUN_PARA_LEN_PATH];
    char sort_file_path[RUN_PARA_LEN_PATH];
    bool sort_desc;
};
#endif //GRID_SORT_READ_CONF_H





void get_run_para(struct run_para *instance_para, int argc, char **args);
int get_run_para_get_conf_pos(int num, char **input_para);
void get_run_para_read_file(char *conf_file, struct run_para *instance_para);
void get_run_para_get_sort_file(int conf_pos, char **args, struct run_para *instance_para);
