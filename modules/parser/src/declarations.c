#include "uniform/parser/shared.h"

// ============================
//          Locals
// ============================

// ============================
//          Forwards
// ============================

static UniformASTNode* process(UniformParser* parser, UniformASTModuleNode* module);
static UniformASTNode* constant_declaration(UniformParser* parser, UniformASTModuleNode* module);


// ============================
//        Implementation
// ============================

/*

*/
static UniformASTNode* process(UniformParser* parser, UniformASTModuleNode* module) {
  UniformLogger.log_info("UniformParser::Declarations::process");

  UniformToken* t = UniformParserModule.get_token(parser);
  switch(t->code) {
    case T_CONST:
      return constant_declaration(parser, module);
    default:
      break;
  }

  return NULL;
}

/*
  const T_CONSTANT = < expressions >
*/
static UniformASTNode* constant_declaration(UniformParser* parser, UniformASTModuleNode* module) {
  UniformLogger.log_info("UniformParser::Declarations::constant_declaration");

  // T_CONST
  UniformToken* t = UniformParserModule.get_token(parser);
  UniformASTNode* node = UniformASTNodeModule.token_to_node(t);
  UniformASTConstantNode* data = (UniformASTConstantNode*)(node->data);
  data->module = module;

  // T_CONSTANT
  UniformParserModule.next(parser);
  t = UniformParserModule.get_token(parser);
  strcpy(data->identifier, t->token_string);

  // T_EQUAL
  UniformParserModule.next(parser);

  // < expressions >
  UniformParserModule.next(parser);

  data->expressions = UniformParserExpression.process(parser, NULL);
  data->type = data->expressions->type;

  UniformSymbolTableNode* constsymtab = UniformSymbolTableModule.insert_global(
    module->symbol->definition.info.module.symbol_table,
    data->identifier
  );

  constsymtab->type = data->type->type;
  data->symbol = constsymtab;

  return node;
}

// ============================
//            Module
// ============================

struct UniformParserDeclarationStruct UniformParserDeclaration = {
  .process = process
};