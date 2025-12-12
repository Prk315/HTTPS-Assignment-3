# Assignment 3: K-Nearest Neighbors with K-D Trees

## Description

This repository contains the implementation of Assignment 3 for the HPPS course. The project implements k-nearest neighbor (k-NN) search using both brute-force and k-d tree approaches for efficient spatial search in multi-dimensional spaces.

### Project Structure

- `Assignment3/HandIn/` - Complete implementation with all source files
- `handout/` - Original materials and reference code provided for the assignment

## Implementations

### Core Components

1. **K-D Tree Implementation** (`kdtree.c/h`)
   - Efficient spatial partitioning data structure
   - Recursive tree construction with median-based splitting
   - Optimized k-NN search with branch pruning

2. **Brute Force k-NN** (`bruteforce.c/h`)
   - Baseline implementation for comparison
   - Linear search through all points

3. **Utilities** (`util.c/h`)
   - Euclidean distance calculation
   - Sorted k-nearest neighbor tracking with `insert_if_closer()`

4. **I/O Operations** (`io.c/h`)
   - Point and query data reading/writing
   - Index file handling

5. **Quicksort** (`sort.c/h`)
   - Custom quicksort implementation with auxiliary data support
   - Used for k-d tree construction

### Executables

- `knn-genpoints` - Generate random test points
- `knn-bruteforce` - Brute-force k-NN search
- `knn-kdtree` - K-d tree accelerated k-NN search
- `knn-svg` - Visualize k-d tree partitioning (2D only)
- `sort-example` - Demonstrate sorting functionality

## Building and Running

### Prerequisites

See `requirements.txt` for system dependencies.

### Build

```bash
cd Assignment3/HandIn
make
```

### Usage Examples

Generate test data:
```bash
make points    # Generate 10000 2D points
make queries   # Generate 1000 2D queries
```

Run k-NN search:
```bash
# Brute force approach
./knn-bruteforce points queries 5 indexes

# K-d tree approach (faster for large datasets)
./knn-kdtree points queries 5 indexes
```

Generate visualization:
```bash
make points.svg  # Creates SVG showing k-d tree partitioning
```

### Clean

```bash
make clean
```

## Report

The assignment report is written in LaTeX (`Assignment3/a3_handin.tex`) and compiled to PDF format using pdflatex.
