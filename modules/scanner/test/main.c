#include "awry/awry.h"
#include "uniform/scanner/shared.h"

define_fixture(before, scanner_before_all) {
  *subject = UniformScannerModule.init("modules/scanner/test/files/tokens.u");
}

define_fixture(before, scanner_before_populate_chartable) {
  UniformScanner *scanner = *subject;
  int ch;

  for (ch = 0;   ch < 256;  ++ch) scanner->char_table[ch] = SPECIAL_CHAR_CODE;
  for (ch = '0'; ch <= '9'; ++ch) scanner->char_table[ch] = DIGIT_CHAR_CODE;
  for (ch = 'A'; ch <= 'Z'; ++ch) scanner->char_table[ch] = UPPERCASE_LETTER_CHAR_CODE;
  for (ch = 'a'; ch <= 'z'; ++ch) scanner->char_table[ch] = LETTER_CHAR_CODE;
  scanner->char_table['\''] = QUOTE_CHAR_CODE;
  scanner->char_table['"'] = QUOTE_CHAR_CODE;
  scanner->char_table['\n'] = NEWLINE_CHAR_CODE;
  scanner->char_table['_'] = UNDERSCORE_CHAR_CODE;
  scanner->char_table[EOF_CHAR] = EOF_CHAR_CODE;
}

define_fixture(after, scanner_after_all) {
  UniformScanner *scanner = *subject;
  UniformScannerModule.close(scanner);
}

describe("Scanner Test Suite", scanner_test_suite)

  before(scanner_before_all)
  after(scanner_after_all)

  context(".init")
    when("A file is not found")
      it("Sets the scanner's errored status to 1")
        UniformScanner *scanner = UniformScannerModule.init("notfound.u"); 
        expect(scanner->errored) to equal(1)
      end
    end

    when("A file is found")
      it("Sets the current character to a null terminator")
        UniformScanner *scanner = subject();
        expect(scanner->current_char) to equal('\0')
      end

      it("Sets the source name to the file name")
        UniformScanner *scanner = subject();
        expect(scanner->source_name) to equal("modules/scanner/test/files/tokens.u")
      end

      it("Sets the source buffer to an empty string")
        UniformScanner *scanner = subject();
        expect(scanner->source_buffer) to equal("")
      end

      it("Sets the line number to 0")
        UniformScanner *scanner = subject();
        expect(scanner->line_number) to equal(0)
      end

      it("Sets the level to 0")
        UniformScanner *scanner = subject();
        expect(scanner->level) to equal(0)
      end

      it("Sets the buffer offset to 0")
        UniformScanner *scanner = subject();
        expect(scanner->buffer_offset) to equal(0)
      end

      it("Sets the source buffer pointer to the source buffer")
        UniformScanner *scanner = subject();
        // Test pending character reading
        //expect(scanner->source_bufferp) to equal(scanner->source_buffer)
      end

      it("Sets the scanner's errored status to 0")
        UniformScanner *scanner = subject();
        expect(scanner->errored) to equal(0)
      end

      it("Sets all characters in the char table to UNDEFINED_CHAR_CODE")
        UniformScanner *scanner = subject();
        for (int ch = 0; ch < 256; ++ch) {
          expect(scanner->char_table[ch]) to equal(UNDEFINED_CHAR_CODE);
        }
      end
    end
  end

  context(".get_token")
    before(scanner_before_populate_chartable)

    when("A file is found")
      it("extracts the expected tokens")
        UniformScanner *scanner = subject();
        UniformScannerModule.get_token(scanner);

        expect(scanner->current_token.code) to equal(T_MACRO)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_IDENTIFIER)
        expect(scanner->current_token.token_string) to equal("import")

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_LPAREN)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_STRING)
        expect(scanner->current_token.token_string) to equal("\"./import.u\"")

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_RPAREN)

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
        expect(scanner->current_token.code) to equal(T_END)

        UniformScannerModule.get_token(scanner);
        expect(scanner->current_token.code) to equal(T_END_OF_FILE)
      end
    end
  end

end

int main(void) {
  Awry.run();
  Awry.clear(&Awry);
  return 0;
}