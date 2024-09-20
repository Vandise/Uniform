#include "awry/awry.h"
#include "uniform/parser/shared.h"

// for unit testing only
#include "uniform/preprocessor/shared.h"

#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__)
  #define SCANNER_LIB "lib/libuniformscanner.dll"
#endif

#ifdef __linux__
  #define SCANNER_LIB "lib/libuniformscanner.so"
#endif

#ifdef __APPLE__
  #define SCANNER_LIB "lib/libuniformscanner.dylib.1.0.0"
#endif

UniformPreprocessor* expressions_preprocessor = NULL;
UniformParser* expressions_parser = NULL;

define_fixture(before, before_expressions) {
  expressions_preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
  UniformPreprocessorModule.process(expressions_preprocessor, "modules/parser/test/files/expressions.u", NULL);
  
  expressions_parser = UniformParserModule.init(expressions_preprocessor->token_array);
}

define_fixture(after, after_expressions) {
  UniformParserModule.close(expressions_parser);
  UniformPreprocessorModule.close(expressions_preprocessor);
}

describe("Parser Test Suite", parser_test_suite)
  context(".init")
    it("sets the token array")
      UniformParser* parser_ini = UniformParserModule.init(NULL);
      expect((void*)(parser_ini->token_arr)) to be_null
      UniformParserModule.close(parser_ini);
    end

    it("sets the token index to 0")
      UniformParser* parser_ini = UniformParserModule.init(NULL);
      expect(parser_ini->token_index) to equal(0)
      UniformParserModule.close(parser_ini);
    end
  end

  context(".next")
    it("increments the token_index")
      UniformParser* parser_ini = UniformParserModule.init(NULL);
      UniformParserModule.next(parser_ini);
      expect(parser_ini->token_index) to equal(1)
      UniformParserModule.close(parser_ini);
    end
  end

  context(".get_token")
    it("returns the token at the token_index")
      UniformPreprocessor* preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
      UniformPreprocessorModule.process(preprocessor, "modules/parser/test/files/get_token.u", NULL);

      UniformParser* parser = UniformParserModule.init(preprocessor->token_array);

      expect(UniformParserModule.get_token(parser)->code) to equal(T_NUMERIC)

      UniformParserModule.close(parser);
      UniformPreprocessorModule.close(preprocessor);
    end

    it("returns NULL if the token index exceeds the array size")
      UniformPreprocessor* preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
      UniformPreprocessorModule.process(preprocessor, "modules/parser/test/files/get_token.u", NULL);

      UniformParser* parser = UniformParserModule.init(preprocessor->token_array);
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

      UniformParser* parser = UniformParserModule.init(preprocessor->token_array);

      expect(UniformParserModule.peek(parser, 1)->code) to equal(T_STRING)

      UniformParserModule.close(parser);
      UniformPreprocessorModule.close(preprocessor);
    end

    it("returns NULL if the token index exceeds the array size")
      UniformPreprocessor* preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, 0);
      UniformPreprocessorModule.process(preprocessor, "modules/parser/test/files/get_token.u", NULL);

      UniformParser* parser = UniformParserModule.init(preprocessor->token_array);
      parser->token_index = 0;
      UniformToken* t = UniformParserModule.peek(parser, 5);

      expect((void*)t) to equal(NULL)

      UniformParserModule.close(parser);
      UniformPreprocessorModule.close(preprocessor);
    end
  end

  context(".expressions")
    before(before_expressions)
    after(after_expressions)

    it("parses addition expressions")
      UniformParserExpression.process(expressions_parser);
    end
  end
end

int main(void) {
  //UniformPreprocessorModule.log_level = UNIFORM_LOG_NONE;
  //UniformLogger.log_level = UNIFORM_LOG_NONE;

  Awry.run();
  Awry.clear(&Awry);

  return 0;
}