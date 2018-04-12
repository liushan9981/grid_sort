//
// Created by liushan on 17-8-19.
//

#include "srv_master_node.h"
#include "get_run_para.h"
#include "sort_file/sort_file.h"
#include "sub_func/get_last_char.h"
#include "log_level.h"
#include "write_log.h"

// node节点
void srv_node(const struct run_para *run_para_info, const struct log_level * log_write_level, FILE * f_log_file)
{
    int server_sockfd;
    struct sockaddr_in server_sockaddr;
    const int myqueue = 100;
    const int buffer_size = 1024;
    int conn;
    int len;
    FILE * f;
    char file_name[buffer_size];
    struct sockaddr_in servaddr_master;
    int sock_cli;
    char taskid_str[10];
    char task_pre[7];
    int index_ch;
    int index_num;
    int taskid;
    bool flag = true;
    const int res_len = 30;
    const int res_len_compare = 2 * res_len;
    long long result[res_len_compare], rank_num, index;
    char sendbuf[buffer_size];
    char check_fin_str[LEN_FIN_STR_SEND_PROC_FILE];
    char buffer[buffer_size];
    char log_msg[LEN_LOG_MSG];


    bzero(&server_sockaddr, sizeof(server_sockaddr));
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(run_para_info->node_port);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_sockfd, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr) ) == -1 )
    {
        write_log(run_para_info->log_level, &(log_write_level->error), "bind error!", f_log_file);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sockfd, myqueue) == -1 )
    {
        write_log(run_para_info->log_level, &(log_write_level->error), "listen error!", f_log_file);
        exit(EXIT_FAILURE);
    }




    while (1)
    {
        if ( (conn = accept(server_sockfd, (struct sockaddr *) NULL, NULL) ) < 0)
        {
            write_log(run_para_info->log_level, &(log_write_level->error), "connect error!", f_log_file);
            exit(2);
        }

        memset(buffer, 0, sizeof(buffer));
        len = read(conn, buffer, buffer_size - 1);
        buffer[len] = '\0';

        memset(task_pre, 0, sizeof(task_pre));
        memset(taskid_str, 0, sizeof(taskid_str));

        // 获取的任务字符串处理
        // taskid:12345 分别赋值到两个字符串变量
        flag = true;
        for (index_ch = 0, index_num = 0; index_ch < strlen(buffer); index_ch++)
        {
            if (flag)
            {
                if (buffer[index_ch] == ':')
                {
                    task_pre[index_ch] = '\0';
                    flag = false;
                    continue;
                }
                task_pre[index_ch] = buffer[index_ch];
            }
            else
            {
                taskid_str[index_num] = buffer[index_ch];
                index_num++;
            }
        }
        taskid_str[index_num] = '\0';
        sprintf(log_msg, "taskid_str: %s", taskid_str);
        write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
        sprintf(log_msg, "task_pre: %s", task_pre);
        write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
        // 判断获取的任务，如果正常获取任务则进行下一步操作
        if (strcmp("taskid", task_pre) == 0)
            taskid = atoi(taskid_str);
        else
            taskid = 0;

        sprintf(log_msg, "taskid is: %d", taskid);
        write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
        if (taskid != 0)
        {
            write(conn, CONN_OK, strlen(CONN_OK) );
            sprintf(file_name, "%s/%s_%d", run_para_info->temp_dir, FILE_RCV_NODE_PRE, taskid);
            if ( (f = fopen(file_name, "w")) == NULL )
            {
                sprintf(log_msg, "can not open file: %s", file_name);
                write_log(run_para_info->log_level, &(log_write_level->error), log_msg, f_log_file);
                exit(EXIT_FAILURE);
            }

            while (1)
            {
                memset(buffer, 0, sizeof(buffer));
                len = read(conn, buffer, buffer_size - 1);
                if (len == 0)
                {
                    fclose(f);
                    write_log(run_para_info->log_level, &(log_write_level->notice), "close file", f_log_file);
                    break;
                }
                buffer[len] = '\0';
                get_last_char(buffer, check_fin_str, LEN_FIN_STR_SEND_PROC_FILE);
                if (strcmp(check_fin_str, FIN_STR_SEND_PROC_FILE) == 0)
                {
                    fclose(f);
                    write_log(run_para_info->log_level, &(log_write_level->notice),
                              "read end. now close file", f_log_file);
                    break;
                }

                fputs(buffer, f);
            }

            sock_cli = socket(AF_INET, SOCK_STREAM, 0);
            memset(&servaddr_master, 0, sizeof(servaddr_master));
            servaddr_master.sin_family = AF_INET;
            servaddr_master.sin_port = htons(run_para_info->master_port);
            servaddr_master.sin_addr.s_addr = inet_addr(run_para_info->master);

            sleep(3);
            if (connect(sock_cli, (struct sockaddr *)&servaddr_master, sizeof(servaddr_master) ) < 0)
            {
                write_log(run_para_info->log_level, &(log_write_level->error),
                          "connect error!", f_log_file);
                exit(2);
            }
            write_log(run_para_info->log_level, &(log_write_level->info),
                      "begine send to queen data", f_log_file);

            rank_num = sort_file(run_para_info, file_name, run_para_info->sort_desc,
                                 res_len, result, log_write_level, f_log_file);
            sprintf(log_msg, "rank_num: %lli", rank_num);
            write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
            for (index = 0; index < rank_num; index++)
            {
                sprintf(sendbuf, "%lli\n", result[index]);
                write(sock_cli, sendbuf, strlen(sendbuf) );
                memset(sendbuf, 0, sizeof(sendbuf));
            }

            write_log(run_para_info->log_level, &(log_write_level->info),
                      "this is task_str finished", f_log_file);
            sleep(30);
            write(sock_cli, FIN_STR_SEND_RES_FILE, strlen(FIN_STR_SEND_RES_FILE));
            close(sock_cli);
            write_log(run_para_info->log_level, &(log_write_level->info),
                      "waiting for next task", f_log_file);


        }
        else
        {
            write(conn, CONN_FAIL, strlen(CONN_FAIL) );
            write_log(run_para_info->log_level, &(log_write_level->error), "conn task error!", f_log_file);
        }


    }

}
