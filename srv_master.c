//
// Created by liushan on 17-8-19.
//
#include <sys/wait.h>
#include "srv_master_node.h"
#include "get_run_para.h"
#include "sub_func/get_file_line_num.h"
#include "sort_file/sort_file.h"
#include "sub_func/get_last_char.h"
#include "sub_func/merge_file.h"
#include "srv_master.h"
#include "log_level.h"
#include "write_log.h"

char log_msg[LEN_LOG_MSG];


int srv_master(const struct run_para *run_para_info, char *file_proc,
               const struct log_level * log_write_level, FILE * f_log_file)
{
    FILE * file_read_sent;
    pid_t pid_srv = 3;
    char node_ip_list[100][20];
    int node_num_sum, index, taskid = 1;
    bool is_server_fork = false;
    char taskstr[50];
    unsigned long long line_num_per_sent, line_num_to_send, line_num_sum, line_num_remain;

    // taskid计划自动生成，现在暂时写死
    sprintf(taskstr, "taskid:%d", taskid);
    sprintf(log_msg, "master taskstr: %s", taskstr);
    write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);

    if ( (file_read_sent = fopen(file_proc, "r")) == NULL )
    {
        sprintf(log_msg, "can not open sort file: %s", file_proc);
        write_log(run_para_info->log_level, &(log_write_level->error), log_msg, f_log_file);
        exit(EXIT_FAILURE);
    }

    node_num_sum = srv_master_gen_ip_list(run_para_info->node_list,
                                          (int) (sizeof(run_para_info->node_list) / sizeof(char)),
                                          node_ip_list, 100, log_write_level, f_log_file);
    line_num_sum = get_file_line_num(file_proc);
    sprintf(log_msg, "line_num_sum value: %llu", line_num_sum);
    write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
    line_num_per_sent = line_num_sum / node_num_sum;
    line_num_remain = line_num_sum;

    for (index = 0; index < node_num_sum; index++)
    {
        if (pid_srv != 0)
        {
            if (line_num_remain < (2 * line_num_per_sent) )
                line_num_to_send = line_num_remain;
            else
                line_num_to_send = line_num_per_sent;

            // 只fork一次，fork server接收node发来的处理结果
            // 根据变量is_server_fork来判断是否已经fork
            if (! is_server_fork)
            {
                is_server_fork = true;
                if ( (pid_srv = fork() ) < 0)
                {
                    write_log(run_para_info->log_level, &(log_write_level->error),
                              "fork pid_srv error, now exit", f_log_file);
                    exit(EXIT_FAILURE);
                }
                else if (pid_srv == 0)
                {
                    // fork的子进程
                    srv_master_start_server(run_para_info, taskstr, node_num_sum, log_write_level, f_log_file);
                }
            }
            else
            {
                write_log(run_para_info->log_level, &(log_write_level->info), "server is already forked", f_log_file);
            }

            srv_master_send_data(run_para_info, node_ip_list[index], taskstr, line_num_to_send,
                                 file_read_sent, log_write_level, f_log_file);
            line_num_remain -= line_num_to_send;

        }

    }


    wait(NULL);
    write_log(run_para_info->log_level, &(log_write_level->info), "all task is finished", f_log_file);

    return 0;

}


void srv_master_send_data(const struct run_para *run_para_info, const char *node_ip, char *taskstr, unsigned long long line_sent_num,
                          FILE *file_read_sent, const struct log_level * log_write_level, FILE * f_log_file)
{
    struct sockaddr_in servaddr;
    int sock_cli = socket(AF_INET, SOCK_STREAM, 0);
    const int buffer_size = 1024;
    char sendbuf[buffer_size];
    char recvbuf[buffer_size];
    unsigned long long line_sent_num_count;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(run_para_info->node_port);
    servaddr.sin_addr.s_addr = inet_addr(node_ip);

    sprintf(log_msg, "info node_ip_list[index] is: %s", node_ip);
    write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);

    if (connect(sock_cli, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
        write_log(run_para_info->log_level, &(log_write_level->error), "from srv_master_send_data connect error!", f_log_file);
        exit(2);
    }

    write(sock_cli, taskstr, strlen(taskstr));
    read(sock_cli, recvbuf, buffer_size - 1);
    sprintf(log_msg, "recv msg: %s", recvbuf);
    write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);

    if (strcmp(recvbuf, CONN_OK) == 0)
    {
        sprintf(log_msg, "task connect ok, recv: %s", recvbuf);
        write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));

        line_sent_num_count = 0;
        while (fgets(sendbuf, sizeof(sendbuf), file_read_sent) != NULL)
        {
            write(sock_cli, sendbuf, strlen(sendbuf));
            line_sent_num_count++;
            memset(sendbuf, 0, sizeof(sendbuf));
            // 发送给一个node的行数达到平均分配值后，跳出本次循环，发送给下一个node
            if (line_sent_num_count >= line_sent_num)
                break;
        }
        sleep(30);
        write(sock_cli, FIN_STR_SEND_PROC_FILE, strlen(FIN_STR_SEND_PROC_FILE));
        write_log(run_para_info->log_level, &(log_write_level->info), "out of send file while", f_log_file);
    }
}


void srv_master_start_server(const struct run_para *run_para_info, char *taskstr, int node_num_sum,
                             const struct log_level * log_write_level, FILE * f_log_file)
{
    int sockfd, conn, num_node_rcv = 0, file_index = 0, len;
    const int buffer_size = 1024;
    const int myqueue = 1000;
    char buffer[buffer_size];
    char check_fin_str[LEN_FIN_STR_SEND_PROC_FILE];
    struct sockaddr_in server_sockaddr;
    FILE * file_write_res;
    char file_path_res[RUN_PARA_LEN_PATH];
    int sock_cli = socket(AF_INET, SOCK_STREAM, 0);

    write_log(run_para_info->log_level, &(log_write_level->info),
              "from srv_master_start_server child fork ok", f_log_file);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&server_sockaddr, sizeof(server_sockaddr));

    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(run_para_info->master_port);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr) ) == -1 )
    {
        write_log(run_para_info->log_level, &(log_write_level->error),
                  "from srv_master_start_server bind error!", f_log_file);
        exit(2);
    }

    if (listen(sockfd, myqueue) == -1 )
    {
        write_log(run_para_info->log_level, &(log_write_level->error),
                  "from srv_master_start_server listen error!", f_log_file);
        exit(2);
    }
    write_log(run_para_info->log_level, &(log_write_level->notice),
              "from srv_master_start_server child is start listen ok", f_log_file);

    while (1)
    {
        if ( (conn = accept(sockfd, (struct sockaddr *) NULL, NULL) ) < 0)
        {
            write_log(run_para_info->log_level, &(log_write_level->error),
                      "from srv_master_start_server connect error!", f_log_file);
            exit(2);
        }

        write_log(run_para_info->log_level, &(log_write_level->notice),
                  "from srv_master_start_server conn sendback ok", f_log_file);

        sprintf(file_path_res, "%s/res_%s_%d", run_para_info->temp_dir, taskstr, file_index);
        sprintf(log_msg, "file_path_res: %s", file_path_res);
        write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);

        if ( (file_write_res = fopen(file_path_res, "w") ) == NULL)
        {
            write_log(run_para_info->log_level, &(log_write_level->error),
                      "from srv_master_start_server can not open file_write_res file!", f_log_file);
            exit(EXIT_FAILURE);
        }
        file_index++;

        while (1)
        {
            memset(buffer, 0, sizeof(buffer));
            len = read(conn, buffer, buffer_size - 1);
            if (len == 0)
            {
                num_node_rcv++;
                break;
            }
            buffer[len] = '\0';
            get_last_char(buffer, check_fin_str, LEN_FIN_STR_SEND_RES_FILE);
            if (strcmp(check_fin_str, FIN_STR_SEND_RES_FILE) == 0)
            {
                num_node_rcv++;
                write_log(run_para_info->log_level, &(log_write_level->info),
                          "from srv_master_start_server read result finished", f_log_file);
                fclose(file_write_res);
                break;
            }
            fprintf(file_write_res, "%s", buffer);
        }
        write_log(run_para_info->log_level, &(log_write_level->info),
                  "from srv_master_start_server recive res finished", f_log_file);
        sprintf(log_msg, "num_node_rcv: %d", num_node_rcv);
        write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
        sprintf(log_msg, "node_num_sum: %d", node_num_sum);
        write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
        if (num_node_rcv == node_num_sum)
        {
            srv_master_sort_node_res(run_para_info, taskstr, node_num_sum, log_write_level, f_log_file);
        }
        close(sock_cli);
    }
}



void srv_master_sort_node_res(const struct run_para *run_para_info, char *taskstr, int node_count,
                              const struct log_level * log_write_level, FILE * f_log_file)
{
    bool sort_desc = true;
    const int buffer_size = 1024;
    char buffer[buffer_size];

    const int res_len = 30;
    const int res_len_compare = 2 * res_len;
    long long rank_num, rank_index, result[res_len_compare];
    char file_path_res[RUN_PARA_LEN_PATH];

    sprintf(buffer, "%s/res_%s_", run_para_info->temp_dir, taskstr);
    sprintf(file_path_res, "%s/res_%s_all", run_para_info->temp_dir, taskstr);
    sprintf(log_msg, "file path pre: %s", buffer);
    write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
    sprintf(log_msg, "file_path_res: %s", file_path_res);
    write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
    merge_file(run_para_info, buffer, file_path_res, node_count, log_write_level, f_log_file);

    rank_num = sort_file(run_para_info, file_path_res, sort_desc, res_len, result, log_write_level, f_log_file);
    sprintf(log_msg, "rank_num: %lli", rank_num);
    write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
    printf("--------------sort result--------------\n");
    for (rank_index = 0; rank_index < rank_num; rank_index++)
    {
        printf("%lli    %lli\n", rank_index + 1, result[rank_index]);
    }
    // close(conn);
    // close(sockfd);
}



// 处理ip字符串，放入数组
// 例如：　10.10.10.10, 10.10.10.2 以逗号风格，如果有空格也做处理
int srv_master_gen_ip_list(const char *ch, int ch_len, char ip_list[][20], int ip_num,
                           const struct log_level * log_write_level, FILE * f_log_file)
{
    int index;
    int num, ip_pos;

    for (index = 0, num = 0, ip_pos = 0; index < ch_len; index++)
    {
        if (num >= ip_num)
        {
            num = ip_num;
            return num;
        }

        if (ch[index] == ',')
        {
            ip_list[num][ip_pos] = '\0';
            num++;
            ip_pos = 0;
        }
        else
        {
            if (ch[index] != ' ')
            {
                ip_list[num][ip_pos] = ch[index];
                ip_pos++;
            }

        }
    }
    num++;

    return num;
}

