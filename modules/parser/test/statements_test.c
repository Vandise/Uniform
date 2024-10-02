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

UniformSymbolTable* statements_table = NULL;

UniformPreprocessor* statements_preprocessor  = NULL;
UniformParser* statements_parser = NULL;


define_fixture(before, before_statements) {
  statements_table = UniformSymbolTableModule.init();
  UniformCoreIntegerModule.init(statements_table);

  statements_preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
  UniformPreprocessorModule.process(statements_preprocessor, "modules/parser/test/files/statements.u", NULL);
  
  statements_parser = UniformParserModule.init(statements_table, statements_preprocessor->token_array);
}

define_fixture(after, after_statements) {
  UniformParserModule.close(statements_parser);
  UniformPreprocessorModule.close(statements_preprocessor);
  UniformSymbolTableModule.clear(statements_table);
}

describe("Statements Test Suite", statements_test_suite)
  before(before_statements)
  after(after_statements)

  context(".statements")
    it("parses assignment statements")
      UniformASTNode* node = UniformParserStatement.process(statements_parser);
      expect(node->type) to equal(UNIFORM_ASSIGNMENT_NODE)
    end
  end
end