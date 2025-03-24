// cd /mnt/d/0x02_LearningAndWorking/Aalto/Thesis/OpenMpiTest
// mpirun -np 4 ./mpi_scan_demo

#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    // 初始化MPI环境
    MPI_Init(&argc, &argv);

    int world_size;
    int world_rank;

    // 获取进程的数量
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // 获取当前进程的rank
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // 初始化每个进程的局部数据
    int local_value = world_rank + 1; // 每个进程持有自己的值，简化为 rank+1
    int scan_result = 0;

    // 打印每个进程持有的初始值
    std::cout << "Process " << world_rank << " has initial value: " << local_value << std::endl;

    // 并行前缀和计算 (关联扫描)
    MPI_Scan(&local_value, &scan_result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // 打印每个进程计算后的前缀和结果
    std::cout << "Process " << world_rank << " has prefix sum: " << scan_result << std::endl;

    // 结束MPI环境
    MPI_Finalize();
    return 0;
}
