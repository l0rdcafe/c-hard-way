#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include "dbg.h"

struct Logfiles {
  char** filenames;
  int num_files;
};

const char* get_base_path() {
  const char* base_path = getenv("HOME");
  if (base_path == NULL) {
    base_path = getpwuid(getuid())->pw_dir;
    check(base_path != NULL, "Could not access user home directory");
  }

  return base_path;
  error:
    return NULL;
}

struct Logfiles get_logfile_paths() {
  int num_files = 0;
  char** filenames = malloc(num_files * sizeof *filenames);
  struct Logfiles logfiles = { .num_files = 0 };

  const char* home_dir = get_base_path();

  int len = strlen(home_dir) + 1;
  char* file_path = malloc(strlen("/.logfind") + len + 1);
  memcpy(file_path, home_dir, len);
  strcat(file_path, "/.logfind");
  log_info("Opening logfind dotfile at %s", file_path);
  FILE* dot_file = fopen(file_path, "r");

  check(dot_file != NULL, "Failed to open ~/.logfind config file");

  char line[256];
  while (fgets(line, sizeof(line), dot_file)) {
    size_t len = strlen(line) - 1;
    filenames[num_files] = malloc(len);
    memcpy(filenames[num_files], line, len);
    num_files++;
  }
  logfiles.filenames = filenames;
  logfiles.num_files = num_files;

  check(num_files > 0, "~/.logfind must have at least 1 log file");
  fclose(dot_file);
  return logfiles;

  error:
    free(filenames);
    return logfiles;
}

int has_words(char** keywords, int keywords_size, FILE* logfile, int is_or) {
  int match_count = 0;
  char line[256];
  while (fgets(line, sizeof(line) - 1, logfile)) {
    printf("%s", line);
    for (int i = 0; i < keywords_size; i++) {
      char* keyword_match = strstr(line, keywords[i]);
      if (is_or == 1 && keyword_match != NULL) {
        return 1;
      }

      if (is_or == 0 && keyword_match != NULL) {
        match_count++;
      }
    }
  }

  fclose(logfile);

  if (is_or == 1 && match_count > 0) {
    return 1;
  }

  if (keywords_size > match_count) {
    return 0;
  }

  return 1;
}

int main(int argc, char* argv[]) {
  char** keywords = malloc((argc - 1) * sizeof *keywords);
  check(argc > 1, "Must provide at least 1 keyword to logfind");

  int is_or = 0;
  int input_size = 0;
  // skip first arg since it's command name
  for (int i = 1; i < argc; i++) {
    char* arg = argv[i];

    if (strncmp(arg, "-o", 4) == 0) {
      is_or = 1;
    } else {
      size_t len = strlen(arg) + 1;
      keywords[input_size] = malloc(len);
      memcpy(keywords[input_size], arg, len);

      check(keywords[input_size] != NULL, "Failed to parse arguments");
      input_size++;
    }
  }

  struct Logfiles logfiles = get_logfile_paths();
  char** matched_files = malloc(logfiles.num_files * sizeof *matched_files);
  int matched_file_count = 0;

  for (int i = 0; i < logfiles.num_files; i++) {
    char* file_name = logfiles.filenames[i];
    const char* base_path = get_base_path();
    char* path_prefix = "/**/*";
    char* full_path = malloc(strlen(base_path) + strlen(path_prefix) + strlen(file_name) + 1);

    strcpy(full_path, base_path);
    strcat(full_path, path_prefix);
    strcat(full_path, file_name);

    glob_t globbuf = {};
    int rc = glob(file_name, GLOB_DOOFFS, NULL, &globbuf);
    if (rc != 0) {
      log_info("Could not find match for file %s", full_path);
      continue;
    }

    for (size_t j = 0; j < globbuf.gl_pathc; j++) {
      FILE* logfile = fopen(globbuf.gl_pathv[j], "r");

      log_info("Reading '%s'", globbuf.gl_pathv[j]);
      int has_match = has_words(keywords, input_size, logfile, is_or);
      if (has_match == 1) {
        int filename_len = strlen(globbuf.gl_pathv[j]) + 1;
        matched_files[matched_file_count] = malloc(filename_len);
        memcpy(matched_files[matched_file_count], globbuf.gl_pathv[j], filename_len);
        matched_file_count++;
      }
    }
  }

  if (matched_file_count == 0) {
    log_info("No matches found for keywords");
    return 1;
  }

  log_info("Matched Log Files:");
  for (int i = 0; i < matched_file_count; i++) {
    log_info("- %s", matched_files[i]);
  }

  return 0;

  error:
    free(keywords);
    return 1;
}
