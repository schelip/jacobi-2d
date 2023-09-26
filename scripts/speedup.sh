#!/bin/bash

make clean
make

output_file="speedup_results.csv"
rm "$output_file"
echo "PROGRAM,DATASET,THREADS,PROCESSES,PARALLEL_TIME,SPEEDUP" >> "$output_file"

# Define the executable programs and dataset sizes
programs=("pthread" "mpi" "mpi-pthread")
datasets=("SMALL" "MEDIUM" "LARGE")

# Define the number of repetitions
repetitions=10

# Declare an associative array to store serial execution times for each dataset
declare -A serial_times

# Function to calculate speedup
calculate_speedup() {
    program=$1
    dataset=$2
    threads=$3
    processes=$4

    # Retrieve the serial time for the current dataset from the associative array
    serial_time=${serial_times[$dataset]}

    parallel_time=0

    echo "Calculating speedup for jacobi-2d-$program with dataset $dataset, $threads threads, and $processes processes..."

    # Execute the parallel program and calculate the average time
    for ((i=1; i<=$repetitions; i++)); do
        if [ "$program" == "pthread" ]; then
            parallel_time=$(echo "$parallel_time + $(env time -f "%e" bin/jacobi-2d-$program -d $dataset -t $threads 2>&1)" | bc -l)
        elif [ "$program" == "mpi" ]; then
            parallel_time=$(echo "$parallel_time + $(env time -f "%e" mpirun -np $processes bin/jacobi-2d-$program -d $dataset 2>&1)" | bc -l)
        else
            parallel_time=$(echo "$parallel_time + $(env time -f "%e" mpirun -np $processes bin/jacobi-2d-$program -d $dataset -t $threads 2>&1)" | bc -l)
        fi
        echo "jacobi-2d-$program $dataset execution $i time: $parallel_time"
    done

    # Calculate the average time
    parallel_time=$(bc -l <<< "$parallel_time / $repetitions")
    echo "jacobi-2d-$program $dataset average execution time: $parallel_time"

    # Calculate speedup
    speedup=$(bc -l <<< "$serial_time / $parallel_time")
    echo -e "Speedup of $program with dataset $dataset, $threads threads, and $processes processes: $speedup\n"

    # Save results to file
    echo "$program,$dataset,$threads,$processes,$parallel_time,$speedup" >> "$output_file"
}

# Calculate the serial execution time once for each dataset and store it in the associative array
for dataset in "${datasets[@]}"; do
    serial_time=0
    for ((i=1; i<=$repetitions; i++)); do
        echo "Calculating jacobi-2d-serial execution time for dataset $dataset..."
        serial_time=$(echo "$serial_time + $(env time -f "%e" bin/jacobi-2d-serial -d $dataset 2>&1)" | bc -l)
        echo "jacobi-2d-serial $dataset execution $i time: $serial_time"
    done

    serial_times[$dataset]=$(bc -l <<< "$serial_time / $repetitions")
    echo -e "jacobi-2d-serial $dataset average execution time: ${serial_times[$dataset]}\n"
    echo "serial,$dataset,1,1,${serial_times[$dataset]},1" >> "$output_file"
done

# Loop through all programs, datasets, threads, and processes

for dataset in "${datasets[@]}"; do
    threads=(2 4 8 16)
    for t in "${threads[@]}"; do
        calculate_speedup "pthread" "$dataset" "$t" "1"
    done
done

for dataset in "${datasets[@]}"; do
    processes=(2 4 8)
    for p in "${processes[@]}"; do
        calculate_speedup "mpi" "$dataset" "1" "$p"
    done
done

for dataset in "${datasets[@]}"; do
    processes=(2 4 8)
    for p in "${processes[@]}"; do
        threads=(2 4 8 16)
        for t in "${threads[@]}"; do
            calculate_speedup "mpi-pthread" "$dataset" "$t" "$p"
        done
    done
done
