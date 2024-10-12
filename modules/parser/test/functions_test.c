
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

UniformSymbolTable* functions_table = NULL;
UniformPreprocessor* functions_preprocessor = NULL;
UniformParser* functions_parser = NULL;

define_fixture(before, before_functions) {
  functions_table = UniformSymbolTableModule.init();
  UniformCoreIntegerModule.init(functions_table);

  functions_preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
  UniformPreprocessorModule.process(functions_preprocessor, "modules/parser/test/files/functions.u", NULL);

  functions_parser = UniformParserModule.init(functions_table, functions_preprocessor->token_array);
}

define_fixture(after, after_functions) {
  UniformParserModule.close(functions_parser);
  UniformPreprocessorModule.close(functions_preprocessor);
  UniformSymbolTableModule.clear(functions_table);
}

describe("Functions Parser Test Suite", functions_parser_test_suite)
  before(before_functions)
  after(after_functions)

  context("function declarations")
    it("adds the function definitions to the modules symbol table")
      UniformASTNode* node = UniformModuleParser.process(functions_parser, NULL);
      UniformASTModuleNode* module = (UniformASTModuleNode*)(node->data);

      UniformSymbolTableNode* mainsym = UniformSymbolTableModule.search_global(functions_table, "Main");

      UniformSymbolTableNode* mainfnctsym = UniformSymbolTableModule.search_global(mainsym->definition.info.module.symbol_table, "main");
      expect(mainfnctsym->name) to equal("main")
      expect(mainfnctsym->definition.type) to equal(UNIFORM_FUNCTION_DEFINITION)
      expect(mainfnctsym->definition.info.func.return_type->type_idp->name) to equal("Integer")
      expect(mainfnctsym->definition.info.func.locals_count) to equal(2)
      expect(mainfnctsym->definition.info.func.locals_size) to equal(12)

      // local vars
      UniformSymbolTableNode* xvarsym = UniformSymbolTableModule.search_global(mainfnctsym->definition.info.func.local_symbol_table, "x");
      UniformSymbolTableNode* yvarsym = UniformSymbolTableModule.search_global(mainfnctsym->definition.info.func.local_symbol_table, "y");
      expect(xvarsym->name) to equal("x")
      expect(xvarsym->definition.info.data.offset) to equal(-8)
      expect(yvarsym->name) to equal("y")
      expect(yvarsym->definition.info.data.offset) to equal(-12)

      UniformSymbolTableNode* submainfnctsym = UniformSymbolTableModule.search_global(mainsym->definition.info.module.symbol_table, "sub_main");
      expect(submainfnctsym->name) to equal("sub_main")
      expect(submainfnctsym->definition.type) to equal(UNIFORM_FUNCTION_DEFINITION)
      expect(submainfnctsym->definition.info.func.return_type->type_idp->name) to equal("Integer")
    end
  end
end