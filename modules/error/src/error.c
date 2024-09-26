#include "uniform/error/shared.h"

// ============================
//          Locals
// ============================

static const char *error_codes[] = {
  "Unknown Error",
  "File Not Found",
  "Syntax Error",
  "Macro Syntax Error",
  "Invalid Macro Error",
  "Missing Core Library"
};

static const char *error_message_fmt[] = {
  "An unknown error has occurred.",
  "File was not found: %s.",
  "Unexpected token %s, expected %s",
  "Unexpected token %s, expected %s.",
  "Invalid macro \"%s\" called.",
  "Uniform was unable to load the required library %s."
};

// ============================
//          Forwards
// ============================

static void print_header(UNIFORM_ERROR_CODE);
static void trace_error(UNIFORM_ERROR_CODE, const char*, unsigned int, unsigned int, ...);

// ============================
//        Implementation
// ============================

static void print_header(UNIFORM_ERROR_CODE code) {
  const char* err = error_codes[code];
  printf("\n\033[36m-- %s -----------------------\033[0m\n", err);
}

static void print_error_message(UNIFORM_ERROR_CODE code, va_list fmt_args) {
  const char* err = error_message_fmt[code];

  printf("\n\033[0;31m\tError:\033[0m\n");
  printf("\n\t\t");
  vprintf(err, fmt_args);
  printf("\n");
}

static void print_file_message_heading(const char* filename, unsigned int line_number) {
  printf("\n\033[36m\t%s %s %s %d:\033[0m\n", "In file", filename, "on line", line_number + 1);
}

static void trace_error(
    UNIFORM_ERROR_CODE code,
    const char* filename,
    unsigned int line_number,
    unsigned int buffer_offset,
    ...
) {
  va_list fmt_args;
  int line_min = (line_number - 10) > 0 ? (line_number - 10) : 0;
  int line_max = (line_number + 10);
  int current_line = 0;

  va_start(fmt_args, buffer_offset);

  print_header(code);
  print_error_message(code, fmt_args);

  FILE *file = fopen(filename, "r");

  if (file != NULL) {
    print_file_message_heading(filename, line_number);
    char line[1024];
  
    while (fgets(line, sizeof line, file) != NULL) {
      if (current_line >= line_min || current_line <= line_max) {
        printf("\t\t%04d: %s", (current_line + 1), line);
        if (current_line == line_number) {
          int errorpos = 0;
          printf("\033[0;31m\t\t      ");
          // -1 for 0 index, -1 for ^ symbol
          while (errorpos < (buffer_offset > 1 ? buffer_offset - 2 : buffer_offset)) {
            printf("~");
            errorpos++;
          }
          printf("^\033[0m\n");
        }
      } else {
        break;
      }
      current_line++;
    }
    fclose(file);
  }

  va_end(fmt_args);

  printf("\n\n");
}

// ============================
//        Module
// ============================

UniformErrorUtilModule UniformErrorUtil = {
  .trace_error = trace_error
};