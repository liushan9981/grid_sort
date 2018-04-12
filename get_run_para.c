//
// Created by liushan on 17-8-19.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "get_run_para.h"
#include "usage.h"





void get_run_para(struct run_para *instance_para, int argc, char **args)
{
    // 配置文件的位置
    int conf_pos;

    instance_para->master[0] = '#';
    instance_para->master_port = -1;
    instance_para->node_port = -1;
    instance_para->log_path[0] = '#';
    instance_para->log_level[0] = '#';
    instance_para->role[0] = '#';
    instance_para->node_list[0] = '#';
    instance_para->temp_dir[0] = '#';
    instance_para->sort_desc = false;

    conf_pos = get_run_para_get_conf_pos(argc, args);
    get_run_para_read_file(args[conf_pos], instance_para);
    if (strcmp(instance_para->role, "master") == 0)
        get_run_para_get_sort_file(conf_pos, args, instance_para);
}


void get_run_para_read_file(char *conf_file, struct run_para *instance_para)
{
    FILE * f_conf;
    char key[RUN_PARA_LEN_LOG_KEY];
    char ch_value[RUN_PARA_LEN_PATH];
    int read_num = RUN_PARA_LEN_PATH + RUN_PARA_LEN_LOG_KEY + 20;
    char line[read_num];
    int i;
    int equal_num = 0;
    int equal_pos;
    int index;
    bool flag = false;
    bool ignore = false;


    f_conf = fopen(conf_file, "r");
    while (fgets(line, read_num - 1, f_conf) != NULL)
    {
        ignore = false;
        for (i = 0; i < strlen(line); i++)
        {
            if (! isblank(line[i]))
            {
                if (line[i] == '#')
                    ignore = true;
                break;
            }
        }
        // 如果第一个非空字符为#则忽略该行，跳出循环，读取下一行
        if (ignore)
        {
            // printf("ignore this line: %s\n", line);
            continue;
        }

        strcpy(key, "");
        strcpy(ch_value, "");
        // 等号的数量
        equal_num = 0;
        flag = false;
        for (i = 0; i < strlen(line); i++)
        {
            if (! isspace(line[i]))
                flag = true;
            if (line[i] == '=')
            {
                equal_num += 1;
                equal_pos = i;
            }
        }

        // 等号的数量不等于１且出现非空字符，提示错误，直接退出
        // 否则忽略该行，跳出循环，读取下一行
        if (equal_num != 1)
        {
            if (flag)
            {
                printf("read conf fail at line: %s, equal_num != 1\n", line);
                exit(EXIT_FAILURE);
            }
            else
            {
                // printf("ignore this empty line\n");
                continue;
            }
        }
        else
        {
            flag = false;
            // 从该行的第一个字符到等号结束，中间的非空字符赋值给key数组
            // 正确读取key值，则flag设为真
            for (i = 0, index = 0; i < equal_pos; i++)
            {
                if (! isblank(line[i]))
                {
                    flag = true;
                    key[index] = line[i];
                    index++;
                }
            }

            //　正确读取key值，则继续读取value值
            // 否则报错退出
            if (flag)
            {
                // key数组最后一位设为空白字符
                key[index] = '\0';
                flag = false;
                // 从等号字符出现位置的下一位开始，读取到该行结束
                for (i = (equal_pos + 1), index = 0; i < strlen(line); i++)
                {
                    if ((! isblank(line[i])) && line[i] != '\n')
                    {
                        flag = true;
                        ch_value[index] = line[i];
                        index++;
                    }
                }
                // 如果value值读取正常，则最后一个字符设为空白字符
                // 否则报错退出
                if (flag)
                    ch_value[index] = '\0';
                else
                {
                    printf("read conf fail at line: %s, have no value\n", line);
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                printf("read conf fail at line: %s\n, have no key\n", line);
                exit(EXIT_FAILURE);
            }


        }

        // 读取到的value值，如果出现换行符则替换为空白字符
        for (i = 0; i < strlen(ch_value); i++)
        {
            if (ch_value[i] == '\n')
            {
                ch_value[i] = '\0';
                break;
            }
        }

        // 读取到的配置信息，赋值给结构
        if (strcmp(key, "master") == 0)
        {
            strncpy(instance_para->master, ch_value, RUN_PARA_LEN_IP_ADDR - 1);
            instance_para->master[RUN_PARA_LEN_IP_ADDR -1] = '\0';
        }
        else if (strcmp(key, "master_port") == 0)
            instance_para->master_port = atoi(ch_value);
        else if (strcmp(key, "node_port") == 0)
            instance_para->node_port = atoi(ch_value);
        else if (strcmp(key, "log_path") == 0)
        {
            strncpy(instance_para->log_path, ch_value, RUN_PARA_LEN_PATH - 1);
            instance_para->log_path[RUN_PARA_LEN_PATH - 1] = '\0';
        }
        else if (strcmp(key, "l/home/liushanog_level") == 0)
        {
            strncpy(instance_para->log_level, ch_value, RUN_PARA_LEN_LOG_LEVEL - 1);
            instance_para->log_level[RUN_PARA_LEN_LOG_LEVEL - 1] = '\0';
        }
        else if (strcmp(key, "role") == 0)
        {
            strncpy(instance_para->role, ch_value, RUN_PARA_LEN_ROLE - 1);
            instance_para->role[RUN_PARA_LEN_ROLE - 1] = '\0';
        }
        else if (strcmp(key, "node_list") == 0)
        {
            strncpy(instance_para->node_list, ch_value, RUN_PARA_LEN_NODE_LIST - 1);
            instance_para->node_list[RUN_PARA_LEN_NODE_LIST - 1] = '\0';
        }
        else if (strcmp(key, "temp_dir") == 0)
        {
            strncpy(instance_para->temp_dir, ch_value, RUN_PARA_LEN_PATH - 1);
            instance_para->temp_dir[RUN_PARA_LEN_PATH - 1] = '\0';
        }
        else if (strcmp(key, "sort_desc") == 0)
        {
            if (strcmp(ch_value, "true") == 0)
                instance_para->sort_desc = true;
            else if (strcmp(ch_value, "false") == 0)
                instance_para->sort_desc = false;
            else
            {
                fputs("get sort_desc error, use default value: false ", stderr);
            }
        }
    }


    fclose(f_conf);

    if (instance_para->master[0] == '#' || instance_para->master_port == -1 ||
            instance_para->node_port == -1 || instance_para->log_path[0] == '#' ||
            instance_para->log_level[0] == '#' || instance_para->role[0] == '#' ||
            instance_para->node_list[0] == '#' || instance_para->temp_dir[0] == '#')
    {
        fputs("Error! At last read conf fail, please check you conf file, now exit", stderr);
        exit(EXIT_FAILURE);
    }
}



int get_run_para_get_conf_pos(int num, char **input_para)
{
    // 配置文件位置初始化为异常值
    int conf_pos = CONF_ERR;
    int i;
    FILE * f_conf;

    // 判断输入参数的个数
    if (num >= (INPUT_PARA_LEN - 1) )
    {
        for (i = 0; i < num; i++)
        {
            if (strcmp(input_para[i], CONF_FILE_PARA_MARK) == 0)
            {
                i++;
                // 检查能否正常打开配置文件
                f_conf = fopen(input_para[i], "r");
                if (f_conf != NULL)
                {
                    // 如匹配到文件路径，退出循环
                    conf_pos = i;
                    fclose(f_conf);
                    break;
                }
                else
                    printf("Error! can not read conf file \"%s\" \n", input_para[i]);
            }
        }
    }

    // 如果检查配置文件通过，返回配置文件的位置，否则打印帮助信息，退出程序
    if (conf_pos != CONF_ERR)
        return conf_pos;
    else
    {
        usage(input_para[0]);
        exit(2);
    }
}


void get_run_para_get_sort_file(int conf_pos, char **args, struct run_para *instance_para)
{
    int index;
    FILE * f_sort_file;
    bool flag = false;

    for (index = 1; index < INPUT_PARA_LEN; index++)
        if (index != conf_pos && index != (conf_pos - 1) )
        {
            if ( (f_sort_file = fopen(args[index], "r") ) != NULL)
            {
                strncpy(instance_para->sort_file_path, args[index], RUN_PARA_LEN_PATH - 1);
                instance_para->sort_file_path[RUN_PARA_LEN_PATH - 1] = '\0';
                flag = true;
                break;
            }

        }
    //　如果读取sort_file失败，则打印失败信息，然后退出
    if (! flag)
    {
        fputs("read sort file fail, or not find sort file\n", stderr);
        usage(args[0]);
        exit(2);
    }
}