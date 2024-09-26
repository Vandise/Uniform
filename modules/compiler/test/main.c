#include "awry/awry.h"
#include "uniform/compiler/shared.h"
#include "uniform/parser/shared.h"
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

UniformCompiler* compiler;
UniformSymbolTable* table;

define_fixture(before, before_all) {
  compiler = UniformCompilerModule.init(UNIFORM_X_86_64);
  table = UniformSymbolTableModule.init();
  UniformCoreIntegerModule.init(table);
}

define_fixture(after, after_all) {
  free(compiler);
  compiler = NULL;
  UniformSymbolTableModule.clear(table);
}

describe("Compiler Test Suite", compiler_test_suite)
  before(before_all)
  after(after_all)

  context("UniformCompilerModule.init")
    it("sets initialized to 1")
      expect(UniformCompilerModule.initialized) to equal(1)
    end

    it("sets the compile action")
      expect((void*)(UniformCompilerModule.compile)) to not be_null
    end
  end

  context("UniformCompilerModule.compile")
    it("compiles expressions")
      UniformPreprocessor* expressions_preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
        UniformPreprocessorModule.process(expressions_preprocessor, "modules/compiler/test/files/expression.u", NULL);
      UniformParser* expressions_parser = UniformParserModule.init(table, expressions_preprocessor->token_array);

      UniformASTExpressionNode* tree = UniformParserExpression.process(expressions_parser, NULL);
      expect(tree->used) to equal(16)

      int status = UniformCompilerModule.compile(compiler, tree);

      UniformParserModule.close(expressions_parser);
      UniformPreprocessorModule.close(expressions_preprocessor);
    end
  end
end

int main(void) {
  Awry.run();
  Awry.clear(&Awry);

  return 0;
}