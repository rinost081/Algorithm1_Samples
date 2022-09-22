#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int linear_search(int* table, int length, int x) {
  for (int i = 0; i < length; i++) {
    if (x == table[i]) {
      return true;
    }
  }

  return false;
}

int main() {
  int* table;
  int length;
  int target = 1;

  printf("Type in the length of the array...\n");
  scanf("%d", &length);

  table = (int*)malloc(length * sizeof(int));

  printf("Type in the table numbers (number >= 0)...\n");
  for (int i = 0; i < length; i++) {
    scanf("%d", &table[i]);
  }

  while (true) {
    printf("\nType in the target number... (Type in number < 0 to exit)\n");
    scanf("%d", &target);

    if (target < 0) {
      break;
    }

    double start_clock, end_clock;
    start_clock = (double)clock();
    bool search_result = linear_search(table, length, target);
    end_clock = (double)clock();

    printf("CPU execution time: %.8lf s.\nIs target in the table: %s\n",
           (end_clock - start_clock) / CLOCKS_PER_SEC, search_result ? "Yes" : "No");
  }

  free(table);

  return 0;
}
