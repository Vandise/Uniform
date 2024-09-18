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

static void ts_sleep(int s) {
  int now = (int)time(NULL);
  do {
    int ls = (int)time(NULL);
    if (ls - now >= s) {
      break;
    }
  } while(1);
}

static int await_file(char* file_name, int timeout) {
  int now = (int)time(NULL);
  int failed = 0;

  do {
    int ls = (int)time(NULL);
    if (ls - now > timeout) {
      failed = 1;
      break;
    }
  } while(access(file_name, F_OK) != 0);

  return failed;
}

int emit = UNIFORM_PREPROCESSOR_EMIT;
UniformPreprocessor* preprocessor = NULL;

define_fixture(before, before_all) {
  preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, emit);
  UniformPreprocessorModule.register_macro(preprocessor, "import", UniformMacrosModule.import_macro);
}

define_fixture(before, before_register_macro) {}

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
      expect(preprocessor->n_macro_size) to equal(10)
      expect(preprocessor->n_macro_used) to equal(1)
      expect(preprocessor->macros[0].name) to equal("import")
    end
  end

  context(".process")
    it("fails when a file is not found")
      UniformPreprocessorModule.process(preprocessor, "modules/preprocessor/test/files/dne.u", NULL);

      expect(preprocessor->errored) to equal(1)
    end

    it("emits the tokens to a file in the tmp directory")
      UniformPreprocessorModule.process(preprocessor, "modules/preprocessor/test/files/import.u", NULL);
      UniformPreprocessorModule.close_file_streams(preprocessor);

      await_file(preprocessor->emit_file_name, 1);

      FILE *file = fopen(preprocessor->emit_file_name, "r");
  
      expect((void*)file) to not be_null

      fclose(file);
    end

    it("emits the expected tokens")
      UniformPreprocessorModule.process(preprocessor, "modules/preprocessor/test/files/import.u", NULL);
      UniformPreprocessorModule.close_file_streams(preprocessor);

      await_file(preprocessor->emit_file_name, 1);

      UniformToken t = { .code = T_END_OF_FILE };
      preprocessor->token_module->commit_token(preprocessor->token_array, t);

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
        expect(preprocessed_tokens[i]) to equal(preprocessor->token_array->tokens[i].code)
      }

      preprocessor->scanner_module->close(scanner);
    end

    it("terminates processing on error")
      UniformPreprocessorModule.process(preprocessor, "modules/preprocessor/test/files/import_failure.u", NULL);
      UniformPreprocessorModule.close_file_streams(preprocessor);

      expect(preprocessor->errored) to equal(1)
    end

    it("terminates on the expected token")
      UniformPreprocessorModule.process(preprocessor, "modules/preprocessor/test/files/import_failure.u", NULL);
      UniformPreprocessorModule.close_file_streams(preprocessor);

      await_file(preprocessor->emit_file_name, 1);

      UniformScanner *scanner = preprocessor->scanner_module->init(preprocessor->emit_file_name);
      init_chartable(scanner);

      UNIFORM_TOKEN_CODE preprocessed_tokens[] = {
        T_MODULE, T_CONSTANT, T_NEWLINE, T_STRING, T_NEWLINE, T_END, T_NEWLINE,
        T_MODULE, T_CONSTANT, T_NEWLINE, T_END, T_NEWLINE,
        T_END_OF_FILE
      };

      for (int i = 0; i < (sizeof(preprocessed_tokens) / sizeof(UNIFORM_TOKEN_CODE)); i++) {
        preprocessor->scanner_module->get_token(scanner);
        expect(scanner->current_token.code) to equal(preprocessed_tokens[i]) 
      }

      preprocessor->scanner_module->close(scanner);
    end
  end

  context("syntax errors")
    it("fails when an invalid macro is defined")
      UniformPreprocessorModule.process(preprocessor, "modules/preprocessor/test/files/syntax_errors/import.u", NULL);

      expect(preprocessor->errored) to equal(1)
    end
  end
end

int main(void) {
  UniformPreprocessorModule.log_level = UNIFORM_LOG_NONE;
  UniformLogger.log_level = UNIFORM_LOG_NONE; //UniformPreprocessorModule.log_level;

  UniformLogger.log_info("Running testsuite for Preprocessor Module version %d", UNIFORM_PREPROCESSOR_VERSION);

  Awry.run();
  Awry.clear(&Awry);

  return 0;
}