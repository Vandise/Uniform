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

UniformSymbolTable* table = NULL;

UniformPreprocessor* expressions_preprocessor = NULL;
UniformParser* expressions_parser = NULL;

define_fixture(before, before_expressions) {
  table = UniformSymbolTableModule.init();
  UniformCoreIntegerModule.init(table);

  expressions_preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
  UniformPreprocessorModule.process(expressions_preprocessor, "modules/parser/test/files/expressions.u", NULL);
  
  expressions_parser = UniformParserModule.init(table, expressions_preprocessor->token_array);
}

define_fixture(after, after_expressions) {
  UniformParserModule.close(expressions_parser);
  UniformPreprocessorModule.close(expressions_preprocessor);
  UniformSymbolTableModule.clear(table);
}

describe("Expressions Test Suite", expressions_test_suite)
  before(before_expressions)
  after(after_expressions)

  context(".expressions")
    it("parses complex expressions")
      // 17 neg 42 + 4 / 2 3 * - 9 3 - 21 + *
      UNIFORM_NODE_TYPE node_types[] = {
        UNIFORM_LITERAL_NODE,UNIFORM_OPERATOR_NODE,UNIFORM_LITERAL_NODE,
        UNIFORM_OPERATOR_NODE,UNIFORM_LITERAL_NODE,UNIFORM_OPERATOR_NODE,
        UNIFORM_LITERAL_NODE,UNIFORM_LITERAL_NODE,UNIFORM_OPERATOR_NODE,
        UNIFORM_OPERATOR_NODE,UNIFORM_LITERAL_NODE,UNIFORM_LITERAL_NODE,UNIFORM_OPERATOR_NODE,
        UNIFORM_LITERAL_NODE,UNIFORM_OPERATOR_NODE,UNIFORM_OPERATOR_NODE
      };

      UniformASTExpressionNode* tree = UniformParserExpression.process(expressions_parser, NULL, NULL);
      expect(tree->used) to equal(16)

      for (int i = 0; i < tree->used; i++) {
        expect(tree->nodes[i]->type) to equal(node_types[i])
      }
    end
  end
end