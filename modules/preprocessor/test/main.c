#include "awry/awry.h"
#include "uniform/preprocessor/shared.h"

#ifdef _WIN32
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
    UniformPreprocessor* preprocessor = UniformPreprocessorModule.init(SCANNER_LIB);

    it("sets the scanner module structure")
      expect(preprocessor->scanner_module->version) to equal(100)
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