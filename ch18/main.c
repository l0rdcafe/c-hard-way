#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void die(const char* message) {
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

typedef int (*sort_cb) (int a, int b);

int* bubble_sort(int* nums, int count, sort_cb cb) {
  int temp = 0;
  int* target = malloc(count * sizeof(int));

  if (target == NULL) {
    die("Memory error");
  }

  memcpy(target, nums, count * sizeof(int));

  for (int i = 0; i < count; i++) {
    for (int j = 0; j < count - 1; j++) {
      if (cb(target[j], target[j + 1]) > 0) {
        temp = target[j + 1];
        target[j + 1] = target[j];
        target[j] = temp;
      }
    }
  }
  return target;
}

int sorted_order(int a, int b) {
  return a - b;
}

int reverse_order(int a, int b) {
  return b - a;
}

int strange_order(int a, int b) {
  if (a == 0 || b == 0) {
    return 0;
  } else {
    return a % b;
  }
}

void test_sorting(int* nums, int count, sort_cb cb) {
  int* sorted = bubble_sort(nums, count, cb);
  if (sorted == NULL) {
    die("Failed to sort as requested");
  }

  for (int i = 0; i < count; i++) {
    printf("%d ", sorted[i]);
  }
  printf("\n");
  free(sorted);

  unsigned char* data = (unsigned char*) cb;
  for (int i = 0; i < 25; i++) {
    printf("%02x:", data[i]);
  }
  printf("\n");
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    die("USAGE: a.out 4 3 1 5 6");
  }

  int count = argc - 1;
  char** inputs = argv + 1;
  int* nums = malloc(count * sizeof(int));

  if (nums == NULL) {
    die("Memory error");
  }

  for (int i = 0; i < count; i++) {
    nums[i] = atoi(inputs[i]);
  }

  test_sorting(nums, count, sorted_order);
  test_sorting(nums, count, reverse_order);
  test_sorting(nums, count, strange_order);

  free(nums);
  return 0;
}
