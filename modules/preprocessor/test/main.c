#include "awry/awry.h"
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

static void init_chartable(UniformScanner* scanner) {
  int ch;

  for (ch = 0;   ch < 256;  ++ch) scanner->char_table[ch] = SPECIAL_CHAR_CODE;
  for (ch = '0'; ch <= '9'; ++ch) scanner->char_table[ch] = DIGIT_CHAR_CODE;
  for (ch = 'A'; ch <= 'Z'; ++ch) scanner->char_table[ch] = UPPERCASE_LETTER_CHAR_CODE;
  for (ch = 'a'; ch <= 'z'; ++ch) scanner->char_table[ch] = LETTER_CHAR_CODE;
  scanner->char_table['\''] = QUOTE_CHAR_CODE;
  scanner->char_table['"'] = QUOTE_CHAR_CODE;
  scanner->char_table[NEWLINE_CHAR] = NEWLINE_CHAR_CODE;
  scanner->char_table[CARRIAGE_RETURN_CHAR] = CARRIAGE_RETURN_CHAR_CODE;
  scanner->char_table['_'] = UNDERSCORE_CHAR_CODE;
  scanner->char_table[EOF_CHAR] = EOF_CHAR_CODE;
}

int emit = UNIFORM_PREPROCESSOR_EMIT;
UniformPreprocessor* preprocessor = NULL;

define_fixture(before, before_all) {
  preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, emit);
}

define_fixture(before, before_register_macro) {
  UniformPreprocessorModule.register_macro(preprocessor, "import", UniformMacrosModule.import_macro);
}

define_fixture(after, after_all) {
  UniformPreprocessorModule.close(preprocessor);
}

describe("Preprocessor Test Suite", preprocessor_test_suite)
  before(before_all)
  after(after_all)

  context(".init")
    it("finds the scanner module from the dynamic library")
      expect(preprocessor->scanner_module->version) to equal(100)
    end
  end

  context(".register_macro")
    it("adds the macro to the macros list")
      UniformPreprocessorModule.register_macro(preprocessor, "import", UniformMacrosModule.import_macro);
      expect(preprocessor->n_macro_size) to equal(10)
      expect(preprocessor->n_macro_used) to equal(1)
      expect(preprocessor->macros[0].name) to equal("import")
    end
  end

  context(".process")
    before(before_register_macro)

    it("fails when a file is not found")
      UniformPreprocessorModule.process(preprocessor, "modules/preprocessor/test/files/dne.u", NULL);
    end

    it("emits a compiled set of tokens to the tmp directory")
      UniformPreprocessorModule.process(preprocessor, "modules/preprocessor/test/files/import.u", NULL);
      // todo: ensure file exists
    end

    it("emits the expected tokens")
      UniformScanner *scanner = preprocessor->scanner_module->init(preprocessor->emit_file_name);
      init_chartable(scanner);

      UNIFORM_TOKEN_CODE preprocessed_tokens[] = {
        T_MODULE, T_CONSTANT, T_NEWLINE, T_STRING, T_NEWLINE, T_END, T_NEWLINE,
        T_MODULE, T_CONSTANT, T_NEWLINE, T_END, T_NEWLINE,
        T_MODULE, T_CONSTANT, T_NEWLINE, T_NUMERIC, T_NUMERIC, T_NEWLINE,
        T_DOT, T_EQUAL, T_OPEN_CURLY_BRACE, T_CLOSE_CURLY_BRACE, T_OPEN_BRACKET, T_CLOSE_BRACKET, T_PLUS, T_MINUS, T_STAR, T_SLASH, T_PIN, T_COLON, T_SEMICOLON, T_COMMA,
        T_QUESTION, T_BANG, T_OPEN_PAREN, T_CLOSE_PAREN, T_PIPE, T_LESS_THAN, T_GREATER_THAN, T_TILDE, T_PERCENT, T_NEWLINE,
        T_PIPE_OPERATOR, T_LAMBDA, T_NEWLINE,
        T_FUNC, T_STRUCT, T_CASE, T_RETURN, T_NEWLINE,
        T_END, T_NEWLINE,
        T_END_OF_FILE
      };

      for (int i = 0; i < (sizeof(preprocessed_tokens) / sizeof(UNIFORM_TOKEN_CODE)); i++) {
        preprocessor->scanner_module->get_token(scanner);
        expect(scanner->current_token.code) to equal(preprocessed_tokens[i]) 
      }

      preprocessor->scanner_module->close(scanner);
    end
  end
end

int main(void) {
  UniformLogger.log_level = UniformPreprocessorModule.log_level;

  UniformLogger.log_info("Running testsuite for Preprocessor Module version %d", UNIFORM_PREPROCESSOR_VERSION);

  Awry.run();
  Awry.clear(&Awry);

  return 0;
}