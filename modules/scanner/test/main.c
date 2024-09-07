#include "awry/awry.h"
#include "uniform/scanner/shared.h"

define_fixture(before, scanner_before_all) {
  *subject = UniformScannerModule.init("modules/scanner/test/files/tokens.u");
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
    when("A file is found")
      it("extracts the token")
        UniformScanner *scanner = subject();
        UniformScannerModule.get_token(scanner);

        expect(scanner->current_token.code) to equal(T_MACRO)
      end
    end
  end

end

int main(void) {
  Awry.run();
  Awry.clear(&Awry);
  return 0;
}