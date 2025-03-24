//
// Created by 10152 on 2024/10/30.
//
// 第一轮: 进程 1, 3, 5, 7 接收数据
if (k == 0) {
if (world_rank % 2 == 1 && partner_receive >= 0) { // 进程 1, 3, 5, 7
int received_value;
MPI_Recv(&received_value, 1, MPI_INT, partner_receive, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
partial_result += received_value;
//                std::cout << "Round 1: Process " << world_rank << " received from " << partner_receive << ", prefix sum: " << partial_result << std::endl;
}
}
// 第二轮: 进程 3, 7 接收数据
else if (k == 1) {
if (world_rank % 4 == 3 && partner_receive >= 0) { // 进程 3, 7
int received_value;
MPI_Recv(&received_value, 1, MPI_INT, partner_receive, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
partial_result += received_value;
//                std::cout << "Round 2: Process " << world_rank << " received from " << partner_receive << ", prefix sum: " << partial_result << std::endl;
}
}
// 第三轮: 进程 7 接收数据
else if (k == 2) {
if (world_rank % 8 == 7 && partner_receive >= 0) { // 进程 7
int received_value;
MPI_Recv(&received_value, 1, MPI_INT, partner_receive, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
partial_result += received_value;
//                std::cout << "Round 3: Process " << world_rank << " received from " << partner_receive << ", prefix sum: " << partial_result << std::endl;
}
}

// 发送数据
if (k == 0) {
// 第一轮: 进程 0, 2, 4, 6 发送数据
if (world_rank % 2 == 0 && partner_send < world_size) { // 进程 0, 2, 4, 6
MPI_Send(&partial_result, 1, MPI_INT, partner_send, 0, MPI_COMM_WORLD);
//                std::cout << "Round 1: Process "  << world_rank << " sent to " << partner_send << ", partial sum: " << partial_result << std::endl;
}
} else if (k == 1) {
// 第二轮: 进程 1, 5 发送数据
if (world_rank % 4 == 1 && partner_send < world_size) { // 进程 1, 5
MPI_Send(&partial_result, 1, MPI_INT, partner_send, 0, MPI_COMM_WORLD);
//                std::cout << "Round 2: Process " << world_rank << " sent to " << partner_send << ", partial sum: " << partial_result << std::endl;
}
} else if (k == 2) {
// 第三轮: 进程 3 发送数据
if (world_rank % 8 == 3 && partner_send < world_size) { // 进程 3
MPI_Send(&partial_result, 1, MPI_INT, partner_send, 0, MPI_COMM_WORLD);
//                std::cout << "Round 3: Process " << world_rank << " sent to " << partner_send << ", partial sum: " << partial_result << std::endl;
}
}
}