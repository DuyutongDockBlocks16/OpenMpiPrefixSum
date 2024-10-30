// mpic++ -o normal_binomial_tree normal_binomial_tree.cpp
// mpirun -np 8 ./normal_binomial_tree


#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <list>
#include <cmath>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    list<int> local_value_list{1, 2, 0, 1, 2, 0, 1, 2};
//    list<int> local_value_list{1, 2, 0, 1};
    size_t length = local_value_list.size();
    int local_value = *next(local_value_list.begin(), world_rank);

    int partial_result = local_value;
//    std::cout << "Process " << world_rank << " has partial value: " << local_value << std::endl;
//    std::cout << length << std::endl;

    int rounds = static_cast<int>(std::log2(world_size));
    for (int k = 0; k < rounds; ++k) {
        int partner_receive = world_rank - (1 << k);
        int partner_send = world_rank + (1 << k);

//        std::cout << "k: " << k << " world_rank: " << world_rank << " partner_receive: " << partner_receive << " received from: " << partner_send << std::endl;

        if (world_rank % (int) pow(2, k + 1) == (int) pow(2, k + 1) - 1 && partner_receive >= 0) {
            int received_value;
            MPI_Recv(&received_value, 1, MPI_INT, partner_receive, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            partial_result += received_value;
//            std::cout << "Round 1: Process " << world_rank << " received from " << partner_receive << ", prefix sum: " << partial_result << std::endl;
        }

        if (world_rank % (int) pow(2, k + 1) == (int) pow(2, k) - 1 && partner_send < world_size) { // 进程 0, 2, 4, 6
            MPI_Send(&partial_result, 1, MPI_INT, partner_send, 0, MPI_COMM_WORLD);
//                std::cout << "Round 1: Process "  << world_rank << " sent to " << partner_send << ", partial sum: " << partial_result << std::endl;
        }
    }

    //    std::cout << "Process " << world_rank << " has partial value: " << partial_result << std::endl;

    for (int k = rounds - 2; k >= 0; --k) {
        int partner_receive = world_rank - (1 << k); // Receiving partner
        int partner_send = world_rank + (1 << k); // Sending partner


        // Handle receiving values
        if (partner_receive >= 0 && world_rank % (int) pow(2, k + 1) == (int) pow(2, k) - 1 ) {
            std::cout << "k: " << k << " world_rank: " << world_rank << " partner_receive: " << partner_receive << " partner_send: " << partner_send << std::endl;
            int received_value;
            MPI_Recv(&received_value, 1, MPI_INT, partner_receive, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            partial_result += received_value;
//            std::cout << "Process " << world_rank << " received from " << partner_receive << ": " << received_value << ", new partial sum: " << partial_result << std::endl;
        }
//
        // Handle sending values
        if (partner_send < world_size && world_rank % (int) pow(2, k + 1) == (int) pow(2, k + 1) - 1 ) {
            std::cout << "k: " << k << " world_rank: " << world_rank << " partner_receive: " << partner_receive << " partner_send: " << partner_send << std::endl;
            MPI_Send(&partial_result, 1, MPI_INT, partner_send, 0, MPI_COMM_WORLD);
//            std::cout << "Process " << world_rank << " sent to " << partner_send << ": " << partial_result << std::endl;
        }
    }


    std::cout << "Process " << world_rank << " has partial value: " << partial_result << std::endl;

    MPI_Finalize();
    return 0;
}
