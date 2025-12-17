#include "util.h"
#include <math.h>
#include <stdio.h>
#include <assert.h>

double distance(int d, const double *x, const double *y) {
    double sum = 0.0;
    for (int i = 0; i < d; i++) {
        double diff = x[i] - y[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

int insert_if_closer(int k, int d,
                     const double *points, int *closest, const double *query,
                     int candidate) {
    // Compute candidate distance.
    double cand_dist = distance(d, &points[candidate * d], query);

    // Build local arrays of distances and indexes.
    double dist[/*VLA*/k];
    int idx[/*VLA*/k];

    int valid = 0;
    for (int i = 0; i < k; i++) {
        idx[i] = closest[i];
        if (closest[i] == -1) {
            dist[i] = INFINITY;
        } else {
            dist[i] = distance(d, &points[closest[i] * d], query);
            valid++;
        }
    }

    if (valid < k) {
        // We have free slots; must insert candidate.
        int insert_pos = -1;
        for (int i = 0; i < k; i++) {
            if (idx[i] == -1) {
                insert_pos = i;
                break;
            }
        }
        if (insert_pos == -1) {
            insert_pos = valid;
        }
        idx[insert_pos] = candidate;
        dist[insert_pos] = cand_dist;
    } else {
        // Full; only insert if candidate is closer than the farthest.
        int far = 0;
        for (int i = 1; i < k; i++) {
            if (dist[i] > dist[far]) {
                far = i;
            }
        }
        if (cand_dist >= dist[far]) {
            // No change.
            return 0;
        }
        idx[far] = candidate;
        dist[far] = cand_dist;
    }

    // Sort by distance ascending (simple insertion sort).
    for (int i = 1; i < k; i++) {
        double keyd = dist[i];
        int keyi = idx[i];
        int j = i - 1;
        while (j >= 0 && dist[j] > keyd) {
            dist[j + 1] = dist[j];
            idx[j + 1] = idx[j];
            j--;
        }
        dist[j + 1] = keyd;
        idx[j + 1] = keyi;
    }

    // Write back.
    for (int i = 0; i < k; i++) {
        closest[i] = idx[i];
    }

    return 1;
}
