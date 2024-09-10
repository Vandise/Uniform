#include "uniform/scanner/shared.h"

UniformKeyWord kw1[] = {
  { NULL, 0 }
};

UniformKeyWord kw2[] = {
  { NULL, 0 }
};

UniformKeyWord kw3[] = {
  {"end", T_END }, { NULL, 0 }
};

UniformKeyWord kw4[] = {
  {"func", T_FUNC }, {"case", T_CASE }, { NULL, 0 }
};

UniformKeyWord kw5[] = {
  { NULL, 0 }
};

UniformKeyWord kw6[] = {
  {"module", T_MODULE }, {"struct", T_STRUCT }, {"return", T_RETURN }, { NULL, 0 }
};

UniformKeyWord *kw_table[] = { NULL, kw1, kw2, kw3, kw4, kw5, kw6 };

static int string_is_reserved_word(const char* word_string) {
  int word_length = strlen(word_string);
  UniformKeyWord *kwp;

  if (word_length >= UNIFORM_MIN_RESERVED_WORD_LENGTH && word_length <= UNIFORM_MAX_RESERVED_WORD_LENGTH) {
    for (kwp = kw_table[word_length]; kwp->string != NULL; ++kwp) {
      if (strcmp(word_string, kwp->string) == 0) {
        return 1;
      }
    }
  }
  return 0;
}

static UNIFORM_TOKEN_CODE get_token_code(const char* word_string) {
  int word_length = strlen(word_string);
  UniformKeyWord *kwp;

  if (word_length >= UNIFORM_MIN_RESERVED_WORD_LENGTH && word_length <= UNIFORM_MAX_RESERVED_WORD_LENGTH) {
    for (kwp = kw_table[word_length]; kwp->string != NULL; ++kwp) {
      if (strcmp(word_string, kwp->string) == 0) {
        return kwp->token_code;
      }
    }
  }
  return UNDEFINED_TOKEN;
}

const struct UniformTokenModuleStruct UniformTokenModule = {
  .version = UNIFORM_TOKEN_MODULE_VERSION,

  .string_is_reserved_word = string_is_reserved_word,
  .get_token_code = get_token_code
};