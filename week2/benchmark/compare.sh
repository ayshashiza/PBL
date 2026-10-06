#!/bin/sh
set -eu

cd ~/PBL/week2

iterations=10000
benchmark_dir=benchmark

# Build a copy of the standalone reference with the same iteration count.
sed 's/#define ITERATIONS 1000000ULL/#define ITERATIONS 10000ULL/' \
    "$benchmark_dir/standalone.c" > "$benchmark_dir/standalone_10000.c"
gcc -O2 -Wall -Wextra -std=c11 \
    -o "$benchmark_dir/standalone_10000" "$benchmark_dir/standalone_10000.c"

# Prepare equal load-and-run cycles for the three-process simulator.
{
    i=0
    while [ "$i" -lt "$iterations" ]; do
        printf 'load\nrun\n'
        i=$((i + 1))
    done
    printf 'exit\n'
} > "$benchmark_dir/multiprocess_commands.txt"

# Start Logger before Core so its message queue is available.
./logger/logger > "$benchmark_dir/logger.log" 2>&1 &
logger_pid=$!
sleep 1

./core/core > "$benchmark_dir/core.log" 2>&1 &
core_pid=$!
sleep 1

# Time each workload and save outputs and elapsed times in benchmark/.
{
    printf 'Standalone reference:\n'
    /usr/bin/time -f 'Elapsed seconds: %e
CPU usage: %P
Maximum memory: %M KB' \
        -o "$benchmark_dir/standalone_time.txt" \
        "$benchmark_dir/standalone_10000"
    cat "$benchmark_dir/standalone_time.txt"

    printf '\nThree-process simulator (10,000 load/run cycles):\n'
    /usr/bin/time -f 'Elapsed seconds: %e
CPU usage: %P
Maximum memory: %M KB' \
        -o "$benchmark_dir/multiprocess_time.txt" \
        ./ui/ui < "$benchmark_dir/multiprocess_commands.txt" \
        > "$benchmark_dir/multiprocess_output.txt"
    cat "$benchmark_dir/multiprocess_time.txt"
} | tee "$benchmark_dir/comparison.txt"

wait "$core_pid" || true
wait "$logger_pid" || true
