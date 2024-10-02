#include "uniform/parser/shared.h"

// ============================
//          Locals
// ============================

// ============================
//          Forwards
// ============================

static UniformASTNode* process(UniformParser* parser, UniformASTModuleNode* parent);
static UniformASTNode* module_definition(UniformParser* parser, UniformASTModuleNode* parent);
static void body(UniformParser* parser, UniformASTModuleNode* module);

// ============================
//        Implementation
// ============================

/*
  module T_CONSTANT
    < body >
  T_END
*/
static UniformASTNode* process(UniformParser* parser, UniformASTModuleNode* parent) {
  UniformLogger.log_info("UniformParser::Modules::process");

  UniformToken* t = UniformParserModule.get_token(parser);
  switch(t->code) {
    case T_MODULE:
      return module_definition(parser, parent);
    default:
      // all programs are modules
      break;
  }

  return NULL;
}

static UniformASTNode* module_definition(UniformParser* parser, UniformASTModuleNode* parent) {
  UniformLogger.log_info("UniformParser::Modules::module_definition");

  // T_MODULE
  UniformToken* t = UniformParserModule.get_token(parser);

  UniformASTNode* node = UniformASTNodeModule.token_to_node(t);
  UniformASTModuleNode* module = (UniformASTModuleNode*)(node->data);
  module->parent = parent;
  module->body   = UniformASTModule.init_tree(10);

  //
  // todo: create a modularized name for the module that include parent
  // T_CONSTANT
  //
  UniformParserModule.next(parser);
  t = UniformParserModule.get_token(parser);
  strcpy(module->identifier, t->token_string);

  printf("Module id: %s \n", module->identifier);

  // T_NEWLINE
  UniformParserModule.next(parser);

  printf("Expected newline before body: %d / %d \n", T_NEWLINE, UniformParserModule.get_token(parser)->code);

  UniformParserModule.skip_newlines(parser);

  body(parser, module);

  return node;
}

/*
  body
    module | T_CONST | struct | func
  end
*/
static void body(UniformParser* parser, UniformASTModuleNode* module) {
  UniformLogger.log_info("UniformParser::Modules::body(module: %s)", module->identifier);

  UniformToken* t = UniformParserModule.get_token(parser);

  switch(t->code) {
    case T_MODULE: {
      UniformASTModule.insert_node(
        module->body,
        module_definition(parser, module)
      );
      break;
    }
    default:
      break;
  }

  UniformParserModule.skip_newlines(parser);

  printf("Expected end after body: %d / %d \n", T_END, UniformParserModule.get_token(parser)->code);

  UniformParserModule.next(parser);
}

// ============================
//            Module
// ============================

struct UniformModuleParserStruct UniformModuleParser = {
  .process = process
};