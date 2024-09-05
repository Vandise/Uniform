#include "uniform/scanner/shared.h"

// ============================
//          Forwards
// ============================

static UniformScanner* init(const char *file_name);
static int open_source_file(UniformScanner *scanner);

// ============================
//        Implementation
// ============================

static UniformScanner* init(const char *file_name) {
  errno = 0;

  UniformScanner* scanner = malloc(sizeof(UniformScanner));
  strcpy(scanner->source_name, file_name);
  strcpy(scanner->source_buffer, "");

  scanner->current_char = '\0';
  scanner->source_bufferp = NULL;
  scanner->line_number = 0;
  scanner->level = 0;
  scanner->buffer_offset = 0;
  scanner->errored = 0;

  if (open_source_file(scanner) == 0) {
    scanner->errored = 1;
  }

  for (int ch = 0; ch < 256; ++ch) {
    scanner->char_table[ch] = UNDEFINED_CHAR_CODE;
  }

  return scanner;
}

static int open_source_file(UniformScanner *scanner) {
  scanner->source_file = fopen(scanner->source_name, "r");

  if (errno != 0) { return 0; }

  scanner->source_bufferp = scanner->source_buffer;

  return 1;
}

static void close(UniformScanner *scanner) {
  if (scanner->source_file != NULL) {
    fclose(scanner->source_file);
    scanner->source_file = NULL;
  }

  free(scanner);
}

// ============================
//            Module
// ============================

const struct uniform_scanner_module UniformScannerModule = {
  .version = UNIFORM_SCANNER_VERSION,
  .init = init,
  .close = close
};