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

describe("Preprocessor Test Suite", preprocessor_test_suite)
  context(".init")
    UniformPreprocessor* preprocessor = UniformPreprocessorModule.init(SCANNER_LIB, UNIFORM_PREPROCESSOR_EMIT);

    it("finds the scanner module from the dynamic library")
      expect(preprocessor->scanner_module->version) to equal(100)
    end

    it("sets the emit configuration")
      expect(preprocessor->emit) to equal(1)
    end

    UniformPreprocessorModule.close(preprocessor);
  end
end

int main(void) {
  UniformLogger.log_level = UNIFORM_LOG_INFO;

  UniformLogger.log_info("Running testsuite for Preprocessor Module version %d", UNIFORM_PREPROCESSOR_VERSION);

  Awry.run();
  Awry.clear(&Awry);

  return 0;
}