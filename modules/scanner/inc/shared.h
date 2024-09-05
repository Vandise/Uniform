#ifndef __UNIFORM_SCANNER_SHAREDH
#define __UNIFORM_SCANNER_SHAREDH 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define EOF_CHAR     '\x7f'
#define NEWLINE_CHAR '\n'
#define COMMENT_CHAR '#'
#define UNIFORM_SCANNER_VERSION 100

typedef enum {
  UNDEFINED_CHAR_CODE,
  LETTER_CHAR_CODE,
  UPPERCASE_LETTER_CHAR_CODE,
  DIGIT_CHAR_CODE,
  QUOTE_CHAR_CODE,
  UNDERSCORE_CHAR_CODE,
  MACRO_CHAR_CODE,
  SPECIAL_CHAR_CODE,
  NEWLINE_CHAR_CODE,
  EOF_CHAR_CODE
} CHAR_CODE;

typedef struct UniformScannerStruct {
  unsigned int line_number;
  unsigned int level;
  unsigned int buffer_offset;
  unsigned int digit_count;

  CHAR_CODE char_table[256];

  char source_buffer[1024];
  char source_name[64];
  FILE *source_file;

  char *source_bufferp;
  char current_char;

  unsigned int errored;

} UniformScanner;

struct uniform_scanner_module {
  const signed int version;

  UniformScanner* (*init)(const char*);
  void (*close)(UniformScanner*);
};

extern const struct uniform_scanner_module UniformScannerModule;

#endif