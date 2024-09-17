#include "awry/awry.h"
#include "uniform/scanner/shared.h"

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

describe("Scanner Test Suite", scanner_test_suite)

  context(".init")
    when("A file is not found")
      UniformScanner *s = UniformScannerModule.init("notfound.u"); 

      it("Sets the scanner's errored status to 1")
        expect(s->errored) to equal(1)
      end

      UniformScannerModule.close(s);
    end

    when("A file is found")
      UniformScanner *s = UniformScannerModule.init("modules/scanner/test/files/tokens.u");

      it("Sets the current character to a null terminator")
        expect(s->current_char) to equal('\0')
      end

      it("Sets the line number to -1")
        expect(s->line_number) to equal(-1)
      end

      it("Sets the level to 0")
        expect(s->level) to equal(0)
      end

      it("Sets the buffer offset to 0")
        expect(s->buffer_offset) to equal(0)
      end

      it("Sets the scanner's errored status to 0")
        expect(s->errored) to equal(0)
      end

      it("Sets the source name to the file name")
        expect(s->source_name) to equal("modules/scanner/test/files/tokens.u")
      end

      it("Sets all characters in the char table to UNDEFINED_CHAR_CODE")
        for (int ch = 0; ch < 256; ++ch) {
          expect(s->char_table[ch]) to equal(UNDEFINED_CHAR_CODE);
        }
      end

      UniformScannerModule.close(s);
    end
  end

  context(".get_token")
    UniformScanner *scanner = UniformScannerModule.init("modules/scanner/test/files/tokens.u");

    init_chartable(scanner);

    when("A file is found")
      it("extracts the expected tokens")
        UniformScannerModule.get_token(scanner);

        expect(scanner->current_token.code) to equal(T_MACRO)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_IDENTIFIER)
        expect(scanner->current_token.token_string) to equal("import")

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_OPEN_PAREN)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_STRING)
        expect(scanner->current_token.token_string) to equal("\"./import\"")

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_CLOSE_PAREN)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_NEWLINE)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_MODULE)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_CONSTANT)
        expect(scanner->current_token.token_string) to equal("User")

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_NEWLINE)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_NUMERIC)
        expect(scanner->current_token.literal.value.i32) to equal(42)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_NUMERIC)
        expect(scanner->current_token.literal.value.f32) to equal(0.42)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_NEWLINE)

        UNIFORM_TOKEN_CODE special_tokens[] = {
          T_DOT, T_EQUAL, T_OPEN_CURLY_BRACE, T_CLOSE_CURLY_BRACE, T_OPEN_BRACKET, T_CLOSE_BRACKET,
          T_PLUS, T_MINUS, T_STAR, T_SLASH, T_PIN, T_COLON, T_SEMICOLON, T_COMMA, T_QUESTION, T_BANG,
          T_OPEN_PAREN, T_CLOSE_PAREN, T_PIPE, T_GREATER_THAN, T_LESS_THAN, T_TILDE, T_PERCENT
        };

        for (int i = 0; i < 23; i++) {
          UniformScannerModule.get_token(scanner);
          expect(scanner->current_token.code) to equal(special_tokens[i]) 
        }

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_NEWLINE)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_PIPE_OPERATOR)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_LAMBDA)
        
        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_NEWLINE)

        UNIFORM_TOKEN_CODE keyword_tokens[] = {
          T_FUNC, T_STRUCT, T_CASE, T_RETURN
        };

        for (int i = 0; i < 4; i++) {
          UniformScannerModule.get_token(scanner);
          expect(scanner->current_token.code) to equal(keyword_tokens[i]) 
        }
        
        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_NEWLINE)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_END)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_END_OF_FILE)
      end
    end

    UniformScannerModule.close(scanner);
  end

end

int main(void) {
  UniformLogger.log_level = UNIFORM_LOG_INFO;

  UniformLogger.log_info("Running testsuite for Scanner Module version %d", UNIFORM_SCANNER_VERSION);

  Awry.run();
  Awry.clear(&Awry);
  return 0;
}