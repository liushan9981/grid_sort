//
// Created by liushan on 17-8-19.
//

#include "stdio.h"
#include "sub_func/get_file_line_num.h"
#define BUFFERSIZE 1024

#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <sys/wait.h>

#define LONG 10

void read_file(FILE * f, long long  num, int index);

void mytest(void)
{
    char * file_name = "/home/liushan/mylab/clang/sort_sample/sort_list_1.txt";
    FILE * f;
    int num = 10, index, status;
    pid_t pid = 1;
    int count_num = 1000;
    char buffer[1024];
    int node_count = 5;
    unsigned long long line_count;
    long long size_per_sent, line_remain, size_sent_fact;

    line_count = get_file_line_num("/home/liushan/mylab/clang/sort_sample/sort_list_1.txt");
    size_per_sent = line_count / node_count;
    line_remain = line_count;

    if ( (f = fopen(file_name, "r") ) == NULL)
    {
        printf("open file failed\n");
        exit(EXIT_FAILURE);
    }



    for (index = 0; index < 5; index++)
    {
        if (pid != 0)
        {
            // printf("line_remain: %lli\n", line_remain);
            if (line_remain < (2 * size_per_sent) )
                size_sent_fact = line_remain;
            else
                size_sent_fact = size_per_sent;
            // printf("size_sent_fact: %lli\n", size_sent_fact);

            if ( (pid = fork() ) < 0)
            {
                printf("fork error");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                read_file(f, size_sent_fact, index);
                // printf("------------------\n");
            }
            else
            {
                if (wait(&status) != pid)
                {
                    printf("wait error\n");
                    exit(EXIT_FAILURE);
                }
                // if (index == 0)
                    // printf("####  start server\n");
                // printf("this is parent, status is: %d\n", status);
                // printf("*temp: %d\n", count_num);
                line_remain = line_remain - size_sent_fact;
                // printf("---line_remain: %lli\n", line_remain);
            }
        }

    }

}


void read_file(FILE * f, long long  num, int index)
{
    long long  count = 0;
    char buffer[1024];

    while (fgets(buffer, 1024, f) != NULL)
    {
        printf("%d  %s", index, buffer);
        // (*temp) = (*temp + 1);
        count++;
        if (count >= num)
            break;
    }

}


/*
void mytest(void)
{
    int count = 5;
    long long  sum;
    int index;
    int size_per_sent;
    char ip_list[5][20] = {{"192.168.1.1"}, {"192.168.1.2"}, {"192.168.1.3"}, {"192.168.1.4"}, {"192.168.1.5"}};
    char * file = "/home/liushan/mylab/clang/sort_sample/sort_list_1.txt";
    FILE * f;
    char readbuf[BUFFERSIZE];
    long long size_sent_fact, size_sent_fact_index;
    bool flag = false;


    f = fopen(file, "r");
    sum = get_file_line_num(file);

    size_per_sent = sum / count;


    for (index = 0; index < count; index++)
    {
        if (sum < (size_per_sent * 2) )
        {
            // printf("%s  %lli\n", ip_list[index], sum);
            size_sent_fact = sum;
        }
        else
        {
            // printf("%s  %s\n", ip_list[index], size_per_sent);
            size_sent_fact = size_per_sent;
            sum -= size_per_sent;
        }

        if (flag)
        {
            printf("%s  %s", ip_list[index], readbuf);
            size_sent_fact--;
        }


        size_sent_fact_index = 0;
        while (fgets(readbuf, BUFFERSIZE, f) != NULL)
        {
            if (size_sent_fact_index < size_sent_fact)
            {
                printf("%s  %s", ip_list[index], readbuf);
                size_sent_fact_index++;
            }
            else
            {
                flag = true;
                break;
            }

        }
    }

}
*/