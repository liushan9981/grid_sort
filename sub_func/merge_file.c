//
// Created by liushan on 17-9-2.
//
#include <stdio.h>
#include "../write_log.h"
#include "../log_level.h"
#include "../get_run_para.h"

void merge_file(const struct run_para *run_para_info, char * file_pre, char * file_res, int num,
                const struct log_level * log_write_level, FILE * f_log_file)
{
    int index;
    FILE * f_read;
    FILE * f_write;
    char ch;
    char file_path[100];
    char log_msg[2048];

    f_write = fopen(file_res, "w");
    if (f_write == NULL)
    {
        sprintf(log_msg, "from merge_file open file \"%s\" WRITE failed", file_res);
        write_log(run_para_info->log_level, &(log_write_level->error), log_msg, f_log_file);
    }

    for (index = 0; index < num; index++)
    {
        sprintf(file_path, "%s%d", file_pre, index);
        sprintf(log_msg, "from merge_file file_path: %s", file_path);
        write_log(run_para_info->log_level, &(log_write_level->info), log_msg, f_log_file);
        f_read = fopen(file_path, "r");
        if (f_read == NULL)
        {
            sprintf(log_msg, "from merge_file read file \"%s\"failed", file_path);
            write_log(run_para_info->log_level, &(log_write_level->error), log_msg, f_log_file);
        }


        while ( (ch = getc(f_read) ) != EOF)
        {
            putc(ch, f_write);
        }
        fclose(f_read);

    }

    fclose(f_write);

}