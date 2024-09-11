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

int emit = UNIFORM_PREPROCESSOR_EMIT;
UniformPreprocessor* preprocessor = NULL;

define_fixture(before, before_all) {
  preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, emit);
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

    it("sets the emit configuration")
      expect(preprocessor->emit) to equal(1)
    end
  end

  context(".process")
    it("emits a compiled set of tokens to the tmp directory")
      UniformPreprocessorModule.process(preprocessor, "modules/preprocessor/test/files/import.u");
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