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

static UniformASTExpressionNode* process(UniformParser* parser, UniformASTExpressionNode* tree, UniformSymbolTableNode* context);
static void expression(UniformParser* parser, UniformASTExpressionNode* tree, UniformSymbolTableNode* context);
static void term(UniformParser* parser, UniformASTExpressionNode* tree, UniformSymbolTableNode* context);
static void factor(UniformParser* parser, UniformASTExpressionNode* tree, UniformSymbolTableNode* context);

// ============================
//        Implementation
// ============================

// returns a tree in post-fix notation
static UniformASTExpressionNode* process(UniformParser* parser, UniformASTExpressionNode* tree, UniformSymbolTableNode* context) {
  UniformLogger.log_info("UniformParser::Expressions::process");

  if (tree == NULL) {
    tree = UniformASTModule.init_tree(10);
  }

  UniformToken* operator;
  expression(parser, tree, context);

  UniformToken* t = UniformParserModule.get_token(parser);

  if(
    UniformParserModule.token_in_list(t->code, rel_op_list) ||
    UniformParserModule.token_in_list(t->code, add_op_list) ||
    UniformParserModule.token_in_list(t->code, mult_op_list)
  ) {
    operator = UniformParserModule.get_token(parser);
    UniformParserModule.next(parser);
    expression(parser, tree, context);

    UniformASTModule.insert_node(
      tree,
      UniformASTNodeModule.token_to_node(operator)
    );
  }

  return tree;
}

static void expression(UniformParser* parser, UniformASTExpressionNode* tree, UniformSymbolTableNode* context) {
  UniformLogger.log_info("UniformParser::Expressions::expression");

  UniformToken* operator = NULL;
  UNIFORM_TOKEN_CODE unary_op = T_PLUS;

  UniformToken* t = UniformParserModule.get_token(parser);

  if (t->code == T_PLUS || t->code == T_MINUS) {
    operator = UniformParserModule.get_token(parser);
    UniformParserModule.next(parser);
  }

  term(parser, tree, context);

  if (operator != NULL && operator->code == T_MINUS) {
    t->code = T_NEGATE;
    UniformASTModule.insert_node(
      tree,
      UniformASTNodeModule.token_to_node(t)
    );
  }

  t = UniformParserModule.get_token(parser);

  while(UniformParserModule.token_in_list(t->code, add_op_list)) {
    operator = UniformParserModule.get_token(parser);

    UniformParserModule.next(parser);
    t = UniformParserModule.get_token(parser);
    term(parser, tree, context);

    UniformASTModule.insert_node(
      tree,
      UniformASTNodeModule.token_to_node(operator)
    );
  }
}

static void term(UniformParser* parser, UniformASTExpressionNode* tree, UniformSymbolTableNode* context) {
  UniformLogger.log_info("UniformParser::Expressions::term");

  UniformToken* operator;

  factor(parser, tree, context);

  UniformToken* t = UniformParserModule.get_token(parser);

  while(UniformParserModule.token_in_list(t->code, mult_op_list)) {
    operator = UniformParserModule.get_token(parser);
    UniformParserModule.next(parser);
    t = UniformParserModule.get_token(parser);

    factor(parser, tree, context);
    
    UniformASTModule.insert_node(
      tree,
      UniformASTNodeModule.token_to_node(operator)
    );
  }
}

static void factor(UniformParser* parser, UniformASTExpressionNode* tree, UniformSymbolTableNode* context) {
  UniformLogger.log_info("UniformParser::Expressions::factor");

  UniformToken* t = UniformParserModule.get_token(parser);

  switch(t->code) {
    case T_IDENTIFIER:
      UniformParserModule.next(parser);
      break;

    case T_NUMERIC: {

      if (tree->type == NULL) {
        UNIFORM_LITERAL ltype = t->literal.type;
        UniformSymbolTableNode* type = NULL;
        if (ltype == I_32LIT || ltype == I_64LIT) {
          tree->type = UniformSymbolTableModule.search_global(parser->symbol_table, UNIFORM_INTEGER_TYPE)->type;
        }
      }

      UniformASTModule.insert_node(
        tree,
        UniformASTNodeModule.token_to_node(t)
      );

      UniformParserModule.next(parser);

      break;
    }

    case T_STRING:
      UniformParserModule.next(parser);
      break;

    case T_OPEN_PAREN:
      UniformParserModule.next(parser);
      process(parser, tree, context);
      // todo: assert is T_CLOSE_PAREN
      UniformParserModule.next(parser);
      break;

    default:
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