// This program is an example of how to use hpps_quicksort() from
// sort.h.
//
// We sort the strings containing in the argv array provided to
// main().  However, instead of creating an sorting the strings
// directly, we sort an array of *indexes*, where each index
// identifies a string in argv.  This shows how we can look up
// "auxiliary" information inside the sorting function.
//
// Usage:
//
// $ ./sort-example troels hpps diku
// ./sort-example
// diku
// hpps
// troels

#include <string.h>
#include <stdio.h>
#include "sort.h"

int cmp_strings(const void* x, const void* y, void* aux) {
  // Cast x to int pointer and dereference.
  int i = *(const int*)x;
  // Cast y to int pointer and dereference.
  int j = *(const int*)y;
  // Cast aux to char* pointer.
  char** argv = aux;
  // Treat i and j as indexes into argv, fetch the strings, and use
  // strcmp to compare them.
  return strcmp(argv[i],argv[j]);
}

int main(int argc, char** argv) {
  int indexes[argc];

  for (int i = 0; i < argc; i++) {
    indexes[i] = i;
  }

  hpps_quicksort(indexes, argc, sizeof(int), cmp_strings, argv);

  for (int i = 0; i < argc; i++) {
    puts(argv[indexes[i]]);
  }
}
