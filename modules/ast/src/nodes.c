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
  UniformASTNode* node = malloc(sizeof(UniformASTNode));

  //printf("token_to_node %s \n", UniformTokenModule.t_to_s(token->code));

  switch(token->code) {
    case UNDEFINED_TOKEN:
    case T_ERROR:
    case T_END_OF_FILE:
    case T_STRING:
    case T_MODULE:
    case T_END:
    case T_STRUCT:
    case T_FUNC:
    case T_CASE:
    case T_RETURN:
    case T_IDENTIFIER: {
      UniformASTAssignmentNode* data = malloc(sizeof(UniformASTAssignmentNode));
      strcpy(data->identifier, token->token_string);
      node->type = UNIFORM_ASSIGNMENT_NODE;
      node->data = (void*)data;
      return node;      
    }
    case T_CONSTANT:
      free(node);
      break;
    case T_NUMERIC: {
      UniformASTLiteralNode* data = malloc(sizeof(UniformASTLiteralNode));
      data->literal = token->literal;
      node->type = UNIFORM_LITERAL_NODE;
      node->data = (void*)data;
      return node;
    }
    case T_EQUAL:
    case T_PLUS:
    case T_MINUS:
    case T_STAR:
    case T_SLASH:
    case T_NEGATE:
    case T_GREATER_THAN:
    case T_LESS_THAN: {
      UniformASTOperatorNode* data = malloc(sizeof(UniformASTOperatorNode));
      data->operator = token->code;
      node->type = UNIFORM_OPERATOR_NODE;
      node->data = (void*)data;
      return node;
    }
    case T_PIN:
      break;
    case T_COMMA:
      break;
    case T_QUESTION:
      break;
    case T_BANG:
      break;
    case T_PIPE:
      break;
    case T_TILDE:
      break;
    case T_PERCENT:
      break;
    case T_PIPE_OPERATOR:
      break;
    case T_LAMBDA:
      break;
    default:
      free(node);
      break;
  }

  //printf("\n unknown node %d \n", token->code);

  return NULL;
}

// ============================
//            Module
// ============================

struct UniformASTNodeModuleStruct UniformASTNodeModule = {
  .token_to_node = token_to_node
};