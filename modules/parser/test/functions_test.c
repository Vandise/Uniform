
#include "awry/awry.h"
#include "uniform/parser/shared.h"

// for unit testing only
#include "uniform/preprocessor/shared.h"
#include "uniform/core/shared.h"

/*
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
    it("processes functions with no arguments and a return type")
      UniformASTNode* node = UniformModuleParser.process(functions_parser, NULL);
      UniformASTModuleNode* module = (UniformASTModuleNode*)(node->data);
    end
  end
end
*/