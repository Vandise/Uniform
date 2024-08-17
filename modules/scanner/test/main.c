#include "awry/awry.h"

describe("Scanner Test Suite", scanner_test_suite)
  it("is instrumented properly")
    expect(0) to equal(0)
  end
end

int main(void) {
  Awry.run();
  Awry.clear(&Awry);
  return 0;
}