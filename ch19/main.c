#include "dbg.h"
#include <stdlib.h>
#include <stdio.h>

void test_debug() {
  debug("I have nice hair.");

  debug("I am %d years yung", -29);
}

void test_log_err() {
  log_err("Everything breaks for a reason");

  log_err("There is a count of %d wigs in %s", 92, "my room");
}

void test_log_warn() {
  log_warn("ignore me");
  log_warn("Scope: %s", "/etc/passwd");
}

void test_log_info() {
  log_info("Well ");
  log_info("Happended %f times on avg", 1.3f);
}

int test_check(char* filename) {
  FILE* input = NULL;
  char* block = NULL;

  block = malloc(100);
  check_mem(block);

  input = fopen(filename, "r");
  check(input, "Failed to open %s", filename);

  free(block);
  fclose(input);
  return 0;

  error:
    if (block != NULL) {
      free(block);
    }

    if (input != NULL) {
      fclose(input);
    }

    return -1;
}

int test_sentinel(int code) {
  char* temp = malloc(100);
  check_mem(temp);

  switch (code) {
    case 1:
      log_info("It works");
      break;
    default:
      sentinel("I should never run bro");
  }

  free(temp);
  return 0;

  error:
    if (temp != NULL) {
      free(temp);
    }

    return -1;
}

int test_check_mem() {
  char* test = NULL;
  check_mem(test);
  free(test);
  return 1;

  error:
    return -1;
}

int test_check_debug() {
  int i = 0;
  check_debug(i != 0, "oops i WAS 0 but now i am %d", i);
  return 0;

  error:
    return -1;
}

int main(int argc, char* argv[]) {
  check(argc == 2, "Need an argument");

  test_debug();
  test_log_err();
  test_log_warn();
  test_log_info();

  check(test_check("main.c") == 0, "failed with ex19.c");
  check(test_check(argv[1]) == -1, "failed with argv");

  check(test_sentinel(1) == 0, "test_sentinel_failed");
  check(test_sentinel(100) == -1, "test sentinel failed");

  check(test_check_mem() == -1, "test check mem failed");
  check(test_check_debug() == -1, "test check debug failed");
  return 0;

  error:
    return 1;
}
