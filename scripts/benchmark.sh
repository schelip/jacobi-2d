#!/bin/bash

make clean
make

output_file="perf_results.csv"
rm "$output_file"
echo "PROGRAM,DATASET,N,CPU_CLOCK,TASK_CLOCK,CONTEXT_SWITCHES,CPU_MIGRATIONS,PAGE_FAULTS,BRANCHES,BRANCH_MISSES,CACHE_REFERENCES,CACHE_MISSES,CYCLES,INSTRUCTIONS" >> "$output_file"

# Define the executable programs and dataset sizes
datasets=("SMALL")
ns=(2 4 8 16)

# Function to execute perf stat and save the results to the CSV file
execute_perf_stat() {
    program=$1
    dataset=$2
    n=$3

    echo "Running perf stat for jacobi-2d-$program with dataset $dataset and $n threads/processes..."

    # Define the command to execute based on the program type
    if [ "$program" == "pthread" ]; then
        command="bin/jacobi-2d-pthread -d $dataset -t $n"
    elif [ "$program" == "mpi" ]; then
        command="mpirun --hostfile hostfile -np $n bin/jacobi-2d-mpi -d $dataset"
    else
        command="mpirun --hostfile hostfile -np $n bin/jacobi-2d-mpi-pthread -d $dataset -t $n"
    fi

    # Execute perf stat and capture the results
    perf_result=$(perf stat -e cpu-clock,task-clock,context-switches,cpu-migrations,page-faults,branches,branch-misses,cache-references,cache-misses,cycles,instructions $command 2>&1)

    # Extract the relevant metrics from the perf stat output
    cpu_clock=$(echo "$perf_result" | grep "cpu-clock" | awk '{print $1}')
    task_clock=$(echo "$perf_result" | grep "task-clock" | awk '{print $1}')
    context_switches=$(echo "$perf_result" | grep "context-switches" | awk '{print $1}')
    cpu_migrations=$(echo "$perf_result" | grep "cpu-migrations" | awk '{print $1}')
    page_faults=$(echo "$perf_result" | grep "page-faults" | awk '{print $1}')
    branches=$(echo "$perf_result" | grep "branches" | awk '{print $1}')
    branch_misses=$(echo "$perf_result" | grep "branch-misses" | awk '{print $1}')
    cache_references=$(echo "$perf_result" | grep "cache-references" | awk '{print $1}')
    cache_misses=$(echo "$perf_result" | grep "cache-misses" | awk '{print $1}')
    cycles=$(echo "$perf_result" | grep "cycles" | awk '{print $1}')
    instructions=$(echo "$perf_result" | grep "instructions" | awk '{print $1}')

    # Save the results to the CSV file
    echo "$program,$dataset,$n,$cpu_clock,$task_clock,$context_switches,$cpu_migrations,$page_faults,$branches,$branch_misses,$cache_references,$cache_misses,$cycles,$instructions" >> "$output_file"

    echo "perf stat for jacobi-2d-$program with dataset $dataset and $n threads/processes completed."
}

# Loop through all programs, datasets, and thread/process counts
for dataset in "${datasets[@]}"; do
    for n in "${ns[@]}"; do
        execute_perf_stat "pthread" "$dataset" "$n"
    done

    for n in "${ns[@]}"; do
        execute_perf_stat "mpi" "$dataset" "$n"
    done

    for n in "${ns[@]}"; do
        execute_perf_stat "mpi-pthread" "$dataset" "$n"
    done
done

echo "Perf stat results saved to $output_file."
