#include "uniform/parser/shared.h"

// ============================
//          Locals
// ============================

// ============================
//          Forwards
// ============================

static UniformAST* process(UniformParser* parser);

// ============================
//        Implementation
// ============================

static UniformAST* process(UniformParser* parser) {
  UniformAST* program_tree = UniformASTModule.init_tree(10);

  UniformParserModule.skip_newlines(parser);

  UniformToken* t = UniformParserModule.get_token(parser);

  while(t != NULL && t->code == T_MODULE) {
    UniformASTModule.insert_node(
      program_tree,
      UniformModuleParser.process(parser, NULL)
    );

    UniformParserModule.skip_newlines(parser);

    t = UniformParserModule.get_token(parser);
  }

  return program_tree;
}

// ============================
//            Module
// ============================

struct UniformProgramParserStruct UniformProgramParser = {
  .process = process
};