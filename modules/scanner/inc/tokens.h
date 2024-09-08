#ifndef __UNIFORM_SCANNER_TOKENSH
#define __UNIFORM_SCANNER_TOKENSH 1

#define UNIFORM_TOKEN_MODULE_VERSION 100
#define UNIFORM_MIN_RESERVED_WORD_LENGTH 1
#define UNIFORM_MAX_RESERVED_WORD_LENGTH 6

typedef enum {
  UNDEFINED_TOKEN, T_ERROR, T_NEWLINE, T_END_OF_FILE,
  T_MACRO, T_IDENTIFIER, T_STRING, T_CONSTANT,
  T_LPAREN, T_RPAREN,
  T_MODULE, T_END
} UNIFORM_TOKEN_CODE;

typedef struct UniformKeyWordStruct {
  char *string;
  UNIFORM_TOKEN_CODE token_code;
} UniformKeyWord;

struct UniformTokenModuleStruct {
  const signed int version;

  int (*string_is_reserved_word)(const char*);
  UNIFORM_TOKEN_CODE (*get_token_code)(const char*);

};

extern const struct UniformTokenModuleStruct UniformTokenModule;

#endif