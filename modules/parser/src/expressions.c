#include "uniform/parser/shared.h"

// ============================
//          Locals
// ============================

// todo: >= <= %, binary ops etc
static UNIFORM_TOKEN_CODE rel_op_list[]  = { T_LESS_THAN, T_EQUAL, T_GREATER_THAN, 0 };
static UNIFORM_TOKEN_CODE add_op_list[]  = { T_PLUS, T_MINUS, 0 };
static UNIFORM_TOKEN_CODE mult_op_list[] = { T_STAR, T_SLASH, 0 };

// ============================
//          Forwards
// ============================

static void process(UniformParser* parser);
static void expression(UniformParser* parser);
static void term(UniformParser* parser);
static void factor(UniformParser* parser);

// ============================
//        Implementation
// ============================

static void process(UniformParser* parser) {
  UniformLogger.log_info("UniformParser::Expressions::process");

  UNIFORM_TOKEN_CODE operator;
  expression(parser);

  UniformToken* t = UniformParserModule.get_token(parser);

  if(UniformParserModule.token_in_list(t->code, rel_op_list)) {
    operator = t->code;
    UniformParserModule.next(parser);
    expression(parser);
  }
}

static void expression(UniformParser* parser) {
  UniformLogger.log_info("UniformParser::Expressions::expression");

  UNIFORM_TOKEN_CODE operator;
  UNIFORM_TOKEN_CODE unary_op = T_PLUS;

  UniformToken* t = UniformParserModule.get_token(parser);

  if (t->code == T_PLUS || t->code == T_MINUS) {
    unary_op = t->code;
    UniformParserModule.next(parser);
  }

  term(parser);

  t = UniformParserModule.get_token(parser);

  while(UniformParserModule.token_in_list(t->code, add_op_list)) {
    operator = t->code;
    UniformParserModule.next(parser);
    t = UniformParserModule.get_token(parser);
    term(parser);
  }
}

static void term(UniformParser* parser) {
  UniformLogger.log_info("UniformParser::Expressions::term");

  UNIFORM_TOKEN_CODE operator;

  factor(parser);

  UniformToken* t = UniformParserModule.get_token(parser);

  while(UniformParserModule.token_in_list(t->code, mult_op_list)) {
    operator = t->code;
    UniformParserModule.next(parser);
    t = UniformParserModule.get_token(parser);
    factor(parser);
  }
}

static void factor(UniformParser* parser) {
  UniformLogger.log_info("UniformParser::Expressions::factor");

  UniformToken* t = UniformParserModule.get_token(parser);

  switch(t->code) {
    case T_IDENTIFIER:
      UniformParserModule.next(parser);
      break;
    case T_NUMERIC:
      UniformParserModule.next(parser);
      break;
    case T_STRING:
      UniformParserModule.next(parser);
      break;
    case T_OPEN_PAREN:
      UniformParserModule.next(parser);
      process(parser);
      // todo: assert is T_CLOSE_PAREN
      UniformParserModule.next(parser);
      break;
    default:
      printf("\n todo: error \n");
      break;
  }
}

// ============================
//            Module
// ============================

struct UniformParserExpressionStruct UniformParserExpression = {
  .process = process
};