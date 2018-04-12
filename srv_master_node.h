//
// Created by liushan on 17-8-19.
//

#ifndef GRID_SORT_SRV_CLI_H
#define GRID_SORT_SRV_CLI_H
#define CONN_OK "conn_ok"
#define CONN_FAIL "conn_failed"
#define FILE_RES_MASTER_PRE "task_master_collect"
#define FILE_RCV_NODE_PRE "task_node_rcv"
#define FIN_STR_SEND_PROC_FILE "END_FILE\n"
#define LEN_FIN_STR_SEND_PROC_FILE 10
#define FIN_STR_SEND_RES_FILE "END_RES\n"
#define LEN_FIN_STR_SEND_RES_FILE 9
#define SRV_RUN_ROLE_MASTER "master"
#define SRV_RUN_ROLE_NODE "node"
#define LEN_LOG_MSG 2048
#endif //GRID_SORT_SRV_CLI_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <stdbool.h>




