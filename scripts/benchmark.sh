#!/bin/bash

make clean
make

output_file="out/benchmark_results_2.csv"
rm "$output_file"
echo "PROGRAM,DATASET,N,CYCLES,INSTRUCTIONS,CPU-CLOCK,TASK-CLOCK,CPU-MIGRATIONS,PAGE-FAULTS,BRANCHES,BRANCH-MISSES,CONTEXT-SWITCHES,CACHE-REFERENCES,CACHE-MISSES" >> "$output_file"

# Define the dataset sizes and amount of threads
datasets=("SMALL" "MEDIUM" "LARGE")
ns=(2 4 8 16)

# Define the number of repetitions
repetitions=10

# Path where the perf binary is
perf_path="./ext/perf"

# Function to execute perf stat and save the results to the CSV file
execute_perf_stat() {
    program=$1
    dataset=$2
    n=$3

    echo "Running perf stat for jacobi-2d-$program with dataset $dataset and $n threads/processes..."

    # Define the command to execute based on the program type
    if [ "$program" == "serial" ]; then
        command="bin/jacobi-2d-serial -d $dataset"
    elif [ "$program" == "pthread" ]; then
        command="bin/jacobi-2d-pthread -d $dataset -t $n"
    elif [ "$program" == "mpi" ]; then
        command="mpirun --hostfile hostfile -np $n bin/jacobi-2d-mpi -d $dataset"
    else
        command="mpirun --hostfile hostfile -np $n bin/jacobi-2d-mpi-pthread -d $dataset -t $n"
    fi

    # Execute perf stat and capture the results
    perf_result=$($perf_path/perf stat -r $repetitions -e cycles,instructions,cpu-clock,task-clock,cpu-migrations,page-faults,branches,branch-misses,context-switches,cache-references,cache-misses $command 2>&1)
    
    # Extract the relevant metrics from the perf stat output
    cycles=$(echo "$perf_result" | grep "cycles" | awk '{print $1}')
    instructions=$(echo "$perf_result" | grep "instructions" | awk '{print $1}')
    cpu_clock=$(echo "$perf_result" | grep "cpu-clock" | awk '{printf("%06.2f\n", $1)}')
    task_clock=$(echo "$perf_result" | grep "task-clock" | awk '{printf("%06.2f\n", $1)}')
    cpu_migrations=$(echo "$perf_result" | grep "cpu-migrations" | awk '{print $1}')
    page_faults=$(echo "$perf_result" | grep "page-faults" | awk '{print $1}')
    branches=$(echo "$perf_result" | grep "branches" | awk '{print $1}')
    branch_misses=$(echo "$perf_result" | grep "branch-misses" | awk '{print $1}')
    context_switches=$(echo "$perf_result" | grep "context-switches" | awk '{print $1}')
    cache_references=$(echo "$perf_result" | grep "cache-references" | awk '{print $1}')
    cache_misses=$(echo "$perf_result" | grep "cache-misses" | awk '{print $1}')

    # Save the results to the CSV file
    echo "$program,$dataset,$n,$cycles,$instructions,$cpu_clock,$task_clock,$cpu_migrations,$page_faults,$branches,$branch_misses,$context_switches,$cache_references,$cache_misses" >> "$output_file"

    echo "perf stat for jacobi-2d-$program with dataset $dataset and $n threads/processes completed."
}

# Loop through all programs, datasets, and thread/process counts
for dataset in "${datasets[@]}"; do
    execute_perf_stat "serial" "$dataset" "1"
done

for dataset in "${datasets[@]}"; do
    for n in "${ns[@]}"; do
        execute_perf_stat "pthread" "$dataset" "$n"
    done
done

for dataset in "${datasets[@]}"; do
    for n in "${ns[@]}"; do
        execute_perf_stat "mpi" "$dataset" "$n"
    done
done

for dataset in "${datasets[@]}"; do
    for n in "${ns[@]}"; do
        execute_perf_stat "mpi-pthread" "$dataset" "$n"
    done
done

echo "Perf stat results saved to $output_file."
