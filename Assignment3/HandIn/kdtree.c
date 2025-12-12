#include "kdtree.h"
#include "sort.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

struct node {
  // Index of this node's point in the corresponding 'indexes' array.
  int point_index;

  // Axis along which this node has been splot.
  int axis;

  // The left child of the node; NULL if none.
  struct node *left;

  // The right child of the node; NULL if none.
  struct node *right;
};

struct kdtree {
  int d;
  const double *points;
  struct node* root;
};

// Auxiliary data structure for the comparator
struct comparator_data {
  const double *points;
  int d;
  int axis;
};

// Comparator function for sorting indexes by their coordinate values
static int compare_indexes(const void *a, const void *b, void *arg) {
  const int *index_a = (const int *)a;
  const int *index_b = (const int *)b;
  struct comparator_data *data = (struct comparator_data *)arg;

  double val_a = data->points[(*index_a) * data->d + data->axis];
  double val_b = data->points[(*index_b) * data->d + data->axis];

  if (val_a < val_b) {
    return -1;
  } else if (val_a > val_b) {
    return 1;
  } else {
    return 0;
  }
}

struct node* kdtree_create_node(int d, const double *points,
                                int depth, int n, int *indexes) {
  if (n == 0) {
    return NULL;
  }

  // Choose axis based on depth
  int axis = depth % d;

  // Set up comparator data
  struct comparator_data comp_data;
  comp_data.points = points;
  comp_data.d = d;
  comp_data.axis = axis;

  // Sort the indexes by the points along the chosen axis
  hpps_quicksort(indexes, n, sizeof(int), compare_indexes, &comp_data);

  // Pick the median
  int median = n / 2;

  // Create the node
  struct node *node = malloc(sizeof(struct node));
  node->point_index = indexes[median];
  node->axis = axis;

  // Recursively build left and right subtrees
  node->left = kdtree_create_node(d, points, depth + 1, median, indexes);
  node->right = kdtree_create_node(d, points, depth + 1, n - median - 1, indexes + median + 1);

  return node;
}

struct kdtree *kdtree_create(int d, int n, const double *points) {
  struct kdtree *tree = malloc(sizeof(struct kdtree));
  tree->d = d;
  tree->points = points;

  int *indexes = malloc(sizeof(int) * n);

  for (int i = 0; i < n; i++) {
    indexes[i] = i;
  }

  tree->root = kdtree_create_node(d, points, 0, n, indexes);

  free(indexes);

  return tree;
}

void kdtree_free_node(struct node *node) {
  if (node == NULL) {
    return;
  }
  kdtree_free_node(node->left);
  kdtree_free_node(node->right);
  free(node);
}

void kdtree_free(struct kdtree *tree) {
  kdtree_free_node(tree->root);
  free(tree);
}

static void recompute_radius(const struct kdtree *tree,
                             int k,
                             const double *query,
                             const int *closest,
                             double *radius) {
  int valid = 0;
  double maxd = 0.0;

  for (int i = 0; i < k; i++) {
    int idx = closest[i];
    if (idx == -1) {
      continue;
    }
    double dist = distance(tree->d,
                           &tree->points[idx * tree->d],
                           query);
    if (dist > maxd) {
      maxd = dist;
    }
    valid++;
  }

  if (valid < k) {
    *radius = INFINITY;
  } else {
    *radius = maxd;
  }
}

void kdtree_knn_node(const struct kdtree *tree, int k, const double* query,
                     int *closest, double *radius,
                     const struct node *node) {
  if (node == NULL) {
    return;
  }

  // Check if this node's point should be added to closest
  int updated = insert_if_closer(k, tree->d, tree->points, closest, query, node->point_index);

  if (updated) {
    recompute_radius(tree, k, query, closest, radius);
  }

  // Get the coordinate of this node's point along the split axis
  double node_coord = tree->points[node->point_index * tree->d + node->axis];
  double diff = node_coord - query[node->axis];

  // Recurse into children based on the split
  if (diff >= 0 || *radius > fabs(diff)) {
    kdtree_knn_node(tree, k, query, closest, radius, node->left);
  }

  if (diff <= 0 || *radius > fabs(diff)) {
    kdtree_knn_node(tree, k, query, closest, radius, node->right);
  }
}

int* kdtree_knn(const struct kdtree *tree, int k, const double* query) {
  int* closest = malloc(k * sizeof(int));
  double radius = INFINITY;

  for (int i = 0; i < k; i++) {
    closest[i] = -1;
  }

  kdtree_knn_node(tree, k, query, closest, &radius, tree->root);

  return closest;
}

void kdtree_svg_node(double scale, FILE *f, const struct kdtree *tree,
                     double x1, double y1, double x2, double y2,
                     const struct node *node) {
  if (node == NULL) {
    return;
  }

  double coord = tree->points[node->point_index*2+node->axis];
  if (node->axis == 0) {
    // Split the X axis, so vertical line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            coord*scale, y1*scale, coord*scale, y2*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, coord, y2,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    coord, y1, x2, y2,
                    node->right);
  } else {
    // Split the Y axis, so horizontal line.
    fprintf(f, "<line x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" stroke-width=\"1\" stroke=\"black\" />\n",
            x1*scale, coord*scale, x2*scale, coord*scale);
    kdtree_svg_node(scale, f, tree,
                    x1, y1, x2, coord,
                    node->left);
    kdtree_svg_node(scale, f, tree,
                    x1, coord, x2, y2,
                    node->right);
  }
}

void kdtree_svg(double scale, FILE* f, const struct kdtree *tree) {
  assert(tree->d == 2);
  kdtree_svg_node(scale, f, tree, 0, 0, 1, 1, tree->root);
}
