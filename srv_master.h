//
// Created by liushan on 17-8-19.
//

#ifndef GRID_SORT_START_SRV_MASTER_H
#include "log_level.h"
#define GRID_SORT_START_SRV_MASTER_H

#endif //GRID_SORT_START_SRV_MASTER_H


int srv_master(const struct run_para *run_para_info, char *file_proc,
               const struct log_level * log_write_level, FILE * f_log_file);
int srv_master_gen_ip_list(const char *ch, int ch_len, char ip_list[][20], int ip_num,
                           const struct log_level * log_write_level, FILE * f_log_file);
void srv_master_start_server(const struct run_para *run_para_info, char *taskstr, int node_num_sum,
                             const struct log_level * log_write_level, FILE * f_log_file);
void srv_master_sort_node_res(const struct run_para *run_para_info, char *taskstr, int node_count,
                              const struct log_level * log_write_level, FILE * f_log_file);
void srv_master_send_data(const struct run_para *run_para_info, const char *node_ip, char *taskstr, unsigned long long line_sent_num,
                          FILE *file_read_sent, const struct log_level * log_write_level, FILE * f_log_file);