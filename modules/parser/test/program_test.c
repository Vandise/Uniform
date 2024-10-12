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

UniformSymbolTable* program_table = NULL;
UniformPreprocessor* program_preprocessor = NULL;
UniformParser* program_parser = NULL;

define_fixture(before, before_modules) {
  program_table = UniformSymbolTableModule.init();
  UniformCoreIntegerModule.init(program_table);

  program_preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
  UniformPreprocessorModule.process(program_preprocessor, "modules/parser/test/files/program.u", NULL);

  program_parser = UniformParserModule.init(program_table, program_preprocessor->token_array);
}

define_fixture(after, after_modules) {
  UniformParserModule.close(program_parser);
  UniformPreprocessorModule.close(program_preprocessor);
  UniformSymbolTableModule.clear(program_table);
}

describe("Program Parser Test Suite", program_parser_test_suite)
  before(before_modules)
  after(after_modules)

  context("modules")
    it("processes the program")
      UniformAST* program_tree = UniformProgramParser.process(program_parser);

      UniformSymbolTableNode* mainsym = UniformSymbolTableModule.search_global(program_table, "Main");
      expect(mainsym->name) to equal("Main")

      UniformSymbolTableNode* subsym = UniformSymbolTableModule.search_global(mainsym->definition.info.module.symbol_table, "SubMain");
      expect(subsym->name) to equal("SubMain")

      UniformSymbolTableNode* configsym = UniformSymbolTableModule.search_global(program_table, "Config");
      expect(configsym->name) to equal("Config")
    end
  end
end