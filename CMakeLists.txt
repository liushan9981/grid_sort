cmake_minimum_required(VERSION 3.8)
project(grid_sort)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c get_run_para.h get_run_para.c write_log.h write_log.c srv_master.c srv_node.c srv_node.h srv_master_node.h srv_master.h test.c sub_func/get_file_line_num.h sub_func/get_file_line_num.c sort_file/sort_file.c sort_file/bubble_sort.h sort_file/bubble_sort.c sort_file/sort_file.h sub_func/get_last_char.c sub_func/get_last_char.h sub_func/merge_file.c sub_func/merge_file.h usage.h usage.c test.h log_level.h)
add_executable(grid_sort ${SOURCE_FILES})