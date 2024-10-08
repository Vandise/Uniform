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

UniformSymbolTable* parser_table = NULL;

define_fixture(before, before_parser) {
  parser_table = UniformSymbolTableModule.init();
  UniformCoreIntegerModule.init(parser_table);
}

define_fixture(after, after_parser) {
  UniformSymbolTableModule.clear(parser_table);
}

describe("Parser Test Suite", parser_test_suite)
  before(before_parser)
  after(after_parser)

  context(".init")
    it("sets the token array")
      UniformParser* parser_ini = UniformParserModule.init(NULL, NULL);
      expect((void*)(parser_ini->token_arr)) to be_null
      UniformParserModule.close(parser_ini);
    end

    it("sets the token index to 0")
      UniformParser* parser_ini = UniformParserModule.init(NULL, NULL);
      expect(parser_ini->token_index) to equal(0)
      UniformParserModule.close(parser_ini);
    end
  end

  context(".next")
    it("increments the token_index")
      UniformParser* parser_ini = UniformParserModule.init(NULL, NULL);
      UniformParserModule.next(parser_ini);
      expect(parser_ini->token_index) to equal(1)
      UniformParserModule.close(parser_ini);
    end
  end

  context(".get_token")
    it("returns the token at the token_index")
      UniformPreprocessor* preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
      UniformPreprocessorModule.process(preprocessor, "modules/parser/test/files/get_token.u", NULL);

      UniformParser* parser = UniformParserModule.init(parser_table, preprocessor->token_array);

      expect(UniformParserModule.get_token(parser)->code) to equal(T_NUMERIC)

      UniformParserModule.close(parser);
      UniformPreprocessorModule.close(preprocessor);
    end

    it("returns NULL if the token index exceeds the array size")
      UniformPreprocessor* preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
      UniformPreprocessorModule.process(preprocessor, "modules/parser/test/files/get_token.u", NULL);

      UniformParser* parser = UniformParserModule.init(parser_table, preprocessor->token_array);
      parser->token_index = 3;
      UniformToken* t = UniformParserModule.get_token(parser);

      expect((void*)t) to equal(NULL)

      UniformParserModule.close(parser);
      UniformPreprocessorModule.close(preprocessor);
    end
  end

  context(".peek_token")
    it("returns the token at the requested index")
      UniformPreprocessor* preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
      UniformPreprocessorModule.process(preprocessor, "modules/parser/test/files/get_token.u", NULL);

      UniformParser* parser = UniformParserModule.init(parser_table, preprocessor->token_array);

      expect(UniformParserModule.peek(parser, 1)->code) to equal(T_STRING)

      UniformParserModule.close(parser);
      UniformPreprocessorModule.close(preprocessor);
    end

    it("returns NULL if the token index exceeds the array size")
      UniformPreprocessor* preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
      UniformPreprocessorModule.process(preprocessor, "modules/parser/test/files/get_token.u", NULL);

      UniformParser* parser = UniformParserModule.init(parser_table, preprocessor->token_array);
      parser->token_index = 0;
      UniformToken* t = UniformParserModule.peek(parser, 5);

      expect((void*)t) to equal(NULL)

      UniformParserModule.close(parser);
      UniformPreprocessorModule.close(preprocessor);
    end
  end
end