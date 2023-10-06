#!/bin/bash
make clean
make

# Function to generate a random seed
generate_random_seed() {
  echo $((RANDOM % 1000))
}

# Define amount of threads
ns=(2 4 8 16)

# Number of random seeds to be tested
num_seeds=10

# Execute the programs for various random seeds and compare the outputs
for ((i=1; i<=num_seeds; i++)); do
    seed=$(generate_random_seed)
    echo "Testing with random seed: $seed"
    serial=$(bin/jacobi-2d-serial -s $seed -p)

    error=0
    
    for n in "${ns[@]}"; do
        pthread=$(bin/jacobi-2d-pthread -s $seed -t $n -p)
        mpi=$(mpirun --hostfile hostfile -np $n bin/jacobi-2d-mpi -s $seed -p)
        mpi_pthread=$(mpirun --hostfile hostfile -np $n bin/jacobi-2d-mpi-pthread -s $seed -t $n -p)

        # Compare the program outputs
        if [ "$serial" != "$pthread" ]; then
            echo "Error: 'jacobi-2d-pthread' produced a different output for seed $seed when executing with $n threads/processes."
            error=1
            break
        fi
        echo "OK 'jacobi-2d-pthread' produced the same output for seed $seed when executing with $n threads/processes."

        if [ "$serial" != "$mpi" ]; then
            echo "Error: 'jacobi-2d-mpi' produced a different output for seed $seed when executing with $n threads/processes."
            error=1
            break
        fi
        echo "OK 'jacobi-2d-mpi' produced the same output for seed $seed when executing with $n threads/processes."

        if [ "$serial" != "$mpi_pthread" ]; then
            echo "Error: 'jacobi-2d-mpi-pthread' produced a different output for seed $seed when executing with $n threads/processes."
            error=1
            break
        fi
        echo "OK 'jacobi-2d-mpi-pthread' produced the same output for seed $seed when executing with $n threads/processes."
    done

    if [ $error -eq 0 ]; then
        echo "All programs resulted the same output for seed $seed."
    else
        break
    fi
done
