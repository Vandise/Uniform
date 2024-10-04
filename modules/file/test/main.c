#include "awry/awry.h"
#include "uniform/file/shared.h"


describe("File Test Suite", file_test_suite)

  context(".get_file_path")
    when("absolute path")
      it("returns the same path")
        expect(UniformFileUtil.get_file_path("/a/b")) to equal("/a/b")
        expect(UniformFileUtil.get_file_path("/a/")) to equal("/a/")
      end
    end

    when("absolute path with '..'")
      it("returns correct path")
        expect(UniformFileUtil.get_file_path("/a/../b")) to equal("/b")
        expect(UniformFileUtil.get_file_path("/../b/")) to equal("/b/")
      end
    end

    when("absolute path with './'")
      it("returns the correct path")
        expect(UniformFileUtil.get_file_path("/a/./b")) to equal("/a/b")
        expect(UniformFileUtil.get_file_path("/./b/")) to equal("/b/")
      end
    end
  end
end

int main(void) {
  Awry.run();
  Awry.clear(&Awry);
  return 0;
}