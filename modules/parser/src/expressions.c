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

static UniformASTExpressionNode* process(UniformParser* parser, UniformASTExpressionNode* tree);
static void expression(UniformParser* parser, UniformASTExpressionNode* tree);
static void term(UniformParser* parser, UniformASTExpressionNode* tree);
static void factor(UniformParser* parser, UniformASTExpressionNode* tree);

// ============================
//        Implementation
// ============================

// returns a tree in post-fix notation
static UniformASTExpressionNode* process(UniformParser* parser, UniformASTExpressionNode* tree) {
  UniformLogger.log_info("UniformParser::Expressions::process");

  if (tree == NULL) {
    //tree = UniformASTModule.init_tree(10);
  }

  UNIFORM_TOKEN_CODE operator;
  expression(parser, tree);

  UniformToken* t = UniformParserModule.get_token(parser);

  // postfix: printf("%s ", t->token_string);
  //UniformASTNodeModule.token_to_node(t);

  if(
    UniformParserModule.token_in_list(t->code, rel_op_list) ||
    UniformParserModule.token_in_list(t->code, add_op_list) ||
    UniformParserModule.token_in_list(t->code, mult_op_list)
  ) {
    operator = t->code;
    UniformParserModule.next(parser);
    expression(parser, tree);
    // postfix: printf("%s ", UniformTokenModule.t_to_s(operator));
  }

  return tree;
}

static void expression(UniformParser* parser, UniformASTExpressionNode* tree) {
  UniformLogger.log_info("UniformParser::Expressions::expression");

  UNIFORM_TOKEN_CODE operator;
  UNIFORM_TOKEN_CODE unary_op = T_PLUS;

  UniformToken* t = UniformParserModule.get_token(parser);

  if (t->code == T_PLUS || t->code == T_MINUS) {
    unary_op = t->code;
    UniformParserModule.next(parser);
  }

  term(parser, tree);

  // postfix: printf("%s ", unary_op == T_PLUS ? "" : "neg");

  t = UniformParserModule.get_token(parser);

  while(UniformParserModule.token_in_list(t->code, add_op_list)) {
    operator = t->code;
    UniformParserModule.next(parser);
    t = UniformParserModule.get_token(parser);
    term(parser, tree);
    // postfix: printf("%s ", UniformTokenModule.t_to_s(operator));
  }
}

static void term(UniformParser* parser, UniformASTExpressionNode* tree) {
  UniformLogger.log_info("UniformParser::Expressions::term");

  UNIFORM_TOKEN_CODE operator;

  factor(parser, tree);

  UniformToken* t = UniformParserModule.get_token(parser);

  while(UniformParserModule.token_in_list(t->code, mult_op_list)) {
    operator = t->code;
    UniformParserModule.next(parser);
    t = UniformParserModule.get_token(parser);
    factor(parser, tree);
    // postfix: printf("%s ", UniformTokenModule.t_to_s(operator));
  }
}

static void factor(UniformParser* parser, UniformASTExpressionNode* tree) {
  UniformLogger.log_info("UniformParser::Expressions::factor");

  UniformToken* t = UniformParserModule.get_token(parser);

  switch(t->code) {
    case T_IDENTIFIER:
      UniformParserModule.next(parser);
      break;
    case T_NUMERIC:
      UniformParserModule.next(parser);
      // postfix: printf("%s ", t->token_string);
      break;
    case T_STRING:
      UniformParserModule.next(parser);
      break;
    case T_OPEN_PAREN:
      UniformParserModule.next(parser);
      process(parser, tree);
      // todo: assert is T_CLOSE_PAREN
      UniformParserModule.next(parser);
      break;
    default:
      // postfix: printf("\n todo: error \n");
      t->code = T_ERROR;
      break;
  }
}

// ============================
//            Module
// ============================

struct UniformParserExpressionStruct UniformParserExpression = {
  .process = process
};