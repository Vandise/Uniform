#include "uniform/parser/shared.h"

// ============================
//          Locals
// ============================

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
  UniformSymbolTableNode* n = UniformSymbolTableModule.search(parser->symbol_table, t->token_string);

  if (n == NULL) {
    UniformErrorUtil.trace_error(
      UNIFORM_UNDEFINED_TYPE_ERROR,
      t->source_name,
      t->line_number,
      t->buffer_offset,
      t->token_string
    );
    free(node);
    return NULL;
  }

  UniformLogger.log_info("UniformParser::Statements::assignment_statement(id: %s, type: %s)", data->identifier, t->token_string);

  UniformParserModule.next(parser); // T_EQUAL
  UniformParserModule.next(parser);

  data->expressions = UniformParserExpression.process(parser, NULL);

  if(UniformParserType.assign_compatible(n->type, data->expressions->type->type)) {
    return node;
  }

  UniformErrorUtil.trace_error(
    UNIFORM_TYPE_ERROR,
    t->source_name,
    t->line_number,
    t->buffer_offset,
    t->token_string,
    data->expressions->type->name
  );

  free(node);
  return NULL;
}

// ============================
//            Module
// ============================

struct UniformParserStatementStruct UniformParserStatement = {
  .process = process
};