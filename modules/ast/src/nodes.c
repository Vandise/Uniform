#include "uniform/ast/shared.h"

// ============================
//          Forwards
// ============================

/*
  UNDEFINED_TOKEN, T_ERROR, T_NEWLINE, T_END_OF_FILE,
  T_MACRO, T_IDENTIFIER, T_STRING, T_CONSTANT, T_NUMERIC,
  T_DOT, T_EQUAL, T_OPEN_CURLY_BRACE, T_CLOSE_CURLY_BRACE, T_OPEN_BRACKET, T_CLOSE_BRACKET, T_OPEN_PAREN, T_CLOSE_PAREN,
  T_PLUS, T_MINUS, T_STAR, T_SLASH, T_PIN, T_COLON, T_SEMICOLON, T_COMMA, T_QUESTION, T_BANG, T_PIPE, T_GREATER_THAN, T_LESS_THAN, T_TILDE, T_PERCENT,
  T_PIPE_OPERATOR, T_LAMBDA,
  T_MODULE, T_END, T_STRUCT, T_FUNC, T_CASE, T_RETURN,
  T_NEGATE
*/

static UniformASTNode* token_to_node(UniformToken* token);

// ============================
//        Implementation
// ============================

static UniformASTNode* token_to_node(UniformToken* token) {
  printf("\n creating token for %d \n", token->code);
  switch(token->code) {
    case UNDEFINED_TOKEN:
    case T_ERROR:
    case T_MACRO:
      break;
    case T_END_OF_FILE:
    case T_NEWLINE:
      break;
    case T_STRING:
    case T_MODULE:
    case T_END:
    case T_STRUCT:
    case T_FUNC:
    case T_CASE:
    case T_RETURN:
    case T_IDENTIFIER:
    case T_CONSTANT:
    case T_NUMERIC:
      break;
    case T_DOT:
      break;
    case T_EQUAL:
      break;
    case T_OPEN_CURLY_BRACE:
      break;
    case T_CLOSE_CURLY_BRACE:
      break;
    case T_OPEN_BRACKET:
      break;
    case T_CLOSE_BRACKET:
      break;
    case T_OPEN_PAREN:
      break;
    case T_CLOSE_PAREN:
      break;
    case T_PLUS:
      break;
    case T_MINUS:
      break;
    case T_STAR:
      break;
    case T_SLASH:
      break;
    case T_PIN:
      break;
    case T_COLON:
      break;
    case T_SEMICOLON:
      break;
    case T_COMMA:
      break;
    case T_QUESTION:
      break;
    case T_BANG:
      break;
    case T_PIPE:
      break;
    case T_GREATER_THAN:
      break;
    case T_LESS_THAN:
      break;
    case T_TILDE:
      break;
    case T_PERCENT:
      break;
    case T_PIPE_OPERATOR:
      break;
    case T_LAMBDA:
      break;
    case T_NEGATE:
      break;
    default:
      break;
  }
  return NULL;
}

// ============================
//            Module
// ============================

struct UniformASTNodeModuleStruct UniformASTNodeModule = {
  .token_to_node = token_to_node
};