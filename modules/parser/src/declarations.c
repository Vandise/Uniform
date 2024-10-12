#include "uniform/parser/shared.h"

// ============================
//          Locals
// ============================

// ============================
//          Forwards
// ============================

static UniformASTNode* process(UniformParser* parser, UniformASTModuleNode* module);
static UniformASTNode* constant_declaration(UniformParser* parser, UniformASTModuleNode* module);
static UniformASTNode* function_declaration(UniformParser* parser, UniformASTModuleNode* module, int private_flag);

// ============================
//        Implementation
// ============================

/*

*/
static UniformASTNode* process(UniformParser* parser, UniformASTModuleNode* module) {
  UniformLogger.log_info("UniformParser::Declarations::process");

  UniformToken* t = UniformParserModule.get_token(parser);
  int private_flag = 0;

  switch(t->code) {
    case T_CONST:
      return constant_declaration(parser, module);
    case T_DEFP: {
      private_flag = 1;
    }
    case T_DEF: {
      return function_declaration(parser, module, private_flag);
    }
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

  data->expressions = UniformParserExpression.process(parser, NULL, module->symbol);
  data->data_type = data->expressions->type;

  UniformSymbolTableNode* constsymtab = UniformSymbolTableModule.insert_global(
    module->symbol->definition.info.module.symbol_table,
    data->identifier
  );

  constsymtab->type = data->data_type;
  constsymtab->definition.type = UNIFORM_CONSTANT_DEFINITION;
  data->symbol = constsymtab;

  return node;
}

/*
  def | defp T_IDENTIFIER (< args >) T_LAMBDA T_CONSTANT T_DO T_NEWLINE
    < statements >
  T_END
*/
static UniformASTNode* function_declaration(UniformParser* parser, UniformASTModuleNode* module, int private) {
  UniformLogger.log_info("UniformParser::Declarations::function_declaration");

  UniformToken* t = UniformParserModule.get_token(parser);

  UniformASTNode* node = UniformASTNodeModule.token_to_node(t);
  UniformASTFunctionDeclarationNode* data = (UniformASTFunctionDeclarationNode*)(node->data);
  data->module = module;

  // T_IDENTIFIER
  UniformParserModule.next(parser);
  t = UniformParserModule.get_token(parser);
  strcpy(data->identifier, t->token_string);

  // (
  UniformParserModule.next(parser);
  // todo:
  //  args + no parens
  // )
  UniformParserModule.next(parser);

  // T_LAMBDA
  UniformParserModule.next(parser);

  // T_CONSTANT
  UniformParserModule.next(parser);
  t = UniformParserModule.get_token(parser);
  UniformSymbolTableNode* n = UniformSymbolTableModule.search_global(parser->symbol_table, t->token_string);

  data->return_type = n->type;

  //
  // enter ref for recursion
  //
  UniformSymbolTableNode* fnctsymtab = UniformSymbolTableModule.insert_global(
    module->symbol->definition.info.module.symbol_table,
    data->identifier
  );

  fnctsymtab->definition.type = UNIFORM_FUNCTION_DEFINITION;
  fnctsymtab->definition.info.func.return_type = n->type;
  fnctsymtab->definition.info.func.local_symbol_table = UniformSymbolTableModule.init();
  fnctsymtab->definition.info.func.param_count = 0;
  fnctsymtab->definition.info.func.param_size = 0;

  // add space on the stack for the return type if any
  fnctsymtab->definition.info.func.locals_size = data->return_type->size;
  fnctsymtab->definition.info.func.locals_count = 0;

  data->symbol = fnctsymtab;

  // T_DO
  UniformParserModule.next(parser);
  // T_NEWLINE
  UniformParserModule.next(parser);

  UniformParserModule.skip_newlines(parser);

  /*
    todo: do-while statement processor is not null
  */

  data->body = UniformASTModule.init_tree(10);
  UniformASTNode* bodynode = NULL;

  do {
    bodynode = UniformParserStatement.process(parser, fnctsymtab);
    UniformASTModule.insert_node(data->body, bodynode);

    t = UniformParserModule.get_token(parser);

    //
    // todo: assert newline
    //

    UniformParserModule.next(parser);
    UniformParserModule.skip_newlines(parser);
  } while(bodynode != NULL);

  return node;
}

// ============================
//            Module
// ============================

struct UniformParserDeclarationStruct UniformParserDeclaration = {
  .process = process
};