#ifndef __UNIFORM_SCANNER_SHAREDH
#define __UNIFORM_SCANNER_SHAREDH 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <errno.h>
#include "uniform/logger/shared.h"
#include "uniform/tokens/shared.h"

#define FILE_PATH_MAX 1024
#define EOF_CHAR     '\x7f'
#define NEWLINE_CHAR '\n'
#define CARRIAGE_RETURN_CHAR '\r'
#define COMMENT_CHAR '#'
#define UNIFORM_SCANNER_VERSION 100
#define UNIFORM_SCANNER_MAX_SOURCE_LINE 1024

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
  CARRIAGE_RETURN_CHAR_CODE,
  EOF_CHAR_CODE
} UNIFORM_CHAR_CODE;

typedef struct UniformScannerStruct {
  unsigned int line_number;
  unsigned int level;
  unsigned int buffer_offset;
  unsigned int digit_count;

  UNIFORM_CHAR_CODE char_table[256];

  char source_buffer[1024];
  char source_name[FILE_PATH_MAX];
  FILE *source_file;

  char *source_bufferp;
  char current_char;

  UniformToken current_token;

  unsigned int errored;

} UniformScanner;

struct UniformScannerModuleStruct {
  const signed int version;
  void (*set_log_level)(int);

  UniformScanner* (*init)(const char*);
  void (*close)(UniformScanner*);
  void (*get_token)(UniformScanner* scanner);
};

extern const struct UniformScannerModuleStruct UniformScannerModule;

#endif