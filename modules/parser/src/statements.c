#include "uniform/parser/shared.h"

// ============================
//          Locals
// ============================

// todo: >= <= %, binary ops etc
// static UNIFORM_TOKEN_CODE rel_op_list[]  = { T_LESS_THAN, T_EQUAL, T_GREATER_THAN, 0 };


// ============================
//          Forwards
// ============================

static UniformASTNode* process(UniformParser* parser);
static UniformASTNode* assignment_statement(UniformParser* parser);

// ============================
//        Implementation
// ============================

static UniformASTNode* process(UniformParser* parser) {
  UniformLogger.log_info("UniformParser::Statements::process");

  UniformToken* t = UniformParserModule.get_token(parser);
  switch(t->code) {
    case T_IDENTIFIER:
      return assignment_statement(parser);
    default:
      break;
  }

  return NULL;
}

/*
  T_IDENTIFIER:T_CONSTANT = <expressions>
  todo: symbol table types and validation.
        handle syntax errors
*/
static UniformASTNode* assignment_statement(UniformParser* parser) {
  UniformLogger.log_info("UniformParser::Statements::assignment_statement");
  UniformToken* t = UniformParserModule.get_token(parser);
  UniformASTNode* node = UniformASTNodeModule.token_to_node(t);
  UniformASTAssignmentNode* data = (UniformASTAssignmentNode*)(node->data);

  UniformParserModule.next(parser); // :
  UniformParserModule.next(parser); // T_CONSTANT

  t = UniformParserModule.get_token(parser);
  strcpy(data->data_type, t->token_string);

  UniformLogger.log_info("UniformParser::Statements::assignment_statement(id: %s, type: %s)", data->identifier, data->data_type);

  UniformParserModule.next(parser); // T_EQUAL
  UniformParserModule.next(parser);

  data->expressions = UniformParserExpression.process(parser, NULL);

  return node;
}

// ============================
//            Module
// ============================

struct UniformParserStatementStruct UniformParserStatement = {
  .process = process
};