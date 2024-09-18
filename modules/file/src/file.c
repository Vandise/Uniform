#include "uniform/file/shared.h"

// ============================
//          Forwards
// ============================

static char* get_file_path(const char* s);
static char* uniform_strndup(const char *s, size_t n);
static size_t uniform_strnlen(const char *src, size_t n);
static char* uniform_basename(char *path);

// ============================
//        Implementation
// ============================

static size_t uniform_strnlen(const char *src, size_t n) {
  size_t len = 0;
  while (len < n && src[len]) { len++; }
  return len;
}

static char* uniform_strndup(const char *s, size_t n) {
  size_t len = uniform_strnlen(s, n);
  char *p = malloc(len + 1);
  if (p) {
    memcpy(p, s, len);
    p[len] = '\0';
  }
  return p;
}

static char* uniform_basename(char *path) {
  char *base = strrchr(path, '/');
  return base ? base+1 : path;
}

static char* get_file_path(const char* s) {
  //
  // todo: expand ..'s
  //
  char *path_buffer = malloc(FILE_PATH_MAX);
  memset(&path_buffer[0], 0, sizeof(path_buffer));

  // a full path was already provided
  if (s[0] != '/') {
    getcwd(path_buffer, FILE_PATH_MAX);
    strcat(path_buffer, "/");
  }

  char *last_slash = strrchr(s, '/');
  char *file_path = uniform_strndup(s, ((last_slash + 1) - s));

  strcat(path_buffer, file_path);
  free(file_path);

  return path_buffer;
}

UniformFileUtilModule UniformFileUtil = {
  .get_file_path = get_file_path,
  .basename = uniform_basename
};