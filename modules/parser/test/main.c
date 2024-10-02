#include "awry/awry.h"
#include "uniform/parser/shared.h"
#include "uniform/preprocessor/shared.h"
#include "uniform/core/shared.h"

int main(void) {
  UniformPreprocessorModule.log_level = UNIFORM_LOG_NONE;
  UniformLogger.log_level = UNIFORM_LOG_NONE;

  Awry.run();
  Awry.clear(&Awry);

  return 0;
}