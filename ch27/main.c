#undef NDEBUG
#include "../dbg/dbg.h"
#include <stdio.h>
#include <assert.h>

void copy(char to[], char from[]) {
  int i = 0;
  while ((to[i] = from[i]) != '\0') {
    i++;
  }
}

int safercopy(int from_len, char* from, int to_len, char* to) {
  assert(from != NULL && to != NULL && "from and to can't be NULL");

  if (from_len < 0 || to_len <= 0) {
    return -1;
  }

  int max = from_len > to_len - 1 ? to_len - 1 : from_len;
  int i = 0;
  for (i = 0; i < max; i++) {
    to[i] = from[i];
  }

  return i;
}

int main() {
  char from[] = "0123456789";
  int from_len = sizeof(from);

  char to[] = "0123456";
  int to_len = sizeof(to);

  debug("Copying '%s':%d to '%s':%d", from, from_len, to, to_len);

  int rc = safercopy(from_len, from, to_len, to);
  check(rc > 0, "Failed to safercopy");

  check(to[to_len - 1] == '\0', "String not terminated");

  debug("Result is '%s':%d", to, to_len);

  rc = safercopy(from_len * -1, from, to_len, to);
  check(rc == -1, "safecopy should fail #1");

  check(to[to_len - 1] == '\0', "String not terminated");

  rc = safercopy(from_len, from, 0, to);
  check(rc == -1, "safercopy should fail #2");

  check(to[to_len - 1] == '\0', "String not terminated");
  return 0;

  error:
    return 1;
}
