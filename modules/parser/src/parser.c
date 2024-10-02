#include "uniform/parser/parser.h"

// ============================
//          Forwards
// ============================

static UniformParser* init(UniformSymbolTable* symbol_table, UniformTokenArray* tokens);
static UniformToken* get_token(UniformParser* parser);
static void skip_newlines(UniformParser* parser);
static int token_in_list(UNIFORM_TOKEN_CODE code, UNIFORM_TOKEN_CODE token_list[]);
static void close(UniformParser* parser);

// ============================
//        Implementation
// ============================

static UniformParser* init(UniformSymbolTable* symbol_table, UniformTokenArray* tokens) {
  UniformParser* parser = malloc(sizeof(UniformParser));
  parser->token_arr = tokens;
  parser->token_index = 0;
  parser->symbol_table = symbol_table;

  return parser;
}

static UniformToken* get_token(UniformParser* parser) {
  if (parser->token_index >= parser->token_arr->used ) {
    return NULL;
  }

  return &(parser->token_arr->tokens[parser->token_index]);
}

static UniformToken* peek(UniformParser* parser, int n) {
  if (parser->token_index + n >= parser->token_arr->used ) {
    return NULL;
  }
  return &(parser->token_arr->tokens[parser->token_index + n]);
}

static void next(UniformParser* parser) {
  parser->token_index += 1;
}

static int token_in_list(UNIFORM_TOKEN_CODE code, UNIFORM_TOKEN_CODE token_list[]) {
  UNIFORM_TOKEN_CODE* tptr;

  for(tptr = &token_list[0]; *tptr; ++tptr) {
    if (code == *tptr) {
      return 1;
    }
  }

  return 0;
}

static void skip_newlines(UniformParser* parser) {
  UniformToken* token = get_token(parser);
  while(token->code == T_NEWLINE) {
    next(parser);
    token = get_token(parser);
  }
}

static void close(UniformParser* parser) {
  //
  // tokens get cleared when the preprocessor is closed
  //
  free(parser);
}

// ============================
//            Module
// ============================

struct UniformParserModuleStruct UniformParserModule = {
  .init = init,
  .get_token = get_token,
  .peek = peek,
  .next = next,
  .token_in_list = token_in_list,
  .close = close,
  .skip_newlines = skip_newlines
};