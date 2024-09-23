#ifndef __UNIFORM_TOKENSH
#define __UNIFORM_TOKENSH 1

#include <stdlib.h>
#include <string.h>

#define UNIFORM_TOKEN_MODULE_VERSION 100
#define UNIFORM_MIN_RESERVED_WORD_LENGTH 1
#define UNIFORM_MAX_RESERVED_WORD_LENGTH 6

#define I_32_LOWER -2147483648
#define I_32_UPPER  2147483647
#define I_64_LOWER -9223372036854775807
#define I_64_UPPER  9223372036854775806

typedef enum {
  I_32LIT, I_64LIT, F_32LIT, F_64LIT, STRING_LIT,
} UNIFORM_LITERAL;

typedef enum {
  UNDEFINED_TOKEN, T_ERROR, T_NEWLINE, T_END_OF_FILE,
  T_MACRO, T_IDENTIFIER, T_STRING, T_CONSTANT, T_NUMERIC,
  T_DOT, T_EQUAL, T_OPEN_CURLY_BRACE, T_CLOSE_CURLY_BRACE, T_OPEN_BRACKET, T_CLOSE_BRACKET, T_OPEN_PAREN, T_CLOSE_PAREN,
  T_PLUS, T_MINUS, T_STAR, T_SLASH, T_PIN, T_COLON, T_SEMICOLON, T_COMMA, T_QUESTION, T_BANG, T_PIPE, T_GREATER_THAN, T_LESS_THAN, T_TILDE, T_PERCENT,
  T_PIPE_OPERATOR, T_LAMBDA,
  T_MODULE, T_END, T_STRUCT, T_FUNC, T_CASE, T_RETURN,
  T_NEGATE
} UNIFORM_TOKEN_CODE;

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
  char source_name[1024];
  char word_string[1024];
  char token_string[1024];

  UNIFORM_TOKEN_CODE code;
  UniformScannerLiteral literal;
} UniformToken;

typedef struct UniformTokenArrayStruct {
  unsigned int size;
  unsigned int used;

  UniformToken* tokens;
} UniformTokenArray;

typedef struct UniformKeyWordStruct {
  char *string;
  UNIFORM_TOKEN_CODE token_code;
} UniformKeyWord;

struct UniformTokenModuleStruct {
  const signed int version;

  UniformTokenArray* (*init)(int);
  void (*commit_token)(UniformTokenArray*, UniformToken);
  void (*clear)(UniformTokenArray*);

  int (*string_is_reserved_word)(const char*);
  UNIFORM_TOKEN_CODE (*get_token_code)(const char*);
  const char* (*t_to_s)(UNIFORM_TOKEN_CODE code);
};

extern const struct UniformTokenModuleStruct UniformTokenModule;

#endif