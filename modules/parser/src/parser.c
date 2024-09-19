#include "uniform/parser/parser.h"

// ============================
//          Forwards
// ============================

static UniformParser* init(UniformTokenArray* tokens);
static UniformToken* get_token(UniformParser* parser);
static void close(UniformParser* parser);

// ============================
//        Implementation
// ============================

static UniformParser* init(UniformTokenArray* tokens) {
  UniformParser* parser = malloc(sizeof(UniformParser));
  parser->token_arr = tokens;
  parser->token_index = 0;

  return parser;
}

static UniformToken* get_token(UniformParser* parser) {
  if (parser->token_index >= parser->token_arr->used) {
    return NULL;
  }
  return &(parser->token_arr->tokens[parser->token_index]);
}

static UniformToken* peek(UniformParser* parser, int n) {
  if (parser->token_index + n >= parser->token_arr->used) {
    return NULL;
  }
  return &(parser->token_arr->tokens[parser->token_index + n]);
}

static void next(UniformParser* parser) {
  parser->token_index += 1;
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
  .close = close
};