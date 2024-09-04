#include "awry/awry.h"
#include "uniform/scanner/shared.h"

describe("Scanner Test Suite", scanner_test_suite)
  it("is instrumented properly")
    expect(0) to equal(0)
  end

  context(".init")
    when("A file is not found")
      UniformScanner *scanner = UniformScannerModule.init("test/files/notfound.u");
  
      it("Sets the scanner's errored status to 1")
        expect(scanner->errored) to equal(1)
      end
    end

    when("A file is found")
      UniformScanner *scanner = UniformScannerModule.init("modules/scanner/test/files/tokens.u");
  
      it("Sets the scanner's errored status to 0")
        expect(scanner->errored) to equal(0)
      end   
    end
  end
end

int main(void) {
  Awry.run();
  Awry.clear(&Awry);
  return 0;
}