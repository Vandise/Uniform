#ifndef __UNIFORM_SCANNER_SHAREDH
#define __UNIFORM_SCANNER_SHAREDH 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "uniform/scanner/tokens.h"

#define EOF_CHAR     '\x7f'
#define NEWLINE_CHAR '\n'
#define CARRIAGE_RETURN_CHAR '\r'
#define COMMENT_CHAR '#'
#define UNIFORM_SCANNER_VERSION 100
#define UNIFORM_SCANNER_MAX_SOURCE_LINE 1024

typedef enum {
  I_32LIT, I_64LIT, F_32LIT, F_64LIT, STRING_LIT,
} UNIFORM_LITERAL;

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

typedef struct UniformScannerLiteralStruct {
  UNIFORM_LITERAL type;
  size_t size;
  union {
    int i32;
    long int i64;
    float f32;
    double f64;
    char  string[1024];
  } value;
} UniformScannerLiteral;

typedef struct UniformScannerTokenStruct {
  unsigned int line_number;
  unsigned int level;

  char *tokenp;
  char source_name[64];
  char word_string[1024];
  char token_string[1024];

  UNIFORM_TOKEN_CODE code;
  UniformScannerLiteral literal;
} UniformToken;

typedef struct UniformScannerStruct {
  unsigned int line_number;
  unsigned int level;
  unsigned int buffer_offset;
  unsigned int digit_count;

  UNIFORM_CHAR_CODE char_table[256];

  char source_buffer[1024];
  char source_name[64];
  FILE *source_file;

  char *source_bufferp;
  char current_char;

  UniformToken current_token;

  unsigned int errored;

} UniformScanner;

struct UniformScannerModuleStruct {
  const signed int version;

  UniformScanner* (*init)(const char*);
  void (*close)(UniformScanner*);
  void (*get_token)(UniformScanner* scanner);
};

extern const struct UniformScannerModuleStruct UniformScannerModule;

#endif