# Assignment 3: K-Nearest Neighbors

HPPS course assignment implementing k-NN search with k-d trees.

## What's in here

Two approaches to finding nearest neighbors in point clouds:
- Brute force (check every point)
- K-d tree (spatial partitioning for faster search)

The k-d tree version is way faster for large datasets since it can skip entire regions of space.

## File structure

```
kdtree.c/h       - K-d tree implementation
bruteforce.c/h   - Simple linear search
util.c/h         - Distance calculations and neighbor tracking
io.c/h           - Read/write binary point files
sort.c/h         - Quicksort (needed for tree construction)
```

## Programs you can run

- `knn-genpoints` - Makes random test points
- `knn-bruteforce` - Finds neighbors the slow way
- `knn-kdtree` - Finds neighbors using the tree
- `knn-svg` - Draws the tree partitioning (2D only)
- `sort-example` - Shows how the sorting works

## How to use

Build everything:
```bash
cd Assignment3/HandIn
make
```

Quick test:
```bash
make points queries    # Generate test data
./knn-bruteforce points queries 5 indexes
./knn-kdtree points queries 5 indexes
```

The number (5) is how many neighbors to find.

For visualization (2D points only):
```bash
make points.svg
```
Open `points.svg` in a browser to see how the tree splits the space.

Clean up:
```bash
make clean
```

## Notes

- The k-d tree gets built by recursively splitting on median values
- Distance function is standard Euclidean (sqrt of sum of squared diffs)
- Binary files use int32_t for integers and doubles for coordinates
- VLAs are used in `insert_if_closer()` - perfectly fine for C99

Everything compiles with `-Wall -Wextra -pedantic -std=c99` without warnings.
