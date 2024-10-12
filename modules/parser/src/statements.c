#include "uniform/parser/shared.h"

// ============================
//          Locals
// ============================

// ============================
//          Forwards
// ============================

static UniformASTNode* process(UniformParser* parser, UniformSymbolTableNode* context);
static UniformASTNode* assignment_statement(UniformParser* parser, UniformSymbolTableNode* context);

// ============================
//        Implementation
// ============================

static UniformASTNode* process(UniformParser* parser, UniformSymbolTableNode* context) {
  UniformLogger.log_info("UniformParser::Statements::process");

  UniformToken* t = UniformParserModule.get_token(parser);
  switch(t->code) {
    case T_IDENTIFIER:
      return assignment_statement(parser, context);
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
static UniformASTNode* assignment_statement(UniformParser* parser, UniformSymbolTableNode* context) {
  UniformLogger.log_info("UniformParser::Statements::assignment_statement");

  UniformToken* t = UniformParserModule.get_token(parser);
  UniformASTNode* node = UniformASTNodeModule.token_to_node(t);
  UniformASTAssignmentNode* data = (UniformASTAssignmentNode*)(node->data);

  // todo:
  //  is the local present in the current context?

  UniformParserModule.next(parser); // :
  UniformParserModule.next(parser); // T_CONSTANT

  t = UniformParserModule.get_token(parser);
  UniformSymbolTableNode* n = UniformSymbolTableModule.search_global(parser->symbol_table, t->token_string);

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

  t = UniformParserModule.get_token(parser);

  data->expressions = UniformParserExpression.process(parser, NULL, context);

  if(UniformParserType.assign_compatible(n->type, data->expressions->type)) {
    // assignment will always be in a function or lambda expression
    UniformSymbolTableNode* assignsymtab = UniformSymbolTableModule.insert_global(
      context->definition.info.func.local_symbol_table,
      data->identifier
    );

    assignsymtab->type = data->expressions->type;
    data->symbol = assignsymtab;

    context->definition.info.func.locals_count++;
    context->definition.info.func.locals_size += assignsymtab->type->size;

    assignsymtab->definition.info.data.offset = (0 - context->definition.info.func.locals_size);

    return node;
  }

  UniformErrorUtil.trace_error(
    UNIFORM_TYPE_ERROR,
    t->source_name,
    t->line_number,
    t->buffer_offset,
    t->token_string,
    data->expressions->type->type_idp->name
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