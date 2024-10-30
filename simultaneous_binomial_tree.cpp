// mpic++ -o simultaneous_binomial_tree simultaneous_binomial_tree.cpp
// mpirun -np 8 ./simultaneous_binomial_tree

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
    size_t length = local_value_list.size();
    int local_value = *next(local_value_list.begin(), world_rank);

    int partial_result = local_value;
    int local_result = local_value;

    int rounds = static_cast<int>(std::log2(world_size));

    for (int k = 0; k < rounds  ; k++) {
        int partner_receive = world_rank -  (int) pow(2, k) ;
        int partner_send = world_rank + (int) pow(2, k);


        if (partner_receive >= 0) {

            if (local_result != partial_result){
                local_result = partial_result;
            }
            int received_value;
            MPI_Recv(&received_value, 1, MPI_INT, partner_receive, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            partial_result = local_result + received_value;

        }

        if (partner_send < world_size) {
            MPI_Send(&local_result, 1, MPI_INT, partner_send, 0, MPI_COMM_WORLD);

            local_result = partial_result;
        }
    }

    std::cout << "Process " << world_rank << " has partial value: " << partial_result << std::endl;


    MPI_Finalize();
    return 0;
}