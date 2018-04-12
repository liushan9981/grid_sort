作用：
运行角色为master和node，其中node可以有多个，master只有一个。
需要处理的文本文件里面是纯整数，待处理的文本文件路径作为参数，master读取文件内容，把任务下发给node，node节点处理后，返回结果给master，master读取所有node返回的结果，再次处理后打印到终端

安装:
执行./install.sh 即可生成可执行文件 ./cmake-build-debug/grid_sort

编辑conf下的配置文件，分别为master.conf 和node.conf
首先在各个节点上运行node:
./grid_sort -c conf/node.conf

然后运行master:
./grid_sort -c conf/master.conf sort_file.txt


执行和输出结果示例：
 % cmake-build-debug/grid_sort -c conf/master.conf /home/liushan/mylab/clang/sort_sample/sort_list_5.txt
--------------sort result--------------
1    23455
2    23455
3    23455
4    23455
5    23455
6    23455
7    23455
8    23455
9    23455
10    23455
11    23455
12    23455

