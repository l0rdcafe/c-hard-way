#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
  int id;
  int set;
  char* name;
  char* email;
};

struct Database {
  struct Address* rows;
};

struct Connection {
  FILE* file;
  struct Database* db;
};

struct Database* Database_init(int max_rows, int max_data) {
  struct Database* db = malloc(sizeof(struct Database));
  struct Address* addr = malloc(sizeof(struct Address));
  addr->name = malloc(sizeof(char) * max_data);
  addr->email = malloc(sizeof(char) * max_data);

  db->rows = malloc(sizeof(addr) * max_rows);
  return db;
}

void Database_close(struct Connection* conn) {
  if (conn != NULL) {
    if (conn->file != NULL) {
      fclose(conn->file);
    }
    if (conn->db != NULL) {
      free(conn->db);
    }
    free(conn);
  }
}

void die(const char* message, struct Connection* conn) {
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  Database_close(conn);
  exit(1);
}

void Address_print(struct Address* addr) {
  printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection* conn, int max_rows, int max_data) {
  struct Database* db = Database_init(max_rows, max_data);
  int rc = fread(conn->db, sizeof(*db), 1, conn->file);
  if (rc != 1) {
    die("Failed to load database.", conn);
  }
}

struct Connection* Database_open(const char* filename, char mode, int max_rows, int max_data) {
  struct Connection* conn = malloc(sizeof(struct Connection));
  if (conn == NULL) {
    die("Memory error", conn);
  }

  conn->db = Database_init(max_rows, max_data);
  if (conn->db == NULL) {
    die("Memory error", conn);
  }

  if (mode == 'c') {
    conn->file = fopen(filename, "w");
  } else {
    conn->file = fopen(filename, "r+");
    if (conn->file != NULL) {
      Database_load(conn, max_rows, max_data);
    }
  }

  if (conn->file == NULL) {
    die("Failed to open the file", conn);
  }

  return conn;
}


void Database_write(struct Connection* conn, int max_rows, int max_data) {
  rewind(conn->file);

  struct Database* db = Database_init(max_rows, max_data);
  int rc = fwrite(conn->db, sizeof(*db), 1, conn->file);

  if (rc != 1) {
    die("Failed to write to the database", conn);
  }

  rc = fflush(conn->file);
  if (rc == -1) {
    die("Cannot flush database", conn);
  }
}

void Database_create(struct Connection* conn) {
  for (int i = 0; i < MAX_ROWS; i++) {
    struct Address addr = { .id = i, .set = 0 };
    conn->db->rows[i] = addr;
  }
}

void Database_set(struct Connection* conn, int id, const char* name, const char* email, int max_data) {
  struct Address* addr = &conn->db->rows[id];
  if (addr->set) {
    die("Already set, delete it first", conn);
  }

  addr->set = 1;

  char* res = strncpy(addr->name, name, max_data);
  res[max_data - 1] = '\0';
  if (res == NULL) {
    die("Name copy failed", conn);
  }

  res = strncpy(addr->email, email, max_data);
  res[max_data - 1] = '\0';
  if (res == NULL) {
    die("Email copy failed", conn);
  }
}

void Database_get(struct Connection* conn, int id) {
  struct Address* addr = &conn->db->rows[id];
  if (addr->set) {
    Address_print(addr);
  } else {
    die("ID is not set", conn);
  }
}

void Database_delete(struct Connection* conn, int id) {
  struct Address addr = { .id = id, .set = 0 };
  conn->db->rows[id] = addr;
}

void Database_list(struct Connection* conn, int max_rows) {
  struct Database* db = conn->db;
  for (int i = 0; i < max_rows; i++) {
    struct Address* addr = &db->rows[i];
    if (addr->set) {
      Address_print(addr);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    die("USAGE: ex17 <dbfile> <action> [action params]", NULL);
  }

  char* filename = argv[1];
  char action = argv[2][0];

  int max_rows = MAX_ROWS;
  int max_data = MAX_DATA;
  if (action == 'c' && argc > 3) {
    max_rows = atoi(argv[3]);
    if (argc > 4) {
      max_data = atoi(argv[4]);
    }
  }
  struct Connection* conn = Database_open(filename, action, max_rows, max_data);

  int id = 0;
  if (argc > 3) {
    id = atoi(argv[3]);
  }

  if (id >= max_rows && action != 'c') {
    die("There's not that many records", conn);
  }

  switch (action) {
    case 'c':
      if (argc > 3) {
        max_rows = atoi(argv[3]);
      }

      Database_create(conn);
      Database_write(conn, max_rows, max_data);
      break;
    case 'g':
      if (argc != 4) {
        die("Need an id to get", conn);
      }
      Database_get(conn, id);
      break;
    case 's':
      if (argc != 6) {
        die("Need id, name, email to set", conn);
      }
      max_rows = MAX_ROWS;
      max_data = MAX_DATA;
      if (argv[6] != NULL) {
        max_rows = atoi(argv[6]);
      }

      if (argv[7] != NULL) {
        max_data = atoi(argv[7]);
      }
      Database_set(conn, id, argv[4], argv[5], max_data);
      Database_write(conn, max_rows, max_data);
      break;
    case 'd':
      if (argc != 4) {
        die("Need id to delete", conn);
      }
      Database_delete(conn, id);
      max_rows = MAX_ROWS;
      if (argc > 4) {
        max_rows = atoi(argv[4]);
      }

      max_data = MAX_DATA;
      if (argc > 5) {
        max_data = atoi(argv[5]);
      }

      Database_write(conn, max_rows, max_data);
      break;
    case 'l':
      Database_list(conn, max_rows);
      break;
    default:
      die("Invalid action: c=create, g=get, s=set, d=del, l=list", conn);
  }

  Database_close(conn);
  return 0;
}
