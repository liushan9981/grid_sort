#include <stdio.h>
#include <stdbool.h>
#include "test.h"
#include <stdlib.h>
#include <string.h>
#include "get_run_para.h"
#include "write_log.h"
#include "srv_node.h"
#include "srv_master.h"
#include "srv_master_node.h"

//　４个日志级别的名称和值
struct log_level log_write_level = {
        .info = {"INFO", 0},
        .notice = {"NOTICE", 1},
        .warn = {"WARN", 2},
        .error = {"ERROR", 3}
};


int main(int argc, char * argv[])
{
    struct run_para instance_para;
    FILE * f_log_file;
    char log_msg[LEN_LOG_MSG];

    get_run_para(&instance_para, argc, argv);
    f_log_file = fopen(instance_para.log_path, "a");
    if (f_log_file == NULL)
    {
        fputs("open log file fail\n", stderr);
        exit(EXIT_FAILURE);
    }

    write_log(instance_para.log_level, &(log_write_level.notice), "conf para get below:", f_log_file);
    sprintf(log_msg, "instance_para->master: %s", instance_para.master);
    write_log(instance_para.log_level, &(log_write_level.notice), log_msg, f_log_file);
    sprintf(log_msg, "instance_para->master_port: %d", instance_para.master_port);
    write_log(instance_para.log_level, &(log_write_level.notice), log_msg, f_log_file);
    sprintf(log_msg, "instance_para->node_port: %d", instance_para.node_port);
    write_log(instance_para.log_level, &(log_write_level.notice), log_msg, f_log_file);
    sprintf(log_msg, "instance_para->log_path: %s", instance_para.log_path);
    write_log(instance_para.log_level, &(log_write_level.notice), log_msg, f_log_file);
    sprintf(log_msg, "instance_para->log_level: %s", instance_para.log_level);
    write_log(instance_para.log_level, &(log_write_level.notice), log_msg, f_log_file);
    sprintf(log_msg, "instance_para->role: %s", instance_para.role);
    write_log(instance_para.log_level, &(log_write_level.notice), log_msg, f_log_file);
    sprintf(log_msg, "instance_para->node_list: %s", instance_para.node_list);
    write_log(instance_para.log_level, &(log_write_level.notice), log_msg, f_log_file);
    sprintf(log_msg, "instance_para->temp_dir: %s", instance_para.temp_dir);
    write_log(instance_para.log_level, &(log_write_level.notice), log_msg, f_log_file);

    if (strcmp(instance_para.role, SRV_RUN_ROLE_MASTER) == 0)
    {
        write_log(instance_para.log_level, &(log_write_level.notice), "run as master", f_log_file);
        sprintf(log_msg, "instance_para->sort_file_path: %s", instance_para.sort_file_path);
        write_log(instance_para.log_level, &(log_write_level.notice), log_msg, f_log_file);
        srv_master(&instance_para, instance_para.sort_file_path, &log_write_level, f_log_file);
    }
    else if (strcmp(instance_para.role, SRV_RUN_ROLE_NODE) == 0)
    {
        write_log(instance_para.log_level, &(log_write_level.notice), "run as node", f_log_file);
        srv_node(&instance_para, &log_write_level, f_log_file);
    }

    fclose(f_log_file);

    return 0;
}