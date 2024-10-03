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

  // T_NEWLINE
  UniformParserModule.next(parser);

  UniformParserModule.skip_newlines(parser);

  //
  // add symbol before processing body
  //
  UniformSymbolTableNode* symnode = malloc(sizeof(UniformSymbolTableNode));
  symnode->name = malloc(strlen(module->identifier) + 1);
  strcpy(symnode->name, module->identifier);

  if (parent != NULL) {
    UniformSymbolTableNode* parentsym = UniformSymbolTableModule.search_global(parser->symbol_table, parent->identifier);
    symnode->definition.info.module.parent = parentsym;
  }

  module->symbol = UniformSymbolTableModule.insert_module(parser->symbol_table, symnode);

  //
  // Body
  //

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
    case T_CONST: {
      UniformASTModule.insert_node(
        module->body,
        UniformParserDeclaration.process(parser, module)
      );
    }
    case T_IDENTIFIER: {
      //UniformASTModule.insert_node(
      //  module->body,
      //  UniformParserDeclaration.process(parser, module)
      //);
      UniformParserDeclaration.process(parser, module);
      break;
    }
    default:
      break;
  }

  UniformParserModule.skip_newlines(parser);

  UniformParserModule.next(parser);
}

// ============================
//            Module
// ============================

struct UniformModuleParserStruct UniformModuleParser = {
  .process = process
};