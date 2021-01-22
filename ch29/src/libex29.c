#include <stdio.h>
#include <ctype.h>
#include "dbg.h"

int print_message(const char* msg) {
  printf("A String: %s\n", msg);
  return 0;
}

int uppercase(const char* msg, int len) {
  for (int i = 0; i < len; i++) {
    printf("%c", toupper(msg[i]));
  }
  printf("\n");
  return 0;
}

int lowercase(const char* msg, int len) {
  for (int i = 0; i < len; i++) {
    printf("%c", tolower(msg[i]));
  }
  printf("\n");
  return 0;
}

int fail_on_purpose(const char* msg) {
  return 1;
}

