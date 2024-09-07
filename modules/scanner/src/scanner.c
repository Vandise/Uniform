#include "uniform/scanner/shared.h"

// ============================
//          Forwards
// ============================

static UniformScanner* init(const char *file_name);
static int open_source_file(UniformScanner *scanner);
static int get_source_line(UniformScanner *scanner);
static void get_character(UniformScanner *scanner);
static void skip_blanks(UniformScanner* scanner);
static void get_special(UniformScanner *scanner);

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
    // todo: Fatal error with errors module.
    scanner->errored = 1;
    return scanner;
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

static int get_source_line(UniformScanner *scanner) {
  if (scanner != NULL && fgets(scanner->source_buffer, UNIFORM_SCANNER_MAX_SOURCE_LINE, scanner->source_file) != NULL) {
    scanner->line_number += 1;
    return 1;
  }
  return 0;
}

static void get_character(UniformScanner *scanner) {
  if (*(scanner->source_bufferp) == '\0') {
    if (!get_source_line(scanner)) {
      scanner->current_char = EOF_CHAR_CODE;
      return;
    }

    scanner->source_bufferp = scanner->source_buffer;
    scanner->buffer_offset = 0;
  }

  scanner->current_char = *(scanner->source_bufferp)++;

  switch(scanner->current_char) {
    // todo: skip comments
    default:
      scanner->buffer_offset += 1;
  }
}

static void skip_blanks(UniformScanner* scanner) {
  while(scanner->current_char == ' ') {
    get_character(scanner);
  }
}

static void get_token(UniformScanner* scanner) {
  get_character(scanner);
  skip_blanks(scanner);

  scanner->current_token.tokenp = scanner->current_token.token_string;

  switch(scanner->char_table[scanner->current_char]) {
    default: get_special(scanner);
  }
}

static void get_special(UniformScanner *scanner) {
  *(scanner->current_token.tokenp) = scanner->current_char;

  switch(scanner->current_char) {
    case '(':   scanner->current_token.code = T_LPAREN; get_character(scanner);  break;
    case ')':   scanner->current_token.code = T_RPAREN; get_character(scanner);  break;
    case '@':   scanner->current_token.code = T_MACRO;  get_character(scanner);  break;
    default:
      scanner->errored = 1;
      scanner->current_token.code = T_ERROR;
  }

  *(scanner->current_token.tokenp) = '\0';
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
  .get_token = get_token,
  .close = close
};