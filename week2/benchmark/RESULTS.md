# Week 2 Benchmark Results

## Workload

Both versions performed 10,000 load-and-run cycles using the values 10, 20, 30, and 40. Each cycle completed with ACC=100 and PC=4.

## Observed timings

| Version | Iterations | Elapsed time |
|---|---:|---:|
| Standalone reference | 10,000 | 0.000033 seconds |
| Three-process simulator | 10,000 | 3.35 seconds |

The multiprocess output recorded 10,000 successful runs. The standalone result check was 1,040,000.

## Timing notes

The standalone program measured its loop internally. The multiprocess timing wrapped the UI run; Core and Logger were started before timing began. The multiprocess measurement includes UI startup and shutdown, plus UI/Core/Logger message handling. These are observed runtimes, not a precise speedup ratio.

## Reproduce

From the project directory, run:

    sh ~/PBL/week2/benchmark/compare.sh

The script saves the comparison and run output in this folder.
