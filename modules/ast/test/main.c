#include "awry/awry.h"
#include "uniform/ast/shared.h"

describe("AST Test Suite", ast_test_suite)
  context(".init_tree")
    it("todo")
      expect(1) to equal(1)
    end
  end

  context(".insert_node")
    it("todo")
      expect(1) to equal(1)
    end
  end

  context(".free_tree")
    it("todo")
      expect(1) to equal(1)
    end
  end
end

int main(void) {
  Awry.run();
  Awry.clear(&Awry);

  return 0;
}