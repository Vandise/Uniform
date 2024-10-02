#include "awry/awry.h"
#include "uniform/parser/shared.h"

// for unit testing only
#include "uniform/preprocessor/shared.h"
#include "uniform/core/shared.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__)
  #define SCANNER_LIB "lib/libuniformscanner.dll"
#endif

#ifdef __linux__
  #define SCANNER_LIB "lib/libuniformscanner.so"
#endif

#ifdef __APPLE__
  #define SCANNER_LIB "lib/libuniformscanner.dylib.1.0.0"
#endif

UniformSymbolTable* modules_table = NULL;
UniformPreprocessor* modules_preprocessor = NULL;
UniformParser* modules_parser = NULL;

define_fixture(before, before_modules) {
  modules_table = UniformSymbolTableModule.init();
  UniformCoreIntegerModule.init(modules_table);

  modules_preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
  UniformPreprocessorModule.process(modules_preprocessor, "modules/parser/test/files/modules.u", NULL);

  modules_parser = UniformParserModule.init(modules_table, modules_preprocessor->token_array);
}

define_fixture(after, after_modules) {
  UniformParserModule.close(modules_parser);
  UniformPreprocessorModule.close(modules_preprocessor);
  UniformSymbolTableModule.clear(modules_table);
}

describe("Modules Parser Test Suite", modules_parser_test_suite)
  before(before_modules)
  after(after_modules)

  context(".modules")
    it("processes modules")
      UniformASTNode* node = UniformModuleParser.process(modules_parser, NULL);
      UniformASTModuleNode* module = (UniformASTModuleNode*)(node->data);

      expect(module->identifier) to equal("Main")

      UniformASTBodyNode* body = module->body;
      UniformASTModuleNode* submodule = (UniformASTModuleNode*)(body->nodes[0]->data);

      expect(submodule->identifier) to equal("SubMain")
    end

    it("adds the symbols to the symbol table")
      UniformASTNode* node = UniformModuleParser.process(modules_parser, NULL);
      UniformSymbolTableNode* mainsym = UniformSymbolTableModule.search_global(modules_table, "Main");

      expect(mainsym->name) to equal("Main")

      UniformSymbolTableNode* subsym = UniformSymbolTableModule.search_global(mainsym->definition.info.module.symbol_table, "SubMain");
      expect(subsym->name) to equal("SubMain")

      UniformSymbolTableNode* declsym = UniformSymbolTableModule.search_global(subsym->definition.info.module.symbol_table, "R");
      expect(declsym->name) to equal("R")
    end
  end
end